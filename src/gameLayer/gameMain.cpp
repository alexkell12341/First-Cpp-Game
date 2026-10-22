#include <raylib.h>
#include "gameMain.h"
#include <asserts.h>
#include <assetManager.h>
#include <gameMap.h>
#include <iostream>
#include <helpers.h>

struct GameData {

	GameMap gameMap;
	Camera2D camera;


}gameData;

//assets should be held seperate to game state, so textures are still loaded if map is deleted/changed, therefore assetManager is defined outside of gameData
AssetManager assetManager;


bool initGame() {

	assetManager.loadAll();

	gameData.gameMap.create(30, 10);

	gameData.gameMap.getBlockUnsafe(0, 0).type = Block::dirt;
	gameData.gameMap.getBlockUnsafe(1, 1).type = Block::grassBlock;
	gameData.gameMap.getBlockUnsafe(2, 2).type = Block::goldBlock;
	gameData.gameMap.getBlockUnsafe(3, 3).type = Block::glass;
	gameData.gameMap.getBlockUnsafe(4, 4).type = Block::platform;

	gameData.camera.target = { 0,0 }; //world-space centre of view, we will use this as the camera position
	gameData.camera.rotation = 0.0f;
	gameData.camera.zoom = 100.0f;

	return true;
}

bool updateGame() {

	float deltaTime = GetFrameTime();
	//limits max deltaTime to 5 frames per second to prevent jumping
	if (deltaTime > 1.f / 5) { deltaTime = 1 / 5.f; }

	gameData.camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	
	ClearBackground({ 75,75,150,255 });

#pragma region camera movement

	//7 blocks per second
	if (IsKeyDown(KEY_LEFT)) gameData.camera.target.x -= 7.f * deltaTime;
	if (IsKeyDown(KEY_RIGHT)) gameData.camera.target.x += 7.f * deltaTime;
	if (IsKeyDown(KEY_UP)) gameData.camera.target.y -= 7.f * deltaTime;
	if (IsKeyDown(KEY_DOWN)) gameData.camera.target.y += 7.f * deltaTime;

#pragma endregion

	Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), gameData.camera);
	int blockX = (int)floor(worldPos.x);
	int blockY = (int)floor(worldPos.y);

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

		auto block = gameData.gameMap.getBlockSafe(blockX, blockY);

		if (block) {

			*block = {};
		}
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {

		auto block = gameData.gameMap.getBlockSafe(blockX, blockY);

		if (block) {

			block->type =Block::gold;
		}
	}

#pragma region draw world
	//do all rendering between BeginMode2d and EndMode2d
	BeginMode2D(gameData.camera);

	for (int y = 0; y < gameData.gameMap.h; ++y) {
		for (int x = 0; x < gameData.gameMap.w; ++x) {
			
			auto& block = gameData.gameMap.getBlockUnsafe(x, y);
			
			

			if (block.type != Block::air) {


				DrawTexturePro(
					assetManager.textures, //texture atlas
					getTextureAtlas(block.type, 0, 32, 32), // source
					{ (float)x, (float)y, 1, 1 }, //destination (x position, y position, block width, block height)
					{ 0,0 }, //origin
					0.0f, //rotation
					WHITE //tint
				);
			}

		}
	}

	//draw selected block (must be after other blocks are rendered so frame is drawn on top)
	DrawTexturePro(
		assetManager.frame, //texture atlas
		{ 0, 0, (float)assetManager.frame.width, (float)assetManager.frame.height }, // source
		{ (float)blockX, (float)blockY, 1, 1 }, //destination (x position, y position, block width, block height)
		{ 0,0 }, //origin
		0.0f, //rotation
		WHITE
	);



	//do all rendering between beginmode2d and endmode2d
	EndMode2D();

#pragma endregion

	return true;
}

void closeGame() {
	std::cout << "\n\nCLOSED!!!!!!!\n\n";
}