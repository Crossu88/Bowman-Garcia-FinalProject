#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height)
{
	// Used for the linux OS
#if !defined(__APPLE__) && !defined(MACOSX)
  // cout << glewGetString(GLEW_VERSION) << endl;
	glewExperimental = GL_TRUE;

	auto status = glewInit();

	// This is here to grab the error that comes from glew init.
	// This error is an GL_INVALID_ENUM that has no effects on the performance
	glGetError();

	//Check for error
	if (status != GLEW_OK)
	{
		std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
		return false;
	}
#endif

	// Init Camera
	m_camera = new Camera();
	if (!m_camera->Initialize(width, height))
	{
		printf("Camera Failed to Initialize\n");
		return false;
	}

	// Set up the shaders
	m_shader = new Shader();
	if (!m_shader->Initialize())
	{
		printf("Shader Failed to Initialize\n");
		return false;
	}

	// Add the vertex shader
	if (!m_shader->AddShader(GL_VERTEX_SHADER))
	{
		printf("Vertex Shader failed to Initialize\n");
		return false;
	}

	// Add the fragment shader
	if (!m_shader->AddShader(GL_FRAGMENT_SHADER))
	{
		printf("Fragment Shader failed to Initialize\n");
		return false;
	}

	// Connect the program
	if (!m_shader->Finalize())
	{
		printf("Program to Finalize\n");
		return false;
	}

	// Populate location bindings of the shader uniform/attribs
	if (!collectShPrLocs()) {
		printf("Some shader attribs not located!\n");
	}

	// Create the scene
	// CreateScene();

	// Creates the scene's space skybox
	CreateSkybox();

	// Creates the PBR test
	CreatePBR();

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}

void Graphics::CreatePBR()
{
	// pbrShader = new ShaderFile("shaders\\pbr.vs", "shaders\\pbr.fs");
	// pbrShader = new ShaderFile("shaders\\hybridpbr.vs", "shaders\\hybridpbr.fs");
	pbrShader = new ShaderFile("shaders\\texturepbr.vs", "shaders\\texturepbr.fs");

	diffuse = (new Texture("assets\\rustediron\\diffuse.png"))->getTextureID();
	normal = (new Texture("assets\\rustediron\\normal.png"))->getTextureID();
	metallic = (new Texture("assets\\rustediron\\metallic.png"))->getTextureID();
	roughness = (new Texture("assets\\rustediron\\roughness.png"))->getTextureID();
	ao = (new Texture("assets\\rustediron\\ao.png"))->getTextureID();

	pbrShader->use();
	// pbrShader->setVec3("albedo", 0.5f, 0.0f, 0.0f);
	// pbrShader->setFloat("ao", 1.0f);
	pbrShader->setInt("albedoMap", 0);
	pbrShader->setInt("norrmalMap", 1);
	pbrShader->setInt("metallicMap", 2);
	pbrShader->setInt("roughnessMap", 3);
	pbrShader->setInt("aoMap", 4);

	pbrShader->use();
	pbrShader->setMat4("projection", m_camera->GetProjection());
}

void Graphics::RenderPBR()
{
	pbrShader->use();
	pbrShader->setMat4("view", m_camera->GetView());
	pbrShader->setVec3("camPos", m_camera->GetPosition());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuse);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, metallic);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, roughness);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, ao);

	glm::mat4 model = glm::mat4(1.0f);
	for (int row = 0; row < pbrRows; ++row)
	{
		pbrShader->setFloat("metallic", (float)row / (float)pbrRows);
		for (int col = 0; col < pbrCols; ++col)
		{
			pbrShader->setFloat("roughness", glm::clamp((float)col / (float)pbrCols, 0.05f, 1.0f));

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(
				(col - (pbrCols / 2)) * space,
				(row - (pbrRows / 2)) * space,
				0.0f
			));

			pbrShader->setMat4("model", model);
			RenderSphere();
		}
	}

	for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
	{
		glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
		newPos = lightPositions[i];
		pbrShader->setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
		pbrShader->setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

		model = glm::mat4(1.0f);
		model = glm::translate(model, newPos);
		model = glm::scale(model, glm::vec3(0.5f));
		pbrShader->setMat4("model", model);
		RenderSphere();
	}
}

void Graphics::RenderSphere()
{
    if (sphereVAO == 0)
    {
        glGenVertexArrays(1, &sphereVAO);

        unsigned int vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359f;
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
        {
            for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                positions.push_back(glm::vec3(xPos, yPos, zPos));
                uv.push_back(glm::vec2(xSegment, ySegment));
                normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }

        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y       * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y       * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        indexCount = static_cast<unsigned int>(indices.size());

        std::vector<float> data;
        for (unsigned int i = 0; i < positions.size(); ++i)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);           
            if (normals.size() > 0)
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
            if (uv.size() > 0)
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
        }
        glBindVertexArray(sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        unsigned int stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);        
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));        
    }

    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}

void Graphics::CreateScene()
{
	// The Sun
	m_sun = new Sphere(65, "assets\\2k_sun.jpg");
	// m_sun = new Sphere(65, "assets\\Cubemaps\\Galaxy-cubemap1.png");

	// The Earth
	m_planet = new Sphere(48, "assets\\2k_earth_daymap.jpg");

	// The moon
	m_moon = new Sphere(48, "assets\\2k_moon.jpg");

	// Starship
	m_ship = new Mesh(glm::vec3(2.0f, 3.0f, -5.0f), "assets\\SpaceShip-1.obj", "assets\\SpaceShip-1.png");

	// Player ship mesh
	// m_playerShip = new Mesh(glm::vec3(0.0f, 0.0f, 0.0f), "assets\\SpaceShip-1.obj", "assets\\SpaceShip-1.png");

	// Create player ship
	// m_player = new Ship(m_playerShip, m_camera);
}

void Graphics::CreateSkybox()
{
	skyboxShader = new ShaderFile("shaders\\skybox.vs", "shaders\\skybox.fs");

	float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	skyboxTexture = LoadCubemap("assets\\cubemaps\\milkyway.png");

	skyboxShader->use();
	skyboxShader->setInt("skybox", 0);
}

void Graphics::RenderSkybox()
{
	// Draw skybox
	glDepthFunc(GL_LEQUAL);
	skyboxShader->use();
	glm::mat4 view = glm::mat4(glm::mat3(m_camera->GetView()));
	skyboxShader->setMat4("view", view);
	skyboxShader->setMat4("projection", m_camera->GetProjection());

	// Skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

GLuint Graphics::LoadCubemap(const char* fileName)
{
	GLuint textureID;

	textureID = SOIL_load_OGL_single_cubemap(
		fileName,
		"EWUDNS",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
	);

	if (!textureID)
		printf("Failed: Could not open cubemap texture file!\n");

	return textureID;
}

void Graphics::AnimateScene(double time)
{
  // Storage variables
  float fTime = (float) time;
  glm::mat4 currentTransform;

  // Create the origin matrix transformation
  auto originMat = glm::translate(glm::mat4(1.0f), m_systemOrigin);

  // Push the planet positions onto the stack
  m_modelStack.push(originMat);
  m_modelStack.push(m_modelStack.top() * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.3, 5.0f * glm::vec3(1.0f, 0.0f, 1.0f))));
  m_modelStack.push(m_modelStack.top() * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(1.0f, 0.0f, 1.0f))));
  m_modelStack.push(m_modelStack.top() * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.6, 0.8f * glm::vec3(1.0f, 0.0f, 1.0f))));

  // Animate the ship
  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), -(0.6f * fTime), glm::vec3(0.0f, 1.0f, 0.0f));
  currentTransform *= glm::scale(glm::vec3(0.005f));
  m_modelStack.pop();
  
  if (m_ship != NULL)
	m_ship->SetModel(currentTransform);


  // Animate the first moon
  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.65f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_moon != NULL)
	m_moon->SetModel(currentTransform);

  // Animate the planet
  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.5f * fTime, glm::vec3(0.0f, 1.0f, 0.0f));
  currentTransform *= glm::scale(glm::vec3(0.55f));
  m_modelStack.pop();

  if (m_planet != NULL)
    m_planet->SetModel(currentTransform);

  // Animate the sun
  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.4f * fTime, glm::vec3(0.0f, 1.0f, 0.0f));
  currentTransform *= glm::scale(glm::vec3(2.0f));
  m_modelStack.pop();

  if (m_sun != NULL)
    m_sun->SetModel(currentTransform);
}

glm::vec3 Graphics::CalculateOrbitPos(double time, double speed, glm::vec3 offset)
{
  return glm::vec3(cos(speed * time) * offset.x, sin(speed * time) * offset.y, sin(speed * time) * offset.z);
}

void Graphics::Update(double dt)
{
//   AnimateScene(glfwGetTime());

  m_camera->Update(dt);

  if (m_player != NULL)
	m_player->Update(dt);
}

//void Graphics::

void Graphics::RenderScene()
{
	// Start the correct program
	m_shader->Enable();

	// Send in the projection and view to the shader (stay the same while camera intrinsic(perspective) and extrinsic (view) parameters are the same
	glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

	// Renders the ship
	if (m_ship != NULL) {
		glUniform1i(m_hasTexture, false);
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ship->GetModel()));
		if (m_ship->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ship->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_ship->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_ship->Render(m_positionAttrib, m_colorAttrib);
	}

	// Renders the player ship
	if (m_playerShip != NULL) {
		glUniform1i(m_hasTexture, false);
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_playerShip->GetModel()));
		if (m_playerShip->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_playerShip->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_playerShip->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_playerShip->Render(m_positionAttrib, m_colorAttrib);
	}

	// Renders the sun
	if (m_sun != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sun->GetModel()));
		if (m_sun->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sun->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_sun->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_sun->Render(m_positionAttrib, m_colorAttrib);
	}

	// Renders the planet
	if (m_planet != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_planet->GetModel()));
		if (m_planet->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_planet->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_planet->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_planet->Render(m_positionAttrib, m_colorAttrib);
	}

	// Render the moon
	if (m_moon != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_moon->GetModel()));
		if (m_moon->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_moon->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_moon->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_moon->Render(m_positionAttrib, m_colorAttrib);
	}
}

void Graphics::Render()
{
	//clear the screen
	glClearColor(0.5, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Renders the scene
	// RenderScene();

	// Render the scene's skybox
	RenderSkybox();

	// Render the PBR test
	RenderPBR();

	// Get any errors from OpenGL
	auto error = glGetError();
	if (error != GL_NO_ERROR)
	{
		string val = ErrorString(error);
		std::cout << "Error initializing OpenGL! " << error << ", " << val << std::endl;
	}
}

bool Graphics::collectShPrLocs() {
	bool anyProblem = true;
	// Locate the projection matrix in the shader
	m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
	if (m_projectionMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_projectionMatrix not found\n");
		anyProblem = false;
	}

	// Locate the view matrix in the shader
	m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
	if (m_viewMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_viewMatrix not found\n");
		anyProblem = false;
	}

	// Locate the model matrix in the shader
	m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
	if (m_modelMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_modelMatrix not found\n");
		anyProblem = false;
	}

	// Locate the position vertex attribute
	m_positionAttrib = m_shader->GetAttribLocation("v_position");
	if (m_positionAttrib == -1)
	{
		printf("v_position attribute not found\n");
		anyProblem = false;
	}

	// Locate the color vertex attribute
	m_colorAttrib = m_shader->GetAttribLocation("v_color");
	if (m_colorAttrib == -1)
	{
		printf("v_color attribute not found\n");
		anyProblem = false;
	}

	// Locate the color vertex attribute
	m_tcAttrib = m_shader->GetAttribLocation("v_tc");
	if (m_tcAttrib == -1)
	{
		printf("v_texcoord attribute not found\n");
		anyProblem = false;
	}

	// Located the texture of the shader
	m_hasTexture = m_shader->GetUniformLocation("hasTexture");
	if (m_hasTexture == INVALID_UNIFORM_LOCATION) {
		printf("hasTexture uniform not found\n");
		anyProblem = false;
	}

	return anyProblem;
}

std::string Graphics::ErrorString(GLenum error)
{
	if (error == GL_INVALID_ENUM)
	{
		return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
	}

	else if (error == GL_INVALID_VALUE)
	{
		return "GL_INVALID_VALUE: A numeric argument is out of range.";
	}

	else if (error == GL_INVALID_OPERATION)
	{
		return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
	}

	else if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
	{
		return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
	}

	else if (error == GL_OUT_OF_MEMORY)
	{
		return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
	}
	else
	{
		return "None";
	}
}

