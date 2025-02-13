#pragma once
#include <vector>
#include <string>

struct VertexData {
    struct { float x, y, z; } Position;
    struct { float x, y, z; } Normal;
    struct { float x, y; } Uv;
};

class ObjLoader {
public:
    static bool LoadOBJModel(const std::string& filename,
        std::vector<VertexData>& vertices,
        std::vector<unsigned short>& indices);
};
