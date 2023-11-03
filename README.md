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

## Running openGL in terminal:
```sh
  g++ <filename.cpp> -o <outputFileName> -lGL -lGLU -lglut
  ./<outputFileName>
```

## Offline 1 (Open GL Basics)
- [specification](https://github.com/fardinanam/CSE410-Computer-Graphics/blob/master/Offline_1/cse410_offline1_specification.pdf)
- [solution](https://github.com/fardinanam/CSE410-Computer-Graphics/tree/master/Offline_1/solutions)

## Offline 2 (Rasterization)
- [specification](https://github.com/fardinanam/CSE410-Computer-Graphics/blob/master/Offline_2/Offline%202%20Rasterization%20(Complete).pdf)
- [solution](https://github.com/fardinanam/CSE410-Computer-Graphics/tree/master/Offline_2/solutions)

## Offline 3 (Ray Tracing)
- [specification](https://github.com/fardinanam/CSE410-Computer-Graphics/tree/master/Offline_3/Assignment-RayTracer)
- [solution](https://github.com/fardinanam/CSE410-Computer-Graphics/tree/master/Offline_3/solutions)
