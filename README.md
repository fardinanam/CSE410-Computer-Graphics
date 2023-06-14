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
- Add the following lines in the tasks.json file
```js
  "args": [
    "-lGL",
    "-lGLU",
    "-lglut"
  ],
```