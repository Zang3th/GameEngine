#pragma once

#include "easylogging++.hpp"
#include "Mesh.hpp"
#include "ResourceManager.hpp"
#include "glm.hpp"
#include "CoreDefines.hpp"
#include "tiny_obj_loader.hpp"

#include <vector>

namespace Core
{
    class ObjLoader
    {
        public:
            ObjLoader() = delete;
            static void ObjToMesh(const std::string& filename, const std::string& baseFilepath, Mesh* mesh);
    };
}