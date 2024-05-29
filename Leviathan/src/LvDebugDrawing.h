#ifndef LVDEBUGDRAWING_H
#define LVDEBUGDRAWING_H

namespace Leviathan
{
	enum LVDEBUG_SHAPE_TYPE
	{
		LVDEBUG_SHAPE_POINT,
		LVDEBUG_SHAPE_LINE,
		LVDEBUG_SHAPE_SQUARE,
		LVDEBUG_SHAPE_BOX,
		LVDEBUG_SHAPE_TRIANGLE,
		LVDEBUG_SHAPE_CIRCLE,
		LVDEBUG_SHAPE_SPHERE,
		//LVDEBUG_SHAPE_REGION, // CKA_TODO: Is this viable?
		LVDEBUG_SHAPE_MAX
	};

	struct LvDebugShapeParameters
	{
		struct PointShape
		{
			fVector P{};
		};
		struct LineShape
		{
			fVector A{};
			fVector B{};
		};
		struct SquareShape
		{
			fVector Min{};
			fVector Max{};
		};
		struct BoxShape
		{
			fVector Min{};
			fVector Max{};
		};
		struct TriangleShape
		{
			fVector A{};
			fVector B{};
			fVector C{};
		};
		struct CircleShape
		{
			fVector Center{};
			float Radius{ 0.0f };
		};
		struct SphereShape
		{
			fVector Center{};
			float Radius{ 0.0f };
		};
		union
		{
			// POINT: fVector
			// LINE: fVector[2]
			// SQUARE: fVector[2]
			// BOX: fVector[2]
			// TRIANGLE: fVector[3]
			// CIRCLE: fVector, float
			// SPHERE: fVector, float
			PointShape Point;
			LineShape Line;
			SquareShape Square;
			BoxShape Box;
			TriangleShape Triangle;
			CircleShape Circle;
			SphereShape Sphere;
		};
		// CKA_TODO: Region shape

		LV_STRUCT_DEFAULT_BIG4(LvDebugShapeParameters);
	};

	using DbgShapeIDType = u32;
	struct LvDebugShape
	{
		static constexpr float DefaultThiccness{ 1.0f };
		static const fVector DefaultColor;

		LVDEBUG_SHAPE_TYPE Type;
		DbgShapeIDType ID{ 0u };
		fVector Color{DefaultColor};
		LvDebugShapeParameters Params;
		float fThiccness{ DefaultThiccness };

		LV_STRUCT_DEFAULT_BIG4(LvDebugShape);
	};


	struct LvDebugDrawing
	{
		u32 IDCounter{ 0u };
		LvArray<LvDebugShape> BatchPersistent;
		LvArray<LvDebugShape> BatchTransient;

		void PvDraw(ID3D11DeviceContext* InContext);
		static void Draw(ID3D11DeviceContext* InContext);

		DbgShapeIDType PvAddShape(LVDEBUG_SHAPE_TYPE ShapeType, const LvDebugShapeParameters& InParams, const fVector& InColor = LvDebugShape::DefaultColor, bool bPersis = true, float InThiccness = LvDebugShape::DefaultThiccness);
		static DbgShapeIDType AddShape(LVDEBUG_SHAPE_TYPE ShapeType, const LvDebugShapeParameters& InParams, const fVector& InColor = LvDebugShape::DefaultColor, bool bPersis = true, float InThiccness = LvDebugShape::DefaultThiccness);

		static DbgShapeIDType AddPoint(const fVector& InP, const fVector& InColor = LvDebugShape::DefaultColor, bool bPersis = true, float InThiccness = LvDebugShape::DefaultThiccness);
		static DbgShapeIDType AddLine(const fVector& InA, const fVector& InB, const fVector& InColor = LvDebugShape::DefaultColor, bool bPersis = true, float InThiccness = LvDebugShape::DefaultThiccness);
		static DbgShapeIDType AddSquare(const fVector& InMin, const fVector& InMax, const fVector& InColor = LvDebugShape::DefaultColor, bool bPersis = true, float InThiccness = LvDebugShape::DefaultThiccness);
		static DbgShapeIDType AddBox(const fVector& InMin, const fVector& InMax, const fVector& InColor = LvDebugShape::DefaultColor, bool bPersis = true, float InThiccness = LvDebugShape::DefaultThiccness);
		static DbgShapeIDType AddTriangle(const fVector& InA, const fVector& InB, const fVector& InC, const fVector& InColor = LvDebugShape::DefaultColor, bool bPersis = true, float InThiccness = LvDebugShape::DefaultThiccness);
		static DbgShapeIDType AddCircle(const fVector& InCenter, float InRadius, const fVector& InColor = LvDebugShape::DefaultColor, bool bPersis = true, float InThiccness = LvDebugShape::DefaultThiccness);
		static DbgShapeIDType AddSphere(const fVector& InCenter, float InRadius, const fVector& InColor = LvDebugShape::DefaultColor, bool bPersis = true, float InThiccness = LvDebugShape::DefaultThiccness);
	};
}

#endif // LVDEBUGDRAWING_H