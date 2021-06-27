package com.strongholds.game.net;

/**
 * Interface for simple TCP server
 */
public interface INetworkController extends Runnable{
    /**
     * Starts the server
     */
    void run();

    /**
     *  Adds a new object request
     * @param object object to send
     */
    void addObjectRequest(Object object);

    /**
     * Registers a new listener. This listener will be then notified every time a new object arrived.
     * @param networkListener
     */
    void registerNetworkListener(NetworkListener networkListener);

    /**
     * Sets a port on which we want to receive requests.
     * @param port input port
     */
    void setInPort(int port);

    /**
     * Sets a port on which we want to send requests.
     * @param port output port
     */
    void setOutPort(int port);

    /**
     * Sets an ip address of the host we want to connect to.
     * @param ip - ip address
     */
    void setTargetIp(String ip);

    void setLastSyncedTime(double lastSyncedTime);

    /**
     * Tries to connect to the host.
     * @return True on success, False when there was an error or a timeout.
     */
    boolean connect();

    /**
     * Disposes server.
     * Should be called before server is destroyed.
     */
    void dispose();

    /**
     * Stops the server
     */
    void stop();

    /**
     * Resumes the server
     */
    void resume();
}
