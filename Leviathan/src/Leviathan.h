#ifndef LEVIATHAN_H
#define LEVIATHAN_H

namespace Leviathan
{
	extern bool bLvRunning;

	extern HWND LvWindow;

	void LvInitEngine();
	void LvTermEngine();

	void LvMainEngineLoop();
}

#endif // LEVIATHAN_H
