package com.strongholds.game.exception;

/**
 * Custom exception thrown when network controller can't connect to the opponent
 */
public class CannotConnectException extends RuntimeException{
    /**
     * Creates a CannotConnectException with a message
     * @param message message
     */
    public CannotConnectException(String message){ super(message); }
}
