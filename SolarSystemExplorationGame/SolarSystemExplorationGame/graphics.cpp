/* Graphics Class, Author: Dr. Tavakkoli */

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
	if (!m_shader->AddShader(GL_VERTEX_SHADER, "vertex_shader.glsl"))
	{
		printf("Vertex Shader failed to Initialize\n");
		return false;
	}

	// Add the fragment shader
	if (!m_shader->AddShader(GL_FRAGMENT_SHADER, "fragment_shader.glsl"))
	{
		printf("Fragment Shader failed to Initialize\n");
		return false;
	}

	// Connect the program
	if (!m_shader->Finalize())
	{
		printf("Program Failed to Finalize\n");
		return false;
	}

	m_cubemap_shader = new Shader();
	if (!m_cubemap_shader->Initialize())
	{
		printf("Cubemap Shader Failed to Initialize\n");
		return false;
	}

	// Add the Cubemap Vertex Shader
	if (!m_cubemap_shader->AddShader(GL_VERTEX_SHADER, "vertex_cube_shader.glsl"))
	{
		printf("Cubemap Vertex Shader failed to Initialize\n");
		return false;
	}

	// Add the Cubemap Fragment Shader
	if (!m_cubemap_shader->AddShader(GL_FRAGMENT_SHADER, "fragment_cube_shader.glsl"))
	{
		printf("Cubemap Fragment Shader failed to Initialize\n");
		return false;
	}

	// Connect the program
	if (!m_cubemap_shader->Finalize())
	{
		printf("Program Failed to Finalize\n");
		return false;
	}

	m_light_shader = new Shader();
	if (!m_light_shader->Initialize())
	{
		printf("Light Shader Failed to Initialize\n");
		return false;
	}

	if (!m_light_shader->AddShader(GL_VERTEX_SHADER, "vertex_light_shader.glsl"))
	{
		printf("Light Vertex Shader failed to Initialize\n");
		return false;
	}

	if (!m_light_shader->AddShader(GL_FRAGMENT_SHADER, "fragment_light_shader.glsl"))
	{
		printf("Light Fragment Shader failed to Initialize\n");
		return false;
	}

	if (!m_light_shader->Finalize())
	{
		printf("Program failed to finalize\n");
		return false;
	}

	// Populate location bindings of the shader uniform/attribs
	if (!collectShPrLocs()) {
		printf("Some shader attribs not located!\n");
	}

	// Cubemap
	m_cubemapTex = new CubemapTexture("assets\\Cubemaps\\skybox2\\cubemapPosX.png",
		"assets\\Cubemaps\\skybox2\\cubemapNegX.png",
		"assets\\Cubemaps\\skybox2\\cubemapPosY.png",
		"assets\\Cubemaps\\skybox2\\cubemapNegY.png",
		"assets\\Cubemaps\\skybox2\\cubemapPosZ.png",
		"assets\\Cubemaps\\skybox2\\cubemapNegZ.png");

	// Starship
	// m_mesh = new Mesh(glm::vec3(2.0f, 3.0f, -5.0f), "assets\\SpaceShip-1.obj", "assets\\SpaceShip-1.png");

	// The Sun
	m_sun = new Sphere(64, "assets\\PlanetaryTextures\\2k_sun.jpg", "IMG_TEXTURE");

	// Mercury
	m_mercury = new Sphere(48, "assets\\PlanetaryTextures\\Mercury.jpg", "IMG_TEXTURE");
	bool loadNorm = m_mercury->loadTexture("assets\\PlanetaryTextures\\Mercury-n.jpg", "NORMAL_TEXTURE");

	if (!loadNorm) {
		printf("failed to load normal texture\n");
		return false;
	}

	// The moon
	m_moon = new Sphere(48, "assets\\PlanetaryTextures\\2k_moon.jpg", "IMG_TEXTURE");


	// Set up lighting

	std::vector<float> globalAmbLight = { 0.2f, 0.2f, 0.2f, 1.0f};

	// Sunlight Parameters
	std::vector<float> sunAmbLight = { 0.5f, 0.5f, 0.5f, 1.0f };
	std::vector<float> sunDiffLight = { 0.8f, 0.8f, 0.8f, 1.0f };
	std::vector<float> sunSpecLight = { 0.5f, 0.5f, 0.5f, 1.0f };

	glm::vec3 sunPosition = { 0, 0, 0 };

	m_sunlight = new Light(GL_LIGHT0, m_camera->GetView(), sunPosition, globalAmbLight,
		sunAmbLight, sunDiffLight, sunSpecLight);

	// enable lighting
	//glEnable(GL_LIGHTING);
	//m_sunlight->Init();

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	// enable seamless texture for cubemap
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	return true;
}

void Graphics::HierarchicalUpdate2(double dt) {

	std::vector<float> speed, dist, rotSpeed, scale;
	glm::vec3 rotVector;
	glm::mat4 localTransform;
	// position of the sun	
	modelStack.push(glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0)));  // sun's coordinate
	localTransform = modelStack.top();		// The sun origin
	localTransform *= glm::rotate(glm::mat4(1.0f), (float)dt * .5f, glm::vec3(0.f, 1.f, 0.f));
	localTransform *= glm::scale(glm::vec3(1.25, 1.25, 1.25));
	if (m_sun != NULL)
		m_sun->Update(localTransform);


	// position of the first planet
	speed = { .75, .75, .75 };
	dist = { 7. , 0., 7.};
	rotVector = { 0. , 1., 0. };
	rotSpeed = { 1., 1., 1. };
	scale = { .75,.75,.75 };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_mercury != NULL)
		m_mercury->Update(localTransform);

	

	// position of the first moon
	speed = { 3, 3, 3 };
	dist = { 1.25, .50, 1.25 };
	rotVector = { 1.,1.,1. };
	rotSpeed = { .25, .25, .25 };
	scale = { .20f, .20f, .20f };
	localTransform = modelStack.top();
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store moon-planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_moon != NULL)
		m_moon->Update(localTransform);

	modelStack.pop();
	modelStack.pop();

	speed = { 1, 1., 1. };
	dist = { 0, 6., 6. };
	rotVector = { 1 , 0, 0 };
	rotSpeed = { 1, 1, 1. };
	scale = { .02,.02,.02 };
	localTransform = modelStack.top();	
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(sin(speed[0] * dt) * dist[0], cos(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), -80.f, glm::vec3(1, 0, 0));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt , rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_mesh != NULL)
		m_mesh->Update(localTransform);





	modelStack.pop(); 	// back to the planet coordinate


	modelStack.pop(); 	// back to the sun coordinate

	//modelStack.pop();	// empy stack


}


void Graphics::ComputeTransforms(double dt, std::vector<float> speed, std::vector<float> dist, 
	std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale, glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat) {
	tmat = glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2])
	);
	rmat = glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	smat = glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
}

void Graphics::Render()
{
	//clear the screen
	//glClearColor(0.5, 0.2, 0.2, 1.0);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render Skybox First
	m_cubemap_shader->Enable();

	glUniformMatrix4fv(m_cubeProjectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(m_cubeMVmatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

	/*if (m_cubemapTex != NULL) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTex->getTextureID());
		GLuint sampler = m_cubemap_shader->GetUniformLocation("samp");
		if (sampler == INVALID_UNIFORM_LOCATION)
		{
			printf("Sampler Not found not found\n");
		}
		glUniform1i(sampler, 0);

		m_cubemapTex->Render(m_cubePositionAttrib);
	}*/

	// Render the objects
	/*if (m_cube != NULL){
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_cube->GetModel()));
		m_cube->Render(m_positionAttrib,m_colorAttrib);
	}*/
	
	/*if (m_mesh != NULL) {
		glUniform1i(m_hasTexture, false);
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mesh->GetModel()));
		if (m_mesh->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mesh->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_mesh->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}*/

	/*if (m_pyramid != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_pyramid->GetModel()));
		m_pyramid->Render(m_positionAttrib, m_colorAttrib);
	}*/


	// Start the correct program
	m_light_shader->Enable();

	// Send in the projection and view to the shader (stay the same while camera intrinsic(perspective) and extrinsic (view) parameters are the same
	glUniformMatrix4fv(m_lightProjectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(m_lightViewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));


	if (m_sun != NULL) {
		glUniformMatrix4fv(m_lightModelMatrix, 1, GL_FALSE, glm::value_ptr(m_sun->GetModel()));

		if (m_sun->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sun->getTextureID("IMG_TEXTURE"));
			GLuint sampler = m_light_shader->GetUniformLocation("sampler");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			glUniform1i(m_lightHasTexture, true);
		}
		/*if (m_sun->hasNorm) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_sun->getTextureID("NORMAL_TEXTURE"));
			GLuint sampler = m_shader->GetUniformLocation("sampler1");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			glUniform1i(m_hasNormal, true);
		}*/

		m_sun->Render(m_lightPositionAttrib, m_lightColorAttrib, m_lightTCAttrib, m_lightHasTexture);
	}
	

	// Start the correct program
	m_shader->Enable();

	// Send in the projection and view to the shader (stay the same while camera intrinsic(perspective) and extrinsic (view) parameters are the same
	glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));


	if (m_mercury != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mercury->GetModel()));
		glUniformMatrix3fv(m_normMatrix, 1, GL_FALSE, 
			glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(m_camera->GetView() * m_mercury->GetModel())))));
		
		glUniform4fv(mAmbLoc, 1, (m_sun->matAmbient.data()));
		glUniform4fv(mDiffLoc, 1, (m_sun->matDiff.data()));
		glUniform4fv(mSpecLoc, 1,  (m_sun->matSpec.data()));
		glUniform1f(mShineLoc, (m_sun->matShininess));
		
		if (m_mercury->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mercury->getTextureID("IMG_TEXTURE"));
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			glUniform1i(m_hasNormal, false);
		}
		if (m_mercury->hasNorm) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_mercury->getTextureID("NORMAL_TEXTURE"));
			GLuint sampler = m_shader->GetUniformLocation("sp1");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			glUniform1i(m_hasNormal, true);
		}

		m_mercury->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture, m_hasNormal);
	}

	// Render Moon
	/*if (m_moon != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_moon->GetModel()));
		glUniformMatrix4fv(m_normMatrix, 1, GL_FALSE, 
			glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(m_camera->GetView() * m_moon->GetModel())))));
		if (m_moon->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_moon->getTextureID("IMG_TEXTURE"));
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			glUniform1i(m_hasNormal, false);
		}
		if (m_moon->hasNorm) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_moon->getTextureID("NORMAL_TEXTURE"));
			GLuint sampler = m_shader->GetUniformLocation("sp1");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			glUniform1i(m_hasNormal, true);
		}

		m_moon->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture, m_hasNormal);
	}*/

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

	m_normMatrix = m_shader->GetUniformLocation("normMatrix");
	if (m_normMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_normMatrix not found\n");
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

	// Locate the tc vertex attribute
	m_tcAttrib = m_shader->GetAttribLocation("v_tc");
	if (m_tcAttrib == -1)
	{
		printf("v_texcoord attribute not found\n");
		anyProblem = false;
	}

	m_hasTexture = m_shader->GetUniformLocation("hasTexture");
	if (m_hasTexture == INVALID_UNIFORM_LOCATION) {
		printf("hasTexture uniform not found\n");
		anyProblem = false;
	}

	m_hasNormal = m_shader->GetUniformLocation("hasNormalMap");
	if (m_hasNormal == INVALID_UNIFORM_LOCATION) {
		printf("hasNormalMap uniform not found\n");
		anyProblem = false;
	}

	// Locate the Cube Vertex and Shader attributes
	m_cubeProjectionMatrix = m_cubemap_shader->GetUniformLocation("projection");
	if (m_cubeProjectionMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("cubeProjection uniform not found\n");
		anyProblem = false;
	}

	m_cubeMVmatrix = m_cubemap_shader->GetUniformLocation("modelView");
	if (m_cubeMVmatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("cubeMV uniform not found\n");
		anyProblem = false;
	}

	m_cubePositionAttrib = m_cubemap_shader->GetAttribLocation("cubePosition");
	if (m_cubePositionAttrib == -1)
	{
		printf("Cubemap Position Attribute not found\n");
		anyProblem = false;
	}

	// Locate sun vertex and shader attributes
	m_lightProjectionMatrix = m_light_shader->GetUniformLocation("lightProjection");
	if (m_lightProjectionMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("light projection uniform not found\n");
		anyProblem = false;
	}

	m_lightViewMatrix = m_light_shader->GetUniformLocation("lightView");
	if (m_lightViewMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("light view uniform not found\n");
		anyProblem = false;
	}

	m_lightModelMatrix = m_light_shader->GetUniformLocation("lightModel");
	if (m_lightModelMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("light model uniform not found\n");
		anyProblem = false;
	}

	m_lightHasTexture = m_light_shader->GetUniformLocation("lightHasTexture");
	if (m_lightHasTexture == INVALID_UNIFORM_LOCATION)
	{
		printf("light has texture uniform not found\n");
		anyProblem = false;
	}

	m_lightPositionAttrib = m_light_shader->GetAttribLocation("lightposition");
	if (m_lightPositionAttrib == -1)
	{
		printf("Light Position Attribute not found\n");
		anyProblem = false;
	}

	m_lightColorAttrib = m_light_shader->GetAttribLocation("lightcolor");
	if (m_lightColorAttrib == -1)
	{
		printf("Light color attribute not found\n");
		anyProblem = false;
	}

	m_lightTCAttrib = m_light_shader->GetAttribLocation("lightTC");
	if (m_lightTCAttrib == -1)
	{
		printf("Light TC attrib not found\n");
		anyProblem = false;
	}

	// Locate Light Global Attributes
	globalAmbLoc = m_shader->GetUniformLocation("GlobalAmbient");
	if (globalAmbLoc == INVALID_UNIFORM_LOCATION)
	{
		printf("GlobalAmbient uniform not found\n");
		anyProblem = false;
	}

	lightLoc = m_shader->GetUniformLocation("light.ambient");
	if (lightLoc == INVALID_UNIFORM_LOCATION)
	{
		printf("Ambient light location uniform not found\n");
		anyProblem = false;
	}

	lightDLoc = m_shader->GetUniformLocation("light.diffuse");
	if (lightDLoc == INVALID_UNIFORM_LOCATION)
	{
		printf("Diffuse light location uniform not found\n");
		anyProblem = false;
	}

	lightSLoc = m_shader->GetUniformLocation("light.spec");
	if (lightSLoc == INVALID_UNIFORM_LOCATION)
	{
		printf("Specular light location uniform not found\n");
		anyProblem = false;
	}

	lightPosLoc = m_shader->GetUniformLocation("light.position");
	if (lightPosLoc == INVALID_UNIFORM_LOCATION)
	{
		printf("Light position location uniform not found.\n");
		anyProblem = false;
	}

	// Local Material Uniform Locations
	mAmbLoc = m_shader->GetUniformLocation("material.ambient");
	if (mAmbLoc == INVALID_UNIFORM_LOCATION)
	{
		printf("Material ambient location uniform not found.\n");
		anyProblem = false;
	}

	mDiffLoc = m_shader->GetUniformLocation("material.diffuse");
	if (mDiffLoc == INVALID_UNIFORM_LOCATION)
	{
		printf("Material diffuse location uniform not found.\n");
		anyProblem = false;
	}

	mSpecLoc = m_shader->GetUniformLocation("material.specular");
	if (mSpecLoc == INVALID_UNIFORM_LOCATION)
	{
		printf("Material specular location uniform not found.\n");
		anyProblem = false;
	}

	mShineLoc = m_shader->GetUniformLocation("material.shininess");
	if (mAmbLoc == INVALID_UNIFORM_LOCATION)
	{
		printf("Material ambient location uniform not found.\n");
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