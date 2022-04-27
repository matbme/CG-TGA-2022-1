#ifndef MODEL_H
#define MODEL_H

#include <map>
#include <string>
#include <vector>

#include "Mesh.h"
#include "Material.h"

class Model {
public:
    Model ();
    void draw ();
    void setup_meshes ();
    void set_shader_for_all (Shader *shader);
    void set_shader_for_textured (Shader *shader);
    void set_shader_for_untextured (Shader *shader);

    void rotate (float angle, glm::vec3 axis, bool reset = false);
    void translate (glm::vec3 displacements, bool reset = false);
    void rescale (glm::vec3 scaleFactors, bool reset = false);
    void highlight ();
    void remove_highlight ();

    void update ();

    std::vector<Mesh> meshes;
    std::map<std::string, Material> materials;
};

#endif
