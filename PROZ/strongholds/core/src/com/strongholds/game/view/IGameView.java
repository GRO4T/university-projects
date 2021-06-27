package com.strongholds.game.view;

import com.badlogic.gdx.math.Vector2;
import com.strongholds.game.GameSingleton.ObjectType;

/**
 * Interface for game screen
 */
public interface IGameView {
    /**
     * Updates the view
     * @param deltaTime time since the last update
     */
    void update(float deltaTime);

    /**
     * Draws the view
     */
    void draw();

    /**
     * Loads static textures
     */
    void loadTextures();

    /**
     * Loads animated sprites
     * @param id actor id
     * @param objectType actor type
     */
    void loadActorSprites(String id, ObjectType objectType);

    /**
     * Returns static texture size
     * @param objectType type of the object whose texture size will be returned
     * @return texture size (in pixels)
     */
    Vector2 getTextureSize(ObjectType objectType);

    /**
     * Returns animated actor texture size.
     * It's the size of the first frame of the first clip
     * @param id actor id
     * @return texture size (in pixels)
     */
    Vector2 getTextureSize(String id);

    void init();

    /**
     * Called when the game is finished.
     */
    void gameFinished();
}
