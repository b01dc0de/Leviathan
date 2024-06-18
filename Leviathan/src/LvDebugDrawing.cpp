#include "LvCommon.h"
#include "LvGraphics.h"
#include "LvMesh.h"

#include "LvDebugDrawing.h"

namespace Leviathan
{
	const fVector LvDebugShape::DefaultColor{ 1.0f, 0.0f, 1.0f };

	struct LvDebugDrawingInstance
	{
		DbgShapeIDType IDCounter{ 0u };
		LvArray<LvDebugShape> BatchPersistent;
		LvArray<LvDebugShape> BatchTransient;

		void Draw(ID3D11DeviceContext* InContext);
		bool RemoveShape(DbgShapeIDType ID);
		DbgShapeIDType AddShape(LVDEBUG_SHAPE_TYPE ShapeType, const LvDebugShapeParameters& InParams, const fVector& InColor = LvDebugShape::DefaultColor, bool bPersis = true, float InThiccness = LvDebugShape::DefaultThiccness);
	};

	static LvDebugDrawingInstance Inst;

	void LvDebugDrawingInstance::Draw(ID3D11DeviceContext* InContext)
	{

		auto GetTopologyType = [](LVDEBUG_SHAPE_TYPE ShapeType) -> D3D11_PRIMITIVE_TOPOLOGY
		{
			D3D11_PRIMITIVE_TOPOLOGY Result = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
			switch (ShapeType)
			{
				case LVDEBUG_SHAPE_POINT:
				{
					Result = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
				} break;
				case LVDEBUG_SHAPE_LINE:
				{
					Result = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
				} break;
				case LVDEBUG_SHAPE_SQUARE:
				{
					Result = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
				} break;
				case LVDEBUG_SHAPE_BOX:
				{
					Result = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
				} break;
				case LVDEBUG_SHAPE_TRIANGLE:
				{
					Result = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				} break;
				case LVDEBUG_SHAPE_CIRCLE:
				{
					Result = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				} break;
				case LVDEBUG_SHAPE_SPHERE:
				{
					Result = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				} break;
				default:
				{
					Result = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
				} break;
			}
			return Result;
		};

		VertexColor VxPoint{ fVector{0.0f, 0.0f, 0.0f}, fVector{1.0f, 0.0f, 0.0f} };
		VertexColor VxLine[] = {
			{fVector{0.0f, 0.0f, 0.0f}, VxPoint.Color},
			{fVector{0.0f, 1.0f, 0.0f}, VxPoint.Color}
		};
		VertexColor VxSquare[] = {
			{fVector{0.0f, 0.0f, 0.0f}, VxPoint.Color},
			{fVector{1.0f, 0.0f, 0.0f}, VxPoint.Color},
			{fVector{1.0f, 1.0f, 0.0f}, VxPoint.Color},
			{fVector{0.0f, 1.0f, 0.0f}, VxPoint.Color},
		};
		VertexColor VxBox[] = {
			{fVector{0.0f, 0.0f, 0.0f}, VxPoint.Color},
			{fVector{1.0f, 0.0f, 0.0f}, VxPoint.Color},
			{fVector{1.0f, 1.0f, 0.0f}, VxPoint.Color},
			{fVector{0.0f, 1.0f, 0.0f}, VxPoint.Color},
			{fVector{0.0f, 0.0f, 1.0f}, VxPoint.Color},
			{fVector{1.0f, 0.0f, 1.0f}, VxPoint.Color},
			{fVector{1.0f, 1.0f, 1.0f}, VxPoint.Color},
			{fVector{0.0f, 1.0f, 1.0f}, VxPoint.Color},
		};
		const float TriangleLength = 1.1547f; // ~ 2/sqrt(3)
		VertexColor VxTriangle[] = {
			{fVector{0.0f, 1.0f, 0.0f}, VxPoint.Color},
			{fVector{-0.5f*TriangleLength, 0.0f, 0.0f}, VxPoint.Color},
			{fVector{+0.5f*TriangleLength, 0.0f, 0.0f}, VxPoint.Color},
		};

		(void)InContext;
		{
			// Set common debug drawing state
		}

		{
			// Draw persistent debug shapes
			for (u32 PersisIdx = 0u; PersisIdx < BatchPersistent.NumItems; PersisIdx++)
			{
				// BatchPersistent[PersisIdx];
			}
		}

		{
			// Draw transient debug shapes
			for (u32 TransientIdx = 0u; TransientIdx < BatchTransient.NumItems; TransientIdx++)
			{
				// BatchTransient[TransientIdx];
			}
		}

		{
			// Clear transient shapes
			for (u32 TransientIdx = 0u; TransientIdx < BatchTransient.NumItems; TransientIdx++)
			{
				BatchTransient[TransientIdx].Type = LVDEBUG_SHAPE_INVALID;
				BatchTransient[TransientIdx].ID = 0u;
			}
			BatchTransient.Empty();
		}
	}

	bool LvDebugDrawingInstance::RemoveShape(DbgShapeIDType ID)
	{
		bool bRemoved = false;
		for (u32 PersisIdx = 0; PersisIdx < BatchPersistent.NumItems; PersisIdx++)
		{
			if (BatchPersistent[PersisIdx].ID == ID)
			{
				BatchPersistent.RemoveItemQ(PersisIdx);
				bRemoved = true;
				break;
			}
		}
		return bRemoved;
	}

	DbgShapeIDType LvDebugDrawingInstance::AddShape(LVDEBUG_SHAPE_TYPE ShapeType, const LvDebugShapeParameters& InParams, const fVector& InColor, bool bPersis, float InThiccness)
	{
		Assert(ShapeType != LVDEBUG_SHAPE_MAX);
		LvDebugShape NewShape{};
		NewShape.Type = ShapeType;
		NewShape.ID = IDCounter++;
		NewShape.Color = InColor;
		NewShape.fThiccness = InThiccness;
		switch (ShapeType)
		{
			case LVDEBUG_SHAPE_POINT:
			{
				NewShape.Params.Point = InParams.Point;
			} break;
			case LVDEBUG_SHAPE_LINE:
			{
				NewShape.Params.Line = InParams.Line;
			} break;
			case LVDEBUG_SHAPE_SQUARE:
			{
				NewShape.Params.Square = InParams.Square;
			} break;
			case LVDEBUG_SHAPE_BOX:
			{
				NewShape.Params.Box = InParams.Box;
			} break;
			case LVDEBUG_SHAPE_TRIANGLE:
			{
				NewShape.Params.Triangle = InParams.Triangle;
			} break;
			case LVDEBUG_SHAPE_CIRCLE:
			{
				NewShape.Params.Circle = InParams.Circle;
			} break;
			case LVDEBUG_SHAPE_SPHERE:
			{
				NewShape.Params.Sphere = InParams.Sphere;
			} break;
			default:
			{
				LV_FAIL();
			} break;
		}
		if (bPersis)
		{
			BatchPersistent.AddItem(NewShape);
		}
		else
		{
			BatchTransient.AddItem(NewShape);
		}
		return NewShape.ID;
	}

	void LvDebugDrawing::Draw(ID3D11DeviceContext* InContext)
	{
		Inst.Draw(InContext);
	}

	bool LvDebugDrawing::RemoveShape(DbgShapeIDType ID)
	{
		return Inst.RemoveShape(ID);
	}

	DbgShapeIDType LvDebugDrawing::AddShape(LVDEBUG_SHAPE_TYPE ShapeType, const LvDebugShapeParameters& InParams, const fVector& InColor, bool bPersis, float InThiccness)
	{
		return Inst.AddShape(ShapeType, InParams, InColor, bPersis, InThiccness);
	}

	DbgShapeIDType LvDebugDrawing::AddPoint(const fVector& InP, const fVector& InColor, bool bPersis, float InThiccness)
	{
		LvDebugShapeParameters Params{};
		Params.Point.P = InP;
		return AddShape(LVDEBUG_SHAPE_POINT, Params, InColor, bPersis, InThiccness);
	}
	
	DbgShapeIDType LvDebugDrawing::AddLine(const fVector& InA, const fVector& InB, const fVector& InColor, bool bPersis, float InThiccness)
	{
		LvDebugShapeParameters Params{};
		Params.Line.A = InA;
		Params.Line.B = InB;
		return AddShape(LVDEBUG_SHAPE_LINE, Params, InColor, bPersis, InThiccness);
	}

	DbgShapeIDType LvDebugDrawing::AddSquare(const fVector& InMin, const fVector& InMax, const fVector& InColor, bool bPersis, float InThiccness)
	{
		LvDebugShapeParameters Params{};
		Params.Square.Min = InMin;
		Params.Square.Max = InMax;
		return AddShape(LVDEBUG_SHAPE_SQUARE, Params, InColor, bPersis, InThiccness);
	}

	DbgShapeIDType LvDebugDrawing::AddBox(const fVector& InMin, const fVector& InMax, const fVector& InColor, bool bPersis, float InThiccness)
	{
		LvDebugShapeParameters Params{};
		Params.Box.Min = InMin;
		Params.Box.Max = InMax;
		return AddShape(LVDEBUG_SHAPE_BOX, Params, InColor, bPersis, InThiccness);
	}

	DbgShapeIDType LvDebugDrawing::AddTriangle(const fVector& InA, const fVector& InB, const fVector& InC, const fVector& InColor, bool bPersis, float InThiccness)
	{
		LvDebugShapeParameters Params{};
		Params.Triangle.A = InA;
		Params.Triangle.B = InB;
		Params.Triangle.C = InC;
		return AddShape(LVDEBUG_SHAPE_TRIANGLE, Params, InColor, bPersis, InThiccness);
	}

	DbgShapeIDType LvDebugDrawing::AddCircle(const fVector& InCenter, float InRadius, const fVector& InColor, bool bPersis, float InThiccness)
	{
		LvDebugShapeParameters Params{};
		Params.Circle.Center = InCenter;
		Params.Circle.Radius = InRadius;
		return AddShape(LVDEBUG_SHAPE_CIRCLE, Params, InColor, bPersis, InThiccness);
	}

	DbgShapeIDType LvDebugDrawing::AddSphere(const fVector& InCenter, float InRadius, const fVector& InColor, bool bPersis, float InThiccness)
	{
		LvDebugShapeParameters Params{};
		Params.Sphere.Center = InCenter;
		Params.Sphere.Radius = InRadius;
		return AddShape(LVDEBUG_SHAPE_SPHERE, Params, InColor, bPersis, InThiccness);
	}

	BasicMeshColor* InitDebugShapes()
	{
		// CKA_TODO:
		/*
		BasicMeshColor* DebugShapeMeshes = new BasicMeshColor[LVDEBUG_SHAPE_MAX]{};

		{ // LVDEBUG_SHAPE_POINT
			int NumVerts;
			int NumPrims;
			VertexColor* Verts = new VertexColor[(size_t)NumVerts];
			PrimType* Prims = new PrimType[(size_t)NumPrims];

		}

		{ // LVDEBUG_SHAPE_LINE

		}

		{ // LVDEBUG_SHAPE_SQUARE

		}

		{ // LVDEBUG_SHAPE_BOX

		}

		{ // LVDEBUG_SHAPE_TRIANGLE

		}

		{ // LVDEBUG_SHAPE_CIRCLE

		}

		{ // LVDEBUG_SHAPE_SPHERE

		}
		*/

		return nullptr;
	}
}