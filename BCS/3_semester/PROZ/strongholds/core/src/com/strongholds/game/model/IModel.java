package com.strongholds.game.model;

import com.badlogic.gdx.math.Vector2;
import com.strongholds.game.GameSingleton.ObjectType;
import com.strongholds.game.gameobject.GameObject;
import com.strongholds.game.gameobject.unit.IUnit;

import java.util.Map;

/** Model interface for Controller
 */
public interface IModel extends IReadOnlyModel{
    /**
     * Updates model simulation by a given time step
     * @param timeStep time step (in milliseconds)
     */
    void update(float timeStep);

    /**
     * Called when model is destroyed
     */
    void dispose();

    /**
     * Creates an object
     * @param id object id
     * @param objectType object type
     * @param position object position (in pixels)
     * @param size object size (in pixels)
     * @param isEnemy whether object is on enemy side
     */
    void createObject(String id, ObjectType objectType, Vector2 position, Vector2 size, boolean isEnemy);

    /**
     * Creates an unit
     * @param id unit id
     * @param objectType unit type
     * @param position unit position (in pixels)
     * @param size unit size (in pixels)
     * @param isEnemy whether unit is an enemy
     */
    void createUnit(String id, ObjectType objectType, Vector2 position, Vector2 size, boolean isEnemy);

    /**
     * Adds money
     * @param value money to be added
     */
    void addMoney(long value);

    /**
     * Called when controller has been notified via network that a unit has been hit
     * @param id unit id
     * @param damage damage dealt to the unit
     */
    void unitHit(String id, int damage);

    Map<String, IUnit> getActorMap();
    Map<String, GameObject> getGameObjectMap();

    void updateActorMap(Map<String, IUnit> actorMap);
    void updateGameObjectMap(Map<String, GameObject> gameObjectMap);

    void catchUp(float deltaTime);
}
