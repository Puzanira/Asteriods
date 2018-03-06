#include "Controller.h"
#include <cstdlib>
#include "Game.h"

int main() {
	Controller obj;
	Game game(&obj);
	game.run();
	
	return 0;
}
