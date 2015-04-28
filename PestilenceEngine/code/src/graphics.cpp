#include "graphics.h"

Graphics::Graphics() {

}

Graphics::~Graphics() {

}

void Graphics::init() {
	program = shaderSetup("../assets/shaders/vshader.glsl", "../assets/shaders/fshader.glsl");
	if (!program) {
		cerr << "Error setting up shaders - " << errorString(shaderErrorCode) << endl;

		exit(1);
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	//Vector3 vec[] = { Vector3(-.8, -.8, 0), Vector3(1, 1, 0), Vector3(1, -1, 0) };
	//spine = new Spline(vec, 3);

	createCube();
	//createGCubes();
	buildGeometryBuffers();
}

void Graphics::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

	mat4 mvp = Camera::instance().MVP();
	glUniformMatrix4fv(m_MVPLoc, 1, GL_FALSE, &mvp[0][0]);

	cube.render(program);

	glutSwapBuffers();
}

void Graphics::buildGeometryBuffers() {
	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);

	m_MVPLoc = glGetUniformLocation(program, "MVP");

	cube.buildGeometryBuffers();
}

void Graphics::createCube() {
	cube = Model();
	cube.clearModel();
	cube.loadModel("../assets/models/sm_suzanne.obj");
	cube.loadTexture("../assets/textures/t_suzanne.png");
}

void Graphics::createGCubes() {
	for (int i = 0; i < spine->graphPoints->GetSize(); i++)
	{
		Vector3 currPt = *(spine->graphPoints->GetCopyAt(i));
		if (i == 0)
		{
			cube.position.x = currPt.x;
			cube.position.y = currPt.y;
		}
		//Model m = Model();
		//m.clearModel(); 
		//m.makeCube(); 
		//m.translate(currPt.x, currPt.y, currPt.z, program);
		gCubes.Push(currPt);
	}
	buildGeometryBuffers();
}