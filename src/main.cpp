#include "game.hpp"

int main(int argc, char **argv)
{
    Game game;
    
    if(!game.Init()) return 1;
    
    game.Run();
    
    game.Quit();

    return 0;
}