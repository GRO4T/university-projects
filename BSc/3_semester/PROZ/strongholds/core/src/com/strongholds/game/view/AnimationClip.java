package com.strongholds.game.view;

import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.Animation;
import com.badlogic.gdx.graphics.g2d.TextureRegion;

/**
 * Stores animation clip (e.g. unit idling)
 * and iterates through next frames.
 */
public class AnimationClip {
    /**
     * (libgdx's class)
     * Stores a list of objects representing an animated sequence, e.g. for running or jumping.
     * Each object in the Animation is called a key frame, and multiple key frames make up the animation.
     */
    Animation<TextureRegion> animation;
    /**
     * Time (since the last reset()) animation has been running. Is used to get current frame from animation object.
     */
    float stateTime;

     /**
     * @param spriteSheet      texture consisting of multiple smaller frames that make up the animation
     * @param spriteSheetCols   number of columns in the sprite sheet
     * @param spriteSheetRows   number of rows in the sprite sheet
     * @param frames            number of relevant frames in the sprite sheet (the ones that are not empty)
     * @param interval          time at which frames should be changed
     */
    public AnimationClip(Texture spriteSheet, int spriteSheetCols, int spriteSheetRows, int frames, float interval) {
        TextureRegion[][] spriteSheetIn2DArray = TextureRegion.split(spriteSheet, spriteSheet.getWidth() / spriteSheetCols,
                spriteSheet.getHeight() / spriteSheetRows);

        TextureRegion[] spriteSheetIn1DArray = new TextureRegion[frames];
        int index = 0;
        for (int i = 0; i < spriteSheetRows; i++) {
            for (int j = 0; j < spriteSheetCols; j++) {
                if (i * spriteSheetCols + j + 1 > frames) break;
                spriteSheetIn1DArray[index++] = spriteSheetIn2DArray[i][j];
            }
        }

        animation = new Animation<TextureRegion>(interval, spriteSheetIn1DArray);
        stateTime = 0.0f;
    }

    public AnimationClip(){}

    /**
     * Updates stateTime
     * @param deltaTime time that passed since the last frame
     */
    public void update(float deltaTime){
        stateTime += deltaTime;
    }

    /**
     * Resets stateTime
     */
    public void reset(){ stateTime = 0.0f; }

    /**
     * @return current animation frame
     */
    TextureRegion getCurrentFrame(){
        return animation.getKeyFrame(stateTime, true);
    }
}
