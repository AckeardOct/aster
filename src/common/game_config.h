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

    struct {
        bool enable = false;
        int resolutionX = 0;
        int resolutionY = 0;
        int offsetX = 0; // TODO: Vector type
        int offsetY = 0; // TODO: Color type
    } dbgDrawGrid;

private:
    bool initByLua();

private:
    const char* luaConfigFile = "config.lua";
};
