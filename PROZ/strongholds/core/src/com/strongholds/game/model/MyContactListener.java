package com.strongholds.game.model;

import com.badlogic.gdx.physics.box2d.Contact;
import com.badlogic.gdx.physics.box2d.ContactImpulse;
import com.badlogic.gdx.physics.box2d.ContactListener;
import com.badlogic.gdx.physics.box2d.Manifold;
import com.strongholds.game.gameobject.AnimatedActor;
import com.strongholds.game.gameobject.GameObject;
import com.strongholds.game.gameobject.unit.Unit;

/**
 * Custom contact listener
 */
public class MyContactListener implements ContactListener {
    /**
     * Called when two fixtures begin to touch
     * @param contact object containing information about contact
     */
    @Override
    public void beginContact(Contact contact) {
        GameObject objectA = (GameObject) (contact.getFixtureA().getUserData());
        GameObject objectB = (GameObject) (contact.getFixtureB().getUserData());

        //objectA.setCollision(true);
        //objectB.setCollision(true);

        if (contact.getFixtureA().isSensor()){
            AnimatedActor actorA = (AnimatedActor) objectA;
            if (actorA.isEnemy() != objectB.isEnemy()){
                actorA.addTarget(objectB);
            }
            if (objectB instanceof Unit){
                Unit unitB = (Unit) objectB;
                unitB.setContactingUnit(true);
            }
        }
        if (contact.getFixtureB().isSensor()){
            AnimatedActor actorB = (AnimatedActor) objectB;
            if (actorB.isEnemy() != objectA.isEnemy()){
                actorB.addTarget(objectA);
            }
            if (objectA instanceof Unit){
                Unit unitA = (Unit) objectB;
                unitA.setContactingUnit(true);
            }
        }
    }

    /**
     * Called when two fixtures cease to touch
     * @param contact object containing information about contact
     */
    @Override
    public void endContact(Contact contact) {

        GameObject objectA = (GameObject) (contact.getFixtureA().getUserData());
        GameObject objectB = (GameObject) (contact.getFixtureB().getUserData());

        //objectA.setCollision(false);
        //objectB.setCollision(false);

        if (contact.getFixtureA().isSensor()){
            AnimatedActor actorA = (AnimatedActor) objectA;
            if (actorA.isEnemy() != objectB.isEnemy()){
                actorA.removeTarget(objectB);
            }
            if (objectB instanceof Unit){
                Unit unitB = (Unit) objectB;
                unitB.setContactingUnit(false);
            }
        }
        if (contact.getFixtureB().isSensor()){
            AnimatedActor actorB = (AnimatedActor) objectB;
            if (actorB.isEnemy() != objectA.isEnemy()){
                actorB.removeTarget(objectA);
            }
            if (objectA instanceof Unit){
                Unit unitA = (Unit) objectB;
                unitA.setContactingUnit(false);
            }
        }
    }

    /**
     * not used
     * @param contact
     * @param oldManifold
     */
    @Override
    public void preSolve(Contact contact, Manifold oldManifold) {

    }

    /**
     * not used
     * @param contact
     * @param impulse
     */
    @Override
    public void postSolve(Contact contact, ContactImpulse impulse) {

    }
}
