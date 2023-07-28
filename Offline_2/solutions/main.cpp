#include<bits/stdc++.h>
#include "graphics_lib.h"

using namespace std;

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        cout << "Usage: ./main <scene.txt>" << endl;
        return 1;
    }

    // read input from <scene.txt> file
    string sceneFileName = argv[1];
    Scene scene(sceneFileName);
    scene.draw();

    return 0;
}