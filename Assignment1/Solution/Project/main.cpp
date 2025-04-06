#include "Game.hpp"

#include <windows.h>   // For AllocConsole
#include <cstdio>      // For freopen
#include <iostream>    // For std::cout

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{

#ifdef _DEBUG
	AllocConsole();

	// Redirect STDOUT
	FILE* newStdout;
	freopen_s(&newStdout, "CONOUT$", "w", stdout);

	// Redirect STDIN
	FILE* newStdin;
	freopen_s(&newStdin, "CONIN$", "r", stdin);

	// Redirect STDERR
	FILE* newStderr;
	freopen_s(&newStderr, "CONOUT$", "w", stderr);

	std::cout << "Console successfully allocated!\n";
#endif

	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	try
	{
		Game theApp(hInstance);
		if (!theApp.Initialize())
			return 0;

		return theApp.Run();
	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}

