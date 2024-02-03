package com.strongholds.game.controller;

/**
 * Interface for menu->controller communication
 */
public interface IMenuController {
    /**
     * Tells controller to start a game
     */
    void startGame();

    /**
     * Tells controller to try to connect to a opponent
     * @return true if connection succeded, false on fauilure
     */
    boolean connect();

    /**
     * Tells controller to set target ip address
     * @param ip value of the ip address
     */
    void setIp(String ip);

    /**
     * Tells controller to set input port
     * @param port value of the input port
     */
    void setInPort(int port);

    /**
     * Tells controller to set output port
     * @param port value of the output port
     */
    void setOutPort(int port);

    /**
     * Tells controller to set username
     * @param username username
     */
    void setUsername(String username);
}
