package com.strongholds.game.view;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.assets.AssetManager;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.BitmapFont;
import com.badlogic.gdx.graphics.g2d.Sprite;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.graphics.g2d.TextureAtlas;
import com.badlogic.gdx.scenes.scene2d.InputEvent;
import com.badlogic.gdx.scenes.scene2d.Stage;
import com.badlogic.gdx.scenes.scene2d.ui.*;
import com.badlogic.gdx.scenes.scene2d.utils.ClickListener;
import com.strongholds.game.GameSingleton;
import com.strongholds.game.controller.IMenuController;

public class MenuView extends AbstractView implements IMenuView{
    /**
     * background sprite
     */
    private Sprite background;
    /**
     * reference to controller via its dedicated interface
     */
    private IMenuController controller;

    /**
     * text field where ip is written
     */
    private TextField ipField;
    /**
     * text field where output port is written
     */
    private TextField outPortField;
    /**
     * text field where input port is written
     */
    private TextField inPortField;
    /**
     * text field where username is written
     */
    private TextField usernameField;

    /**
     * status message (e.g. connected, you must provide a port...)
     */
    private String message = "";

    /**
     * flag telling whether we successfully connected to the opponent
     */
    private boolean connected;

    /**
     * Creates a new menu view
     * @param controller reference to controller
     * @param assetManager reference to asset manager
     * @param screenX screen width
     * @param screenY screen height
     */
    public MenuView(IMenuController controller, AssetManager assetManager, int screenX, int screenY){
        this.controller = controller;
        this.screenX = screenX;
        this.screenY = screenY;

        stage = new Stage();

        spriteBatch = new SpriteBatch();

        background = new Sprite((Texture)assetManager.get(GameSingleton.getGameSingleton().menuBackgroundTexture));

        font = new BitmapFont();
        skin = new Skin(Gdx.files.internal("Craftacular_UI_Skin/craftacular-ui.json"));

        buttonAtlas = new TextureAtlas(Gdx.files.internal("Craftacular_UI_Skin/craftacular-ui.atlas"));
        textButtonStyle = new TextButton.TextButtonStyle();
        textButtonStyle.font = font;
        textButtonStyle.up = skin.getDrawable("button");
        textButtonStyle.down = skin.getDrawable("button-hover");
        textButtonStyle.checked = skin.getDrawable("button");

        createUI();
    }

    /**
     * Creates UI
     */
    private void createUI(){
        Table table = new Table();
        table.setPosition(screenX / 2 - 100, screenY / 2);
        stage.addActor(table);

        usernameField = new TextField("", skin);
        usernameField.setMaxLength(10);
        usernameField.setPosition(-30, 90);
        usernameField.setSize(260, 50);
        table.addActor(usernameField);

        final Label usernameLabel = new Label("USERNAME", skin);
        usernameLabel.setPosition(10, 140);
        table.addActor(usernameLabel);

        table.addActor(createButton(0,  5, 200, 80,  "START GAME",
                new ClickListener(){
                    @Override
                    public void clicked(InputEvent event, float x, float y){
                        if (connected){
                            String username = usernameField.getText();
                            if (username.equals("")){
                                message = "You must pick a username";
                                return;
                            }
                            controller.setUsername(usernameField.getText());

                            startGame();
                        }
                        else
                            message = "You need to be connected in order to start the game";
                    }
                }));

        ipField = new TextField("", skin);
        ipField.setMaxLength(15);
        ipField.setPosition(-50, -50);
        ipField.setSize(300, 50);
        table.addActor(ipField);

        Label ipLabel = new Label("IP ADDRESS", skin);
        ipLabel.setPosition(270, -45);
        table.addActor(ipLabel);

        outPortField = new TextField("", skin);
        outPortField.setMaxLength(5);
        outPortField.setPosition(30, -105);
        outPortField.setSize(140, 50);
        table.addActor(outPortField);

        Label outPortLabel = new Label("OUT PORT", skin);
        outPortLabel.setPosition(270, -100);
        table.addActor(outPortLabel);

        inPortField = new TextField("", skin);
        inPortField.setMaxLength(5);
        inPortField.setPosition(30, -160);
        inPortField.setSize(140, 50);
        table.addActor(inPortField);

        Label inPortLabel = new Label("IN PORT", skin);
        inPortLabel.setPosition(270, -150);
        table.addActor(inPortLabel);

        table.addActor(createButton(20,-215 , 160, 50,  "CONNECT",
                new ClickListener(){
                    @Override
                    public void clicked(InputEvent event, float x, float y){
                        int outPort = 0;
                        try{
                            outPort = Integer.parseInt(outPortField.getText());
                        }
                        catch(NumberFormatException e){
                            message = "out port must be a number!";
                            return;
                        }

                        int inPort = 0;
                        try{
                            inPort = Integer.parseInt(inPortField.getText());
                        }
                        catch(NumberFormatException e){
                            message = "in port must be a number!";
                            return;
                        }

                        String ip = ipField.getText();
                        if (ip.equals("")){
                            message = "ip address cannot be an empty string!";
                            return;
                        }

                        controller.setOutPort(outPort);
                        controller.setInPort(inPort);
                        controller.setIp(ipField.getText());

                        if (controller.connect()){
                            connected = true;
                            message = "Connection established";
                        }
                        else{
                            connected = false;
                            message = "Can't establish the connection";
                        }
                    }
                }));

                // for testing purposes

                usernameField.setText("Player");
                inPortField.setText("46000");
                outPortField.setText("46000");
                ipField.setText("127.0.0.1");
    }

    /**
     * Called when start game button is clicked
     */
    private void startGame(){
        controller.startGame();
    }

    public void init(){
        message = "";
        connected = false;
        Gdx.input.setInputProcessor(stage);
    }

    public void draw(){
        spriteBatch.begin();
        spriteBatch.draw(background, 0, 0);

        if (message != ""){
            font.draw(spriteBatch, message, screenX / 2 - message.length() * 3, screenY - 520);
        }

        spriteBatch.end();
        stage.draw();
    }

    public void dispose(){
        stage.dispose();
    }


}
