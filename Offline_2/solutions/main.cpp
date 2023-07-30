#include<bits/stdc++.h>
#include "graphics_lib.h"

using namespace std;

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        cout << "Usage: ./main <scene.txt> <config.txt>" << endl;
        return 1;
    }

    // read input from <scene.txt> file
    string sceneFileName = argv[1];
    string configFileName = argv[2];
    Scene scene(sceneFileName, configFileName);
    scene.draw();

    return 0;
}