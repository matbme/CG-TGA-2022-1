#include "Scenes/MainScene.h"
#include "ModelImport.h"

void MainScene::update () {
	if (key_is_pressed (GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, GL_TRUE);

    if (key_is_pressed (GLFW_KEY_W)) {
        cam->cameraPos += cam->cameraSpeed * cam->cameraFront;
        cam->cameraUpdated = true;
    }

    if (key_is_pressed (GLFW_KEY_S)) {
        cam->cameraPos -= cam->cameraSpeed * cam->cameraFront;
        cam->cameraUpdated = true;
    }

    if (key_is_pressed (GLFW_KEY_A)) {
        cam->cameraPos -= glm::normalize(glm::cross(cam->cameraFront, cam->cameraUp)) * cam->cameraSpeed;
        cam->cameraUpdated = true;
    }

    if (key_is_pressed (GLFW_KEY_D)) {
        cam->cameraPos += glm::normalize(glm::cross(cam->cameraFront, cam->cameraUp)) * cam->cameraSpeed;
        cam->cameraUpdated = true;
    }

    if (key_is_pressed (GLFW_KEY_X)) {
        for (auto&& obj : objects)
            obj->rotate (glm::radians (2.0f), glm::vec3 (1.0f, 0.0f, 0.0f));
    }

    if (key_is_pressed (GLFW_KEY_Y)) {
        for (auto&& obj : objects)
            obj->rotate (glm::radians (2.0f), glm::vec3 (0.0f, 1.0f, 0.0f));
    }

    if (key_is_pressed (GLFW_KEY_Z)) {
        for (auto&& obj : objects)
            obj->rotate (glm::radians (2.0f), glm::vec3 (0.0f, 0.0f, 1.0f));
    }

    if (key_is_pressed (GLFW_KEY_MINUS)) {
        for (auto&& obj : objects)
            obj->rescale (glm::vec3 (0.9f, 0.9f, 0.9f));
        KeyEvent::tempLockKey(GLFW_KEY_MINUS, 0.25);
    }

    if (key_is_pressed (GLFW_KEY_RIGHT_SHIFT) && key_is_pressed (GLFW_KEY_EQUAL)) {
        for (auto&& obj : objects)
            obj->rescale (glm::vec3 (1.1f, 1.1f, 1.1f));
        KeyEvent::tempLockKey(GLFW_KEY_EQUAL, 0.25);
    }

    if (key_is_pressed (GLFW_KEY_LEFT_SHIFT)) {
        cam->cameraPos += cam->cameraSpeed * cam->cameraUp;
        cam->cameraUpdated = true;
    }

    if (key_is_pressed (GLFW_KEY_LEFT_CONTROL)) {
        cam->cameraPos -= cam->cameraSpeed * cam->cameraUp;
        cam->cameraUpdated = true;
    }

    if (cam->changed ()) {
        view = glm::lookAt (cam->cameraPos, cam->cameraPos+cam->cameraFront, cam->cameraUp);
        projection = glm::perspective(glm::radians(cam->fov), (GLfloat) Scene::window_width / (GLfloat) Scene::window_height, 0.1f, 100.0f);  

        glUniformMatrix4fv (viewLoc, 1, GL_FALSE, glm::value_ptr (view));
        glUniformMatrix4fv (projLoc, 1, GL_FALSE, glm::value_ptr (projection));
    }
}

void MainScene::setupScene () {
    // Build and compile our shader program
    addShaders ("shaders/template_vs.glsl", "shaders/template_fs.glsl");
    // addShaders ("shaders/untextured_vs.glsl", "shaders/untextured_fs.glsl");

    std::string path ("/home/matbme/Downloads/3D_Models/Pokemon/Pikachu.obj");
    auto pikachu = ModelImporter::Obj::import (path);
    pikachu->set_shader_for_all (shader);

    path = "/home/matbme/Downloads/3D_Models/Pokemon/PikachuF.obj";
    auto pikachuF = ModelImporter::Obj::import (path);
    pikachuF->set_shader_for_all (shader);

    pikachuF->translate (glm::vec3 (8.0f, 0.0f, 0.0f));
    pikachuF->rescale (glm::vec3 (0.1f, 0.1f, 0.1f));

    add_object (&pikachu);
    add_object (&pikachuF);
    
    // std::string path ("/home/matbme/Downloads/3D_Models/Classic-NoTexture/apple.obj");
    // auto apple = ModelImporter::Obj::import(path);
    // apple->set_shader_for_all(shader);
    //
    // path = "/home/matbme/Downloads/3D_Models/Classic-NoTexture/cat.obj";
    // auto cat = ModelImporter::Obj::import(path);
    // cat->set_shader_for_all(shader);
    // cat->translate (glm::vec3 (8.0f, 0.0f, 0.0f));
    //
    // add_object (&apple);
    // add_object (&cat);

    view = glm::lookAt (cam->cameraPos, cam->cameraPos+cam->cameraFront, cam->cameraUp);
}
