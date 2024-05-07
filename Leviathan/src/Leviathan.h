#ifndef LEVIATHAN_H
#define LEVIATHAN_H

namespace Leviathan
{
	extern const int ResX;
	extern const int ResY;
	extern const WCHAR* AppName;

	extern bool bLvRunning;

	extern HWND LvWindow;

	void LvInitEngine();
	void LvTermEngine();

	void LvMainEngineLoop();
}

#endif // LEVIATHAN_H
