#include <iostream>
#include <raylib.h>
#include <fstream>


struct GameData {

	float positionX = 100;
	float positionY = 100;

}gameData;


bool initGame() {
	return true;
}

bool updateGame() {

	float deltaTime = GetFrameTime();
	//limits max deltaTime to 5 frames per second to prevent jumping
	if (deltaTime > 1.f / 5) { deltaTime = 1 / 5.f; }

	//moves 200 pixels per second
	if (IsKeyDown(KEY_A)) { gameData.positionX -= 200 * deltaTime; }
	if (IsKeyDown(KEY_D)) { gameData.positionX += 200 * deltaTime; }
	if (IsKeyDown(KEY_W)) { gameData.positionY -= 200 * deltaTime; }
	if (IsKeyDown(KEY_S)) { gameData.positionY += 200 * deltaTime; }

	//positionX, positionY, sizeX, sizeY, colour
	DrawRectangle(gameData.positionX, gameData.positionY, 50, 50, { 255,0,200,255 });

	return true;
}

void closeGame() {
	std::cout << "\n\nCLOSED!!!!!!!\n\n";
}