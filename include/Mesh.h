#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <map>
#include <memory>
#include <vector>

#include "Material.h"
#include "Shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};
constexpr bool operator==(const Vertex& lhs, const Vertex& rhs) {
    return lhs.Position  == rhs.Position   &&
           lhs.Normal    == rhs.Normal     &&
           lhs.TexCoords == rhs.TexCoords;
}

enum TextureType { DIFFUSE, SPECULAR };
// NOTE: Must match the number of elements in the above struct
constexpr int TextureTypeNum = 2;
// NOTE: Add new cases when adding new types
inline std::string TextureType_toString (TextureType tt) {
    switch (tt) {
        case DIFFUSE: return "texture_diffuse";
        case SPECULAR: return "texture_specular";
        default: throw std::invalid_argument ("Unimplemented texture type");
    }
}

struct Texture {
    unsigned int id;
    TextureType type;
};

class Mesh {
public:
    Mesh ();

    static std::map<std::string, unsigned int> loaded_tex_rel_path;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    Material *material;

    void setup ();
    void draw ();

    void rotate (float angle, glm::vec3 axis, bool reset = false);
    void translate (glm::vec3 displacements, bool reset = false);
    void rescale (glm::vec3 scaleFactors, bool reset = false);

    void update ();

    void setShader (Shader *shader);
private:
    glm::mat4 model;
    glm::vec3 pos, scale;
    float angle;

    Shader *shader;

    unsigned int VAO, VBO, EBO;
};

#endif
