package com.strongholds.game.gameobject;

/**
 * Interface for GameObject
 */
public interface IGameObject extends IReadOnlyGameObject{
    /**
     * Sets object's id
     * @param id id
     */
    void setId(String id);

    /**
     * Called when object has been hit
     * @param damage damage dealt to the object
     */
    void gotHit(int damage);
}
