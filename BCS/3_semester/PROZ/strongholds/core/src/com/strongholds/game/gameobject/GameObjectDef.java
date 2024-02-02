package com.strongholds.game.gameobject;

import com.badlogic.gdx.physics.box2d.Body;
import com.strongholds.game.GameSingleton;

public class GameObjectDef {
    public Body body;
    public float width;
    public float height;
    public GameSingleton.ObjectType type;
    public String id;
    public boolean isEnemy;

    public GameObjectDef(Body body, float width, float height, GameSingleton.ObjectType type, String id, boolean isEnemy) {
        this.body = body;
        this.width = width;
        this.height = height;
        this.type = type;
        this.id = id;
        this.isEnemy = isEnemy;
    }

    public GameObjectDef(GameObjectDef gameObjectDef){
        this.body = gameObjectDef.body;
        this.width = gameObjectDef.width;
        this.height = gameObjectDef.height;
        this.type = gameObjectDef.type;
        this.id = gameObjectDef.id;
        this.isEnemy = gameObjectDef.isEnemy;
    }
}
