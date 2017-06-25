/////////////////////////////////////
// WinMain.cpp
// Main windows
/////////////////////////////////////

// Inludes
#include "WinMain.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PTSTR pCmdLine, int nCmdShow)
{
	bool result;
	
	result = WindowMain::GetInstance().Initialize(hInstance, hPrevInstance, pCmdLine, nCmdShow);
	if(result)
		WindowMain::GetInstance().Run();

	WindowMain::GetInstance().Shutdown();

	

	return 0;
}

WindowMain::WindowMain()
{
	this->hInstance = 0;
	this->hPrevInstance = 0;
	this->pCmdLine = nullptr;
	this->nCmdShow = 0;

	hwnd = NULL;

	screenWidth = 0;
	screenHeight = 0;

	inputSystem = nullptr;
	graphicSystem = nullptr;
}

WindowMain::WindowMain(const WindowMain&)
{

}

WindowMain::~WindowMain()
{

}

WindowMain& WindowMain::operator=(const WindowMain&)
{
	return *this;
}

bool WindowMain::Initialize(HINSTANCE hInstance, HINSTANCE hPrevInstance, PTSTR pCmdLine, int nCmdShow)
{
	bool result;

	this->hInstance = hInstance;
	this->hPrevInstance = hPrevInstance;
	this->pCmdLine = pCmdLine;
	this->nCmdShow = nCmdShow;
	
	// Initialize windows.
	result = InitializeWindows(screenWidth, screenHeight);
	if (!result)
		return false;

	// Initialize input system.
	result = InputSystem::GetInstance().Initialize();
	if (!result)
	{
		MessageBox(hwnd, _T("Could not initialize the input object."), _T("Error"), MB_OK);
		return false;
	}
	// Create keyboard device.
	result = InputSystem::GetInstance().CreateKeyborad("sys");
	if(!result)
	{
		MessageBox(hwnd, _T("Could not create keyboard device."), _T("Error"), MB_OK);
		return false;
	}
	// Create Mouse device.
	result = InputSystem::GetInstance().CreateMouse("sysMouse");
	if(!result)
	{
		MessageBox(hwnd, _T("Could not create mouse device."), _T("Error"), MB_OK);
		return false;
	}

	// Initialize graphic system.
	graphicSystem = new GraphicSystem;
	if (!graphicSystem)
	{
		return false;
	}
	graphicSystem->Initialize();

	return true;
}

bool WindowMain::InitializeWindows(int& screenWidth, int& screenHeight)
{
	int posX, posY;
	// Full screen setting.
	DEVMODE dmScreenSettings;

	// Register the window class.
	className = TEXT("Game Framwork");

	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = className;

	RegisterClass(&wc);

	// Full screen.
	//bool fullScreen = false;
	if(FULLSCREEN)
	{
		// Get screen size.
		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);
		// Full screen setting.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		// Set position.
		posX = 0;
		posY = 0;
	}
	else
	{
		// Window style setting.
		screenWidth = 800;
		screenHeight = 600;
		// Set position.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}
	// Create the window.
	hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		className,                     // Window class
		_T("Windows"),                     // Window text
		WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX,            // Window style

										// Size and position
		posX, posY, screenWidth, screenHeight,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
		);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	// Set the Window as the mian focus.
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	// Show the mouse cursor.
	ShowCursor(true);

	return true;
}

void WindowMain::Run()
{
	// Run the message loop.
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// Loop
	runFlag = true;
	while (runFlag)
	{
		// Get the windows messages
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to quit then exit out.
		if(msg.message == WM_QUIT)
			runFlag = false;
		else
		{
			bool result = Frame();
			if (!result)
				break;
		}

		// Check if the user pressed escape to quit
		if(InputSystem::GetInstance().IsEscapeDown("sys"))
			runFlag = false;
		if(InputSystem::GetInstance().IsKeyDown("sys", DIK_F1))
		{
			FULLSCREEN = !FULLSCREEN;
			D3DClass::GetInstance().GetSwapChain()->SetFullscreenState(!FULLSCREEN, nullptr);
		}
	}
	return;
}

void WindowMain::Shutdown()
{
	
	// Release graphic system.
	if (graphicSystem)
	{
		graphicSystem->Shutdown();
		delete graphicSystem;
		graphicSystem = nullptr;
	}

	InputSystem::GetInstance().Shutdown();

	ShutdownWindows();

	return;

}

void WindowMain::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(FULLSCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove Window.
	DestroyWindow(hwnd);
	hwnd = nullptr;

	// Remove application instance 
	UnregisterClass(className, hInstance);
	hInstance = nullptr;

	return;
}

bool WindowMain::Frame()
{
	bool result;

	// Do the frame function of input system
	result = InputSystem::GetInstance().Frame();
	if(!result)
	{
		return false;
	}

	// Do the frame function of graphic system
	result = graphicSystem->Frame();
	if (!result)
	{
		return false;
	}

	return true;
}

LRESULT WindowMain::KeyHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		// Check if a key has been pressed on the keyboard.
		case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case VK_ESCAPE:
				PostMessage(hwnd, WM_QUIT, 0, 0);
				break;
			default:
				break;
			}
		}

		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
		{

		}

		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
}

int WindowMain::GetScreenWidth()
{
	return screenWidth;
}

int WindowMain::GetScreenHeight()
{
	return screenHeight;
}

HWND WindowMain::GetHwnd()
{
	return hwnd;
}

WindowMain & WindowMain::GetInstance()
{
	static WindowMain instance;
	return instance;
}

HINSTANCE WindowMain::GetHINSTSANCE()
{
	return hInstance;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		// When Window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
		// When Window is being closed.
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW));

		EndPaint(hwnd, &ps);
		return 0;
		break;
	}
	case WM_SIZE:
	{
	}

	default:
		return WindowMain::GetInstance().KeyHandler(hwnd, uMsg, wParam, lParam);
		break;
	}
}

	