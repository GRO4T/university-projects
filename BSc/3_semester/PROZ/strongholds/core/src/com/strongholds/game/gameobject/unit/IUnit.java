package com.strongholds.game.gameobject.unit;

import com.badlogic.gdx.math.Vector2;
import com.strongholds.game.gameobject.IAnimatedActor;
import com.strongholds.game.gameobject.IReadOnlyUnit;
import com.strongholds.game.model.DeathListener;

/**
 * Interface for Unit
 */
public interface IUnit extends IReadOnlyUnit, IAnimatedActor {
    /**
     * Called when unit is about to perform an attack
     * Changes unit state and waits X seconds to deal damage
     */
    void attack();

    /**
     * Moves a unit in a specified direction
     * @param direction direction to move in
     */
    void move(Vector2 direction);

    /**
     * Updates unit state
     */
    void update();

    /**
     * Called before unit is destroyed
     */
    void dispose();

    /**
     * Sets object waiting for this unit's health to go below 0
     * @param deathListener object implementing DeathListener interface
     */
    void setDeathListener(DeathListener deathListener);
}
