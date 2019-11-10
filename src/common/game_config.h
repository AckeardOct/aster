#pragma once

#include <string>

class GameConfig {
public:
    GameConfig(int argc, char** argv);

    std::string toString() const;

public:
    int width = 800;
    int height = 600;
    int fps = 30;
    std::string title = "Aster";

private:
    bool initByLua();

private:
    const char* luaConfigFile = "config.lua";
};
