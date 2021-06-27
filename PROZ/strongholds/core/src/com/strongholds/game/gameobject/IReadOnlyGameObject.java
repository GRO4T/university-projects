package com.strongholds.game.gameobject;

import com.badlogic.gdx.math.Vector2;
import com.strongholds.game.GameSingleton;

/**
 * Read-only interface for GameObject
 */
public interface IReadOnlyGameObject {
    /**
     * Returns current object body position (in meters)
     * @return
     */
    Vector2 getPosition();

    /**
     * Returns current object body velocity (in meters per second)
     * @return
     */
    Vector2 getVelocity();

    /**
     * Returns whether object is on enemy side
     * @return true if object is on enemy side, false if not
     */
    boolean isEnemy();

    /**
     * Returns object width (in meters)
     * @return object width (width of the first fixture)
     */
    float getWidth();

    /**
     * Returns object height (in meters)
     * @return object height (height of the first fixture)
     */
    float getHeight();

    /**
     * Returns object type
     * @return object type
     */
    GameSingleton.ObjectType getType();

    /**
     * Returns object id
     * @return id
     */
    String getId();

    /**
     * Returns object current health
     * @return current health
     */
    int getHealth();

    /**
     * Returns object max health
     * @return max health
     */
    int getMaxHealth();
}
