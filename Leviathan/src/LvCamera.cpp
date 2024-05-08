#include "LvCommon.h"

#include "LvCamera.h"

namespace Leviathan
{
	CameraSpaceTrans CameraSpaceTrans::Perspective
	(
		float InFOV_Y,
		float InAspectRatio,
		float InNearDist,
		float InFarDist,
		const fVector& InPos,
		const fVector& InUp,
		const fVector& InForward,
		const fVector& InRight,
		const fMatrix& InNDC
	)
	{
		CameraSpaceTrans OutCamera;
		OutCamera.Proj.Zero();
		OutCamera.View.Zero();

		float fD = 1.0f / tanf(InFOV_Y / 2.0f);
		float fDistDelta = InFarDist - InNearDist;

		OutCamera.Proj.Row0.X = fD / InAspectRatio;
		OutCamera.Proj.Row1.Y = fD;
		OutCamera.Proj.Row2.Z = -(InFarDist + InNearDist) / fDistDelta;
		OutCamera.Proj.Row2.W = -1.0f;
		OutCamera.Proj.Row3.Z = (-2.0f * InFarDist * InNearDist) / fDistDelta;
		OutCamera.Proj *= InNDC;

		OutCamera.View.Row0 = { InRight.X, InUp.X, InForward.X, 0.0f };
		OutCamera.View.Row1 = { InRight.Y, InUp.Y, InForward.Y, 0.0f };
		OutCamera.View.Row2 = { InRight.Z, InUp.Z, InForward.Z, 0.0f };
		OutCamera.View.Row3.X = -Dot(InPos, InRight);
		OutCamera.View.Row3.Y = -Dot(InPos, InUp);
		OutCamera.View.Row3.Z = -Dot(InPos, InForward);
		OutCamera.View.Row3.W = 1.0f;

		return OutCamera;
	}

	CameraSpaceTrans CameraSpaceTrans::Orthographic(float InWidth, float InHeight, float InDepth)
	{
		CameraSpaceTrans OutCamera;

		OutCamera.Proj.Row0.X = +2.0f / InWidth;
		OutCamera.Proj.Row1.Y = +2.0f / InHeight;
		OutCamera.View.Row2.Z = -2.0f / InDepth;

		return OutCamera;
	}

	void LvCameraPerspective::Orient(const fVector& InWorldPos, const fVector& InLookAtPos, const fVector& InAbsUp)
	{
		WorldPos = InWorldPos;
		LookAtPos = InLookAtPos;

		vForward = -(LookAtPos - WorldPos);
		vForward.Norm();

		vRight = Cross(InAbsUp, vForward);
		vRight.Norm();

		vUp = Cross(vForward, vRight);
		vUp.Norm();

		CamTrans = CameraSpaceTrans::Perspective
		(
			FOV_Y,
			AspectRatio,
			NearDist,
			FarDist,
			WorldPos,
			vUp,
			vForward,
			vRight,
			NDC
		);
	}

	LvCameraPerspective::LvCameraPerspective(const fVector& InWorldPos, const fVector& InLookAtPos, const fVector& InAbsUp)
		: Width((float)ResX)
		, Height((float)ResY)
		//, Depth(1.0f)
		, FOV_Y(45.0f)
		, AspectRatio((float)ResX / (float)ResY)
		, NearDist(1.0f)
		, FarDist(1000.0f)
		, NDC(Mult(Matrix(TRANS, fVector(0.0f, 0.0f, 1.0f, 1.0f)), Matrix(SCALE, 1.0f, 1.0f, 0.5f)))
	{
		Orient(InWorldPos, InLookAtPos, InAbsUp);
	}

	LvCameraOrthographic::LvCameraOrthographic()
		: CamTrans(CameraSpaceTrans::Orthographic((float)ResX, (float)ResY, 1.0f))
		, Width((float)ResX)
		, Height((float)ResY)
		, Depth(1.0f)
	{
	}
}
