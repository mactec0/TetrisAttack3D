#include "Game.h"


CGame::CGame() : currentScene{MAIN_MENU_SCENE} {
    g3d::engine = g3d::Engine::Initialize();

    g3d::engine->setRenderCallback(std::bind(&CGame::renderFrame, this));
    g3d::engine->setUpdateCallback(std::bind(&CGame::update, this, std::placeholders::_1));


    g3d::resourceManager.load("wall_mesh", g3d::Mesh::create().fromFile("models/wall.obj").build());
    g3d::resourceManager.load("wall_texture", g3d::Texture::create().fromFile("textures/wall.png").build());

    g3d::resourceManager.load("wall_model",  g3d::Model::create().setTexture(g3d::resourceManager.get("wall_texture"))
                                    .setSpecularMap(g3d::resourceManager.get("wall_texture"))
                                    .setMesh(g3d::resourceManager.get("wall_mesh"))
                                    .build());

    for(int i=0;i<(int)BOX_COLOR_MAX;i++){
        g3d::resourceManager.load("box_texture["+std::to_string(i+1)+"]",
        g3d::Texture::create().fromFile("textures/tex_"+std::to_string(i+1)+".png").build());
    }


    g3d::resourceManager.load("block_mesh", g3d::Mesh::create().fromFile("models/block_game.obj").build());
    g3d::resourceManager.load("box_model",  g3d::Model::create().setTexture(g3d::resourceManager.get("box_texture[1]"))
                                    .setSpecularMap(g3d::resourceManager.get("box_texture[1]"))
                                    .setMesh(g3d::resourceManager.get("block_mesh"))
                                    .build());


    g3d::resourceManager.load("selector_mesh", g3d::Mesh::create().fromFile("models/block_selector.obj").build());
    g3d::resourceManager.load("selector_texture", g3d::Texture::create().fromFile("textures/selector.png").build());

    g3d::resourceManager.load("selector_model",  g3d::Model::create().setTexture(g3d::resourceManager.get("selector_texture"))
                                    .setSpecularMap(g3d::resourceManager.get("selector_texture"))
                                    .setMesh(g3d::resourceManager.get("selector_mesh"))
                                    .build());


    for(int i=0; i<10; i++){
        std::string obj_name="num_"+std::to_string(i);
        g3d::resourceManager.load(obj_name+"_mesh", g3d::Mesh::create().fromFile("models/"+obj_name+".obj").build());
        g3d::resourceManager.load(obj_name+"_model",  g3d::Model::create().setMesh(g3d::resourceManager.get(obj_name+"_mesh"))
                                    .setTexture(g3d::resourceManager.get("selector_texture"))
                                    .build());
    }


    g3d::resourceManager.load("singleplayer_entry_mesh", g3d::Mesh::create().fromFile("models/singleplayer.obj").build());
    g3d::resourceManager.load("singleplayer_entry_model",  g3d::Model::create().setMesh(g3d::resourceManager.get("singleplayer_entry_mesh"))
                                .setTexture(g3d::resourceManager.get("selector_texture"))
                                .build());

    g3d::resourceManager.load("multiplayer_entry_mesh", g3d::Mesh::create().fromFile("models/multiplayer.obj").build());
    g3d::resourceManager.load("multiplayer_entry_model",  g3d::Model::create().setMesh(g3d::resourceManager.get("multiplayer_entry_mesh"))
                                .setTexture(g3d::resourceManager.get("selector_texture"))
                                .build());

    g3d::resourceManager.load("quit_entry_mesh", g3d::Mesh::create().fromFile("models/quit.obj").build());
    g3d::resourceManager.load("quit_entry_model",  g3d::Model::create().setMesh(g3d::resourceManager.get("quit_entry_mesh"))
                                .setTexture(g3d::resourceManager.get("selector_texture"))
                                .build());

    g3d::resourceManager.load("difficulty_entry_mesh", g3d::Mesh::create().fromFile("models/difficulty.obj").build());
    g3d::resourceManager.load("difficulty_entry_model",  g3d::Model::create().setMesh(g3d::resourceManager.get("difficulty_entry_mesh"))
                                .setTexture(g3d::resourceManager.get("selector_texture"))
                                .build());

    g3d::resourceManager.load("resume_entry_mesh", g3d::Mesh::create().fromFile("models/resume.obj").build());
    g3d::resourceManager.load("resume_entry_model",  g3d::Model::create().setMesh(g3d::resourceManager.get("resume_entry_mesh"))
                                .setTexture(g3d::resourceManager.get("selector_texture"))
                                .build());

    g3d::resourceManager.load("mainmenu_entry_mesh", g3d::Mesh::create().fromFile("models/mainmenu.obj").build());
    g3d::resourceManager.load("mainmenu_entry_model",  g3d::Model::create().setMesh(g3d::resourceManager.get("mainmenu_entry_mesh"))
                                .setTexture(g3d::resourceManager.get("selector_texture"))
                                .build());

    g3d::resourceManager.load("winner_entry_mesh", g3d::Mesh::create().fromFile("models/winner.obj").build());
    g3d::resourceManager.load("winner_entry_model",  g3d::Model::create().setMesh(g3d::resourceManager.get("winner_entry_mesh"))
                                .setTexture(g3d::resourceManager.get("selector_texture"))
                                .build());

    g3d::resourceManager.load("loser_entry_mesh", g3d::Mesh::create().fromFile("models/loser.obj").build());
    g3d::resourceManager.load("loser_entry_model",  g3d::Model::create().setMesh(g3d::resourceManager.get("loser_entry_mesh"))
                                .setTexture(g3d::resourceManager.get("selector_texture"))
                                .build());

    g3d::resourceManager.load("floor_mesh", g3d::Mesh::create().fromFile("models/floor.obj").build());
    g3d::resourceManager.load("floor_texture", g3d::Texture::create().generateMipmaps(true).fromFile("textures/tiles.png").build());
    g3d::resourceManager.load("floor_model",  g3d::Model::create().setTexture(g3d::resourceManager.get("floor_texture"))
                                .setSpecularMap(g3d::resourceManager.get("floor_texture"))
                                .setMesh(g3d::resourceManager.get("floor_mesh"))
                                .setPosition({5.0f, 0.0f, 0.0f})
                                .setScale({18.0f, 18.0f, 18.0f})
                                .build());


    for (int i = 0; i < BOARD_MAX; i++) {
        boards[i] = new CGameBoard(static_cast<BOARD_ID>(i));
    }

    scenes[MAIN_MENU_SCENE] = new CMainMenuScene(boards);
    scenes[GAMEPLAY_SCENE] = new CGamePlayScene(boards);
    scenes[DIFFICULTY_SCENE] = new CDifficultyScene(boards);
    scenes[PAUSE_SCENE] = new CPauseScene(boards);

}

CGame::~CGame() {
    for (int i = 0; i < BOARD_MAX; i++)
        delete boards[i];

    for (int i = 0; i < SCENE_MAX; i++)
        delete scenes[i];
}

CGame *CGame::create() { return new CGame(); }

void CGame::start() { g3d::engine->run(); }

void CGame::renderFrame() { scenes[currentScene]->draw(); }

void CGame::update(float deltaTime) {
    scenes[currentScene]->control(deltaTime);
    currentScene = scenes[currentScene]->getNextScene();
    if (scenes[currentScene]->quit())
        g3d::engine->shutdown();
}
