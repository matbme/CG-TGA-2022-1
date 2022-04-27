/* Entrega Grau A
 * Computação Gráfica, ministrada pela prof. Rossana Baptista Queiroz.
 *
 * Developers: Felipe Peters, Mateus Melchiades e Pedro Accorsi
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
