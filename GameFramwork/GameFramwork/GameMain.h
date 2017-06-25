#pragma once
/////////////////////////////////////
// GameMain.cpp
// Main scene in the game
/////////////////////////////////////

#ifndef _GAME_MAIN_H_
#define _GAME_MAIN_H_

// Include
#include "Scene.h"
#include "GameBoard.h"
#include "Ball.h"
#include "Racket.h"
#include "GameInfo.h"

// GameMain class
class GameMain : public Scene
{
public:
	GameMain();
	GameMain(const GameMain&);
	~GameMain();

	bool Initialize();
	void Shutdown();

	// Render for 2D.
	void Render2D();
	// Render for 3D.
	void Render3D();
	void Update();

	GameBoard* board;
	Ball* ball;
	Racket* racket;
	GameInfo* info;

};

#endif // !_GAME_MAIN_H_