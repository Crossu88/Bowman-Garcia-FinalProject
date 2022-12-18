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

	// Mercury
	m_mercury = new Sphere(48, "assets\\Mercury.jpg");

	// Venus
	m_venus = new Sphere(48, "assets\\Venus.jpg");

	// The Earth
	m_planet = new Sphere(48, "assets\\2k_earth_daymap.jpg");

	// The  Earth's moon
	m_moon = new Sphere(48, "assets\\2k_moon.jpg");

	// Mars
	m_mars = new Sphere(48, "assets\\Mars.jpg");

	// Mars' moons
	m_deimos = new Sphere(48, "assets\\2k_moon.jpg");
	m_phobos = new Sphere(48, "assets\\2k_moon.jpg");

	// Jupiter
	m_jupiter = new Sphere(65, "assets\\Jupiter.jpg");
	
	// Jupiter's moons
	m_jmoon1 = new Sphere(48, "assets\\2k_moon.jpg");
	m_jmoon2 = new Sphere(48, "assets\\2k_moon.jpg");
	m_jmoon3 = new Sphere(48, "assets\\2k_moon.jpg");
	m_jmoon4 = new Sphere(48, "assets\\2k_moon.jpg");
	m_jmoon5 = new Sphere(48, "assets\\2k_moon.jpg");

	// Saturn
	m_saturn = new Sphere(65, "assets\\Saturn.jpg");
	
	// Saturn's moons
	m_smoon1 = new Sphere(48, "assets\\2k_moon.jpg");
	m_smoon2 = new Sphere(48, "assets\\2k_moon.jpg");
	m_smoon3 = new Sphere(48, "assets\\2k_moon.jpg");
	m_smoon4 = new Sphere(48, "assets\\2k_moon.jpg");
	m_smoon5 = new Sphere(48, "assets\\2k_moon.jpg");

	// Uranus
	m_uranus = new Sphere(65, "assets\\Uranus.jpg");

	//Uranas' moons
	m_umoon1 = new Sphere(48, "assets\\2k_moon.jpg");
	m_umoon2 = new Sphere(48, "assets\\2k_moon.jpg");
	m_umoon3 = new Sphere(48, "assets\\2k_moon.jpg");
	m_umoon4 = new Sphere(48, "assets\\2k_moon.jpg");
	m_umoon5 = new Sphere(48, "assets\\2k_moon.jpg");

	// Neptune
	m_neptune = new Sphere(65, "assets\\Neptune.jpg");

	// Neptune's moons
	m_nmoon1 = new Sphere(48, "assets\\2k_moon.jpg");
	m_nmoon2 = new Sphere(48, "assets\\2k_moon.jpg");
	m_nmoon3 = new Sphere(48, "assets\\2k_moon.jpg");
	m_nmoon4 = new Sphere(48, "assets\\2k_moon.jpg");
	m_nmoon5 = new Sphere(48, "assets\\2k_moon.jpg");

	// Halley's comet
	m_halley = new Sphere(48);

	// Starship
	m_ship = new Mesh(glm::vec3(2.0f, 3.0f, -5.0f), "assets\\SpaceShip-1.obj", "assets\\SpaceShip-1.png");
}

void Graphics::AnimateScene(double time)
{
  // Storage variables
  float fTime = (float) time;
  glm::mat4 currentTransform;

  // Create the origin matrix transformation
  auto originMat = glm::translate(glm::mat4(1.0f), m_systemOrigin);
  auto Halley_center = originMat * glm::translate(glm::mat4(1.0f), glm::vec3(35.0f, 0.0f, 0.0f));

  // Push the planet positions onto the stack
  m_modelStack.push(originMat);
  glm::mat4 sun_pos = m_modelStack.top();
  // Mercury's position
  m_modelStack.push(m_modelStack.top() * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 5.0f * glm::vec3(1.0f, 0.0f, 1.0f))));
  // Venus's position
  m_modelStack.push(sun_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 10.0f * glm::vec3(1.0f, 0.0f, 1.0f))));
  // Earth's position (and its moon)
  m_modelStack.push(sun_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.3, 15.0f * glm::vec3(1.0f, 0.0f, 1.0f))));
  m_modelStack.push(m_modelStack.top() * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(1.0f, 0.0f, 1.0f))));
  // Mars' position (and its moons)
  m_modelStack.push(sun_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.3, 20.0f * glm::vec3(1.0f, 0.0f, 1.0f))));
  glm::mat4 mars_pos = m_modelStack.top();
  m_modelStack.push(mars_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(1.0f, 0.0f, 1.0f))));
  m_modelStack.push(mars_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(-1.0f, 0.0f, -1.0f))));
  // Jupiter's position (and its moons)
  m_modelStack.push(sun_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.3, 40.0f * glm::vec3(1.0f, 0.0f, 1.0f))));
  glm::mat4 j_pos = m_modelStack.top();
  m_modelStack.push(j_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(0.0f, 0.0f, 1.0f))));
  m_modelStack.push(j_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(1.0f, 0.0f, 1.0f))));
  m_modelStack.push(j_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(-1.0f, 0.0f, 1.0f))));
  m_modelStack.push(j_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(-1.0f, 0.0f, -1.0f))));
  m_modelStack.push(j_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(1.0f, 0.0f, -1.0f))));
  // Saturn's position (and its moons)
  m_modelStack.push(sun_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.3, 50.0f * glm::vec3(1.0f, 0.0f, 1.0f))));
  glm::mat4 s_pos = m_modelStack.top();
  m_modelStack.push(s_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(0.0f, 0.0f, 1.0f))));
  m_modelStack.push(s_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(1.0f, 0.0f, 1.0f))));
  m_modelStack.push(s_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(-1.0f, 0.0f, 1.0f))));
  m_modelStack.push(s_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(-1.0f, 0.0f, -1.0f))));
  m_modelStack.push(s_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(1.0f, 0.0f, -1.0f))));
  // Uranas' position (and its moons)
  m_modelStack.push(sun_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.3, 60.0f * glm::vec3(1.0f, 0.0f, 1.0f))));
  glm::mat4 u_pos = m_modelStack.top();
  m_modelStack.push(u_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(0.0f, 0.0f, 1.0f))));
  m_modelStack.push(u_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(1.0f, 0.0f, 1.0f))));
  m_modelStack.push(u_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(-1.0f, 0.0f, 1.0f))));
  m_modelStack.push(u_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(-1.0f, 0.0f, -1.0f))));
  m_modelStack.push(u_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(1.0f, 0.0f, -1.0f))));
  // Neptune's position (and its moons)
  m_modelStack.push(sun_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.3, 70.0f * glm::vec3(1.0f, 0.0f, 1.0f))));
  glm::mat4 n_pos = m_modelStack.top();
  m_modelStack.push(n_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(0.0f, 0.0f, 1.0f))));
  m_modelStack.push(n_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(1.0f, 0.0f, 1.0f))));
  m_modelStack.push(n_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(-1.0f, 0.0f, 1.0f))));
  m_modelStack.push(n_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(-1.0f, 0.0f, -1.0f))));
  m_modelStack.push(n_pos * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(1.0f, 0.0f, -1.0f))));
  // The ship
  m_modelStack.push(m_modelStack.top() * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.6, 0.8f * glm::vec3(1.0f, 0.0f, 1.0f))));

  // Animate the ship
  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), -(0.6f * fTime), glm::vec3(0.0f, 1.0f, 0.0f));
  currentTransform *= glm::scale(glm::vec3(0.005f));
  m_modelStack.pop();
  
  if (m_ship != NULL)
	m_ship->SetModel(currentTransform);

  // Animate Neptune's moons
  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.65f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_nmoon5 != NULL)
	  m_nmoon5->SetModel(currentTransform);

  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.65f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_nmoon4 != NULL)
	  m_nmoon4->SetModel(currentTransform);

  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.65f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_nmoon3 != NULL)
	  m_nmoon3->SetModel(currentTransform);

  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.65f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_nmoon2 != NULL)
	  m_nmoon2->SetModel(currentTransform);

  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.65f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_nmoon1 != NULL)
	  m_nmoon1->SetModel(currentTransform);

  // Animate Neptune
  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.1f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(1.0f));
  m_modelStack.pop();

  if (m_neptune != NULL)
	  m_neptune->SetModel(currentTransform);

  // Animate Uranas' moons
  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.65f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_umoon5 != NULL)
	  m_umoon5->SetModel(currentTransform);

  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.65f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_umoon4 != NULL)
	  m_umoon4->SetModel(currentTransform);

  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.65f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_umoon3 != NULL)
	  m_umoon3->SetModel(currentTransform);

  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.65f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_umoon2 != NULL)
	  m_umoon2->SetModel(currentTransform);

  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.65f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_umoon1 != NULL)
	  m_umoon1->SetModel(currentTransform);

  // Animate Uranus
  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.1f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(1.0f));
  m_modelStack.pop();

  if (m_uranus != NULL)
	  m_uranus->SetModel(currentTransform);

  // Animate Saturn's moons
  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.65f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_smoon5 != NULL)
	  m_smoon5->SetModel(currentTransform);

  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.65f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_smoon4 != NULL)
	  m_smoon4->SetModel(currentTransform);

  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.65f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_smoon3 != NULL)
	  m_smoon3->SetModel(currentTransform);

  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.65f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_smoon2 != NULL)
	  m_smoon2->SetModel(currentTransform);

  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.65f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_smoon1 != NULL)
	  m_smoon1->SetModel(currentTransform);

  // Animate Saturn
  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.1f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(1.0f));
  m_modelStack.pop();

  if (m_saturn != NULL)
	  m_saturn->SetModel(currentTransform);

  // Animate Jupiter's moons
  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.65f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_jmoon5 != NULL)
	  m_jmoon5->SetModel(currentTransform);

  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.65f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_jmoon4 != NULL)
	  m_jmoon4->SetModel(currentTransform);

  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.65f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_jmoon3 != NULL)
	  m_jmoon3->SetModel(currentTransform);

  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.65f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_jmoon2 != NULL)
	  m_jmoon2->SetModel(currentTransform);

  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.65f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_jmoon1 != NULL)
	  m_jmoon1->SetModel(currentTransform);

  // Animate Jupiter
  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.1f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(1.0f));
  m_modelStack.pop();

  if (m_jupiter != NULL)
	  m_jupiter->SetModel(currentTransform);

  // Animate Mars' moons
  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.65f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_phobos != NULL)
	  m_phobos->SetModel(currentTransform);

  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.65f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_deimos != NULL)
	  m_deimos->SetModel(currentTransform);

  // Animate Mars
  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.5f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.55f));
  m_modelStack.pop();

  if (m_mars != NULL)
	  m_mars->SetModel(currentTransform);

  // Animate the Earth's moon
  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.65f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_moon != NULL)
	m_moon->SetModel(currentTransform);

  // Animate the Earth
  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.5f * fTime, glm::vec3(0.0f, 1.0f, 0.0f));
  currentTransform *= glm::scale(glm::vec3(0.55f));
  m_modelStack.pop();

  if (m_planet != NULL)
    m_planet->SetModel(currentTransform);

  // Animate Venus
  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.6f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.45f));
  m_modelStack.pop();

  if (m_venus != NULL)
	  m_venus->SetModel(currentTransform);

  // Animate Mercury
  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.7f * fTime, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  currentTransform *= glm::scale(glm::vec3(0.4f));
  m_modelStack.pop();

  if (m_mercury != NULL)
	  m_mercury->SetModel(currentTransform);

  // Animate the sun
  currentTransform = m_modelStack.top();
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.4f * fTime, glm::vec3(0.0f, 1.0f, 0.0f));
  currentTransform *= glm::scale(glm::vec3(2.0f));
  m_modelStack.pop();

  if (m_sun != NULL)
    m_sun->SetModel(currentTransform);

  // Animate Halley's comet
  currentTransform = Halley_center * glm::translate(glm::mat4(1.0f), CalculateOrbitPos(time, 0.5, 1.8f * glm::vec3(0.0f, 0.0f, 1.0f)));
  currentTransform *= glm::rotate(glm::mat4(1.0f), 0.4f * fTime, glm::vec3(1.0f, 0.0f, 0.0f));
  currentTransform *= glm::scale(glm::vec3(0.1, 0.1, 0.2));

  if (m_halley != NULL)
	  m_halley->SetModel(currentTransform);
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

	// Renders Halley's comet
	if (m_halley != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_halley->GetModel()));
		if (m_halley->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_halley->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_halley->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_halley->Render(m_positionAttrib, m_colorAttrib);

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

	// Renders Mercury
	if (m_mercury != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mercury->GetModel()));
		if (m_mercury->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mercury->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_mercury->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_mercury->Render(m_positionAttrib, m_colorAttrib);

	// Renders Venus
	if (m_venus != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_venus->GetModel()));
		if (m_venus->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_venus->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_venus->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_venus->Render(m_positionAttrib, m_colorAttrib);

	// Renders the Earth
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

	// Render the Earth's moon
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

	// Render Mars
	if (m_mars != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mars->GetModel()));
		if (m_mars->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mars->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_mars->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_mars->Render(m_positionAttrib, m_colorAttrib);

	// Render Mars' moons
	if (m_deimos != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_deimos->GetModel()));
		if (m_deimos->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_deimos->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_deimos->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_deimos->Render(m_positionAttrib, m_colorAttrib);

	if (m_phobos != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_phobos->GetModel()));
		if (m_phobos->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_phobos->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_phobos->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_phobos->Render(m_positionAttrib, m_colorAttrib);

	// Renders Jupiter
	if (m_jupiter != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_jupiter->GetModel()));
		if (m_jupiter->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_jupiter->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_jupiter->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_jupiter->Render(m_positionAttrib, m_colorAttrib);

	// Renders Jupiter's moons
	if (m_jmoon1 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_jmoon1->GetModel()));
		if (m_jmoon1->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_jmoon1->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_jmoon1->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_jmoon1->Render(m_positionAttrib, m_colorAttrib);

	if (m_jmoon2 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_jmoon2->GetModel()));
		if (m_jmoon2->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_jmoon2->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_jmoon2->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_jmoon2->Render(m_positionAttrib, m_colorAttrib);

	if (m_jmoon3 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_jmoon3->GetModel()));
		if (m_jmoon3->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_jmoon3->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_jmoon3->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_jmoon3->Render(m_positionAttrib, m_colorAttrib);

	if (m_jmoon4 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_jmoon4->GetModel()));
		if (m_jmoon4->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_jmoon4->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_jmoon4->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_jmoon4->Render(m_positionAttrib, m_colorAttrib);

	if (m_jmoon5 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_jmoon5->GetModel()));
		if (m_jmoon5->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_jmoon5->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_jmoon5->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_jmoon5->Render(m_positionAttrib, m_colorAttrib);

	// Renders Saturn
	if (m_saturn != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_saturn->GetModel()));
		if (m_saturn->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_saturn->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_saturn->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_saturn->Render(m_positionAttrib, m_colorAttrib);

	// Renders Saturn's moons
	if (m_smoon1 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_smoon1->GetModel()));
		if (m_smoon1->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_smoon1->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_smoon1->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_smoon1->Render(m_positionAttrib, m_colorAttrib);

	if (m_smoon2 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_smoon2->GetModel()));
		if (m_smoon2->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_smoon2->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_smoon2->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_smoon2->Render(m_positionAttrib, m_colorAttrib);

	if (m_smoon3 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_smoon3->GetModel()));
		if (m_smoon3->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_smoon3->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_smoon3->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_smoon3->Render(m_positionAttrib, m_colorAttrib);

	if (m_smoon4 != NULL) {
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_smoon4->GetModel()));
	if (m_smoon4->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_smoon4->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_smoon4->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_smoon4->Render(m_positionAttrib, m_colorAttrib);

	if (m_smoon5 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_smoon5->GetModel()));
		if (m_smoon5->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_smoon5->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_smoon5->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_smoon5->Render(m_positionAttrib, m_colorAttrib);

	// Renders Uranus
	if (m_uranus != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_uranus->GetModel()));
		if (m_uranus->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_uranus->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_uranus->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_uranus->Render(m_positionAttrib, m_colorAttrib);

	// Rendurs Uranus' moons
	if (m_umoon1 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_umoon1->GetModel()));
		if (m_umoon1->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_umoon1->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_umoon1->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_umoon1->Render(m_positionAttrib, m_colorAttrib);

	if (m_umoon2 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_umoon2->GetModel()));
		if (m_umoon2->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_umoon2->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_umoon2->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_umoon2->Render(m_positionAttrib, m_colorAttrib);

	if (m_umoon3 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_umoon3->GetModel()));
		if (m_umoon3->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_umoon3->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_umoon3->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_umoon3->Render(m_positionAttrib, m_colorAttrib);

	if (m_umoon4 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_umoon4->GetModel()));
		if (m_umoon4->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_umoon4->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_umoon4->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_umoon4->Render(m_positionAttrib, m_colorAttrib);

	if (m_umoon5 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_umoon5->GetModel()));
		if (m_umoon5->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_umoon5->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_umoon5->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_umoon5->Render(m_positionAttrib, m_colorAttrib);

	// Renders Neptune
	if (m_neptune != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_neptune->GetModel()));
		if (m_neptune->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_neptune->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_neptune->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_neptune->Render(m_positionAttrib, m_colorAttrib);

	// Renders Neptune's moons
	if (m_nmoon1 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_nmoon1->GetModel()));
		if (m_nmoon1->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_nmoon1->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_nmoon1->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_nmoon1->Render(m_positionAttrib, m_colorAttrib);

	if (m_nmoon2 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_nmoon2->GetModel()));
		if (m_nmoon2->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_nmoon2->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_nmoon2->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_nmoon2->Render(m_positionAttrib, m_colorAttrib);

	if (m_nmoon3 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_nmoon3->GetModel()));
		if (m_nmoon3->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_nmoon3->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_nmoon3->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_nmoon3->Render(m_positionAttrib, m_colorAttrib);

	if (m_nmoon4 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_nmoon4->GetModel()));
		if (m_nmoon4->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_nmoon4->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_nmoon4->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_nmoon4->Render(m_positionAttrib, m_colorAttrib);

	if (m_nmoon5 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_nmoon5->GetModel()));
		if (m_nmoon5->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_nmoon5->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_nmoon5->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		else
			m_nmoon5->Render(m_positionAttrib, m_colorAttrib);
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

