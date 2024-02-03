package com.strongholds.game.gameobject;

import com.badlogic.gdx.math.Vector2;
import com.badlogic.gdx.physics.box2d.*;
import com.strongholds.game.GameSingleton;
import com.strongholds.game.GameSingleton.ObjectState;

import java.util.LinkedList;

/**
 * Represents any animated object with the circular sensor around it (for detecting when something approaches the actor)
 */
public class AnimatedActor extends GameObject implements IAnimatedActor {
    /**
     * current state of the object (e.g. walking, running..)
     */
    private ObjectState state;
    /**
     * list of nearby objects
     */
    private LinkedList<GameObject> targets;

    public AnimatedActor(GameObjectDef objectDef, float range) {
        super(objectDef);
        targets = new LinkedList<>();

        //create sensor definition
        CircleShape sensorShape = new CircleShape();
        sensorShape.setRadius(range);
        FixtureDef sensorDef = new FixtureDef();
        sensorDef.isSensor = true;
        sensorDef.shape = sensorShape;

        //create sensor, set user its userData and collisionFilter
        Fixture sensor  = objectDef.body.createFixture(sensorDef);
        sensor.setUserData(this);
        Filter sensorFilter = new Filter();
        sensorFilter.categoryBits = GameSingleton.SENSOR_COLLISION_MASK;
        sensorFilter.maskBits = GameSingleton.ACTOR_COLLISION_MASK
                | GameSingleton.BASE_COLLISION_MASK;
        sensor.setFilterData(sensorFilter);
    }

    @Override
    public void dispose(){
        super.dispose();
    }

    public GameSingleton.ObjectState getState(){
        return state;
    }

    public void setState(GameSingleton.ObjectState newState){
        state = newState;
    }


    @Override
    public void gotHit(int damage){
        Vector2 impulse;
        float knockbackMultiplier = 15.0f;
        if (isEnemy()){
            impulse = new Vector2(damage * knockbackMultiplier, 0);
        }
        else
            impulse = new Vector2(-damage * knockbackMultiplier, 0);
        getBody().applyLinearImpulse(impulse, getBody().getPosition(), true);
    }

    /**
     * Adds a new target. Called after something collided with actor's circular sensor
     * @param target new target
     */
    public void addTarget(GameObject target){
        if (!targets.contains(target)){
            targets.add(target);
        }
        System.out.println(target.getId());
    }

    /**
     * Removes target from the list
     * @param target target to be removed
     */
    public void removeTarget(GameObject target){
        if (targets.contains(target)){
            targets.remove(target);
        }
    }

    /**
     * Returns list of targets
     * @return list of targets
     */
    public LinkedList<GameObject> getTargets() {
        return targets;
    }
}
