#ifndef LEVIATHAN_CAMERA_H
#define LEVIATHAN_CAMERA_H

#include "Leviathan.h"

namespace Leviathan
{

struct Camera
{
    m4f View;
    m4f Proj;
    void Ortho(float ResX, float ResY, float fDepth = 1.0f);
    void Persp(const v3f& InPos, const v3f& InLookAt);
};

} // namespace Leviathan

#endif // LEVIATHAN_CAMERA_H

