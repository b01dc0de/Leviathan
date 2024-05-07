#include "LvCommon.h"

namespace Leviathan
{
	/*
	void fMatrix::Identity()
	{
		Row0 = { 1.0f, 0.0f, 0.0f, 0.0f };
		Row1 = { 0.0f, 1.0f, 0.0f, 0.0f };
		Row2 = { 0.0f, 0.0f, 1.0f, 0.0f };
		Row3 = { 0.0f, 0.0f, 0.0f, 1.0f };
	}
	*/

	const float PI = 3.14159265359f;

	float Clamp(float C, float A, float B)
	{
		Assert(A <= B);
		if (C < A)
		{
			C = A;
		}
		else if (C > B)
		{
			C = B;
		}
		return C;
	}

	int Clamp(int C, int A, int B)
	{
		Assert(A <= B);
		if (C < A)
		{
			C = A;
		}
		else if (C > B)
		{
			C = B;
		}
		return C;
	}

	float ClampToUnit(float C)
	{
		if (C < 0.0f)
		{
			C = 0.0f;
		}
		else if (C > 1.0f)
		{
			C = 1.0f;
		}
		return C;
	}

	float Min(float A, float B)
	{
		return A < B ? A : B;
	}

	float Max(float A, float B)
	{
		return A > B ? A : B;
	}

	int Min(int A, int B)
	{
		return A < B ? A : B;
	}

	int Max(int A, int B)
	{
		return A > B ? A : B;
	}

	float Lerp(float A, float B, float T)
	{
		return A + (B - A) * T;
	}

	fVector Lerp(fVector A, fVector B, float T)
	{
		return A + (B - A) * T;
	}

	float Dot(const fVector& A, const fVector& B)
	{
		return A.X * B.X + A.Y * B.Y + A.Z * B.Z;
	}

	fVector Cross(const fVector& A, const fVector& B)
	{
		fVector Result;

		Result.X = A.Y * B.Z - A.Z * B.Y;
		Result.Y = A.Z * B.X - A.X * B.Z;
		Result.Z = A.X * B.Y - B.X * A.Y;
		Result.W = 0.0f;

		return Result;
	}

	float Proj(const fVector& A, const fVector& B)
	{
		return Dot(A, B.GetNorm());
	}

	float Dist(const fVector& A, const fVector& B)
	{
		return sqrtf(DistSq(A, B));
	}

	float DistSq(const fVector& A, const fVector& B)
	{
		float DiffX = B.X - A.X;
		float DiffY = B.Y - A.Y;
		float DiffZ = B.Z - A.Z;

		return DiffX * DiffX + DiffY * DiffY + DiffZ * DiffZ;
	}

	fVector& fVector::Norm()
	{
		float Len = Length();
		Assert(Len != 0.0f);
		X = X / Len;
		Y = Y / Len;
		Z = Z / Len;
		return *this;
	}

	fVector fVector::GetNorm() const
	{
		fVector Result = *this;
		return Result.Norm();
	}

	float fVector::LengthSq() const
	{
		return X * X + Y * Y + Z * Z;
	}

	float fVector::Length() const
	{
		return sqrtf(LengthSq());
	}

	fVector fVector::operator-() const
	{
		fVector Result;
		Result.X = -X;
		Result.Y = -Y;
		Result.Z = -Z;
		return Result;
	}

	fVector fVector::operator+(const fVector& B) const
	{
		fVector Result;
		Result.X = X + B.X;
		Result.Y = Y + B.Y;
		Result.Z = Z + B.Z;
		Result.W = W;
		return Result;
	}

	fVector fVector::operator-(const fVector& B) const
	{
		fVector Result;
		Result.X = X - B.X;
		Result.Y = Y - B.Y;
		Result.Z = Z - B.Z;
		return Result;
	}

	fVector fVector::operator*(float s) const
	{
		fVector Result;
		Result.X = X * s;
		Result.Y = Y * s;
		Result.Z = Z * s;
		return Result;
	}

	fVector fVector::operator/(float s) const
	{
		fVector Result;
		Result.X = X / s;
		Result.Y = Y / s;
		Result.Z = Z / s;
		return Result;
	}

	fVector fVector::operator*(const fVector& B) const
	{
		fVector Result;
		Result.X = X * B.X;
		Result.Y = Y * B.Y;
		Result.Z = Z * B.Z;
		return Result;
	}

	fVector fVector::operator/(const fVector& B) const
	{
		fVector Result;
		Result.X = X / B.X;
		Result.Y = Y / B.Y;
		Result.Z = Z / B.Z;
		return Result;
	}

	fVector& fVector::operator+=(const fVector& B)
	{
		*this = operator+(B);
		return *this;
	}

	fVector& fVector::operator-=(const fVector& B)
	{
		*this = operator-(B);
		return *this;
	}

	fVector& fVector::operator*=(float S)
	{
		*this = operator*(S);
		return *this;
	}

	fVector& fVector::operator/=(float S)
	{
		*this = operator/(S);
		return *this;
	}

	fVector& fVector::operator*=(const fVector& B)
	{
		*this = operator*(B);
		return *this;
	}

	fVector& fVector::operator/=(const fVector& B)
	{
		*this = operator/(B);
		return *this;
	}

	fMatrix Mult(const fMatrix& A, const fMatrix& B)
	{
		fMatrix Result;

		Result.Row0.X = (A.Row0.X * B.Row0.X) + (A.Row0.Y * B.Row1.X) + (A.Row0.Z * B.Row2.X) + (A.Row0.W * B.Row3.X);
		Result.Row0.Y = (A.Row0.X * B.Row0.Y) + (A.Row0.Y * B.Row1.Y) + (A.Row0.Z * B.Row2.Y) + (A.Row0.W * B.Row3.Y);
		Result.Row0.Z = (A.Row0.X * B.Row0.Z) + (A.Row0.Y * B.Row1.Z) + (A.Row0.Z * B.Row2.Z) + (A.Row0.W * B.Row3.Z);
		Result.Row0.W = (A.Row0.X * B.Row0.W) + (A.Row0.Y * B.Row1.W) + (A.Row0.Z * B.Row2.W) + (A.Row0.W * B.Row3.W);

		Result.Row1.X = (A.Row1.X * B.Row0.X) + (A.Row1.Y * B.Row1.X) + (A.Row1.Z * B.Row2.X) + (A.Row1.W * B.Row3.X);
		Result.Row1.Y = (A.Row1.X * B.Row0.Y) + (A.Row1.Y * B.Row1.Y) + (A.Row1.Z * B.Row2.Y) + (A.Row1.W * B.Row3.Y);
		Result.Row1.Z = (A.Row1.X * B.Row0.Z) + (A.Row1.Y * B.Row1.Z) + (A.Row1.Z * B.Row2.Z) + (A.Row1.W * B.Row3.Z);
		Result.Row1.W = (A.Row1.X * B.Row0.W) + (A.Row1.Y * B.Row1.W) + (A.Row1.Z * B.Row2.W) + (A.Row1.W * B.Row3.W);

		Result.Row2.X = (A.Row2.X * B.Row0.X) + (A.Row2.Y * B.Row1.X) + (A.Row2.Z * B.Row2.X) + (A.Row2.W * B.Row3.X);
		Result.Row2.Y = (A.Row2.X * B.Row0.Y) + (A.Row2.Y * B.Row1.Y) + (A.Row2.Z * B.Row2.Y) + (A.Row2.W * B.Row3.Y);
		Result.Row2.Z = (A.Row2.X * B.Row0.Z) + (A.Row2.Y * B.Row1.Z) + (A.Row2.Z * B.Row2.Z) + (A.Row2.W * B.Row3.Z);
		Result.Row2.W = (A.Row2.X * B.Row0.W) + (A.Row2.Y * B.Row1.W) + (A.Row2.Z * B.Row2.W) + (A.Row2.W * B.Row3.W);

		Result.Row3.X = (A.Row3.X * B.Row0.X) + (A.Row3.Y * B.Row1.X) + (A.Row3.Z * B.Row2.X) + (A.Row3.W * B.Row3.X);
		Result.Row3.Y = (A.Row3.X * B.Row0.Y) + (A.Row3.Y * B.Row1.Y) + (A.Row3.Z * B.Row2.Y) + (A.Row3.W * B.Row3.Y);
		Result.Row3.Z = (A.Row3.X * B.Row0.Z) + (A.Row3.Y * B.Row1.Z) + (A.Row3.Z * B.Row2.Z) + (A.Row3.W * B.Row3.Z);
		Result.Row3.W = (A.Row3.X * B.Row0.W) + (A.Row3.Y * B.Row1.W) + (A.Row3.Z * B.Row2.W) + (A.Row3.W * B.Row3.W);

		return Result;
	}
	fMatrix Mult(const fMatrix& A, const fMatrix& B, const fMatrix& C)
	{
		return Mult(Mult(A, B), C);
	}
	fMatrix Mult(const fMatrix& A, const fMatrix& B, const fMatrix& C, const fMatrix& D)
	{
		return Mult(Mult(Mult(A, B), C), D);
	}
	fVector Mult(const fVector& fV, const fMatrix& fM)
	{
		fVector Result;

		// row major
		Result.X = fV.X * fM.Row0.X + fV.Y * fM.Row1.X + fV.Z * fM.Row2.X + fV.W * fM.Row3.X;
		Result.Y = fV.X * fM.Row0.Y + fV.Y * fM.Row1.Y + fV.Z * fM.Row2.Y + fV.W * fM.Row3.Y;
		Result.Z = fV.X * fM.Row0.Z + fV.Y * fM.Row1.Z + fV.Z * fM.Row2.Z + fV.W * fM.Row3.Z;
		Result.W = fV.X * fM.Row0.W + fV.Y * fM.Row1.W + fV.Z * fM.Row2.W + fV.W * fM.Row3.W;

		/*
		// column major
		Result.X = fM.Row0.X*fV.X + fM.Row0.Y*fV.Y + fM.Row0.Z*fV.Z + fM.Row0.W*fV.W;
		Result.Y = fM.Row1.X*fV.X + fM.Row1.Y*fV.Y + fM.Row1.Z*fV.Z + fM.Row1.W*fV.W;
		Result.Z = fM.Row2.X*fV.X + fM.Row2.Y*fV.Y + fM.Row2.Z*fV.Z + fM.Row2.W*fV.W;
		Result.W = fM.Row3.X*fV.X + fM.Row3.Y*fV.Y + fM.Row3.Z*fV.Z + fM.Row3.W*fV.W;
		*/

		return Result;
	}

	fVector operator*(const fVector& fV, const fMatrix& fM)
	{
		return Mult(fV, fM);
	}
	fVector& operator*=(fVector& fV, const fMatrix& fM)
	{
		return fV = Mult(fV, fM);
	}
	fMatrix operator*(const fMatrix& A, const fMatrix& B)
	{
		return Mult(A, B);
	}
	fMatrix& operator*=(fMatrix& A, const fMatrix& B)
	{
		return A = Mult(A, B);
	}

	fMatrix& fMatrix::Zero()
	{
		Row0 = { 0.0f, 0.0f, 0.0f, 0.0f };
		Row1 = { 0.0f, 0.0f, 0.0f, 0.0f };
		Row2 = { 0.0f, 0.0f, 0.0f, 0.0f };
		Row3 = { 0.0f, 0.0f, 0.0f, 0.0f };
		return *this;
	}
	fMatrix& fMatrix::Identity()
	{
		Row0 = { 1.0f, 0.0f, 0.0f, 0.0f };
		Row1 = { 0.0f, 1.0f, 0.0f, 0.0f };
		Row2 = { 0.0f, 0.0f, 1.0f, 0.0f };
		Row3 = { 0.0f, 0.0f, 0.0f, 1.0f };
		return *this;
	}

	fMatrix& fMatrix::T()
	{
		float tmp;

		// _1 <-> _4
		tmp = Row0.Y;
		Row0.Y = Row1.X;
		Row1.X = tmp;

		// _2 <-> _8
		tmp = Row0.Z;
		Row0.Z = Row2.X;
		Row2.X = tmp;

		// _3 <-> _c
		tmp = Row0.W;
		Row0.W = Row3.X;
		Row3.X = tmp;

		// _6 <-> _9
		tmp = Row1.Z;
		Row1.Z = Row2.Y;
		Row2.Y = tmp;

		// _7 <-> _d
		tmp = Row1.W;
		Row1.W = Row3.Y;
		Row3.Y = tmp;

		// _b <-> e
		tmp = Row2.W;
		Row2.W = Row3.Z;
		Row3.Z = tmp;

		return *this;
	}
	fMatrix fMatrix::GetT() const
	{
		fMatrix Result = *this;
		return Result.T();
	}

	fMatrix& fMatrix::QInv()
	{
		float tmp;

		// _1 <-> _4
		tmp = Row0.Y;
		Row0.Y = Row1.X;
		Row1.X = tmp;

		// _2 <-> _8
		tmp = Row0.Z;
		Row0.Z = Row2.X;
		Row2.X = tmp;

		// _6 <-> _9
		tmp = Row1.Z;
		Row1.Z = Row2.Y;
		Row2.Y = tmp;

		// "negative-ing" the pos
		Row3.X = -Row3.X;
		Row3.Y = -Row3.Y;
		Row3.Z = -Row3.Z;

		return *this;
	}
	fMatrix fMatrix::GetQInv() const
	{
		fMatrix Result = *this;
		return Result.T();
	}

	fMatrix& fMatrix::Trans(float X, float Y, float Z)
	{
		Row0 = { 1.0f, 0.0f, 0.0f, 0.0f };
		Row1 = { 0.0f, 1.0f, 0.0f, 0.0f };
		Row2 = { 0.0f, 0.0f, 1.0f, 0.0f };
		Row3 = { X,    Y,    Z, 1.0f };
		return *this;
	}
	fMatrix& fMatrix::Trans(const fVector& T)
	{
		Row0 = { 1.0f, 0.0f, 0.0f, 0.0f };
		Row1 = { 0.0f, 1.0f, 0.0f, 0.0f };
		Row2 = { 0.0f, 0.0f, 1.0f, 0.0f };
		Row3 = { T.X,  T.Y,  T.Z, 1.0f };
		return *this;
	}
	fMatrix& fMatrix::Rot(MatrixRotation _Rot, float Angle)
	{
		Identity();
		float Cos = cosf(Angle);
		float Sin = sinf(Angle);
		switch (_Rot)
		{
			case ROT_X:
			{
				Row0 = { 1.0f, 0.0f, 0.0f, 0.0f };
				Row1 = { 0.0f,  Cos, -Sin, 0.0f };
				Row2 = { 0.0f,  Sin,  Cos, 0.0f };
			} break;
			case ROT_Y:
			{
				Row0 = {  Cos, 0.0f,  Sin, 0.0f };
				Row1 = { 0.0f, 1.0f, 0.0f, 0.0f };
				Row2 = { -Sin, 0.0f,  Cos, 0.0f };
			} break;
			case ROT_Z:
			{
				Row0 = { Cos,  -Sin, 0.0f, 0.0f };
				Row1 = { Sin,   Cos, 0.0f, 0.0f };
				Row2 = { 0.0f, 0.0f, 1.0f, 0.0f };
			} break;
			default:
			{
				Assert(false);
			} break;
		}
		return *this;

	}
	fMatrix& fMatrix::Rot(MatrixRotationAxis _Rot, const fVector& Axis, float Angle)
	{
		/*
		float C = cosf(Angle);
		float s = sinf(Angle);
		float c_opp = 1.0f - C;
		fVector axis_norm = axis.GetNorm();

		fVector  right( 1.0f, 0.0f, 0.0f, 0.0f );
		fVector up( 0.0f, 1.0f, 0.0f, 0.0f );
		fVector forward( 0.0f, 0.0f, 1.0f, 0.0f );

		Row0 = (right * C) + (Cross(axis_norm, right) * s) + (axis_norm * c_opp);
		Row1 = (up * C) + (Cross(axis_norm, up) * s) + (axis_norm * c_opp);
		Row2 = (forward * C) + (Cross(axis_norm, forward) * s) + (axis_norm * c_opp);
		Row3 = { 0.0f, 0.0f, 0.0f, 1.0f };

		if ((Row0.Y > 0.0f ? Row0.Y : -Row0.Y) > 0.0f)
		{
			int i = 0;
			Outf("%D\n", i);
		}

		//static int i = 0;
		*/


		fVector k = Axis.GetNorm();

		float C = cosf(Angle);
		float S = sinf(Angle);
		float C_inv = 1.0f - C;

		float k_xx = k.X * k.X;
		float k_yy = k.Y * k.Y;
		float k_zz = k.Z * k.Z;

		float k_xy = k.X * k.Y;
		float k_yz = k.Y * k.Z;
		float k_xz = k.X * k.Z;

		float S_x = S * k.X;
		float S_y = S * k.Y;
		float S_z = S * k.Z;

		Row0 = { C + k_xx * C_inv, +S_z + k_xy * C_inv, -S_y + k_xz * C_inv, 0.0f };
		Row1 = { -S_z + k_xy * C_inv, C + k_yy * C_inv, +S_x + k_yz * C_inv, 0.0f };
		Row2 = { +S_y + k_xz * C_inv, -S_x + k_yz * C_inv, C + k_zz * C_inv, 0.0f };
		Row3 = { 0.0f, 0.0f, 0.0f, 1.0f };

		/*
		Identity();
		V4 k = axis.GetNorm();

		float X = k.X;
		float Y = k.Y;
		float Z = k.Z;

		float C = cosf(Angle);
		float s = sinf(Angle);
		float T = 1.0f - C;

		Row0.X = (T*X*X) + C;
		Row0.Y = (T*X*Y) - (s*Z);
		Row0.Z = (T*X*Z) + (s*Y);

		Row1.X = (T*X*Y) + (s*Z);
		Row1.Y = (T*Y*Y) + C;
		Row1.Z = (T*Y*Z) - (s*X);

		Row2.X = (T*X*Z) - (s*Y);
		Row2.Y = (T*Y*Z) + (s*X);
		Row2.Z = (T*Z*Z) + C;
		*/

		return *this;
	}
	fMatrix& fMatrix::Rot(MatrixRotationOrient _Rot, const fVector& Dir, const fVector& Up)
	{
		Row2 = Dir.GetNorm();
		Row0 = Cross(Up, Row2).GetNorm();
		Row1 = Cross(Row2, Row0).GetNorm();
		Row3 = { 0.0f, 0.0f, 0.0f, 1.0f };
		return *this;
	}
	fMatrix& fMatrix::Scale(float S)
	{
		Identity();
		Row0.X = S;
		Row1.Y = S;
		Row2.Z = S;
		return *this;
	}
	fMatrix& fMatrix::Scale(float SX, float SY, float SZ)
	{
		Identity();
		Row0.X = SX;
		Row1.Y = SY;
		Row2.Z = SZ;
		return *this;
	}

	fMatrix::fMatrix(MatrixZero _Zero)
	{
		Zero();
	}
	fMatrix::fMatrix(MatrixIdentity _Identity)
	{
		Identity();
	}
	fMatrix::fMatrix(MatrixTranslate _Trans, float X, float Y, float Z)
	{
		Trans(X, Y, Z);
	}
	fMatrix::fMatrix(MatrixTranslate _Trans, const fVector& T)
	{
		Trans(T);
	}
	fMatrix::fMatrix(MatrixRotation _Rot, float Angle)
	{
		Rot(_Rot, Angle);
	}
	fMatrix::fMatrix(MatrixRotationAxis _Rot, const fVector& Axis, float Angle)
	{
		Rot(_Rot, Axis, Angle);
	}
	fMatrix::fMatrix(MatrixRotationOrient _Rot, const fVector& Dir, const fVector& Up)
	{
		Rot(_Rot, Dir, Up);
	}
	fMatrix::fMatrix(MatrixScale _Scale, float S)
	{
		Scale(S);
	}
	fMatrix::fMatrix(MatrixScale scale, float SX, float SY, float SZ)
	{
		Scale(SX, SY, SZ);
	}
}
