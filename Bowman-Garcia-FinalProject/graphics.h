#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <stack>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "ShaderFile.h"
#include "object.h"
#include "sphere.h"
#include "mesh.h"
#include "ship.h"
#include "Solarbody.h"

#define numVBOs 2;
#define numIBs 2;


class Graphics
{
  public:
    Graphics();
    ~Graphics();

    // Engine accessible functions
    bool Initialize(int width, int height);
    void Update(double dt);
    void Render();

    // Accessors
    Camera* getCamera() { return m_camera; }

  private:
    // Scene Operations
    Camera *m_camera;

    void CreateScene();
    void AnimateScene(double time);
    void RenderScene();

    // Skybox
    ShaderFile* skyboxShader;
	  GLuint skyboxVAO, skyboxVBO;
    GLuint skyboxTexture;
    void CreateSkybox();
    void RenderSkybox();
    GLuint LoadCubemap(const char* fileName);

    // Legacy Shader
    Shader *m_shader;
    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_positionAttrib;
    GLint m_colorAttrib;
    GLint m_tcAttrib;
    GLint m_hasTexture;

    // Celestial Models
    Sphere* m_sun;
    Solarbody* m_mercury;
    Solarbody* m_venus;
    Solarbody* m_earth;
    Solarbody* m_moon;
    Solarbody* m_mars;
    Solarbody* m_jupiter;
    Solarbody* m_saturn;
    Solarbody* m_uranus;
    Solarbody* m_neptune;
    Solarbody* m_comet;
    Solarbody* m_asteroid;

    std::vector<Solarbody*> m_planets;
    std::vector<glm::vec3> m_asteroidPositions;
    std::vector<float> m_asteroidSizes;

    void GenerateAsteroids(int count, float distance); 

    // Player ship
    Mesh* m_playerShip;
    Ship* m_player;

    // Internal Functions
    glm::vec3 CalculateOrbitPos(double time, double speed, glm::vec3 offset);
    std::string ErrorString(GLenum error);
    bool collectShPrLocs();
};

#endif /* GRAPHICS_H */
