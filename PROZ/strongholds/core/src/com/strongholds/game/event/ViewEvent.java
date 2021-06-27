package com.strongholds.game.event;

import com.strongholds.game.GameSingleton;

import java.io.Serializable;

/**
 * Event created when something happend in View and needs to handled by controller
 */
public class ViewEvent implements Serializable {
    /**
     * flag saying whether player wanted to train a unit
     */
    private boolean trainUnit = false;
    /**
     * type of the unit to train
     */
    private GameSingleton.ObjectType unitType = GameSingleton.ObjectType.SWORDSMAN;
    /**
     * unit id
     */
    private String unitId = "";
    /**
     * flag saying whether unit is an enemy
     */
    private boolean isEnemy = false;

    /**
     * flag saying whether an action is to set a username
     */
    private boolean setUsername = false;
    /**
     * new username
     */
    private String username = "";

    /**
     * flag saying whether game has been paused
     */
    private boolean togglePaused = false;

    /**
     * flag saying whether game has been restarted
     */
    private boolean restart = false;

    /**
     * flag saying whether game has been started
     */
    private boolean start = false;

    /**
     * flag saying whether event came from network
     */
    private boolean fromNetwork = false;

    /**
     * Default constructor
     */
    public ViewEvent(){}

    /**
     * Constructor creating event saying: train unit of type X
     * @param unitType type of the unit
     */
    public ViewEvent(GameSingleton.ObjectType unitType) {
        this.trainUnit = true;
        this.unitType = unitType;
    }

    /**
     * Creates event saying: set username to X
     * @param username
     */
    public ViewEvent(String username){
        this.setUsername = true;
        this.username = username;
    }

    /**
     * Creates event saying: toggle game paused
     * @param togglePaused
     */
    public ViewEvent(boolean togglePaused){
        this.togglePaused = togglePaused;
    }

    /**
     * Sets unit id to be trained
     * @param unitId unit id
     */
    public void setUnitId(String unitId) {
        this.unitId = unitId;
    }

    /**
     * Sets whether unit is an enemy
     * @param enemy if unit enemy
     */
    public void setEnemy(boolean enemy) {
        isEnemy = enemy;
    }

    /**
     * Returns whether unit has to be trained
     * @return true if unit has to be trained, false if not
     */
    public boolean toTrainUnit(){ return trainUnit; }

    /**
     * Returns unit type
     * @return unit type
     */
    public GameSingleton.ObjectType getUnitType(){ return unitType; }

    /**
     * Returns unit id
     * @return unit id
     */
    public String getUnitId(){ return unitId; }

    /**
     * Returns whether unit is an enemy
     * @return true if is an enemy, false if not
     */
    public boolean getIsEnemy(){ return isEnemy; }

    /**
     * Returns whether username has to be set
     * @return true if username has to be set, false if not
     */
    public boolean isSetUsername() {
        return setUsername;
    }

    /**
     * Returns new username
     * @return new username
     */
    public String getUsername() {
        return username;
    }

    /**
     * Returns whether paused has to be toggled
     * @return true if pause has to be toggled, false if not
     */
    public boolean isTogglePaused() {
        return togglePaused;
    }

    /**
     * Returns whether game has to be restarted
     * @return true if game has to be restarted, false if not
     */
    public boolean isRestart() {
        return restart;
    }

    /**
     * Sets game to be restarted
     */
    public void setRestart() {
        restart = true;
    }

    /**
     * Returns whether game has to be started
     * @return true if game has to be started, false if not
     */
    public boolean isStart() {
        return start;
    }

    /**
     * Sets game to be started
     */
    public void setStart() {
        start = true;
    }

    /**
     * Returns whether event is from network
     * @return true if is from network, false if not
     */
    public boolean isFromNetwork(){
        return fromNetwork;
    }

    /**
     * Sets event isFromNetwork to true
     */
    public void setFromNetwork(){
        fromNetwork = true;
    }
}
