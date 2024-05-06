#include "LvCommon.h"

namespace Leviathan
{
	void fMatrix::Identity()
	{
		Row0 = { 1.0f, 0.0f, 0.0f, 0.0f };
		Row1 = { 0.0f, 1.0f, 0.0f, 0.0f };
		Row2 = { 0.0f, 0.0f, 1.0f, 0.0f };
		Row3 = { 0.0f, 0.0f, 0.0f, 1.0f };
	}
}
