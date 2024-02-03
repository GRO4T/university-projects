package com.strongholds.game.model;

import com.badlogic.gdx.math.Vector2;
import com.badlogic.gdx.physics.box2d.*;

import java.util.*;

import com.strongholds.game.GameSingleton;
import com.strongholds.game.GameSingleton.ObjectType;
import com.strongholds.game.controller.IModelController;
import com.strongholds.game.event.ModelEvent;
import com.strongholds.game.gameobject.*;
import com.strongholds.game.gameobject.unit.IUnit;
import com.strongholds.game.gameobject.unit.Unit;

/**
 * Game model
 */
public class Model implements IModel, DeathListener
{
    private World world;
    /**
     * defines how accurately bodies' velocities will be calculated
     */
    private final int velocityIterations = 6;
    /**
     * defines how accurately bodies' positions will be calculated
     */
    private final int positionIterations = 2;
    private final float worldGravity = -15.0f;

    /**
     * timer used to schedule add money task
     */
    private Timer taskScheduler;
    /**
     * flag telling whether we can schedule next add money task
     */
    private boolean addMoney = true;
    /**
     * rate at which money will be added (in milliseconds)
     */
    private int incomeInterval = 1000;
    /**
     * how much money gained on each iteration
     */
    private int moneyGain = 10;
    /**
     * current money
     */
    private long money;
    /**
     * money at the start of the game
     */
    private final long startCash = 1000L;

    /**
     * game objects factory
     */
    private GameObjectsFactory gameObjectsFactory;
    /**
     * map of non-animated game objects
     */
    private Map<String, GameObject> gameObjectMap;
    /**
     * map of units
     */
    private Map<String, IUnit> actorMap;
    /**
     * list of dead units to be handled by model
     */
    private LinkedList<String> listOfDeadUnitId;

    /**
     * custom contact listener
     */
    private MyContactListener contactListener;
    /**
     * reference to controller
     */
    private IModelController controller;

    /**
     * base's initial health
     */
    private final int baseInitialHealth = 100; // 100

    /**
     * Creates a new model
     * @param controller reference to controller
     */
    public Model(IModelController controller)
    {
        this.controller = controller;
        money = startCash;

        world = new World(new Vector2(0, worldGravity), true);
        GameSingleton.getGameSingleton().setWorld(world);
        contactListener = new MyContactListener();
        world.setContactListener(contactListener);

        gameObjectsFactory = new GameObjectsFactory(world);
        gameObjectMap = new HashMap<>();
        actorMap = new HashMap<>();
        listOfDeadUnitId = new LinkedList<>();

        taskScheduler = new Timer(true);
    }

    public void update(float timeStep)
    {
        for (IUnit actor : actorMap.values()){
            actor.update();
        }

        if (addMoney){
            addMoney = false;
            AddMoneyTask addMoneyTask = new AddMoneyTask();
            taskScheduler.schedule(addMoneyTask, incomeInterval);
        }

        world.step(timeStep, velocityIterations, positionIterations);

        if (listOfDeadUnitId.size() > 0){
            String id = listOfDeadUnitId.poll();
            IUnit deadActor = actorMap.get(id);
            if (deadActor != null){
                actorMap.get(id).dispose();
                actorMap.remove(id);
            }
        }
    }

    public void dispose()
    {
        //TODO
    }

    public void createObject(String id, ObjectType objectType, Vector2 position, Vector2 size, boolean isEnemy) {
        GameObject newObject = gameObjectsFactory.createObject(id, objectType, position, size, isEnemy);
        if (objectType == ObjectType.BASE){
            newObject.setHealth(baseInitialHealth);
            newObject.setMaxHealth(baseInitialHealth);
        }

        gameObjectMap.put(id, newObject);
    }

    @Override
    public void createUnit(String id, ObjectType objectType, Vector2 position, Vector2 size, boolean isEnemy) {
        Unit newUnit = (Unit)gameObjectsFactory.createObject(id, objectType, position, size, isEnemy);
        newUnit.setDeathListener(this);
        newUnit.setModel(this);
        actorMap.put(id, newUnit);
    }

    public IReadOnlyGameObject getGameObject(String id){ return gameObjectMap.get(id); }
    public Object[] getGameObjectArray() {
        return gameObjectMap.values().toArray();
    }
    public Map<String, GameObject> getGameObjectMap() { return gameObjectMap; }
    public void updateGameObjectMap(Map<String, GameObject> gameObjectMap) { this.gameObjectMap = gameObjectMap; }

    public IReadOnlyAnimatedActor getActor(String id){ return actorMap.get(id); }
    public Object[] getActorArray(){ return actorMap.values().toArray(); }
    public Map<String, IUnit> getActorMap() { return actorMap; }
    public void updateActorMap(Map<String, IUnit> actorMap){ this.actorMap = actorMap; }

    public long getMoney(){ return money; }
    public void addMoney(long value){
        money += value;
        if (money < 0L)
            money = 0L;
    }

    public void catchUp(float deltaTime){
        money += moneyGain * (int)((1000 * deltaTime) / incomeInterval);
    }

    public int getBaseHealth(){
        return gameObjectMap.get("base").getHealth();
    }
    public int getEnemyBaseHealth(){
        return gameObjectMap.get("enemyBase").getHealth();
    }

    public void notifyDeadUnit(String unitId) {
        listOfDeadUnitId.add(unitId);
    }

    /**
     * Scheduled task used to periodically add money
     */
    private class AddMoneyTask extends TimerTask {

        @Override
        public void run() {
            addMoney(moneyGain);
            addMoney = true;
        }
    }

    public void unitHit(String id, int damage){
        if (id.equals("enemyBase")){
            gameObjectMap.get("base").gotHit(damage);
        }
        else{
            try{
                actorMap.get(id).gotHit(damage);
            }
            catch(NullPointerException e){
                e.printStackTrace();
            }
        }
    }

    /**
     * Called by a unit when it dealt damage to the enemy
     * @param id hit unit id
     * @param damage damage dealt to the unit
     */
    public void enemyUnitHit(String id, int damage){
        controller.addEvent(new ModelEvent(id, damage));
    }
}
