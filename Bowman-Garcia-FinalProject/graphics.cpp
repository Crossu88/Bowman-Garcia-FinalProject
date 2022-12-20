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

	// Initialize PBR shader
	Material::InitializeShader(m_camera);

	// Create the scene
	CreateScene();

	// Creates the scene's space skybox
	CreateSkybox();

	// Creates the PBR test
	// CreatePBR();

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}

void Graphics::CreateScene()
{
	// The Sun
	m_sun = new Sphere(65, "assets\\2k_sun.jpg");

	// Mercury
	m_mercury = new Solarbody("assets\\Mercury.jpg", "assets\\Mercury-n.jpg");
	m_mercury->SetOrbitParameters(5.5f, 14.46f, glm::vec3(1.0f, 0.0f, 1.0f));
	m_mercury->SetBodyParameters(0.22f, 15.0f);
	m_mercury->GetMaterial()->SetMaterialProperties(0.2f, 1.0f, 1.0f);
	m_planets.push_back(m_mercury);

	// Venus
	m_venus = new Solarbody("assets\\Venus.jpg", "assets\\Venus-n.jpg");
	m_venus->SetOrbitParameters(8.0f, 37.0f, glm::vec3(1.0f, 0.0f, 1.0f));
	m_venus->SetBodyParameters(0.5f, 30.0f);
	m_venus->GetMaterial()->SetMaterialProperties(0.2f, 1.0f, 1.0f);
	m_planets.push_back(m_venus);

	// The Earth
	m_earth = new Solarbody("assets\\2k_earth_daymap.jpg", "assets\\2k_earth_daymap-n.jpg");
	m_earth->SetOrbitParameters(10.0f, 60.0f, glm::vec3(1.0f, 0.0f, 1.0f));
	m_earth->SetBodyParameters(0.5f, 5.0f);
	m_planets.push_back(m_earth);

	// The Moon - a child of earth
	m_moon = new Solarbody("assets\\2k_moon.jpg", "assets\\2k_moon-n.jpg");
	m_moon->SetOrbitParameters(1.0f, 4.43f, glm::vec3(1.0f, 0.0f, 1.0f));
	m_moon->SetBodyParameters(0.12f, 0.65f);
	m_moon->GetMaterial()->SetMaterialProperties(0.2f, 1.0f, 1.0f);

	m_earth->AddChild(m_moon);

	// Mars
	m_mars = new Solarbody("assets\\Mars.jpg", "assets\\Mars-n.jpg");
	m_mars->SetOrbitParameters(13.0f, 112.93f, glm::vec3(1.0f, 0.0f, 1.0f));
	m_mars->SetBodyParameters(0.25f, 5.1f);
	m_mars->GetMaterial()->SetMaterialProperties(0.2f, 1.0f, 1.0f);
	m_planets.push_back(m_mars);

	// Jupiter
	m_jupiter = new Solarbody("assets\\Jupiter.jpg", "assets\\Jupiter-n.jpg");
	m_jupiter->SetOrbitParameters(22.0f, 212.0f, glm::vec3(1.0f, 0.0f, 1.0f));
	m_jupiter->SetBodyParameters(5.6f, 2.09f);
	m_jupiter->GetMaterial()->SetMaterialProperties(0.2f, 1.0f, 1.0f);
	m_planets.push_back(m_jupiter);

	// Saturn
	m_saturn = new Solarbody("assets\\Saturn.jpg", "assets\\Uranus-n.jpg");
	m_saturn->SetOrbitParameters(32.0f, 331.0f, glm::vec3(1.0f, 0.0f, 1.0f));
	m_saturn->SetBodyParameters(4.76f, 2.1875f);
	m_saturn->GetMaterial()->SetMaterialProperties(0.2f, 1.0f, 1.0f);
	m_planets.push_back(m_saturn);

	// Uranus
	m_uranus = new Solarbody("assets\\Uranus.jpg", "assets\\Uranus-n.jpg");
	m_uranus->SetOrbitParameters(38.0f, 441.0f, glm::vec3(1.0f, 0.0f, 1.0f));
	m_uranus->SetBodyParameters(2.125f, 3.6f);
	m_uranus->GetMaterial()->SetMaterialProperties(0.2f, 1.0f, 1.0f);
	m_planets.push_back(m_uranus);

	// Neptune
	m_neptune = new Solarbody("assets\\Neptune.jpg", "assets\\Neptune-n.jpg");
	m_neptune->SetOrbitParameters(42.0f, 502.0f, glm::vec3(1.0f, 0.0f, 1.0f));
	m_neptune->SetBodyParameters(1.96f, 3.35f);
	m_neptune->GetMaterial()->SetMaterialProperties(0.2f, 1.0f, 1.0f);
	m_planets.push_back(m_neptune);

	// Comet
	m_comet = new Solarbody("assets\\Haumea.jpg", "assets\\Haumea-n.jpg");
	m_comet->SetOrbitParameters(12.0f, 20.0f, glm::vec3(0.7f, 0.2f, 1.0f));
	m_comet->SetBodyParameters(0.1f, 3.35f);
	m_comet->GetMaterial()->SetMaterialProperties(0.2f, 1.0f, 1.0f);
	m_planets.push_back(m_comet);

	// Asteroid
	m_asteroid = new Solarbody("assets\\Ceres.jpg", "assets\\Ceres-n.jpg", 5);
	m_asteroid->SetOrbitParameters(0.1f, 99999.0f, glm::vec3(0.0f, 0.0f, 0.0f));
	m_asteroid->SetBodyParameters(10.0f, 99999.0f);
	m_asteroid->GetMaterial()->SetMaterialProperties(0.2f, 1.0f, 1.0f);

	// Procedurally generate asteroid positions
	GenerateAsteroids(200, 16.0f);
	GenerateAsteroids(400, 48.0f);

	for (Solarbody* planet : m_planets)
		planet->Update(0.0);
}


void Graphics::GenerateAsteroids(int count, float distance)
{
	float twoPi = 2.0f * glm::pi<float>();
	for (int i = 0; i < count; i++)
	{
		float randomTheta = glm::linearRand(0.0f, twoPi);
		float randomDist = glm::linearRand(0.9f, 1.1f);
		float randomHeight = glm::linearRand(-0.1f, 0.1f);
		m_asteroidPositions.push_back(distance * randomDist * glm::vec3(cos(randomTheta), 0.0f, sin(randomTheta)) + glm::vec3(0.0f, randomHeight, 0.0f));

		float randomSize = glm::linearRand(0.1f, 0.3f);
		m_asteroidSizes.push_back(randomSize);
	}
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
	for (Solarbody* planet : m_planets)
		planet->Update(time);

	float fTime = (float) time;

  	// Animate the sun
  	glm::mat4 currentTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
  	currentTransform *= glm::rotate(glm::mat4(1.0f), 0.4f * fTime, glm::vec3(0.0f, 1.0f, 0.0f));
  	currentTransform *= glm::scale(glm::vec3(9.0f));

	if (m_sun != NULL)
	    m_sun->SetModel(currentTransform);
}

glm::vec3 Graphics::CalculateOrbitPos(double time, double speed, glm::vec3 offset)
{
  return glm::vec3(cos(speed * time) * offset.x, sin(speed * time) * offset.y, sin(speed * time) * offset.z);
}

void Graphics::Update(double dt)
{
  AnimateScene(glfwGetTime());

  m_camera->Update(dt);

  if (m_player != NULL)
	m_player->Update(dt);
}

void Graphics::RenderScene()
{
	// Start the correct program
	m_shader->Enable();

	// Send in the projection and view to the shader (stay the same while camera intrinsic(perspective) and extrinsic (view) parameters are the same
	glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

	// Renders the player ship
	// if (m_playerShip != NULL) {
	// 	glUniform1i(m_hasTexture, false);
	// 	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_playerShip->GetModel()));
	// 	if (m_playerShip->hasTex) {
	// 		glActiveTexture(GL_TEXTURE0);
	// 		glBindTexture(GL_TEXTURE_2D, m_playerShip->getTextureID());
	// 		GLuint sampler = m_shader->GetUniformLocation("sp");
	// 		if (sampler == INVALID_UNIFORM_LOCATION)
	// 		{
	// 			printf("Sampler Not found not found\n");
	// 		}
	// 		glUniform1i(sampler, 0);
	// 		m_playerShip->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
	// 	}
	// 	else
	// 		m_playerShip->Render(m_positionAttrib, m_colorAttrib);
	// }

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
			m_sun->LegacyRender(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Renders the planets and their children (moons)
	for (Solarbody* planet : m_planets)
		planet->Render();


	
	int asteroidCount = glm::min(m_asteroidPositions.size(), m_asteroidSizes.size());
	for (int i = 0; i < asteroidCount; i++)
	{
		float size = m_asteroidSizes.at(i);
		glm::vec3 pos = m_asteroidPositions.at(i);
		m_asteroid->SetBodyParameters(size, 9999.0f);

		glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);

		m_asteroid->Update(1.0, model);
		m_asteroid->Render();
	}

}

void Graphics::Render()
{
	//clear the screen
	glClearColor(0.5, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Renders the scene
	RenderScene();

	// Render the scene's skybox
	RenderSkybox();

	// Render the PBR test
	//RenderPBR();

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

