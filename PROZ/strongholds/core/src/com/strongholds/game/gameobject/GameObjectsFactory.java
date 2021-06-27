package com.strongholds.game.gameobject;


import com.badlogic.gdx.math.Vector2;
import com.badlogic.gdx.physics.box2d.BodyDef;
import com.badlogic.gdx.physics.box2d.World;
import com.strongholds.game.GameSingleton;
import com.strongholds.game.GameSingleton.ObjectType;

import static com.strongholds.game.GameSingleton.getGameSingleton;
import com.strongholds.game.GameSingleton.ObjectState;
import com.strongholds.game.exception.ObjectTypeNotDefinedException;
import com.strongholds.game.gameobject.unit.MeleeUnit;
import com.strongholds.game.gameobject.unit.RangedUnit;
import com.strongholds.game.gameobject.unit.Unit;
import com.strongholds.game.gameobject.unit.UnitDef;

/**
 * Factory for creating game object
 */
public class GameObjectsFactory {
    /**
     * reference to an instance of box2d world
     */
    World world;
    /**
     * reference to an instance of GameSingleton
     */
    GameSingleton gameSingleton;

    private final UnitDef swordsmanDef = new UnitDef(2000.0f, 10.0f, 700,
            1000, 10, 1.5f, 100);
    private final UnitDef archerDef = new UnitDef(2500.0f, 15.0f, 1000,
            1000, 20, 10.0f, 80);

    /**
     * Initializes the factory
     * @param world box2d world
     */
    public GameObjectsFactory(World world) {
        this.world = world;
        gameSingleton = getGameSingleton();
    }

    /**
     * creates a new object
     * @param id object id
     * @param objectType object type
     * @param position object position (in pixels)
     * @param size object size (in pixels)
     * @param isEnemy whether object is on enemy side
     * @return created object of the right type
     */
    public GameObject createObject(String id, ObjectType objectType, Vector2 position, Vector2 size, boolean isEnemy){
        float pixels_per_meter = gameSingleton.getPixels_per_meter();
        Vector2 bodySize = new Vector2(size.x / (2*pixels_per_meter), size.y / (2*pixels_per_meter));
        Vector2 bodyPos = new Vector2(position.x / pixels_per_meter + bodySize.x,
                position.y / pixels_per_meter + bodySize.y);

        BodyDef bodyDef = new BodyDef();
        bodyDef.position.set(bodyPos.x, bodyPos.y);

        GameObjectDef gameObjectDef = new GameObjectDef(null, bodySize.x, bodySize.y, objectType, id, isEnemy);

        if (objectType == ObjectType.PLATFORM || objectType == ObjectType.BASE){
            bodyDef.type = BodyDef.BodyType.StaticBody;
            gameObjectDef.body = world.createBody(bodyDef);
            return new GameObject(gameObjectDef);
        }
        else if (objectType == ObjectType.SWORDSMAN || objectType == ObjectType.ARCHER){
            bodyDef.type = BodyDef.BodyType.DynamicBody;
            gameObjectDef.body = world.createBody(bodyDef);

            Unit unit = null;
            if (objectType == ObjectType.SWORDSMAN){
                unit = new MeleeUnit(gameObjectDef, swordsmanDef);
            }
            else if (objectType == ObjectType.ARCHER){
                unit = new RangedUnit(gameObjectDef, archerDef);
            }
            unit.setState(ObjectState.IDLING);
            return unit;
        }
        throw new ObjectTypeNotDefinedException("ObjectType not handled by GameObjectsFactory: ObjectType = " +
                gameSingleton.toString(objectType));
    }
}
