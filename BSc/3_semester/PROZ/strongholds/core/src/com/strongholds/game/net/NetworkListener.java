package com.strongholds.game.net;

import com.strongholds.game.event.ErrorEvent;

import java.util.concurrent.LinkedBlockingQueue;

/**
 * Interface for listener waiting for objects being received from the network
 */
public interface NetworkListener {
    /**
     * Notifies listener that a new object has been received.
     * @param receivedObjects queue of the received objects
     */
    void notify(LinkedBlockingQueue<Object> receivedObjects);

    /**
     * Notifies listener when an error occurred. (e.g. host disconnected)
     * @param errorEvent event describing what the error is
     */
    void notifyOnError(ErrorEvent errorEvent);

    double getCurrentTime();
}
