package com.strongholds.game.net;

import com.strongholds.game.event.ErrorEvent;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.LinkedBlockingQueue;

/**
 * Simple Tcp server.
 */
public class TcpServer implements INetworkController{
    private ServerSocket opponentClientSocket;
    private LinkedBlockingQueue<Object> objectsToSend;
    private LinkedBlockingQueue<Object> receivedObjects;
    private NetworkListener networkListener;

    private int inPort = 46004;
    private int outPort = 46004;
    private String ip = "127.0.0.1";

    private boolean running = false;

    private final int connectionWaitTimeInMillis = 2000;
    private final int connectionWaitTimeInNanos = connectionWaitTimeInMillis * 1000000;

    private double lastSyncedTime; // time when the game was last sync

    public TcpServer(){
        objectsToSend = new LinkedBlockingQueue<>();
        receivedObjects = new LinkedBlockingQueue<>();
    }

    @Override
    public void run() {
        Thread senderThread = new Thread(new ObjectSender());
        senderThread.start();
        Thread receiverThread = new Thread(new ObjectReceiver());
        receiverThread.start();

        while(true){
            if (receivedObjects.size() > 0)
                networkListener.notify(receivedObjects);

            double currentTime = networkListener.getCurrentTime();
            if (currentTime > lastSyncedTime + 1.0f){
                lastSyncedTime = currentTime;
                //objectsToSend.add(networkListener.sync());
                //objectsToSend.add(new SyncEvent(currentTime));
            }
        }
    }

    private class ObjectSender implements Runnable{
        @Override
        public void run() {
            while(running){
                if (objectsToSend.size() > 0){
                    Object objToSend = objectsToSend.poll();
                    ObjectOutputStream out;
                    Socket s;
                    try {
                        s = opponentClientSocket.accept();
                        out = new ObjectOutputStream(s.getOutputStream());
                        out.writeObject(objToSend);
                        out.flush();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
    }

    private class ObjectReceiver implements Runnable{
        @Override
        public void run(){
            while(running){
                Socket s;
                ObjectInputStream in;
                Object receivedObj = null;
                try {
                    s = new Socket(ip, inPort);
                    in = new ObjectInputStream(s.getInputStream());
                    receivedObj = in.readObject();
                    s.close();
                } catch (IOException e) {
                    ErrorEvent opponentDisconnected = new ErrorEvent();
                    opponentDisconnected.setOpponentDisconnected(true);
                    networkListener.notifyOnError(opponentDisconnected);
                }
                catch (ClassNotFoundException e){
                    e.printStackTrace();
                }
                if (receivedObj != null)
                    receivedObjects.add(receivedObj);
            }
        }
    }

    public void addObjectRequest(Object object) {
        objectsToSend.add(object);
    }

    public void setInPort(int port){
        inPort = port;
    }

    public void setOutPort(int port){
        outPort = port;
    }

    public void setTargetIp(String ip) {
        this.ip = ip;
    }

    public void dispose(){
        if (opponentClientSocket != null) {
            try {
                opponentClientSocket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public void stop(){
        running = false;
    }

    public void resume(){
        running = true;
    }

    public void registerNetworkListener(NetworkListener networkListener) {
        this.networkListener = networkListener;
    }

    public void setLastSyncedTime(double lastSyncedTime) {
        this.lastSyncedTime = lastSyncedTime;
    }

    public boolean connect(){
        try {
            if (opponentClientSocket != null)
                opponentClientSocket.close();
            opponentClientSocket = new ServerSocket(outPort);
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }

        ConnectionEstablisher connectionEstablisher = new ConnectionEstablisher();
        Thread connectionEstablishingThread = new Thread(connectionEstablisher);
        connectionEstablishingThread.start();

        try{
            Thread.sleep(connectionWaitTimeInMillis);
        }
        catch(InterruptedException e){
            e.printStackTrace();
        }

        connectionEstablishingThread.interrupt();
        connectionEstablisher.stop();
        return connectionEstablisher.isConnected();
    }

    /**
     * Tries to establish the connection between the players
     */
    private class ConnectionEstablisher implements Runnable{
        /**
         * flag telling whether connection has been established
         */
        private volatile boolean connected = false;
        private final ExecutorService threadPool;
        private final int poolSize = 2;

        /**
         * Default constructor.
         * Initializes thread pool
         */
        public ConnectionEstablisher(){
            threadPool = Executors.newFixedThreadPool(poolSize);
        }

        @Override
        public void run() {
            connect();
        }

        /**
         * Tries to connect the players.
         * Spawns two threads.
         * One trying to send a stream of bytes.
         * Second listening on input port trying to catch the stream of bytes.
         * When bytes are received, connected is set to true and threads return.
         * In a case when hosts cannot connect
         * Threads finish their work after exceeding connectionWaitTimeInMilliseconds.
         */
        private void connect(){
            threadPool.execute(new Runnable() {
                Socket sender = null;
                @Override
                public void run() {
                    try{
                        sender = opponentClientSocket.accept();
                        DataOutputStream outputStream = new DataOutputStream(sender.getOutputStream());
                        outputStream.writeBytes("hello");
                        outputStream.flush();
                    } catch (IOException e) {
                        e.printStackTrace();
                        System.out.println("sender error");
                        return;
                    } catch (Exception e){
                        e.printStackTrace();
                        return;
                    }
                }
            });


            threadPool.execute(new Runnable() {
                @Override
                public void run() {
                    float listeningTime = 0.0f;
                    float startTime;
                    float endTime;

                    while(listeningTime < connectionWaitTimeInNanos){
                        startTime = System.nanoTime();

                        Socket receiver = null;
                        try{
                            receiver = new Socket(ip, inPort);
                            receiver.setSoTimeout(10000);
                            DataInputStream inputStream = new DataInputStream(receiver.getInputStream());
                            //String bytes = new String(inputStream.readNBytes(5));
                            byte[] bytes = new byte[6];
                            inputStream.read(bytes, 0, 5);
                            String message = new String(bytes);
                            System.out.println("received bytes = " + message);
                            connected = true;
                            receiver.close();
                            return;
                        }
                        catch(IOException e){
                            e.printStackTrace();
                            System.out.println("receiver error");
                            if (receiver != null) {
                                try {
                                    receiver.close();
                                } catch (IOException ex) {
                                    ex.printStackTrace();
                                }
                            }
                        }
                        finally{
                            endTime = System.nanoTime();
                            listeningTime += endTime - startTime;
                        }

                    }
                }
            });
        }

        public boolean isConnected(){
                                       return connected;
                                                        }

        public void stop(){
                         threadPool.shutdownNow();
                }
    }


}
