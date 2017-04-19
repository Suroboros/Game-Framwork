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
	inputSystem = new InputSystem;
	if (!inputSystem)
	{
		MessageBox(hwnd, _T("Could not initialize the input object."), _T("Error"), MB_OK);
		return false;
	}
	inputSystem->Initialize();

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
	//bool result;

	// Get an external pointer to this object.	
	//ApplicationHandle = this;

	// Register the window class.
	className = TEXT("Window Class");

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
		L"Windows",                     // Window text
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,            // Window style

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

	// Hide the mouse cursor.
	ShowCursor(false);

	return true;
}

void WindowMain::Run()
{
	// Run the message loop.
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// Loop
	bool runFlag = true;
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
		if(inputSystem->IsEscapePressed())
			runFlag = false;
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

	// Release input system.
	if (inputSystem)
	{
		inputSystem->Shutdown();
		delete inputSystem;
		inputSystem = nullptr;
	}

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

	// Release the pointer to this class.
	//ApplicationHandle = nullptr;

	return;
}

bool WindowMain::Frame()
{
	bool result;

	// Do the frame function of input system
	result = inputSystem->Frame();
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
	switch (uMsg)
	{
		// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
	{
		// If a key is pressed send it to the input object so it can record that state.
		//inputSystem->KeyDown((unsigned int)wParam);
		//return 0;
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		// If a key is released then send it to the input object so it can unset the state for that key.
		//nputSystem->KeyUp((unsigned int)wParam);
		//return 0;
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

		// When Window is being closed.
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW));

		EndPaint(hwnd, &ps);
		return 0;
	}



	default:
		return WindowMain::GetInstance().KeyHandler(hwnd, uMsg, wParam, lParam);
	}
}

	