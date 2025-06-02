#ifndef ASSETPROCESSOR_H
#define ASSETPROCESSOR_H

#include "Leviathan.h"

namespace Leviathan
{

MeshStateT LoadMeshOBJ(const char* FileName, bool bUseUVs = false, bool bUseNormals = true);

} // namespace Leviathan

#endif // ASSETPROCESSOR_H
