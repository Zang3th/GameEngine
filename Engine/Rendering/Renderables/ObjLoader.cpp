#include "ObjLoader.hpp"

namespace Engine
{
    // ----- Public -----

    void ObjLoader::ObjToMesh(const std::string& filename, const std::string& baseFilepath, std::vector<Mesh>* meshes)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;
        std::string objFullFilepath = baseFilepath + "/" + filename + ".obj";

        if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objFullFilepath.c_str(), baseFilepath.c_str()))
            LOG(ERROR) << "Failed:   Obj-File-Loading | " << objFullFilepath;
        else
            LOG(INFO) << "Loaded:   Obj-File | " << objFullFilepath
                      << " (Shapes: " << shapes.size()
                      << " | Materials: " << materials.size()
                      << " | Vertices: " << attrib.vertices.size()
                      << " | TexCoords: " << attrib.texcoords.size()
                      << " | Indices: " << shapes[0].mesh.indices.size()
                      << ")";

        //Additional error checking to catch edge cases
        if(!warn.empty())
            LOG(WARNING) << "Failed:   " << warn;
        if(!err.empty())
            LOG(ERROR) << "Failed:   " << err;

        //Iterate over all shapes
        for(uint32 i = 0; i < shapes.size(); i++)
        {
            //Create new mesh
            Mesh mesh;

            //Init with default values
            mesh.texture1 = nullptr;
            mesh.texture2 = nullptr;
            mesh.texture3 = nullptr;
            mesh.gotNormalMap = 0;

            //Iterate over all indices
            for(const auto& index : shapes.at(i).mesh.indices)
            {
                //Get vertices
                mesh.vertices.emplace_back(glm::vec3(
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                ));

                //Get normals
                mesh.normals.emplace_back(glm::vec3(
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                ));

                //Get texCoords
                mesh.texCoords.emplace_back(glm::vec2(
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                ));

                //Reserve space for tangents
                mesh.tangents.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f));

                //Auto increment indices
                mesh.indices.push_back(mesh.indices.size());
            }

            //Check for diffuse texture
            if(!materials[i].diffuse_texname.empty())
            {
                //Load diffuse texture
                std::string textureFilepath = baseFilepath + "/" + materials[i].diffuse_texname;
                std::string textureName = filename + "DiffuseTexture";
                ResourceManager::LoadTextureFromFile(textureName, textureFilepath);
                mesh.texture1 = ResourceManager::GetTexture(textureName);
            }

            //Check for normal map
            if(!materials[i].bump_texname.empty())
            {
                //Load bump map
                std::string textureFilepath = baseFilepath + "/" + materials[i].bump_texname;
                std::string textureName = filename + "NormalTexture";
                ResourceManager::LoadTextureFromFile(textureName, textureFilepath);
                mesh.texture2 = ResourceManager::GetTexture(textureName);
                mesh.gotNormalMap = 1;
            }

            //Calculate tangents
            if(mesh.gotNormalMap == 1)
            {
                MeshCreator::CalculateTangents(&mesh);
                LOG(INFO) << "Created:  Tangents for " << filename;
            }

            //Copy mesh into meshes vector
            meshes->push_back(mesh);
        }
    }
}