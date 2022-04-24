#include "Mesh.h"

Mesh::Mesh () {
    model = glm::mat4 (1);
    pos = glm::vec3 (0.0f, 0.0f, 0.0f);
    scale = glm::vec3 (1.0f, 1.0f, 1.0f);
    angle = 0.0f;
}


std::map<std::string, unsigned int> Mesh::loaded_tex_rel_path;

void Mesh::setup () {
    // Setup mesh
    glGenVertexArrays (1, &VAO);
    glGenBuffers (1, &VBO);
    glGenBuffers (1, &EBO);

    glBindVertexArray (VAO);
    glBindBuffer (GL_ARRAY_BUFFER, VBO);
    glBufferData (GL_ARRAY_BUFFER,
                  this->vertices.size () * sizeof (Vertex),
                  &this->vertices[0],
                  GL_STATIC_DRAW);

    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER,
                  this->indices.size () * sizeof (unsigned int),
                  &this->indices[0],
                  GL_STATIC_DRAW);

    // Vertex pos
    glEnableVertexAttribArray (0);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE,
                           sizeof (Vertex), (void *) 0);

    // Vertex normals
    glEnableVertexAttribArray (1);
    glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE,
                           sizeof (Vertex), (void *) offsetof (Vertex, Normal));

    // Vertex texture coords
    glEnableVertexAttribArray (2);
    glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE,
                           sizeof (Vertex), (void *) offsetof (Vertex, TexCoords));

    glBindVertexArray (0);
}

void Mesh::draw () {
    std::array<int, TextureTypeNum> textureType_number = { 0 };
    int texture_number = GL_TEXTURE0;

    for (auto texture : textures) {
        glActiveTexture (texture_number++);

        textureType_number[texture.type]++;

        shader->setFloat(
            ("material." +
            TextureType_toString (texture.type) +
            std::to_string (textureType_number[texture.type])
            ).c_str (),
            texture_number - GL_TEXTURE0
        );

        glBindTexture (GL_TEXTURE_2D, texture.id);
    }
    glActiveTexture (GL_TEXTURE0);

    glBindVertexArray (VAO);
    glDrawElements (GL_TRIANGLES, indices.size (), GL_UNSIGNED_INT, 0);
    glBindVertexArray (0);
}

void Mesh::setShader (Shader *shader) {
    this->shader = shader;
    shader->Use ();
}

void Mesh::rotate (float angle, glm::vec3 axis, bool reset) {
    if (reset) model = glm::mat4 (1);
    model = glm::rotate (model, angle, axis);
    this->angle += angle;
}

void Mesh::translate (glm::vec3 displacements, bool reset) {
    if (reset) model = glm::mat4 (1);
    model = glm::translate (model, displacements);
}

void Mesh::rescale (glm::vec3 scaleFactors, bool reset) {
    if (reset) model = glm::mat4 (1);
    model = glm::scale (model, scaleFactors);
    scale = scaleFactors;
}

void Mesh::update () {
    GLint modelLoc = glGetUniformLocation (shader->ID, "model");
    glUniformMatrix4fv (modelLoc, 1, GL_FALSE, glm::value_ptr (model));
}
