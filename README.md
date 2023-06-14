# CSE410 - Computer Graphics

Contains all the assignment of the course CSE410. 

## OpenGL Installation (linux)
```sh
  sudo apt update && sudo apt upgrade
  sudo apt install freeglut3-dev
```

## Running openGL in VSCode
- Install the C/C++ extension by Microsoft
- Install the Code Runner extension by Jun Han
- Hit the settings icon and select the correct cpp compiler
- Add the following lines in the tasks.json file that is created in the `.vscode` folder

```js
  "args": [
    "-lGL",
    "-lGLU",
    "-lglut"
  ],
```

Or just compile and run the code as follows:
```sh
  g++ -o <filename.cpp> <outputFileName> -lGL -lGLU -lglut
  ./<outputFileName>
```