#include "Game.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	try
	{
		// Create and initialize the game
		Game theApp(hInstance);
		if (!theApp.Initialize())
			return 0;

		// Run the game
		return theApp.Run();
	}
	catch (DxException& e)
	{
		// Handle any DirectX exceptions
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}

