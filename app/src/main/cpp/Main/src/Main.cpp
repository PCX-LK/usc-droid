#include "stdafx.h"
#include "Application.hpp"

#ifdef _WIN32
// Windows entry point
int32 __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	new Application();

	String commandLine = Utility::ConvertToUTF8(GetCommandLineW());
	g_application->SetCommandLine(*commandLine);

	int32 ret = g_application->Run();
	delete g_application;
	return ret;
}
#else

extern "C"
{
	// Linux entry point
	int main(int argc, char **argv)
	{
		new Application();
		g_application->SetCommandLine(argc, argv);
		int32 ret = g_application->Run();
		delete g_application;
		return ret;
	}
}
#endif
