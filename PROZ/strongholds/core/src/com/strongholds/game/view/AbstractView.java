package com.strongholds.game.view;

import com.badlogic.gdx.graphics.g2d.BitmapFont;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.graphics.g2d.TextureAtlas;
import com.badlogic.gdx.scenes.scene2d.Stage;
import com.badlogic.gdx.scenes.scene2d.ui.Skin;
import com.badlogic.gdx.scenes.scene2d.ui.TextButton;
import com.badlogic.gdx.scenes.scene2d.utils.ClickListener;

/**
 * Abstract view
 */
public class AbstractView {
    /**
     * used to handle buttons and labels
     */
    protected Stage stage;

    protected BitmapFont font;
    protected TextButton.TextButtonStyle textButtonStyle;
    /**
     * defines the look of the UI
     */
    protected Skin skin;
    /**
     * libgdx structure for storing the UI
     */
    protected TextureAtlas buttonAtlas;

    /**
     * from libgdx docs "Draws batched quads using indices"
     */
    protected SpriteBatch spriteBatch;

    /**
     * screen width
     */
    protected float screenX;
    /**
     * screen height
     */
    protected float screenY;

    /**
     * Creates a button
     * @param x X position
     * @param y Y position
     * @param width width
     * @param height height
     * @param label label
     * @param clickListener action on button clicked
     * @return reference to newly created button
     */
    protected TextButton createButton(float x, float y, int width, int height, String label, ClickListener clickListener){
        TextButton newButton = new TextButton(label, textButtonStyle);
        newButton.setSize(width, height);
        newButton.setPosition(x, y);
        if (clickListener != null){
            newButton.addListener(clickListener);
        }
        return newButton;
    }

    /**
     * Called just before view starts to be active.
     * Initializes the view.
     */
    public void init(){

    }
}
