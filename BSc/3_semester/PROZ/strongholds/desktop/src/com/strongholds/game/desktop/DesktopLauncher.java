package com.strongholds.game.desktop;

import com.badlogic.gdx.backends.lwjgl.LwjglApplication;
import com.badlogic.gdx.backends.lwjgl.LwjglApplicationConfiguration;
import com.strongholds.game.controller.StrongholdsGame;

public class DesktopLauncher {
    static final private int screenWidth = 1200;
    static final private int screenHeight = 600;

    public static void main (String[] arg) {
        LwjglApplicationConfiguration config = new LwjglApplicationConfiguration();
        config.width = screenWidth;
        config.height = screenHeight;
        new LwjglApplication(new StrongholdsGame(screenWidth, screenHeight), config);
    }
}
