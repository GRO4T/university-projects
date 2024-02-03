package com.strongholds.game.controller;

import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.assets.AssetManager;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.math.Vector2;

import com.strongholds.game.GameSingleton;
import com.strongholds.game.GameSingleton.ObjectType;
import com.strongholds.game.event.ErrorEvent;
import com.strongholds.game.event.ModelEvent;
import com.strongholds.game.event.ViewEvent;
import com.strongholds.game.model.IModel;
import com.strongholds.game.model.Model;
import com.strongholds.game.net.INetworkController;
import com.strongholds.game.net.NetworkListener;
import com.strongholds.game.net.TcpServer;
import com.strongholds.game.view.IGameView;
import com.strongholds.game.view.GameView;
import com.strongholds.game.view.IMenuView;
import com.strongholds.game.view.MenuView;

import java.util.Random;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.LinkedBlockingQueue;

/**
 * game controller
 */
public class StrongholdsGame extends ApplicationAdapter implements IViewController, IModelController, IMenuController, NetworkListener {
	private GameSingleton gameSingleton;
	private AssetManager assetManager;

	private final float Fps = 60.0f;

	private boolean catchingUp;
	private double gameTime; // how long this game has been running
	private double opponentGameTime;
	private final double simulationDisparityTolerance = 0.1f;

	private int screenWidth;
	private int screenHeight;

	private boolean gameStarted;
	private boolean opponentGameStarted;

	private boolean gameRunning = true;

	private int nextObjectId = 0;
	private int playerId;

	private IModel model;
	private IGameView gameView;
	private IMenuView menuView;

	/**
	 * current game message displayed on screen
	 * It is also used to display game errors.
	 */
	private String message = "";
	/**
	 * used to clear message after some time
	 */
	private Timer clearMessageTimer;

	private LinkedBlockingQueue<ViewEvent> queueOfViewEvents;
	private LinkedBlockingQueue<ModelEvent> queueOfModelEvents;

	private INetworkController networkController;
	private Thread networkThread;

	private String username = "";
	private String opponentUsername = "";

	private Vector2 friendlyBaseSpawnPoint;
	private Vector2 enemyBaseSpawnPoint;
	private Vector2 friendliesSpawnPoint;
	private Vector2 enemiesSpawnPoint;

	public StrongholdsGame(int screenWidth, int screenHeight) {
		gameSingleton = GameSingleton.getGameSingleton();
		this.screenWidth = screenWidth;
		this.screenHeight = screenHeight;
		clearMessageTimer = new Timer(true);
	}

	/**
	 * Called when the Application is first started
	 */
	@Override
	public void create () {
		queueOfViewEvents = new LinkedBlockingQueue<>();
		queueOfModelEvents = new LinkedBlockingQueue<>();

		assetManager = new AssetManager();
		loadAssets();

		menuView = new MenuView(this, assetManager, screenWidth, screenHeight);

		networkController = new TcpServer();
		networkController.registerNetworkListener(this);

		menuView.init();

		//test
		//connect();
		//startGame();
	}

	/**
	 * Initializes a new game
	 * Called every time player clicks start game in the menu.
	 */
	private void initGame(){
		gameTime = 0.0f;
		opponentGameTime = 0.0f;
		gameRunning = false;
		gameStarted = false;
		catchingUp = false;
		opponentGameStarted = false;
		message = "";
		model = new Model(this);
		gameView = new GameView(model, this);
		gameView.loadTextures();
		createMap();
	}

	private void createMap(){
		friendlyBaseSpawnPoint = new Vector2(0, 60);
		enemyBaseSpawnPoint = new Vector2(1072, 60);
		friendliesSpawnPoint = new Vector2(friendlyBaseSpawnPoint);
		friendliesSpawnPoint.add(new Vector2(gameView.getTextureSize(ObjectType.BASE).x - 60,0));
		enemiesSpawnPoint = new Vector2((enemyBaseSpawnPoint));
		//enemiesSpawnPoint.add(new Vector2(20, 0));
		createObject("base", ObjectType.BASE, friendlyBaseSpawnPoint, false);
		createObject("enemyBase", ObjectType.BASE, enemyBaseSpawnPoint, true);
		createObject(ObjectType.PLATFORM, new Vector2(0, 0), false);
	}

	/**
	 * Called when the Application should render itself
	 */
	@Override
	public void render () {
		if (!gameStarted){
			menuView.draw();
			return;
		}

		if (gameRunning){
			float deltaTime = Gdx.graphics.getDeltaTime();
			float timeStep = 1.0f / Fps;
			if (deltaTime > 2 * timeStep){
				model.catchUp(deltaTime);
				gameTime += deltaTime;
				while (deltaTime > timeStep){
					deltaTime -= timeStep;
					model.update(timeStep);
				}
				return;
			}

			earlyUpdate();
			gameView.update(deltaTime);
			update();
			model.update(timeStep);
			gameTime += deltaTime;

			if (catchingUp && gameTime - simulationDisparityTolerance > opponentGameTime){
				catchingUp = false;
				ViewEvent pauseEvent = new ViewEvent(true);
				pauseEvent.setFromNetwork();
				networkController.addObjectRequest(pauseEvent);
			}
		}
		else{
			handleViewEvents();
			setMessageAndClearAfterTime(getMessage());
		}

		gameView.draw();
	}

	/**
	 * Called when the Application is destroyed
	 */
	@Override
	public void dispose () {
		if (model != null)
			model.dispose();
		assetManager.dispose();
		if (networkThread != null)
			networkThread.interrupt();
		networkController.dispose();
		menuView.dispose();
	}

	/**
	 * Handles changes made by model.update() in previous frame.
	 * Called before gameView.update().
	 */
	private void earlyUpdate(){
		if (model.getEnemyBaseHealth() <= 0){
			gameRunning = false;
			message = username + " WON!";
			gameView.gameFinished();
		}
		else if (model.getBaseHealth() <= 0){
			gameRunning = false;
			message = opponentUsername + " WON!";
			gameView.gameFinished();
		}
	}

	/**
	 * Handles events that came from views and network controller.
	 * Called after gameView.update().
	 */
	private void update(){
		handleViewEvents();
		handleModelEvents();
	}

	/**
	 * Handles ViewEvents
	 */
	private void handleViewEvents(){
		ViewEvent viewEvent;
		while (queueOfViewEvents.size() > 0){
			viewEvent = queueOfViewEvents.poll();
			if (opponentGameStarted){
				if (viewEvent.toTrainUnit() && gameRunning){
					handleUnitTraining(viewEvent);
				}
				if (viewEvent.isTogglePaused()){
					if (!viewEvent.isFromNetwork()){
						viewEvent.setFromNetwork();
						networkController.addObjectRequest(viewEvent);
					}
					if (gameRunning){
						gameRunning = false;
						message = "GAME PAUSED";
					}
					else{
						gameRunning = true;
						message = "";
					}
				}
				if (viewEvent.isRestart()){
					gameStarted = false;
					networkController.stop();
					menuView.init();
				}
			}
			else{
				if (viewEvent.isSetUsername() && viewEvent.isFromNetwork()){
					setOpponentUsername(viewEvent.getUsername());
				}
				if (viewEvent.isStart() && viewEvent.isFromNetwork()){
					gameRunning = true;
					gameStarted = true;
					opponentGameStarted = true;
					message = "";
					networkController.setLastSyncedTime(0.0f);
				}
			}
		}
	}

	/**
	 * Helper method. Handles ViewEvents considering unit training.
	 * @param viewEvent viewEvent with trainUnit set to true
	 */
	private void handleUnitTraining(ViewEvent viewEvent){
		ObjectType unitType = viewEvent.getUnitType();

		boolean isEnemy = viewEvent.getIsEnemy();
		if (isEnemy){
			createUnit(viewEvent.getUnitId(), unitType, enemiesSpawnPoint, true);
		}
		else{
			long unitCost = gameSingleton.getCost(unitType);
			if (model.getMoney() >= unitCost) {
				String id = createUnit(unitType, friendliesSpawnPoint, false);
				//notify the opponent that you trained the unit
				viewEvent.setEnemy(true);
				viewEvent.setUnitId(id);
				networkController.addObjectRequest(viewEvent);
				model.addMoney(-unitCost);
			}
			else
				setMessageAndClearAfterTime("NOT ENOUGH MONEY");

		}
	}

	/**
	 * Handles ModelEvents
	 */
	private void handleModelEvents(){
		ModelEvent modelEvent;
		while (queueOfModelEvents.size() > 0){
			modelEvent = queueOfModelEvents.poll();
			if (modelEvent.isUnitHit()){
				networkController.addObjectRequest(modelEvent);
			}
		}
	}

	/**
	 * Preloads the assets. (doesn't initialise view's textures)
	 */
	private void loadAssets(){
		for (String filename : GameSingleton.getGameSingleton().getTextureFilenames()){
			assetManager.load(filename, Texture.class);
		}
		assetManager.finishLoading();
	}

	/**
	 * Tells model to create a game object (random id)
	 * @param objectType object type
	 * @param position position
	 * @param isEnemy whether game object is on enemy side
	 */
	private void createObject(ObjectType objectType, Vector2 position, boolean isEnemy){
		String id = Integer.toString(nextObjectId++) + playerId;
		createObject(id, objectType, position, isEnemy);
	}

	/**
	 * Tells model to create a game object (defined id)
	 * @param id object's id
	 * @param objectType object type
	 * @param position position
	 * @param isEnemy whether game object is on enemy side
	 */
	private void createObject(String id, ObjectType objectType, Vector2 position, boolean isEnemy){
		model.createObject(id, objectType, position, gameView.getTextureSize(objectType), isEnemy);
	}

	/**
	 * Tells model to create a unit (random id)
	 * @param objectType object type
	 * @param position position
	 * @param isEnemy whether is enemy
	 * @return the random id that was set for the unit
	 */
	private String createUnit(ObjectType objectType, Vector2 position, boolean isEnemy){
		String id = Integer.toString(nextObjectId++) + playerId;
		createUnit(id, objectType, position, isEnemy);
		return id;
	}

	/**
	 * Tells model to create a unit (defined id)
	 * @param id unit id
	 * @param objectType object type
	 * @param position position
	 * @param isEnemy whether is enemy
	 */
	private void createUnit(String id, ObjectType objectType, Vector2 position, boolean isEnemy){
		gameView.loadActorSprites(id, objectType);
		Vector2 unitSize = gameView.getTextureSize(id);
		model.createUnit(id, objectType, position, unitSize, isEnemy);
	}

	/* IViewController */

	public void addEvent(ViewEvent viewEvent){
		queueOfViewEvents.add(viewEvent);
	}

	public void addEvent(ModelEvent modelEvent){
		queueOfModelEvents.add(modelEvent);
	}

	public String getMessage() {
		return message;
	}

	/**
	 * Sets game message and clears it after X milliseconds
	 * @param message message to be set to
	 */
	private void setMessageAndClearAfterTime(String message){
		if (this.message.equals(message))
			return;

		this.message = message;
		ClearMessageTask clearMessageTask = new ClearMessageTask();
		int clearMessageInterval = 2000;
		clearMessageTimer.schedule(clearMessageTask, clearMessageInterval);
	}

	/**
	 * Task used to schedule message clear
	 */
	private class ClearMessageTask extends TimerTask {
		@Override
		public void run() {
			message = "";
		}
	}

	/* NetworkListener interface */

	public void notify(LinkedBlockingQueue<Object> receivedObjects) {
		while (receivedObjects.size() > 0){
			Object receivedObj = receivedObjects.poll();
			if (receivedObj instanceof ViewEvent){
				queueOfViewEvents.add((ViewEvent)receivedObj);
			}
			else if (receivedObj instanceof ModelEvent){
				ModelEvent modelEvent = (ModelEvent) receivedObj;
				model.unitHit(modelEvent.getUnitId(), modelEvent.getDamage());
			}
		}
	}

	public void notifyOnError(ErrorEvent errorEvent) {
		if (errorEvent.isOpponentDisconnected()){
			if (opponentGameStarted){
				message = "OPPONENT DISCONNECTED!";
				gameRunning = false;
				gameView.gameFinished();
			}
		}
	}

	public double getCurrentTime(){
		return gameTime;
	}

	/* IMenuController */

	public void startGame() {
		startNetworkController();

		initGame();
		Random random = new Random();
		int idRange = 60000;
		playerId = random.nextInt(idRange);

		gameView.init();

		ViewEvent startGameEvent = new ViewEvent();
		startGameEvent.setStart();
		startGameEvent.setFromNetwork();
		networkController.addObjectRequest(startGameEvent);

		gameStarted = true;
		message = "Waiting for the opponent...";
	}

	/**
	 * Starts network controller thread
	 */
	private void startNetworkController(){
		networkThread = new Thread(networkController);
		networkThread.start();
		networkController.resume();
	}

	public boolean connect(){
		return networkController.connect();
	}

	/* GETTERS AND SETTERS */

	/**
	 * Sets target ip address
	 * @param ip value of the ip address
	 */
	public void setIp(String ip) {
		networkController.setTargetIp(ip);
	}

	/**
	 * Sets input port
	 * @param port value of the input port
	 */
	public void setInPort(int port) {
		networkController.setInPort(port);
	}

	/**
	 * Sets output port
	 * @param port value of the output port
	 */
	public void setOutPort(int port) {
		networkController.setOutPort(port);
	}

	/**
	 * Sets host username
	 * @param username username
	 */
	public void setUsername(String username) {
		this.username = username;
		ViewEvent setUsernameEvent = new ViewEvent(username);
		setUsernameEvent.setFromNetwork();
		networkController.addObjectRequest(setUsernameEvent);
	}

	/**
	 * Sets opponent username
	 * @param opponentUsername
	 */
	public void setOpponentUsername(String opponentUsername){
		this.opponentUsername = opponentUsername;
	}

	/**
	 * Returns host username
	 * @return username
	 */
	public String getUsername() {
		return username;
	}

	/**
	 * Returns opponent username
	 * @return username
	 */
	public String getOpponentUsername() {
		return opponentUsername;
	}

	/**
	 * Returns screen width
	 * @return screen width
	 */
	public int getScreenWidth() {
		return screenWidth;
	}

	/**
	 * Returns screen height
	 * @return screen height
	 */
	public int getScreenHeight() {
		return screenHeight;
	}

	/**
	 * Returns instance of libgdx AssetManager
	 * @return asset manager
	 */
	public AssetManager getAssetManager() {
		return assetManager;
	}
}

