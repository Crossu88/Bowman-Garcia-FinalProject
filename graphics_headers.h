#ifndef GRAPHICS_HEADERS_H
#define GRAPHICS_HEADERS_H

#include <iostream>

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#define GLM_ENABLE_EXPERIMENTAL 

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

// GLM for matricies
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/matrix_decompose.hpp>

// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define INVALID_UNIFORM_LOCATION 0x7fffffff

// Vertex definition for meshes
struct Vertex
{
  glm::vec3 vertex;
  glm::vec3 normal;
  glm::vec2 texcoord;

  Vertex(glm::vec3 v, glm::vec3 n, glm::vec2 tc): vertex(v), normal(n) , texcoord(tc) {}
};

#endif /* GRAPHICS_HEADERS_H */
