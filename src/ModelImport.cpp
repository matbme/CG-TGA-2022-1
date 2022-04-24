#include "ModelImport.h"

// Makes getting current mesh less spaghettity 
#define CURRENT_MESH ret_model->meshes.back()

// Simple hashing function to compare instructions at O(n)
constexpr unsigned int hash(const char* str, int h = 0) {
    return !str[h] ? 5381 : (hash(str, h+1)*33) ^ str[h];
}

std::unique_ptr<Model> ModelImporter::Obj::import (std::string const &path) {
    auto ret_model = std::make_unique<Model> ();
    bool new_mesh = false;

    // We store file values in temp vectors until we are done importing everything
    std::vector<glm::vec3> temp_vertex_pos;
    std::vector<glm::vec3> temp_vertex_norm;
    std::vector<glm::vec2> temp_vertex_tex;

    std::ifstream file (path);
    std::string line;
    while (std::getline (file, line)) {
        if (line == "\r") continue; // Ignore carriage returns
        if (line[0] == '#') continue; // Ignore comments

        if (new_mesh) {
            ret_model->meshes.push_back (Mesh ());
            new_mesh = false;
        }

        auto tokens = ModelImporter::tokenize_line (line);
        switch (hash (tokens[0].c_str ())) {
            case hash ("mtllib"): { // Import materials from
                ret_model->materials = ModelImporter::Mtl::import ({ModelImporter::extract_path (path) + tokens[1]});
                break;
            }
            case hash ("o"): {
                std::cout << "Is o" << std::endl;
                break;
            }
            case hash ("v"): { // Vertex position
                temp_vertex_pos.push_back (
                        glm::vec3 (std::strtof (tokens[1].c_str (), NULL), 
                                   std::strtof (tokens[2].c_str (), NULL), 
                                   std::strtof (tokens[3].c_str (), NULL)));
                break;
            }
            case hash ("vt"): { // Vertex texture coords
                temp_vertex_tex.push_back (
                        glm::vec2 (std::strtof (tokens[1].c_str (), NULL), 
                                   std::strtof (tokens[2].c_str (), NULL)));
                break;
            }
            case hash ("vn"): { // Vertex normals
                temp_vertex_norm.push_back (
                        glm::vec3 (std::strtof (tokens[1].c_str (), NULL), 
                                   std::strtof (tokens[2].c_str (), NULL), 
                                   std::strtof (tokens[3].c_str (), NULL)));
                break;
            }
            case hash ("g"): { // New group
                new_mesh = true;
                break;
            }
            case hash ("usemtl"): { // Use material for mesh
                CURRENT_MESH.material = &ret_model->materials[tokens[1]];
                auto [ka_tex_id, kd_tex_id] = loadTextures (*CURRENT_MESH.material, 
                                                            ModelImporter::extract_path (path));
                if (ka_tex_id != std::numeric_limits<unsigned int>::max()) {
                    Texture tex = { ka_tex_id, SPECULAR };
                    CURRENT_MESH.textures.push_back(tex);
                }
                if (kd_tex_id != std::numeric_limits<unsigned int>::max()) {
                    Texture tex = { kd_tex_id, DIFFUSE };
                    CURRENT_MESH.textures.push_back(tex);
                }
                break;
            }
            case hash ("s"): {
                std::cout << "Is s" << std::endl;
                break;
            }
            case hash ("f"): { // Face
                for (int i = 1 ; i <= 3 ; i++) {
                    auto [model_ptr, pos] = ModelImporter::Obj::_add_vertex (
                            tokens[i], 
                            std::move (ret_model), 
                            &temp_vertex_pos, 
                            &temp_vertex_norm, 
                            &temp_vertex_tex);
                    ret_model.swap (model_ptr);
                    CURRENT_MESH.indices.push_back (pos);
                }
                break;
            }
            default: {
                std::cout << "Something else" << std::endl;
            }
        }
    }

    ret_model->setup_meshes ();

    return ret_model;
}

inline std::array<int, 3> ModelImporter::Obj::_tokenize_face_param (std::string face_param) {
    std::array<int, 3> ret;
    std::string bld;
    int it = 0;

    for (const auto character : face_param+"/") {
        if (character != '/') bld += character;
        else {
            ret[it] = std::atoi (bld.c_str ());
            bld.clear ();
            it++;
        }
    }

    return ret;
}

inline std::pair<std::unique_ptr<Model> ,unsigned int> ModelImporter::Obj::_add_vertex (
    std::string vertex_param,
    std::unique_ptr<Model> ret_model,
    std::vector<glm::vec3> const *temp_vertex_pos,
    std::vector<glm::vec3> const *temp_vertex_norm,
    std::vector<glm::vec2> const *temp_vertex_tex)
{
    auto tokenized_param = ModelImporter::Obj::_tokenize_face_param(vertex_param);
    Vertex constructed = {
        temp_vertex_pos->at (tokenized_param[0] - 1),
        temp_vertex_norm->at (tokenized_param[2] - 1),
        tokenized_param[1] ? temp_vertex_tex->at (tokenized_param[1] - 1) : glm::vec2()
    };

    // Checks whether constructed vertex already exists
    auto result = std::find (CURRENT_MESH.vertices.begin (),
                             CURRENT_MESH.vertices.end (),
                             constructed);

    // Append to vertex vector if it doesn't exist, otherwire returns its position
    int pos = -1;
    if (result == CURRENT_MESH.vertices.end ()) {
        CURRENT_MESH.vertices.push_back (constructed);
        pos = CURRENT_MESH.vertices.size() - 1;
    } else {
        pos = std::distance(CURRENT_MESH.vertices.begin (), result);
    }

    assert (pos != -1);
    return std::pair<std::unique_ptr<Model>, unsigned int> (std::move (ret_model), pos);
}


std::map<std::string, Material> ModelImporter::Mtl::import (std::string const &path) {
    std::map<std::string, Material> ret_mats;
    std::string current_mtl;

    std::ifstream file (path);
    std::string line;
    while (std::getline (file, line)) {
        if (line == "\r") continue; // Ignore carriage returns

        auto tokens = ModelImporter::tokenize_line (line);
        switch (hash (tokens[0].c_str ())) {
            case hash ("newmtl"): // New material
                ret_mats[tokens[1]]  = Material ();
                current_mtl = tokens[1];
                ret_mats[current_mtl].set_name (tokens[1]);
                break;
            case hash ("Ns"): // Specular exponent
                ret_mats[current_mtl].set_specular_exp (std::strtof (tokens[1].c_str (), NULL));
                break;
            case hash ("Ni"): // Optical density
                ret_mats[current_mtl].set_optical_density (std::strtof (tokens[1].c_str (), NULL));
                break;
            case hash ("d"): // Dissolve
                ret_mats[current_mtl].set_dissolve (std::strtof (tokens[1].c_str (), NULL));
                break;
            case hash ("Tf"): // Transmission filter
                ret_mats[current_mtl].set_transmission_filter (
                        glm::vec3 (std::strtof (tokens[1].c_str (), NULL),
                                   std::strtof (tokens[2].c_str (), NULL),
                                   std::strtof (tokens[3].c_str (), NULL)));
                break;
            case hash ("illum"): // Illumination
                ret_mats[current_mtl].set_illum (std::atoi (tokens[1].c_str ()));
                break;
            case hash ("Ka"): // Ambient reflectivity
                ret_mats[current_mtl].set_ambient_refl (
                        glm::vec3 (std::strtof (tokens[1].c_str (), NULL),
                                   std::strtof (tokens[2].c_str (), NULL),
                                   std::strtof (tokens[3].c_str (), NULL)));
                break;
            case hash ("Kd"): // Diffuese reflectivity
                ret_mats[current_mtl].set_diffuse_refl (
                        glm::vec3 (std::strtof (tokens[1].c_str (), NULL),
                                   std::strtof (tokens[2].c_str (), NULL),
                                   std::strtof (tokens[3].c_str (), NULL)));
                break;
            case hash ("Ks"): // Specular reflectivity
                ret_mats[current_mtl].set_specular_refl (
                        glm::vec3 (std::strtof (tokens[1].c_str (), NULL),
                                   std::strtof (tokens[2].c_str (), NULL),
                                   std::strtof (tokens[3].c_str (), NULL)));
                break;
            case hash ("map_Ka"): // Ka texture map
                ret_mats[current_mtl].set_ka_tex_map (tokens[1]);
                break;
            case hash ("map_Kd"): // Kd texture map
                ret_mats[current_mtl].set_kd_tex_map (tokens[1]);
                break;
            default:
                std::cout << "Is something else" << std::endl;
                break;
        }
    }

    return ret_mats;
}

std::string ModelImporter::extract_path (std::string const &path) {
    std::vector<std::string> path_components;
    std::string component;
    std::string ret_path;

    char os_sep = 
#ifdef _WIN32
    '\\'
#else
    '/'
#endif
    ;

    for (const auto character : path) {
        if (character != os_sep) component.push_back(character);
        else {
            path_components.push_back (component);
            component.clear ();
        }
    }

    for (auto comp : path_components) ret_path += comp + os_sep;
    return ret_path;
}

inline std::vector<std::string> ModelImporter::tokenize_line(std::string line) {
    std::vector<std::string> tokens;
    std::istringstream iss (line);
    std::copy (std::istream_iterator<std::string>(iss),
               std::istream_iterator<std::string>(),
               std::back_inserter(tokens));

    return tokens;
}

// Friend of Material
std::pair<unsigned int, unsigned int> loadTextures (Material mat, std::string path) {
    std::pair<unsigned int, unsigned int> ret {
        std::numeric_limits<unsigned int>::max (), 
        std::numeric_limits<unsigned int>::max ()
    };
    int it = 0;

    for (auto map : {mat.map_ka, mat.map_kd}) {
        if (map.empty()) continue;

        // Check if texture was already loaded
        auto result = Mesh::loaded_tex_rel_path.find (map);
        if (result != Mesh::loaded_tex_rel_path.end ()) {
            (it ? ret.first : ret.second) = Mesh::loaded_tex_rel_path[map];
            continue;
        }

        std::string filename = path + map;

#ifdef _WIN32
        int pos = 0;
        while (pos != string::npos) {
            pos = filename.find ("/");
            filename.replace (pos, 1, "\\");
        }
#endif

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load (filename.c_str(), &width, &height, &nrComponents, 0);
        if (data) {
            GLenum format;
            if (nrComponents == 1) format = GL_RED;
            else if (nrComponents == 3) format = GL_RGB;
            else if (nrComponents == 4) format = GL_RGBA;

            glBindTexture (GL_TEXTURE_2D, textureID);
            glTexImage2D (GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap (GL_TEXTURE_2D);

            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free (data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free (data);
        }

        Mesh::loaded_tex_rel_path[map] = textureID;

        (it ? ret.first : ret.second) = textureID;
    }

    return ret;
}
