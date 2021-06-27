package com.strongholds.game.model;

import com.strongholds.game.gameobject.IReadOnlyAnimatedActor;
import com.strongholds.game.gameobject.IReadOnlyGameObject;

/**
 * Model interface for View
 */
public interface IReadOnlyModel {
    /**
     * Returns array of all non-animated game objects
     * @return an array of game objects
     */
    Object[] getGameObjectArray();

    /**
     * Returns array of all actors
     * @return an array of actors
     */
    Object[] getActorArray();

    /**
     * Returns an actor with given id
     * @param id actor id
     * @return actor
     */
    IReadOnlyAnimatedActor getActor(String id);

    /**
     * Returns a game object with given id
     * @param id game object id
     * @return game object
     */
    IReadOnlyGameObject getGameObject(String id);

    /**
     * Returns player's money
     * @return money
     */
    long getMoney();

    /**
     * Returns friendly base health
     * @return base health
     */
    int getBaseHealth();

    /**
     * Returns enemy base health
     * @return enemy base health
     */
    int getEnemyBaseHealth();
}
