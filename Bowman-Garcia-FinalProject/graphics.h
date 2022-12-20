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
#include "SoloarBody.h"

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
    // Scene functions
    void CreateScene();
    void AnimateScene(double time);
    void RenderScene();

    // Skybox
    void CreateSkybox();
    void RenderSkybox();
    GLuint LoadCubemap(const char* fileName);

    ShaderFile* skyboxShader;
	  GLuint skyboxVAO, skyboxVBO;
    GLuint skyboxTexture;

    // PBR Testing
    void CreatePBR();
    void RenderPBR();
    void RenderSphere();

    GLuint diffuse;
    GLuint normal;
    GLuint metallic;
    GLuint roughness;
    GLuint ao;

    int pbrRows = 7;
    int pbrCols = 7;
    float space = 2.5f;

    GLuint sphereVAO = 0;
    GLuint indexCount;

    ShaderFile* pbrShader;

    glm::vec3 lightPositions[4] = {
        glm::vec3(-10.0f,  10.0f, 10.0f),
        glm::vec3( 10.0f,  10.0f, 10.0f),
        glm::vec3(-10.0f, -10.0f, 10.0f),
        glm::vec3( 10.0f, -10.0f, 10.0f),
    };

    glm::vec3 lightColors[4] = {
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f)
    };

    // Scene
    stack<glm::mat4> modelStack;

    Camera *m_camera;
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
    SolarBody* m_mercury;
    SolarBody* m_venus;
    SolarBody* m_planet;
    SolarBody* m_mars;
    SolarBody* m_jupiter;
    SolarBody* m_saturn;
    SolarBody* m_uranus;
    SolarBody* m_neptune;

    SolarBody* m_moon;
    SolarBody* m_deimos;
    SolarBody* m_phobos;
    SolarBody* m_jmoon1;
    SolarBody* m_jmoon2;
    SolarBody* m_jmoon3;
    SolarBody* m_jmoon4;
    SolarBody* m_jmoon5;
    SolarBody* m_smoon1;
    SolarBody* m_smoon2;
    SolarBody* m_smoon3;
    SolarBody* m_smoon4;
    SolarBody* m_smoon5;
    SolarBody* m_umoon1;
    SolarBody* m_umoon2;
    SolarBody* m_umoon3;
    SolarBody* m_umoon4;
    SolarBody* m_umoon5;
    SolarBody* m_nmoon1;
    SolarBody* m_nmoon2;
    SolarBody* m_nmoon3;
    SolarBody* m_nmoon4;
    SolarBody* m_nmoon5;

    SolarBody* m_halley;
    Mesh* m_ship;

    // Player ship
    Mesh* m_playerShip;
    Ship* m_player;

    glm::vec3 m_systemOrigin = glm::vec3(0.0f);

    stack<glm::mat4> m_modelStack;

    // Internal Functions
    glm::vec3 CalculateOrbitPos(double time, double speed, glm::vec3 offset);
    std::string ErrorString(GLenum error);
    bool collectShPrLocs();
};

#endif /* GRAPHICS_H */
