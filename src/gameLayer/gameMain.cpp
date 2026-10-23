#include <raylib.h>
#include "gameMain.h"
#include <asserts.h>
#include <assetManager.h>
#include <gameMap.h>
#include <iostream>
#include <helpers.h>
#include <raymath.h>

int global_blockType;

struct GameData {

	GameMap gameMap;
	Camera2D camera;


}gameData;

//assets should be held seperate to game state, so textures are still loaded if map is deleted/changed, therefore assetManager is defined outside of gameData
AssetManager assetManager;


bool initGame() {

	assetManager.loadAll();

	gameData.gameMap.create(30, 10);

	for (int i = 0;  i < 30; ++i) {
		for (int j = 0; j < 10; ++j) {

			if (j == 4) {
				gameData.gameMap.getBlockUnsafe(i, j).type = Block::grassBlock;
			}
			else if (j >4) {
				gameData.gameMap.getBlockUnsafe(i, j).type = Block::dirt;
			}
		}
	}

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
	//converts float to int by truncating
	int blockX = (int)floor(worldPos.x);
	int blockY = (int)floor(worldPos.y);

	//delete block
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

		auto block = gameData.gameMap.getBlockSafe(blockX, blockY);

		if (block) {

			*block = {};
		}
	}

	//place block
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {

		auto block = gameData.gameMap.getBlockSafe(blockX, blockY);

		if (block) {
			if (global_blockType < Block::BLOCKS_COUNT) {

				block->type = global_blockType;
			}
			else {
				block->type = Block::air;
			}
		}
	}

#pragma region draw world

	//do all rendering between BeginMode2d and EndMode2d
	BeginMode2D(gameData.camera);

	//gets top left and bottom right of screen as viewed by camera
	Vector2 topLeftView = GetScreenToWorld2D({ 0, 0 }, gameData.camera);
	Vector2 bottomRightView = GetScreenToWorld2D({ (float)GetScreenWidth(), (float)GetScreenHeight()}, gameData.camera);

	//adds padding so covers a little more than the visible area
	int startXView = (int)floorf(topLeftView.x - 1);
	int endXView = (int)ceilf(bottomRightView.x + 1);
	int startYView = (int)floorf(topLeftView.y - 1);
	int endYView = (int)ceilf(bottomRightView.y + 1);

	//makes sure values wont exceed max and min bounds, all clmap does is places the values within these bounds
	startXView = Clamp(startXView, 0, gameData.gameMap.w - 1); //Clamp(value, min, max)
	endXView = Clamp(endXView, 0, gameData.gameMap.w - 1);

	startYView = Clamp(startYView, 0, gameData.gameMap.h - 1);
	endYView = Clamp(endYView, 0, gameData.gameMap.h - 1);


	for (int y = startYView; y < endYView; ++y) {
		for (int x = startXView; x < endXView; ++x) {
			
			auto& block = gameData.gameMap.getBlockUnsafe(x, y);
			
			

			if (block.type != Block::air) {


#pragma region tree textures
				if (block.type == Block::woodLog) {

					
					//leaves above log
					if (gameData.gameMap.getBlockSafe(x, y - 1) && gameData.gameMap.getBlockSafe(x, y - 1)->type == Block::leaves) {
						DrawTexturePro(
							assetManager.treeTextures, //texture atlas
							getTextureAtlas(5, 0, 32, 32), // source
							{ (float)x, (float)y, 1, 1 }, //destination (x position, y position, block width, block height)
							{ 0,0 }, //origin
							0.0f, //rotation
							WHITE //tint
						);
					}//leaves left and right
					else if (gameData.gameMap.getBlockSafe(x - 1, y)->type && gameData.gameMap.getBlockSafe(x - 1, y)->type == Block::leaves && gameData.gameMap.getBlockSafe(x + 1, y) && gameData.gameMap.getBlockSafe(x + 1, y)->type == Block::leaves) {
						DrawTexturePro(
							assetManager.treeTextures, //texture atlas
							getTextureAtlas(1, 0, 32, 32), // source
							{ (float)x, (float)y, 1, 1 }, //destination (x position, y position, block width, block height)
							{ 0,0 }, //origin
							0.0f, //rotation
							WHITE //tint
						);

					}//leaves on left only
					else if (gameData.gameMap.getBlockSafe(x - 1, y) && gameData.gameMap.getBlockSafe(x - 1, y)->type == Block::leaves) {
						
						DrawTexturePro(
							assetManager.treeTextures, //texture atlas
							getTextureAtlas(3, 0, 32, 32), // source
							{ (float)x, (float)y, 1, 1 }, //destination (x position, y position, block width, block height)
							{ 0,0 }, //origin
							0.0f, //rotation
							WHITE //tint
						);
					
					
					}// leaves on right only
					else if (gameData.gameMap.getBlockSafe(x + 1, y) && gameData.gameMap.getBlockSafe(x + 1, y)->type == Block::leaves) {
						
						DrawTexturePro(
							assetManager.treeTextures, //texture atlas
							getTextureAtlas(2, 0, 32, 32), // source
							{ (float)x, (float)y, 1, 1 }, //destination (x position, y position, block width, block height)
							{ 0,0 }, //origin
							0.0f, //rotation
							WHITE //tint
						);
					
					}//normal log
					else if (gameData.gameMap.getBlockSafe(x, y - 1) && gameData.gameMap.getBlockSafe(x, y - 1)->type == Block::woodLog && gameData.gameMap.getBlockSafe(x, y + 1) && gameData.gameMap.getBlockSafe(x, y + 1)->type == Block::woodLog) {
						DrawTexturePro(
							assetManager.treeTextures, //texture atlas
							getTextureAtlas(0, 0, 32, 32), // source
							{ (float)x, (float)y, 1, 1 }, //destination (x position, y position, block width, block height)
							{ 0,0 }, //origin
							0.0f, //rotation
							WHITE //tint
						);
					}//first log
					else if (gameData.gameMap.getBlockSafe(x, y - 1) && gameData.gameMap.getBlockSafe(x, y - 1)->type == Block::woodLog) {

						DrawTexturePro(
							assetManager.treeTextures, //texture atlas
							getTextureAtlas(4, 0, 32, 32), // source
							{ (float)x, (float)y, 1, 1 }, //destination (x position, y position, block width, block height)
							{ 0,0 }, //origin
							0.0f, //rotation
							WHITE //tint
						);

					}//cutoff log
					else if (gameData.gameMap.getBlockSafe(x, y + 1) && gameData.gameMap.getBlockSafe(x, y + 1)->type == Block::woodLog) {

						DrawTexturePro(
							assetManager.treeTextures, //texture atlas
							getTextureAtlas(6, 0, 32, 32), // source
							{ (float)x, (float)y, 1, 1 }, //destination (x position, y position, block width, block height)
							{ 0,0 }, //origin
							0.0f, //rotation
							WHITE //tint
						);
					}
#pragma endregion 
					else {

						DrawTexturePro(
							assetManager.treeTextures, //texture atlas
							getTextureAtlas(7, 0, 32, 32), // source
							{ (float)x, (float)y, 1, 1 }, //destination (x position, y position, block width, block height)
							{ 0,0 }, //origin
							0.0f, //rotation
							WHITE //tint
						);
					}
				}
				else {

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
	}

	//draw selected block (must be after other blocks are rendered so frame is drawn on top)
	DrawTexturePro(
		assetManager.frame, //texture
		{ 0, 0, (float)assetManager.frame.width, (float)assetManager.frame.height }, // source
		{ (float)blockX, (float)blockY, 1, 1 }, //destination (x position, y position, block width, block height)
		{ 0,0 }, //origin
		0.0f, //rotation
		WHITE
	);

	//do all rendering between beginmode2d and endmode2d
	EndMode2D();

#pragma endregion

	DrawFPS(10, 10);

	return true;
}

void closeGame() {
	std::cout << "\n\nCLOSED!!!!!!!\n\n";
}