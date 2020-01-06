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

class GameConfig {
public:
    GameConfig(int argc, char** argv);

public:
    WindowCfg window;

private:
    void initByLua();

private:
    StringRef luaConfigFile = "config.lua";
};
