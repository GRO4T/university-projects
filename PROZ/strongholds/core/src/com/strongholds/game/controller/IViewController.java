package com.strongholds.game.controller;

import com.badlogic.gdx.assets.AssetManager;
import com.strongholds.game.event.ViewEvent;

/**
 * Interface for gameView->controller communication
 */
public interface IViewController {
    /**
     * Get a game screen width
     * @return screen width
     */
    int getScreenWidth();

    /**
     * Get a game screen height
     * @return screen height
     */
    int getScreenHeight();

    /**
     * Get application's asset manager
     * @return asset manager
     */
    AssetManager getAssetManager();

    /**
     * Add a new ViewEvent for the controller to handel
     * @param event event to handle
     */
    void addEvent(ViewEvent event);

    /**
     * Get current game message
     * @return message
     */
    String getMessage();

    /**
     * Get host's username
     * @return username
     */
    String getUsername();

    /**
     * Get opponent's username
     * @return username
     */
    String getOpponentUsername();
}
