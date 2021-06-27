package com.strongholds.game.event;

import java.io.Serializable;

/**
 * Event created when something happens and needs to be handled by controller.
 */
public class ModelEvent implements Serializable {
    /**
     * flag telling if unit has been hit
     */
    private boolean unitHit;
    /**
     * id of the unit being hit
     */
    private String unitId;
    /**
     * damage to this unit
     */
    private int damage;

    /**
     * Constructor creating ModelEvent saying unit with id=X has been hit for Y
     * @param unitId unit id
     * @param damage damage to this unit
     */
    public ModelEvent(String unitId, int damage) {
        this.unitHit = true;
        this.unitId = unitId;
        this.damage = damage;
    }

    /**
     * @return true if unit has been hit, false if not
     */
    public boolean isUnitHit() {
        return unitHit;
    }

    /**
     * @return unit id
     */
    public String getUnitId() {
        return unitId;
    }

    /**
     * @return damage to the unit
     */
    public int getDamage() {
        return damage;
    }
}
