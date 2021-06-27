package com.strongholds.game.view;

import com.badlogic.gdx.graphics.g2d.TextureRegion;
import com.strongholds.game.GameSingleton.ObjectState;

import java.util.HashMap;
import java.util.Map;

/**
 * Manages actor's animation
 */
public class Animator {
    Map<ObjectState, AnimationClip> clips;
    private ObjectState prevState;

    /**
     * @param idle AnimationClip for idle
     * @param move AnimationClip for move
     * @param attack AnimationClip for attack
     */
    public Animator(AnimationClip idle, AnimationClip move, AnimationClip attack) {
        prevState = ObjectState.IDLING;
        clips = new HashMap<>();
        clips.put(ObjectState.IDLING, idle);
        clips.put(ObjectState.MOVING, move);
        clips.put(ObjectState.ATTACKING, attack);
    }

    /**
     * Updates current clip's state
     * or resets it and changes to another if there was a state change.
     * @param state
     * @param deltaTime
     */
    public void update(ObjectState state, float deltaTime){
        AnimationClip clip = clips.get(state);
        clip.update(deltaTime);
        if (state != prevState){
            clip.reset();
        }
        prevState = state;
    }

    /**
     * Calls Animation.getCurrentFrame() for current clip.
     * @return current frame to draw
     */
    public TextureRegion getCurrentFrame(){
        return clips.get(prevState).getCurrentFrame();
    }
}
