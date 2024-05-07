#ifndef LVMATH_H
#define LVMATH_H

namespace Leviathan
{
	// General math utils
	float Clamp(float c, float a, float b);
	int Clamp(int c, int a, int b);
	float ClampToUnit(float c);

	float Min(float a, float b);
	float Max(float a, float b);
	int Min(int a, int b);
	int Max(int a, int b);

	struct fVector
	{
		float X;
		float Y;
		float Z;
		float W;

		fVector& Norm();
		fVector GetNorm() const;
		float LengthSq() const;
		float Length() const;

		fVector operator-() const;

		fVector operator+(const fVector& b) const;
		fVector operator-(const fVector& b) const;
		fVector operator*(float s) const;
		fVector operator/(float s) const;
		fVector operator*(const fVector& b) const;
		fVector operator/(const fVector& b) const;

		fVector& operator+=(const fVector& b);
		fVector& operator-=(const fVector& b);
		fVector& operator*=(float s);
		fVector& operator/=(float s);
		fVector& operator*=(const fVector& b);
		fVector& operator/=(const fVector& b);

		fVector(float InX = 0.0f, float InY = 0.0f, float InZ = 0.0f, float InW = 1.0f) : X(InX) , Y(InY) , Z(InZ) , W(InW) { }
		fVector(const fVector&) = default;
		fVector& operator=(const fVector&) = default;
		~fVector() = default;
	};

	enum MatrixZero { ZERO };
	enum MatrixIdentity { IDENTITY };
	enum MatrixTranslate { TRANS };
	enum MatrixScale { SCALE };
	enum MatrixRotation { ROT_X, ROT_Y, ROT_Z };
	enum MatrixRotationAxis { ROT_AXIS };
	enum MatrixRotationOrient { ROT_ORIENT };

	struct fMatrix
	{
		fVector Row0;
		fVector Row1;
		fVector Row2;
		fVector Row3;

		fMatrix& Zero();
		fMatrix& Identity();

		fMatrix& T();
		fMatrix GetT() const;

		fMatrix& QInv();
		fMatrix GetQInv() const;

		fMatrix& Trans(float X, float Y, float Z);
		fMatrix& Trans(const fVector& T);
		fMatrix& Rot(MatrixRotation Rot, float Angle);
		fMatrix& Rot(MatrixRotationAxis Rot, const fVector& Axis, float Angle);
		fMatrix& Rot(MatrixRotationOrient Rot, const fVector& dir, const fVector& up = fVector(0.0f, 1.0f, 0.0f, 0.0f));
		fMatrix& Scale(float S);
		fMatrix& Scale(float SX, float SY, float SZ);

		fMatrix() = default;
		fMatrix(MatrixZero Zero);
		fMatrix(MatrixIdentity Identity);
		fMatrix(MatrixTranslate Trans, float X, float Y, float Z);
		fMatrix(MatrixTranslate Trans, const fVector& T);
		fMatrix(MatrixRotation Rot, float Angle);
		fMatrix(MatrixRotationAxis Rot, const fVector& Axis, float Angle);
		fMatrix(MatrixRotationOrient Rot, const fVector& Dir, const fVector& Up = fVector(0.0f, 1.0f, 0.0f, 0.0f));
		fMatrix(MatrixScale Scale, float S);
		fMatrix(MatrixScale Scale, float SX, float SY, float SZ);
	};

	// Common Vector operations
	float Lerp(float a, float b, float t);
	fVector Lerp(fVector a, fVector b, float t);

	float Dot(const fVector& a, const fVector& b);
	fVector Cross(const fVector& a, const fVector& b);
	float Proj(const fVector& a, const fVector& b);
	float Dist(const fVector& a, const fVector& b);
	float DistSq(const fVector& a, const fVector& b);

	fMatrix Mult(const fMatrix& a, const fMatrix& b);
	fMatrix Mult(const fMatrix& a, const fMatrix& b, const fMatrix& c);
	fMatrix Mult(const fMatrix& a, const fMatrix& b, const fMatrix& c, const fMatrix& d);
	fVector Mult(const fVector& v, const fMatrix& m);

	fVector operator*(const fVector& v, const fMatrix& m);
	fVector& operator*=(const fVector& v, const fMatrix& m);
	fMatrix operator*(const fMatrix& a, const fMatrix& b);
	fMatrix& operator*=(fMatrix& a, const fMatrix& b);

	using Vector = fVector;
	using V4 = fVector;
	using Matrix = fMatrix;
	using M4 = fMatrix;
}

#endif // LVMATH_H