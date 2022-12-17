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
	CreateScene();

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}

void Graphics::CreateScene()
{
	// The Sun
	m_sun = new Sphere(65, "assets\\2k_sun.jpg");

	// The Earth
	m_planet = new Sphere(48, "assets\\2k_earth_daymap.jpg");

	// The moon
	m_moon = new Sphere(48, "assets\\2k_moon.jpg");

	// Starship
	m_ship = new Mesh(glm::vec3(2.0f, 3.0f, -5.0f), "assets\\SpaceShip-1.obj", "assets\\SpaceShip-1.png");

	// Create player ship
	m_player = new Ship(
		new Mesh(glm::vec3(2.0f, 3.0f, -5.0f), "assets\\SpaceShip-1.obj", "assets\\SpaceShip-1.png"),
		m_camera
	);
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
  // AnimateScene(dt);
  AnimateScene(glfwGetTime());
  m_camera->Update(dt);
}

void Graphics::Render()
{
	//clear the screen
	glClearColor(0.5, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

