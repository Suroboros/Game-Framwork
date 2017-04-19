#pragma once
/////////////////////////////////////
// WinMain.h
// Main windows
/////////////////////////////////////

#ifndef UNICODE
#define UNICODE
#endif 

#ifndef _WINMAIN_H_
#define _WINMAIN_H_

// PRE-PROCESSING DIRECTIVES
#define WIN32_LEAN_AND_MEAN

// Inludes
#include <Windows.h>
#include "InputSystem.h"
#include "GraphicSystem.h"

//WindowMain Class

class WindowMain
{
	HINSTANCE hInstance; 
	HINSTANCE hPrevInstance; 
	PTSTR pCmdLine; 
	int nCmdShow;
	HWND hwnd;
	LPTSTR className;
	int screenWidth, screenHeight;

	// System object.
	InputSystem* inputSystem;
	GraphicSystem* graphicSystem;

	
public:
	

	bool Initialize(HINSTANCE hInstance, HINSTANCE hPrevInstance, PTSTR pCmdLine, int nCmdShow);
	void Run();
	void Shutdown();
	void ShutdownWindows();
	
	// Frame processing.
	bool Frame();

	// Key processing.
	LRESULT CALLBACK KeyHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// Get screen size
	int GetScreenWidth();
	int GetScreenHeight();
	// Get window`s handle
	HWND GetHwnd();

	// Get the instance of WindowMain(singleton) 
	static WindowMain& GetInstance();
	// Get hInstance
	HINSTANCE GetHINSTSANCE();

private:
	WindowMain();
	WindowMain(const WindowMain&);
	~WindowMain();
	WindowMain& operator=(const WindowMain&);

	bool InitializeWindows(int&, int&);
};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/////////////
// GLOBALS 
/////////////
//static WindowMain* ApplicationHandle = nullptr;

#endif

