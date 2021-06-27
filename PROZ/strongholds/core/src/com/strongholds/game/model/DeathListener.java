package com.strongholds.game.model;

/**
 * Interface for a listener waiting for the unit dying.
 */
public interface DeathListener {
    /**
     * Notifies the listener when the unit died.
     * @param unitId dead unit's id
     */
    void notifyDeadUnit(String unitId);
}
