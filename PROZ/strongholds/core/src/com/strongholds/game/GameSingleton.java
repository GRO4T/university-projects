package com.strongholds.game;

import com.badlogic.gdx.physics.box2d.World;

import java.util.HashMap;

/**
 * game singleton
 */
public class GameSingleton {
    private static volatile GameSingleton INSTANCE;

    /**
     * game scale
     */
    private final float pixels_per_meter = 16.0f;

    /**
     * array of game textures filenames
     */
    private String textureFilenames[] = {
            "platform.png", "base.png", "background-textures.png",
            "Knight/idle.png", "Knight/move.png", "Knight/attack.png",
            "Archer/idle.png", "Archer/move.png", "Archer/attack.png"};

    /**
     * map of actor texture infos
     */
    private HashMap<ObjectType, TextureInfo[]> actorsTextureInfo;

    /**
     * filename of the menu background texture
     */
    public final String menuBackgroundTexture = "background-textures.png";

    /**
     * map of unit costs
     */
    private HashMap<ObjectType, Long> costLedger;

    /**
     * box2d world
     */
    private World world;

    /**
     * object types
     */
    public enum ObjectType{
        PLATFORM, BACKGROUND_IMAGE, BASE, SWORDSMAN, ARCHER;
    }

    /**
     * object possible states
     */
    public enum ObjectState{
        IDLING, MOVING, ATTACKING;
    }

    /**
     * structure containing texture info needed to create animation clip
     */
    public class TextureInfo{
        public String filename;
        public int cols; // cols of the spriteSheet
        public int rows; // rows of the spriteSheet
        public int filledFrames; // non empty frames of the Spritesheet
        public float interval;

        public TextureInfo(String filename, int cols, int rows, int filledFrames, float interval) {
            this.filename = filename;
            this.rows = rows;
            this.cols = cols;
            this.filledFrames = filledFrames;
            this.interval = interval;
        }
    }

    /**
     * used for collision filtering
     */
    public static final short GAME_OBJECT_COLLISION_MASK = 0x0001;
    /**
     * used for collision filtering
     */
    public static final short ACTOR_COLLISION_MASK = 0x0002;
    /**
     * used for collision filtering
     */
    public static final short SENSOR_COLLISION_MASK = 0x0004;
    /**
     * used for collision filtering
     */
    public static final short BASE_COLLISION_MASK = 0x0008;

    /**
     * Returns game scale
     * @return pixels per meter
     */
    public float getPixels_per_meter(){ return pixels_per_meter; }

    /**
     * Returns an array of texture filenames
     * @return array of texture filenames
     */
    public String[] getTextureFilenames(){ return textureFilenames; }

    /**
     * Creates a new game singleton
     */
    private GameSingleton(){
        actorsTextureInfo = new HashMap<>();
        TextureInfo textureInfoKnight[] = {
                new TextureInfo("Knight/idle.png", 4, 1, 4, 0.1f),
                new TextureInfo("Knight/move.png", 8, 1, 8, 0.1f),
                new TextureInfo("Knight/attack.png", 10, 1, 10, 0.1f),
            };
        actorsTextureInfo.put(ObjectType.SWORDSMAN, textureInfoKnight);

        TextureInfo textureInfoArcher[] = {
                new TextureInfo("Archer/idle.png", 8, 1, 4, 0.1f),
                new TextureInfo("Archer/move.png", 8, 1, 8, 0.1f),
                new TextureInfo("Archer/attack.png", 14, 1, 10, 0.1f),
        };
        actorsTextureInfo.put(ObjectType.ARCHER, textureInfoArcher);

        costLedger = new HashMap<>();
        costLedger.put(ObjectType.SWORDSMAN, 100L);
        costLedger.put(ObjectType.ARCHER, 200L);
    }

    /**
     * Returns an intance of the singleton
     * @return game singleton
     */
    public static GameSingleton getGameSingleton(){
        if (INSTANCE == null){
            INSTANCE = new GameSingleton();
        }
        return INSTANCE;
    }

    /**
     * Returns texture info of an actor
     * @param objectType type of an actor
     * @return texture info
     */
    public TextureInfo[] getActorTextureInfo(ObjectType objectType){
        return actorsTextureInfo.get(objectType);
    }

    /**
     * Converts objectType(int) to String
     * @param objectType object type
     * @return object type as String
     */
    public static String toString(ObjectType objectType){
        if (objectType == ObjectType.SWORDSMAN)
            return "ObjectType.SWORDSMAN";
        return "toString not defined for this objectType";
    }

    /**
     * Returns cost of the unit
     * @param objectType unit type
     * @return cost
     */
    public long getCost(ObjectType objectType){
        return costLedger.get(objectType).longValue();
    }

    /**
     * Returns world
     * @return world
     */
    public World getWorld() {
        return world;
    }

    /**
     * Sets world
     * @param world
     */
    public void setWorld(World world) {
        this.world = world;
    }

    /* HELPER METHODS */
    public static float clamp(float val, float min, float max) {
        return Math.max(min, Math.min(max, val));
    }
}
