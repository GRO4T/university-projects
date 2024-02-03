package com.strongholds.game.gameobject;

import com.strongholds.game.GameSingleton;

/** Interface for AnimatedActor
 */
public interface IAnimatedActor extends IReadOnlyAnimatedActor, IGameObject{
    /**
     * Sets actor current state
     * @param newState new state
     */
    void setState(GameSingleton.ObjectState newState);
}
