#include<bits/stdc++.h>
#include "graphics_lib.h"

using namespace std;

int main(int argc, char const *argv[]) {
    string sceneFileName = "scene.txt";
    string configFileName = "config.txt";

    if (argc != 3) {
        cout << "It is recommended to mention the scene and config file names as command line arguments." << endl;
        cout << "Usage: ./main <scene.txt> <config.txt>" << endl;
        cout << "Using default file names: <scene.txt> <config.txt>" << endl;
    } else {
        sceneFileName = argv[1];
        configFileName = argv[2];
    }

    Scene scene(sceneFileName, configFileName);
    scene.draw();
    cout << "Done!" << endl;
    
    return 0;
}