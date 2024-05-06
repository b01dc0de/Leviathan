#ifndef LVMATH_H
#define LVMATH_H

namespace Leviathan
{
	struct fVertex
	{
		float X = 0.0f;
		float Y = 0.0f;
		float Z = 0.0f;
		float W = 0.0f;
	};

	struct fMatrix
	{
		fVertex Row0;
		fVertex Row1;
		fVertex Row2;
		fVertex Row3;

		void Identity();
	};
}

#endif // LVMATH_H