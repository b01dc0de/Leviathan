#include "Labyrinth.h"

namespace Game
{

using namespace Leviathan;

inline namespace LabyrinthState
{

static constexpr int Size = 8;
static constexpr int NumRooms = Size * Size;

struct Room
{
    bool bNorth;
    bool bSouth;
    bool bEast;
    bool bWest;
};

Room RoomMap[NumRooms] = {};

MeshStateT LabyrinthMesh;
float AngleX;
float AngleZ;

} // inline namespace LabyrinthState

void Labyrinth::Update()
{
    if (GamepadState::IsConnected(0))
    {
        v2f LeftStick = GamepadState::GetLeftStick(0);
        AngleX = LeftStick.X * 0.5f;
        AngleZ = LeftStick.Y * 0.5f;
    }
    else
    {
        AngleX = 0.0f;
        AngleZ = 0.0f;
    }
}

void Labyrinth::Draw(LvGFXContext& GFXContext)
{
    GFXContext.ImmContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
    GFXContext.SetShaderConstantBuffers_WVP();
    GFXContext.UpdateShaderViewProj(GFXContext.GameCamera);
    m4f LabyrinthWorld = m4f::RotAxisX(AngleZ) * m4f::RotAxisZ(AngleX);
    GFXContext.UpdateShaderWorld(&LabyrinthWorld);

    DrawMesh(GFXContext.ImmContext, *GFXContext.DrawStateColorNormal, LabyrinthMesh);
}

void InitLabyrinthMesh()
{
    constexpr float VisualSize = 25.0f;
    constexpr float HalfVisualSize = VisualSize * 0.5f;
    constexpr float WallHeight = 2.5f;
    constexpr v4f FloorColor{ 124.0f / 255.0f, 174.0f / 255.0f, 1.0f, 1.0f };
    constexpr v4f WallColor{ 240.0f / 255.0f, 77.0f / 255.0f, 77.0f / 255.0f, 1.0f };
    constexpr v3f PosX{ +1.0f, 0.0f, 0.0f };
    constexpr v3f NegX{ -1.0f, 0.0f, 0.0f };
    constexpr v3f PosY{ 0.0f, +1.0f, 0.0f };
    constexpr v3f NegY{ 0.0f, -1.0f, 0.0f };
    constexpr v3f PosZ{ 0.0f, 0.0f, +1.0f };
    constexpr v3f NegZ{ 0.0f, 0.0f, -1.0f };
    Array<VxColorNormal> Verts;
    Array<unsigned int> Inds;
    int Idx = 0;
    auto AddFrontFace = [&Inds, &Idx]()
    {
        Inds.Add(Idx + 0); Inds.Add(Idx + 1); Inds.Add(Idx + 2);
        Inds.Add(Idx + 2); Inds.Add(Idx + 3); Inds.Add(Idx + 1);
        Idx += 4;
    };
    auto AddBackFace = [&Inds, &Idx]()
    {
        Inds.Add(Idx + 0); Inds.Add(Idx + 2); Inds.Add(Idx + 1);
        Inds.Add(Idx + 2); Inds.Add(Idx + 1); Inds.Add(Idx + 3);
        Idx += 4;
    };
    { // Floor
        Verts.Add(VxColorNormal{ { -HalfVisualSize, 0.0f, -HalfVisualSize, 1.0f }, FloorColor, PosY });
        Verts.Add(VxColorNormal{ { +HalfVisualSize, 0.0f, -HalfVisualSize, 1.0f }, FloorColor, PosY });
        Verts.Add(VxColorNormal{ { -HalfVisualSize, 0.0f, +HalfVisualSize, 1.0f }, FloorColor, PosY });
        Verts.Add(VxColorNormal{ { +HalfVisualSize, 0.0f, +HalfVisualSize, 1.0f }, FloorColor, PosY });
        AddFrontFace();

        Verts.Add(VxColorNormal{ { -HalfVisualSize, 0.0f, -HalfVisualSize, 1.0f }, FloorColor, NegY });
        Verts.Add(VxColorNormal{ { +HalfVisualSize, 0.0f, -HalfVisualSize, 1.0f }, FloorColor, NegY });
        Verts.Add(VxColorNormal{ { -HalfVisualSize, 0.0f, +HalfVisualSize, 1.0f }, FloorColor, NegY });
        Verts.Add(VxColorNormal{ { +HalfVisualSize, 0.0f, +HalfVisualSize, 1.0f }, FloorColor, NegY });
        AddBackFace();
    }

    { // North wall (-Z)
        Verts.Add(VxColorNormal{ {-HalfVisualSize, WallHeight, -HalfVisualSize, 1.0f} , WallColor, PosZ });
        Verts.Add(VxColorNormal{ {+HalfVisualSize, WallHeight, -HalfVisualSize, 1.0f} , WallColor, PosZ });
        Verts.Add(VxColorNormal{ {-HalfVisualSize, 0.0f, -HalfVisualSize, 1.0f} , WallColor, PosZ });
        Verts.Add(VxColorNormal{ {+HalfVisualSize, 0.0f, -HalfVisualSize, 1.0f} , WallColor, PosZ });
        AddFrontFace();

        Verts.Add(VxColorNormal{ {-HalfVisualSize, WallHeight, -HalfVisualSize, 1.0f} , WallColor, NegZ });
        Verts.Add(VxColorNormal{ {+HalfVisualSize, WallHeight, -HalfVisualSize, 1.0f} , WallColor, NegZ });
        Verts.Add(VxColorNormal{ {-HalfVisualSize, 0.0f, -HalfVisualSize, 1.0f} , WallColor, NegZ });
        Verts.Add(VxColorNormal{ {+HalfVisualSize, 0.0f, -HalfVisualSize, 1.0f} , WallColor, NegZ });
        AddBackFace();
    }

    { // South wall (+Z)
        Verts.Add(VxColorNormal{ {-HalfVisualSize, WallHeight, +HalfVisualSize, 1.0f} , WallColor, PosZ });
        Verts.Add(VxColorNormal{ {+HalfVisualSize, WallHeight, +HalfVisualSize, 1.0f} , WallColor, PosZ });
        Verts.Add(VxColorNormal{ {-HalfVisualSize, 0.0f, +HalfVisualSize, 1.0f} , WallColor, PosZ });
        Verts.Add(VxColorNormal{ {+HalfVisualSize, 0.0f, +HalfVisualSize, 1.0f} , WallColor, PosZ });
        AddFrontFace();

        Verts.Add(VxColorNormal{ {-HalfVisualSize, WallHeight, +HalfVisualSize, 1.0f} , WallColor, NegZ });
        Verts.Add(VxColorNormal{ {+HalfVisualSize, WallHeight, +HalfVisualSize, 1.0f} , WallColor, NegZ });
        Verts.Add(VxColorNormal{ {-HalfVisualSize, 0.0f, +HalfVisualSize, 1.0f} , WallColor, NegZ });
        Verts.Add(VxColorNormal{ {+HalfVisualSize, 0.0f, +HalfVisualSize, 1.0f} , WallColor, NegZ });
        AddBackFace();
    }

    { // West wall (-X)
        Verts.Add(VxColorNormal{ {-HalfVisualSize, WallHeight, +HalfVisualSize, 1.0f}, WallColor, NegX });
        Verts.Add(VxColorNormal{ {-HalfVisualSize, WallHeight, -HalfVisualSize, 1.0f}, WallColor, NegX });
        Verts.Add(VxColorNormal{ {-HalfVisualSize, 0.0f, +HalfVisualSize, 1.0f}, WallColor, NegX });
        Verts.Add(VxColorNormal{ {-HalfVisualSize, 0.0f, -HalfVisualSize, 1.0f}, WallColor, NegX });
        AddFrontFace();

        Verts.Add(VxColorNormal{ {-HalfVisualSize, WallHeight, +HalfVisualSize, 1.0f}, WallColor, PosX });
        Verts.Add(VxColorNormal{ {-HalfVisualSize, WallHeight, -HalfVisualSize, 1.0f}, WallColor, PosX });
        Verts.Add(VxColorNormal{ {-HalfVisualSize, 0.0f, +HalfVisualSize, 1.0f}, WallColor, PosX });
        Verts.Add(VxColorNormal{ {-HalfVisualSize, 0.0f, -HalfVisualSize, 1.0f}, WallColor, PosX });
        AddBackFace();
    }

    { // East wall (+X)
        Verts.Add(VxColorNormal{ {+HalfVisualSize, WallHeight, +HalfVisualSize, 1.0f}, WallColor, NegX });
        Verts.Add(VxColorNormal{ {+HalfVisualSize, WallHeight, -HalfVisualSize, 1.0f}, WallColor, NegX });
        Verts.Add(VxColorNormal{ {+HalfVisualSize, 0.0f, +HalfVisualSize, 1.0f}, WallColor, NegX });
        Verts.Add(VxColorNormal{ {+HalfVisualSize, 0.0f, -HalfVisualSize, 1.0f}, WallColor, NegX });
        AddFrontFace();

        Verts.Add(VxColorNormal{ {+HalfVisualSize, WallHeight, +HalfVisualSize, 1.0f}, WallColor, PosX });
        Verts.Add(VxColorNormal{ {+HalfVisualSize, WallHeight, -HalfVisualSize, 1.0f}, WallColor, PosX });
        Verts.Add(VxColorNormal{ {+HalfVisualSize, 0.0f, +HalfVisualSize, 1.0f}, WallColor, PosX });
        Verts.Add(VxColorNormal{ {+HalfVisualSize, 0.0f, -HalfVisualSize, 1.0f}, WallColor, PosX });
        AddBackFace();
    }

    LabyrinthMesh = CreateMeshState
    (
        Graphics::Device(),
        sizeof(VxColorNormal),
        Verts.Num, Verts.Data,
        Inds.Num, Inds.Data
    );
}

void Labyrinth::Init()
{
    for (int Row = 0; Row < Size; Row++)
    {
        for (int Col = 0; Col < Size; Col++)
        {
            Room& CurrRoom = RoomMap[Row * Size + Col];
            CurrRoom = { false, false, false, false };
            if (Row == 0) { CurrRoom.bNorth = true; }
            if (Row == Size - 1) { CurrRoom.bSouth = true; }
            if (Col == 0) { CurrRoom.bWest = true; }
            if (Col == Size - 1) { CurrRoom.bEast = true; }
        }
    }
    InitLabyrinthMesh();
}

void Labyrinth::Term()
{

}

} // namespace Game

