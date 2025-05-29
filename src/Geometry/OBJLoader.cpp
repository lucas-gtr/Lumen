#include "Geometry/OBJLoader.hpp"
#include "Core/CommonTypes.hpp"
#include "Core/Math/Vec3.hpp"
#include "Geometry/Mesh.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace {
void parseVertexLine(const std::string& line, std::vector<lin::Vec3>& positions);
void parseNormalLine(const std::string& line, std::vector<lin::Vec3>& normals);
void parseUVLine(const std::string& line, std::vector<TextureUV>& uvs);
Face parseFaceLine(const std::string& line, const std::vector<lin::Vec3>& positions,
                   const std::vector<lin::Vec3>& normals, const std::vector<TextureUV>& uvs,
                   std::vector<Vertex>& vertices, std::unordered_map<std::string, int>& vertexMap);

} // namespace

Mesh OBJLoader::load(const std::string& filename) {
  std::ifstream file(filename);
  if(!file.is_open()) {
    std::cerr << "Error: Could not open file " << filename << '\n';
    return {};
  }

  std::vector<lin::Vec3>               positions;
  std::vector<lin::Vec3>               normals;
  std::vector<TextureUV>               uvs;
  std::vector<Vertex>                  vertices;
  std::vector<Face>                    faces;
  std::unordered_map<std::string, int> vertexMap;

  std::string line;
  while(std::getline(file, line)) {
    if(line.empty() || line[0] == '#') {
      continue;
    }

    std::istringstream iss(line);
    std::string        prefix;
    iss >> prefix;

    if(prefix == "v") {
      parseVertexLine(line, positions);
    } else if(prefix == "vn") {
      parseNormalLine(line, normals);
    } else if(prefix == "vt") {
      parseUVLine(line, uvs);
    } else if(prefix == "f") {
      faces.push_back(parseFaceLine(line, positions, normals, uvs, vertices, vertexMap));
    }
  }

  file.close();
  return {vertices, faces};
}

namespace {
void parseVertexLine(const std::string& line, std::vector<lin::Vec3>& positions) {
  std::istringstream iss(line.substr(2));
  lin::Vec3          pos;
  iss >> pos.x >> pos.y >> pos.z;
  positions.push_back(pos);
}

void parseNormalLine(const std::string& line, std::vector<lin::Vec3>& normals) {
  std::istringstream iss(line.substr(3));
  lin::Vec3          normal;
  iss >> normal.x >> normal.y >> normal.z;
  normals.push_back(normal);
}

void parseUVLine(const std::string& line, std::vector<TextureUV>& uvs) {
  std::istringstream iss(line.substr(3));
  TextureUV          uv;
  iss >> uv.u >> uv.v;
  uvs.push_back(uv);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
Face parseFaceLine(const std::string& line, const std::vector<lin::Vec3>& positions,
                   const std::vector<lin::Vec3>& normals, const std::vector<TextureUV>& uvs,
                   std::vector<Vertex>& vertices, std::unordered_map<std::string, int>& vertexMap) {
  std::istringstream iss(line);
  std::string        prefix;
  iss >> prefix;

  Face face;
  for(std::uint64_t i = 0; i < 3; ++i) {
    std::string vertexData;
    iss >> vertexData;

    if(vertexMap.find(vertexData) == vertexMap.end()) {
      std::istringstream vss(vertexData);
      std::string        vStr;
      std::string        vtStr;
      std::string        vnStr;
      std::getline(vss, vStr, '/');
      std::getline(vss, vtStr, '/');
      std::getline(vss, vnStr, '/');

      const int vIndex  = std::stoi(vStr) - 1;
      const int vtIndex = vtStr.empty() ? -1 : std::stoi(vtStr) - 1;
      const int vnIndex = vnStr.empty() ? -1 : std::stoi(vnStr) - 1;

      Vertex vertex;
      vertex.position = positions[vIndex];
      if(vtIndex >= 0) {
        vertex.uvCoord = uvs[vtIndex];
      }
      if(vnIndex >= 0) {
        vertex.normal = normals[vnIndex];
      }

      vertexMap[vertexData] = static_cast<int>(vertices.size());
      vertices.push_back(vertex);
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
    face.vertexIndices[i] = vertexMap[vertexData];
  }

  return face;
}
} // namespace