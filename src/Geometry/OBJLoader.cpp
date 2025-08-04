#include <cstdint>
#include <fstream>
#include <iostream>
#include <linalg/Vec3.hpp>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Core/ImageTypes.hpp"
#include "Geometry/Mesh.hpp"
#include "Geometry/OBJLoader.hpp"

namespace {
void parseVertexLine(const std::string& line, std::vector<linalg::Vec3d>& positions);
void parseNormalLine(const std::string& line, std::vector<linalg::Vec3d>& normals);
void parseUVLine(const std::string& line, std::vector<TextureUV>& uvs);
Face parseFaceLine(const std::string& line, const std::vector<linalg::Vec3d>& positions,
                   const std::vector<linalg::Vec3d>& normals, const std::vector<TextureUV>& uvs,
                   std::vector<Vertex>& vertices, std::unordered_map<std::string, int>& vertex_map);

} // namespace

Mesh OBJLoader::load(const std::string& filename) {
  std::ifstream file(filename);
  if(!file.is_open()) {
    std::cerr << "Error: Could not open file " << filename << '\n';
    return {};
  }

  std::vector<linalg::Vec3d>           positions;
  std::vector<linalg::Vec3d>           normals;
  std::vector<TextureUV>               uvs;
  std::vector<Vertex>                  vertices;
  std::vector<Face>                    faces;
  std::unordered_map<std::string, int> vertex_map;

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
      faces.push_back(parseFaceLine(line, positions, normals, uvs, vertices, vertex_map));
    }
  }

  file.close();
  return {vertices, faces};
}

namespace {
void parseVertexLine(const std::string& line, std::vector<linalg::Vec3d>& positions) {
  std::istringstream iss(line.substr(2));
  linalg::Vec3d      pos;
  iss >> pos.x >> pos.y >> pos.z;
  positions.push_back(pos);
}

void parseNormalLine(const std::string& line, std::vector<linalg::Vec3d>& normals) {
  std::istringstream iss(line.substr(3));
  linalg::Vec3d      normal;
  iss >> normal.x >> normal.y >> normal.z;
  normals.push_back(normal);
}

void parseUVLine(const std::string& line, std::vector<TextureUV>& uvs) {
  std::istringstream iss(line.substr(3));
  TextureUV          uv;
  iss >> uv.u >> uv.v;
  uvs.push_back(uv);
}

Face parseFaceLine(const std::string& line, const std::vector<linalg::Vec3d>& positions,
                   const std::vector<linalg::Vec3d>& normals, const std::vector<TextureUV>& uvs,
                   std::vector<Vertex>& vertices, std::unordered_map<std::string, int>& vertex_map) {
  std::istringstream iss(line);
  std::string        prefix;
  iss >> prefix;

  Face face;
  for(std::uint64_t i = 0; i < 3; ++i) {
    std::string vertex_data;
    iss >> vertex_data;

    if(vertex_map.find(vertex_data) == vertex_map.end()) {
      std::istringstream vss(vertex_data);
      std::string        v_str;
      std::string        vt_str;
      std::string        vn_str;
      std::getline(vss, v_str, '/');
      std::getline(vss, vt_str, '/');
      std::getline(vss, vn_str, '/');

      const int v_index  = std::stoi(v_str) - 1;
      const int vt_index = vt_str.empty() ? -1 : std::stoi(vt_str) - 1;
      const int vn_index = vn_str.empty() ? -1 : std::stoi(vn_str) - 1;

      Vertex vertex;
      vertex.position = positions[v_index];
      if(vt_index >= 0) {
        vertex.uv_coord = uvs[vt_index];
      }
      if(vn_index >= 0) {
        vertex.normal = normals[vn_index];
      }

      vertex_map[vertex_data] = static_cast<int>(vertices.size());
      vertices.push_back(vertex);
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
    face.vertex_indices[i] = vertex_map[vertex_data];
  }

  return face;
}
} // namespace