/* Template para desenvolvimento de atividades da disciplina
 * Computação Gráfica, ministrada pela prof. Rossana Baptista Queiroz.
 *
 * Developer: Mateus Melchiades <mateusbme@edu.unisinos.br>
 */

#include "KeyEvent.h"
#include "Scenes/MainScene.h"

#include "ModelImport.h"

int main () {
    MainScene *main_scene = new MainScene (1920, 1080, "template");

    main_scene->run ();
    main_scene->finish();

    return EXIT_SUCCESS;
}
