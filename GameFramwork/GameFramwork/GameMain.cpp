#include "GameMain.h"
#include "GameEngine.h"
#include "D3DClass.h"
#include "Ending.h"


GameMain::GameMain()
{
}

GameMain::GameMain(const GameMain &)
{
}

GameMain::~GameMain()
{
}

bool GameMain::Initialize()
{
	bool result;

	// Load background image.
	result = GameEngine::GetInstance().CreateImage2D("background", _T("./data/image/background.png"));
	if(!result)
		return false;

	// Load block, ball, racket image
	result = GameEngine::GetInstance().CreateImage2D("source", _T("./data/image/block.png"));
	if(!result)
		return false;

	// Initialize board
	board = new GameBoard;
	result = board->Initialize();
	if(!result)
		return false;
	board->LoadData("./data/StageData/Map.txt");
	GameEngine::GetInstance().PushBackObject("board", board);

	// Initialize ball
	ball = new Ball;
	result = ball->Initialize();
	if(!result)
		return false;
	GameEngine::GetInstance().PushBackObject("ball", ball);

	// Initialize racket
	racket = new Racket;
	result = racket->Initialize();
	if(!result)
		return false;
	GameEngine::GetInstance().PushBackObject("racket", racket);

	// Initialize info
	info = new GameInfo;
	result = info->Initialize();
	if(!result)
		return false;
	GameEngine::GetInstance().PushBackObject("info", info);

	// Regist next scene
	Ending* end = new Ending;
	if(!end)
		return false;
	GameEngine::GetInstance().PushBackScene("Ending", end);

	return true;
}

void GameMain::Shutdown()
{
	GameEngine::GetInstance().DestroyImage2D("background");
	GameEngine::GetInstance().DestroyObject("board");
	GameEngine::GetInstance().DestroyObject("ball");
	GameEngine::GetInstance().DestroyObject("racket");
}

void GameMain::Render2D()
{
	// Draw background
	Rect src = { 0,0,480,700 };
	Rect dest = { 0,0,480,700 };
	GameEngine::GetInstance().DrawImage2D("background", dest, src);

	// Draw board
	board->Render2D();
	// Draw ball
	ball->Render2D();
	// Draw racket
	racket->Render2D();
	// Draw info
	info->Render2D();

}

void GameMain::Render3D()
{
}

void GameMain::Update()
{
	if(info->gameClearFlag || info->gameOverFlag)
	{
		GameEngine::GetInstance().ChangeScene("Ending");
		return;
	}
	// Update board
	board->Update();
	// Update ball
	ball->Update();
	// Update racket;
	racket->Update();
	// Update info
	info->Update();
}
