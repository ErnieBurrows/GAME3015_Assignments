#include "ObjLoader.h"

#include <iostream>
#include <DirectXMath.h> 

#define TINYOBJLOADER_IMPLEMENTATION  // Only in one .cpp file
#include "tiny_obj_loader.h"


bool ObjLoader::LoadOBJModel(
    const std::string& filename,
    std::vector<VertexData>& vertices,
    std::vector<unsigned short>& indices)
{
    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig config;
    config.mtl_search_path = "./"; // Optional: path to material files

    if (!reader.ParseFromFile(filename, config)) {
        std::cerr << "Failed to load OBJ file: " << filename << std::endl;
        return false;
    }

    const auto& attrib = reader.GetAttrib();
    const auto& shapes = reader.GetShapes();

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            VertexData vertex = {};

            // Position
            vertex.Position = { 
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2] 
            };

            // Normal (if available)
            if (index.normal_index >= 0) {
                vertex.Normal = { 
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2] 
                };
            }

            // Texture Coordinates (if available)
            if (index.texcoord_index >= 0) {
                vertex.Uv = { 
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1] 
                };
            }

            vertices.push_back(vertex);
            indices.push_back(static_cast<unsigned short>(vertices.size() - 1));
        }
    }

    return true;
}
