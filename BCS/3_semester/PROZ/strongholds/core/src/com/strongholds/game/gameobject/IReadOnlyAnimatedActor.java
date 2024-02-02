package com.strongholds.game.gameobject;

import com.strongholds.game.GameSingleton.ObjectState;

/** Read-only interface for AnimatedActor
 *
 */
public interface IReadOnlyAnimatedActor extends IReadOnlyGameObject {
   /**
    * Returns actor current state
    * @return actor state
    */
   ObjectState getState();
}
