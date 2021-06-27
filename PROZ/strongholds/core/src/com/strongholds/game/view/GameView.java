package com.strongholds.game.view;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.Color;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.*;
import com.badlogic.gdx.graphics.glutils.ShapeRenderer;
import com.badlogic.gdx.math.Vector2;

import java.util.HashMap;
import java.util.Map;

import com.badlogic.gdx.scenes.scene2d.InputEvent;
import com.badlogic.gdx.scenes.scene2d.Stage;
import com.badlogic.gdx.scenes.scene2d.ui.Skin;
import com.badlogic.gdx.scenes.scene2d.ui.TextButton;
import com.badlogic.gdx.scenes.scene2d.utils.ClickListener;
import com.strongholds.game.GameSingleton;
import com.strongholds.game.GameSingleton.ObjectType;
import com.strongholds.game.controller.IViewController;
import com.strongholds.game.event.ViewEvent;
import com.strongholds.game.model.IReadOnlyModel;
import com.strongholds.game.gameobject.IReadOnlyAnimatedActor;
import com.strongholds.game.gameobject.IReadOnlyGameObject;

/**
 * View displayed when the game started
 */
public class GameView extends AbstractView implements IGameView
{
    private IReadOnlyModel model;
    private IViewController controller;

    private float pixels_per_meter;

    private OrthographicCamera cam;
    //private final int cameraSpeed = 10;
    private final float cameraZoom = 1.0f;

    private Map<ObjectType, Sprite> staticObjectsTextureMap;
    private Map<String, Animator> actorsMap;

    private TextButton pauseButton;
    private ClickListener pauseListener;
    private ClickListener restartListener;

    // stores game message layout
    private GlyphLayout msgLayout;

    private final Vector2 globalPadding = new Vector2(10, 10);

    /**
     * position of the game message
     * It starts in the left-bottom corner.
     * Goes to the middle when the game is finished.
     */
    private final Vector2 msgLeftBottomCornerPos = new Vector2(-10, -10);
    private Vector2 msgPosition = msgLeftBottomCornerPos;

    public GameView(IReadOnlyModel model, final IViewController controller)
    {
        this.model = model;
        this.controller = controller;

        pixels_per_meter = GameSingleton.getGameSingleton().getPixels_per_meter();
        screenX = controller.getScreenWidth();
        screenY = controller.getScreenHeight();

        staticObjectsTextureMap = new HashMap<>();
        actorsMap = new HashMap<>();
        spriteBatch = new SpriteBatch();
        stage = new Stage();

        cam = new OrthographicCamera(controller.getScreenWidth(), controller.getScreenHeight());
        cam.position.set(screenX / 2, screenY / 2, 0);
        cam.zoom = cameraZoom;

       //load application skin
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

    private void createUI(){
        stage.addActor(createButton(20, screenY - 70, 100, 50, "swordsman",
                new ClickListener(){
                    @Override
                    public void clicked(InputEvent event, float x, float y){
                        controller.addEvent(new ViewEvent(ObjectType.SWORDSMAN));
                    }
                }));

        stage.addActor(createButton(150, screenY - 70, 100, 50, "archer",
                new ClickListener(){
                    @Override
                    public void clicked(InputEvent event, float x, float y){
                        controller.addEvent(new ViewEvent(ObjectType.ARCHER));
                    }
                }));

        pauseListener = new ClickListener(){
            @Override
            public void clicked(InputEvent event, float x, float y){
                controller.addEvent(new ViewEvent(true));
            }};

        pauseButton = createButton(screenX / 2 - 50,  screenY - 70, 100, 50, "pause", pauseListener);

        stage.addActor(pauseButton);

        restartListener = new ClickListener() {
            @Override
            public void clicked(InputEvent event, float x, float y) {
                ViewEvent restartEvent = new ViewEvent();
                restartEvent.setRestart();
                controller.addEvent(restartEvent);
            }
        };
    }

    public void init(){
        Gdx.input.setInputProcessor(stage);
        msgPosition = msgLeftBottomCornerPos;
        msgLayout = new GlyphLayout();
    }

    @Override
    public void gameFinished() {
        pauseButton.removeListener(pauseListener);
        pauseButton.setText("back to menu");
        pauseButton.addListener(restartListener);
        float buttonWidth = pauseButton.getWidth();
        float buttonHeight = pauseButton.getHeight();
        pauseButton.setPosition(screenX / 2 - buttonWidth / 2, screenY / 2 - buttonHeight - 20);

        // update msgLayout's message, so it can calculate message width
        msgLayout.setText(font, controller.getMessage());
        // get message width
        float msgWidth = msgLayout.width;
        msgPosition = new Vector2(screenX / 2 - msgWidth / 2, screenY / 2);
    }

    public void update(float deltaTime)
    {
        cam.update();
        spriteBatch.setProjectionMatrix(cam.combined);

        for (Object obj : model.getActorArray()){
            IReadOnlyAnimatedActor actor = (IReadOnlyAnimatedActor)obj;
            Animator animator = actorsMap.get(actor.getId());
            animator.update(actor.getState(), deltaTime);
        }
    }

    public void draw()
    {
        spriteBatch.begin();

        Sprite backgroundTexture = staticObjectsTextureMap.get(ObjectType.BACKGROUND_IMAGE);
        spriteBatch.draw(backgroundTexture, 0, 0);

        // draw non-animated objects
        for (Object gameObject : model.getGameObjectArray())
        {
            drawGameObject((IReadOnlyGameObject)gameObject);
        }
        //draw actors
        for (Object actor : model.getActorArray()){
            drawGameObject((IReadOnlyAnimatedActor)actor);
        }

        font.draw(spriteBatch, model.getMoney() + " $", screenX - 50, screenY - 20);
        font.draw(spriteBatch, controller.getUsername(), 30, screenY / 2 + 60);
        font.draw(spriteBatch, controller.getOpponentUsername(), screenX - 80, screenY / 2 + 60);

        String msg = controller.getMessage();
        if (!msg.equals("")){
            // save message position copy so we can restore it later
            Vector2 msgPositionCopy = new Vector2(msgPosition);

            msgLayout.setText(font, msg);
            float msgWidth = msgLayout.width;
            float msgHeight = msgLayout.height;
            // constrain the message
            if (msgPosition.x + msgWidth + globalPadding.x > screenX){
                msgPosition.x = screenX - msgWidth - globalPadding.x;
            }
            else if (msgPosition.x < globalPadding.y){
                msgPosition.x = globalPadding.x;
            }

            if (msgPosition.y + msgHeight + globalPadding.y > screenY){
                msgPosition.y = screenY - globalPadding.y;
            }
            else if (msgPosition.y <= globalPadding.y){
                msgPosition.y = globalPadding.y + msgHeight;
            }

            font.draw(spriteBatch, msg, msgPosition.x, msgPosition.y);
            msgPosition = msgPositionCopy;
        }

        spriteBatch.end();

        stage.draw();
    }

    private void drawGameObject(IReadOnlyGameObject gameObject){
        Sprite texture = staticObjectsTextureMap.get(gameObject.getType());
        float x = (gameObject.getPosition().x - gameObject.getWidth()) * pixels_per_meter;
        float y = (gameObject.getPosition().y - gameObject.getHeight()) * pixels_per_meter;

        if (gameObject.isEnemy()){
            texture.flip(true, false);
            spriteBatch.draw(texture, x, y);
            texture.flip(true, false);
        }
        else {
            spriteBatch.draw(texture, x, y);
        }

        if (gameObject.getType() == ObjectType.BASE){
            drawHealthBar(gameObject, x, y);
        }
    }

    private void drawGameObject(IReadOnlyAnimatedActor gameObject){
        String id = gameObject.getId();
        Animator animator = actorsMap.get(id);
        TextureRegion textureRegion = animator.getCurrentFrame();
        float x = (gameObject.getPosition().x - gameObject.getWidth()) * pixels_per_meter;
        float y = (gameObject.getPosition().y - gameObject.getHeight()) * pixels_per_meter;

        if (gameObject.isEnemy()){
            textureRegion.flip(true, false);
            spriteBatch.draw(textureRegion, x, y);
            textureRegion.flip(true, false);
        }
        else {
            spriteBatch.draw(textureRegion, x, y);
        }

        if (gameObject.getType() == ObjectType.SWORDSMAN || gameObject.getType() == ObjectType.ARCHER){
            drawHealthBar(gameObject, x , y);
        }
    }

    private void drawHealthBar(IReadOnlyGameObject actor, float x, float y){
        float healthBarWidth;
        float healthBarHeight;
        float offsetY;
        float offsetX;

        if (actor.getType() == ObjectType.BASE){
            healthBarWidth = 100;
            healthBarHeight = 10;
            offsetY = 265.0f;
            if (actor.isEnemy()){
                offsetX = 10.0f;
            }
            else{
                offsetX = 20.0f;
            }
        }
        else{
            healthBarWidth = 35;
            healthBarHeight = 5;
            offsetX = 0.0f;
            offsetY = getTextureSize(actor.getId()).y;
        }

        float effectiveHealthBarWidth = (float)actor.getHealth() / (float)actor.getMaxHealth() * healthBarWidth;

        spriteBatch.end();
        ShapeRenderer shapeRenderer = new ShapeRenderer();
        shapeRenderer.setProjectionMatrix(spriteBatch.getProjectionMatrix());
        shapeRenderer.begin(ShapeRenderer.ShapeType.Filled);
        shapeRenderer.setColor(Color.WHITE);
        shapeRenderer.rect(x + offsetX, y + offsetY, healthBarWidth, healthBarHeight);
        shapeRenderer.setColor(Color.RED);
        shapeRenderer.rect(x + offsetX, y + offsetY, effectiveHealthBarWidth, healthBarHeight);
        shapeRenderer.end();
        spriteBatch.begin();
    }

    public void loadTextures(){
        staticObjectsTextureMap.put(ObjectType.BACKGROUND_IMAGE, getSprite("background-textures.png"));
        staticObjectsTextureMap.put(ObjectType.PLATFORM, getSprite("platform.png"));
        staticObjectsTextureMap.put(ObjectType.BASE, getSprite("base.png"));
    }

    public void loadActorSprites(String id, ObjectType objectType){
        AnimationClip idle = null, move = null, attack = null;
        GameSingleton.TextureInfo textureInfos[] = GameSingleton.getGameSingleton().getActorTextureInfo(objectType);

        idle = loadAnimationClip(textureInfos[0], "IDLE TextureInfo not set " + GameSingleton.getGameSingleton().toString(objectType));
        move = loadAnimationClip(textureInfos[1], "MOVE TextureInfo not set " + GameSingleton.getGameSingleton().toString(objectType));
        attack = loadAnimationClip(textureInfos[2], "ATTACK TextureInfo not set " + GameSingleton.getGameSingleton().toString(objectType));

        actorsMap.put(id, new Animator(idle, move, attack));
    }

    private AnimationClip loadAnimationClip(GameSingleton.TextureInfo textureInfo, String msgOnNull){
        AnimationClip clip;
        try{
            clip = new AnimationClip(
                    getTexture(textureInfo.filename),
                    textureInfo.cols,
                    textureInfo.rows,
                    textureInfo.filledFrames,
                    textureInfo.interval
            );
        }
        catch (NullPointerException e){
            System.out.println(msgOnNull);
            clip = new AnimationClip();
        }
        return clip;
    }

        /* GETTERS AND SETTERS */

    public Vector2 getTextureSize(ObjectType objectType){
        Sprite texture = staticObjectsTextureMap.get(objectType);
        return new Vector2(texture.getWidth(), texture.getHeight());
    }

    public Vector2 getTextureSize(String id){
        TextureRegion texture = null;
        try{
            texture = actorsMap.get(id).getCurrentFrame();
        }
        catch(NullPointerException e){
            System.out.println("Animator not set for actor of type " +
                    GameSingleton.getGameSingleton().toString(model.getActor(id).getType()));
        }
        return new Vector2(texture.getRegionWidth(), texture.getRegionHeight());
    }

    private Texture getTexture(String filename){

        return (Texture)controller.getAssetManager().get(filename);
    }

    private Sprite getSprite(String filename){

        return new Sprite((Texture)controller.getAssetManager().get(filename));
    }
}
