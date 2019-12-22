#pragma once

#include "common/defines.h"
#include "common/string.h"
#include "gen/gen_game_config.h"

struct WindowCfg GEN_LUA GEN_JSON {
    int width = 800;
    int height = 600;
    int fps = 30;
    String title = "Aster";
};

struct DbgGridCfg GEN_LUA GEN_JSON {
    bool enable = false;
    int resolutionX = 0;
    int resolutionY = 0;
    int offsetX = 0; // TODO: Vector type
    int offsetY = 0; // TODO: Color type
};

class GameConfig {
public:
    GameConfig(int argc, char** argv);

public:
    WindowCfg window;
    DbgGridCfg dbgDrawGrid;

private:
    void initByLua();

private:
    StringRef luaConfigFile = "config.lua";
};
