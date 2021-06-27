package com.strongholds.game.event;

/**
 * Event created when an error occurred and needs to be handled by controller.
 */
public class ErrorEvent{
    private boolean opponentDisconnected;

    /**
     * Returns whether opponent disconnected
     * @return true if opponent disconnected, false if not
     */
    public boolean isOpponentDisconnected() {
        return opponentDisconnected;
    }

    /**
     * Sets opponentDisconnected flag
     * @param opponentDisconnected whether opponent disconnected
     */
    public void setOpponentDisconnected(boolean opponentDisconnected) {
        this.opponentDisconnected = opponentDisconnected;
    }
}
