package com.strongholds.game.controller;

import com.strongholds.game.event.ModelEvent;

/** Interface for communication model->controller
 */
public interface IModelController {
    /**
     * Adds a new ModelEvent for the controller to handle
     * @param modelEvent event to handle
     */
    void addEvent(ModelEvent modelEvent);
}
