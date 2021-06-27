package com.strongholds.game.exception;

/**
 * Custom exception thrown when GameSingleton.ObjectType is not handled by function
 */
public class ObjectTypeNotDefinedException extends RuntimeException{
    /**
     * Creates ObjectTypeNotDefinedException with a message
     * @param message message
     */
    public ObjectTypeNotDefinedException(String message){
        super(message);
    }
}
