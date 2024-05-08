#ifndef LVCAMERA_H
#define LVCAMERA_H

namespace Leviathan
{
	struct CameraSpaceTrans
	{
		fMatrix View;
		fMatrix Proj;

		CameraSpaceTrans()
		{
			View.Identity();
			Proj.Identity();
		}

		static CameraSpaceTrans Perspective
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
		);

		static CameraSpaceTrans Orthographic(float InWidth, float InHeight, float InDepth);
	};

	struct LvCameraPerspective
	{
		CameraSpaceTrans CamTrans;

		const float Width;
		const float Height;
		//const float Depth;

		float FOV_Y;
		float AspectRatio;
		float NearDist;
		float FarDist;

		fVector vUp;
		fVector vForward;
		fVector vRight;

		fVector WorldPos;
		fVector LookAtPos;

		fMatrix NDC;

		void Orient(const fVector& InWorldPos, const fVector& InLookAtPos, const fVector& InAbsUp);
		LvCameraPerspective(const fVector& InWorldPos, const fVector& InLookAtPos, const fVector& InAbsUp);
	};

	struct LvCameraOrthographic
	{
		const CameraSpaceTrans CamTrans;

		const float Width;
		const float Height;
		const float Depth;

		// LvCameraOrthographic(float InWidth = ResX, float InHeight = ResY, float InDepth = 1.0f)
		LvCameraOrthographic();
	};
}

#endif // LV_CAMERA_H