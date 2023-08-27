
#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "sphere.hpp"
#include "cube.hpp"
#include "pyramid.hpp"
#include "quadrilateral.hpp"

struct description {
  double near, far;
  double fovY;
  double aspectRatio;
  int levelOfRecursion;
  int numPixelsX, numPixelsY;
  double checkerBoardCellWidth;
  double checkerBoardAmbient, checkerBoardDiffuse, checkerBoardReflection;
};

struct normalLight {
  Vector position;
  double fallOff;
};

struct spotLight {
  Vector position;
  Vector lookAt;
  double fallOff;
  double cutOffAngle;
};

class DescriptionParser {
private:
  std::vector<Object*> objects;
  description viewDescription;
  std::vector<normalLight> normalLights;
  std::vector<spotLight> spotLights;
  std::string descriptionFileName;

  void pushCheckerBoard() {
    int numberOfCells = 1000;
    double widthOfEachCell = viewDescription.checkerBoardCellWidth;

    double x = -numberOfCells * widthOfEachCell/ 2;
    double y = 0;
    double z = numberOfCells * widthOfEachCell / 2;

    for (int i = 0; i < numberOfCells; i++) {
      for (int j = 0; j < numberOfCells; j++) {
        Vector color;
        if ((i + j) % 2 == 0) {
          color = { 1, 1, 1 };
        } else {
          color = { 0, 0, 0 };
        }

        Vector lowerLeft = { x, y, z };
        Vector lowerRight = { x + widthOfEachCell, y, z };
        Vector upperLeft = { x, y, z - widthOfEachCell };
        Vector upperRight = { x + widthOfEachCell, y, z - widthOfEachCell };

        objects.push_back(new Quadrilateral(lowerLeft, lowerRight, upperLeft, upperRight
          , color, viewDescription.checkerBoardAmbient, viewDescription.checkerBoardDiffuse, viewDescription.checkerBoardReflection, 0, 0));
        x += widthOfEachCell;
      }
      x = -numberOfCells * widthOfEachCell / 2;
      z -= widthOfEachCell;
    }
  }
public:
  DescriptionParser() {
    descriptionFileName = "description.txt";
  }

  DescriptionParser(std::string filename) : DescriptionParser() {
    descriptionFileName = filename;
  }

  void printDescription() {
    std::cout << "Near: " << viewDescription.near << std::endl;
    std::cout << "Far: " << viewDescription.far << std::endl;
    std::cout << "FovY: " << viewDescription.fovY << std::endl;
    std::cout << "Aspect Ratio: " << viewDescription.aspectRatio << std::endl;
    std::cout << "Level of Recursion: " << viewDescription.levelOfRecursion << std::endl;
    std::cout << "Num Pixels X: " << viewDescription.numPixelsX << std::endl;
    std::cout << "Num Pixels Y: " << viewDescription.numPixelsY << std::endl;
    std::cout << "Checkerboard Cell Width: " << viewDescription.checkerBoardCellWidth << std::endl;
    std::cout << "Checkerboard Ambient: " << viewDescription.checkerBoardAmbient << std::endl;
    std::cout << "Checkerboard Diffuse: " << viewDescription.checkerBoardDiffuse << std::endl;
    std::cout << "Checkerboard Reflection: " << viewDescription.checkerBoardReflection << std::endl;
  }

  description parse() {
    std::cout << "Parsing description file..." << std::endl;
    std::ifstream descriptionFile(descriptionFileName);
    std::string line;
    int lineNum = 0;

    while (getline(descriptionFile, line)) {
      if (line.empty()) {
        continue;
      }

      lineNum++;
      std::stringstream ss(line);

      if (lineNum == 1) {
        ss >> viewDescription.near >> viewDescription.far >> viewDescription.fovY >> viewDescription.aspectRatio;
      } else if (lineNum == 2) {
        ss >> viewDescription.levelOfRecursion;
      } else if (lineNum == 3) {
        ss >> viewDescription.numPixelsX;
        viewDescription.numPixelsY = viewDescription.numPixelsX;
      } else if (lineNum == 4) {
        ss >> viewDescription.checkerBoardCellWidth;
      } else if (lineNum == 5) {
        ss >> viewDescription.checkerBoardAmbient >> viewDescription.checkerBoardDiffuse >> viewDescription.checkerBoardReflection;
      } else if (lineNum == 6) {
        int numObjects;
        ss >> numObjects;

        for (int i = 0; i < numObjects; i++) {
          std::string objectType;

          while(getline(descriptionFile, objectType) && objectType.empty());

          if (objectType == "pyramid") {
            Vector lowest;
            GLfloat width, height;
            Vector color;
            double ambient, diffuse, reflection, specular, shininess;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> lowest.x >> lowest.y >> lowest.z;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> width >> height;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> color.x >> color.y >> color.z;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> ambient >> diffuse >> specular >> reflection;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> shininess;

            objects.push_back(new Pyramid(lowest, width, height, color, ambient, diffuse, reflection, specular, shininess));
          }

          else if (objectType == "sphere") {
            Vector center;
            GLfloat radius;
            Vector color;
            double ambient, diffuse, reflection, specular, shininess;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> center.x >> center.y >> center.z;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> radius;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> color.x >> color.y >> color.z;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> ambient >> diffuse >> specular >> reflection;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> shininess;

            objects.push_back(new Sphere(center, radius, color, ambient, diffuse, reflection, specular, shininess));
          } else if (objectType == "cube") {
            Vector bottomLowerLeft;
            GLfloat length;
            Vector color;
            double ambient, diffuse, reflection, specular, shininess;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> bottomLowerLeft.x >> bottomLowerLeft.y >> bottomLowerLeft.z;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> length;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> color.x >> color.y >> color.z;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> ambient >> diffuse >> specular >> reflection;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> shininess;

            objects.push_back(new Cube(bottomLowerLeft, length, color, ambient, diffuse, reflection, specular, shininess));
          }
        }
      } else if (lineNum == 7) {
        int numNormalLights;
        ss >> numNormalLights;

        for (int i = 0; i < numNormalLights; i++) {
          Vector position;
          double fallOff;
          
          while (getline(descriptionFile, line) && line.empty());

          ss = std::stringstream(line);
          ss >> position.x >> position.y >> position.z >> fallOff;

          normalLight light;
          light.position = position;
          light.fallOff = fallOff;
          normalLights.push_back(light);
        }
      } else if (lineNum == 8) {
        int numSpotLights;
        ss >> numSpotLights;

        for (int i = 0; i < numSpotLights; i++) {
          Vector position, lookAt;
          double fallOff, cutOffAngle;

          while (getline(descriptionFile, line) && line.empty());

          ss = std::stringstream(line);
          ss >> position.x >> position.y >> position.z >> fallOff;

          getline(descriptionFile, line);
          ss = std::stringstream(line);
          ss >> lookAt.x >> lookAt.y >> lookAt.z;

          getline(descriptionFile, line);
          ss = std::stringstream(line);
          ss >> cutOffAngle;

          spotLight light;
          light.position = position;
          light.lookAt = lookAt;
          light.fallOff = fallOff;
          light.cutOffAngle = cutOffAngle;
          spotLights.push_back(light);
        }
      } else {
        break;
      }
    }

    descriptionFile.close();

    pushCheckerBoard();
    return viewDescription;
  }

  description getViewDescription() {
    return viewDescription;
  }

  std::vector<Object*> getObjects() {
    return objects;
  }

  std::vector<normalLight> getNormalLights() {
    return normalLights;
  }

  std::vector<spotLight> getSpotLights() {
    return spotLights;
  }

  ~DescriptionParser() {
    for (int i = 0; i < objects.size(); i++) {
      delete objects[i];
    }
  }
};

#endif