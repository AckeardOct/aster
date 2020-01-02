#include "common/game_window.h"


int main(int argc, char** argv)
{
    GameWindow window(argc, argv);
    window.runLoop();

    return 0;
}
