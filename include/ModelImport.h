#ifndef MODELIMPORT_H
#define MODELIMPORT_H

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <array>
#include <limits>

#include "Model.h"
#include "Material.h"

namespace ModelImporter {

namespace Obj {
std::unique_ptr<Model> import (std::string const &path);
inline std::array<int, 3> _tokenize_face_param (std::string face_param);

inline std::pair<std::unique_ptr<Model> ,unsigned int> _add_vertex (
    std::string vertex_param,
    std::unique_ptr<Model> ret_model,
    std::vector<glm::vec3> const *temp_vertex_pos,
    std::vector<glm::vec3> const *temp_vertex_norm,
    std::vector<glm::vec2> const *temp_vertex_tex);
}

namespace Mtl {
std::map<std::string, Material> import (std::string const &path);
}

std::string extract_path (std::string const &path);
unsigned int _loadTex (std::string filename);
inline std::vector<std::string> tokenize_line (std::string line);
}

#endif
