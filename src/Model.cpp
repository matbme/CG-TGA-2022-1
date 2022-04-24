#include "Model.h"

// Reference to https://learnopengl.com/Model-Loading/Model
Model::Model () {}

void Model::draw () {
    for (int i = 0 ; i < meshes.size() ; i++) {
        meshes[i].draw ();
    }
}

void Model::setup_meshes () {
    for (int i = 0 ; i < meshes.size() ; i++) {
        meshes[i].setup();
    }
}

void Model::set_shader(Shader *shader) {
    for (int i = 0 ; i < meshes.size() ; i++) {
        meshes[i].setShader (shader);
    }
}

void Model::rotate (float angle, glm::vec3 axis, bool reset) {
    for (int i = 0 ; i < meshes.size() ; i++) {
        meshes[i].rotate (angle, axis, reset);
    }
}

void Model::translate (glm::vec3 displacements, bool reset) {
    for (int i = 0 ; i < meshes.size() ; i++) {
        meshes[i].translate (displacements, reset);
    }
}

void Model::rescale (glm::vec3 scaleFactors, bool reset) {
    for (int i = 0 ; i < meshes.size() ; i++) {
        meshes[i].rescale (scaleFactors, reset);
    }
}

void Model::update () {
    for (auto mesh : meshes) {
        mesh.update ();
    }
}
