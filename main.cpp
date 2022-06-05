#include"TisGame.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Framework* game=new TisGame();

	game->Run();

	delete game;

	return 0;
}