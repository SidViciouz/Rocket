#include "Game.h"

Game::Game(HINSTANCE hInstance) : mWindow(hInstance)
{

}

void Game::Initialize()
{
	mWindow.Initialize();
	mDirectX.Initialize();
}

void Game::Run()
{
	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}