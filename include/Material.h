#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <glm/glm.hpp>

class Material {
public:
    Material () {
        ka_tex_id = -1;
        kd_tex_id = -1;
    }

    std::string get_name () { return this->name; }
    float get_specular_exp () { return this->ns; }
    float get_optical_density () { return this->ni; }
    float get_dissolve () { return this->d; }
    int get_illum () { return this->illum; }
    glm::vec3 get_transmission_filter () { return this->tf; }
    glm::vec3 get_ambient_refl () { return this->ka; }
    glm::vec3 get_diffuse_refl () { return this->kd; }
    glm::vec3 get_specular_refl () { return this->ks; }
    std::string get_ka_tex_map () { return this->map_ka; }
    std::string get_kd_tex_map () { return this->map_kd; }

    void set_name (std::string name) { this->name = name; }
    void set_specular_exp (float ns) { this->ns = ns; }
    void set_optical_density (float ni) { this->ni = ni; }
    void set_dissolve (float d) { this->d = d; }
    void set_illum (int illum) { this->illum = illum; }
    void set_transmission_filter (glm::vec3 tf) { this->tf = tf; }
    void set_ambient_refl (glm::vec3 ka) { this->ka = ka; }
    void set_diffuse_refl (glm::vec3 kd) { this->kd = kd; }
    void set_specular_refl (glm::vec3 ks) { this->ks = ks; }
    void set_ka_tex_map (std::string map_ka) { this->map_ka = map_ka; }
    void set_kd_tex_map (std::string map_kd) { this->map_kd = map_kd; }

    int ka_tex_id, kd_tex_id;
    friend std::pair<unsigned int, unsigned int> loadTextures (Material mat, std::string path);
private:
    std::string name;

    float ns, ni, d;
    int illum;
    glm::vec3 tf, ka, kd, ks;
    std::string map_ka, map_kd;
};

#endif
