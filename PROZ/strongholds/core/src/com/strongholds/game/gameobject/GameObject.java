package com.strongholds.game.gameobject;

import com.badlogic.gdx.math.Vector2;
import com.badlogic.gdx.physics.box2d.*;
import com.strongholds.game.GameSingleton;

/**
 * Represents static game object. Base class for AnimatedActor
 */
public class GameObject implements IGameObject{
    /**
     * reference to an instance of global GameSingleton
     */
    private GameSingleton gameSingleton;
    /**
     * reference to an instance of box2d world
     */
    private World world;

    /**
     * object current health
     */
    protected int health;
    /**
     * object max health
     */
    protected int maxHealth;

    protected boolean collision = false;

    private GameObjectDef objectDef;

    public GameObject(GameObjectDef objectDef){
        gameSingleton = GameSingleton.getGameSingleton();
        this.world = gameSingleton.getWorld();

        this.objectDef = new GameObjectDef(objectDef);
        createFixture();
    }

    /**
     * Creates a new fixture and sets its collision filter
     */
    private void createFixture(){
        //create main fixture's definition
        PolygonShape polygonShape = new PolygonShape();
        polygonShape.setAsBox(getWidth(), getHeight());
        FixtureDef fixtureDef = new FixtureDef();
        fixtureDef.shape = polygonShape;
        fixtureDef.density = 3.0f;
        fixtureDef.friction = 0.6f;
        fixtureDef.restitution = 0.0f;
        //create body, main fixture and its userData and collisionFilter(collisionFilter can be overridden by inheriting classes)
        Fixture fixture = getBody().createFixture(fixtureDef);
        fixture.setUserData(this);

        setCollisionFilter(fixture);
    }

    /**
     * Sets fixture's collision filter
     * @param fixture fixture that needs to have its collision filter set
     */
    private void setCollisionFilter(Fixture fixture){
        Filter filter = new Filter();
        GameSingleton.ObjectType type = getType();

        if (type == GameSingleton.ObjectType.BASE){
            filter.categoryBits = GameSingleton.BASE_COLLISION_MASK;
            filter.maskBits = GameSingleton.GAME_OBJECT_COLLISION_MASK
                    | GameSingleton.SENSOR_COLLISION_MASK;
        }
        else if (type == GameSingleton.ObjectType.SWORDSMAN || type == GameSingleton.ObjectType.ARCHER){
            filter.categoryBits = GameSingleton.ACTOR_COLLISION_MASK;
            filter.maskBits = GameSingleton.GAME_OBJECT_COLLISION_MASK
                    | GameSingleton.SENSOR_COLLISION_MASK
                    | GameSingleton.ACTOR_COLLISION_MASK;
        }
        else{
            filter.categoryBits = GameSingleton.GAME_OBJECT_COLLISION_MASK;
            filter.maskBits = GameSingleton.GAME_OBJECT_COLLISION_MASK
                    | GameSingleton.SENSOR_COLLISION_MASK
                    | GameSingleton.ACTOR_COLLISION_MASK
                    | GameSingleton.BASE_COLLISION_MASK;
        }

        fixture.setFilterData(filter);
    }

    public void dispose(){
        world.destroyBody(getBody());
    }

    public Vector2 getPosition(){
        return getBody().getPosition();
    }

    public Vector2 getVelocity(){ return getBody().getLinearVelocity(); }

    public float getWidth() {
        return objectDef.width;
    }

    public float getHeight() {
        return objectDef.height;
    }

    public GameSingleton.ObjectType getType() {
        return objectDef.type;
    }

    public void gotHit(int damage){
        health -= damage;
        if (health < 0)
            health = 0;
    }

    public String getId() {
        return objectDef.id;
    }

    public void setId(String id) {
        objectDef.id = id;
    }

    public boolean isEnemy(){ return objectDef.isEnemy; }

    public int getHealth() {
        return health;
    }

    public int getMaxHealth(){
        return maxHealth;
    }

    public void setHealth(int health) {
        this.health = health;
    }

    protected Body getBody(){
        return objectDef.body;
    }

    protected void setBody(Body body){
        objectDef.body = body;
    }

    /**
     * Sets object max health
     * @param value max health
     */
    public void setMaxHealth(int value) {
        maxHealth = value;
    }

    public boolean isCollision() {
        return collision;
    }

    public void setCollision(boolean collision) {
        this.collision = collision;
    }
}
