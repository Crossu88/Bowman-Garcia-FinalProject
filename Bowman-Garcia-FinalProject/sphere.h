#ifndef SPHERE_H
#define SPHERE_H

#include <math.h>
#include "object.h"
#include "Material.h"
#include "Texture.h"

class Sphere // : public Object 
{
    public:
        Sphere(int precision, const char* diffuse, const char* normal);
        Sphere(int prec, const char* fname);
        // Sphere(int prec)
        // Sphere();

        // void Render(GLint positionAttribLoc, GLint colorAttribLoc);
        void LegacyRender(GLint positionAttribLoc, GLint colorAttribLoc, GLint tcAttribLoc, GLint hasTex);
        void Render();

        glm::mat4 GetModel() { return model; }
        void SetModel(glm::mat4 matModel);


        // int getNumVertices();
        // int getNumIndices();
        // std::vector<int> getIndices();
        // std::vector<glm::vec3> getVertices();
        // std::vector<glm::vec2> getTexCoords();
        // std::vector<glm::vec3> getNormals();

        GLuint getTextureID() { return m_texture->getTextureID(); }
        Material* GetMaterial() { return m_material; }
        
        bool hasTex;

    private:
        glm::vec3 pivotLocation;
        glm::mat4 model;
        std::vector<Vertex> Vertices;
        std::vector<unsigned int> Indices;
        GLuint VB;
        GLuint IB;
        Texture* m_texture;
        Material* m_material;


        GLuint vao;

        float angle;

        // void setupVertices();
        void setupBuffers();
        void setupModelMatrix(glm::vec3 pivotLoc, float angle, float scale);
        void SetupSphere(int precision);

        // int numVertices;
        // int numIndices;
        // std::vector<int> indices;
        // std::vector<glm::vec3> vertices;
        // std::vector<glm::vec2> texCoords;
        // std::vector<glm::vec3> normals;
        // float toRadians(float degrees);

        // std::vector<float> pvalues; // vertex positions
        // std::vector<float> tvalues; // texture coordinates
        // std::vector<float> nvalues; // normal vectors
};

#endif