#include <cstdlib>
#include <iostream>
#include <ctime>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/GL.h>
#include <SOIL.h>

#include "shaderManager.h"
#include "model.h"
#include "Spline.h"

#include "sse\quaternionX.h"

#include "logger.h"

// For CSS parsing
#include <Awesomium\WebCore.h>

#define FPS  60

using namespace std;

#define BUFFER_OFFSET(i) ((char*)NULL + (i))


enum exampleType{ QUATERNION, SPLINE };
exampleType mode = SPLINE;

GLuint buffer[1];
GLuint ebuffer[1];
int NumElements;

GLuint program;
int numVerts[1];

bool bufferInit = false;

int index = 0;
int cnt = 0;
float timer = 0.1;

Model cube;
MyVector<Vector3> gCubes;
// test spline

Vector3 vec[] = { Vector3(-.8, -.8, 0), Vector3(1,1,0), Vector3(1, -1,0) };
Spline* spine = new Spline(vec, 3);

clock_t currTime;

Logger gamelog = Logger();

void render() {

	//if (mode == QUATERNION)
	//{
		NumElements = cube.nVertices();
		float* points = cube.getVertices();
		int dataSize = cube.nVertices() * 4 * sizeof(float);
		float* texCoords = cube.getUV();
		int tdataSize = cube.nVertices() * 2 * sizeof(float);
		GLushort* elements = cube.getIndices();
		int edataSize = cube.nVertices() * sizeof(GLushort);

		glGenBuffers(1, &buffer[0]);
		glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);

		glBufferData(GL_ARRAY_BUFFER, dataSize + tdataSize, 0, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, points);
		glBufferSubData(GL_ARRAY_BUFFER, dataSize, tdataSize, texCoords);

		glGenBuffers(1, &ebuffer[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebuffer[0]);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, edataSize, elements, GL_STATIC_DRAW);

		numVerts[0] = cube.nVertices();
	/*}
	else
	{

	}*/
}

void idle() {
	clock_t endTime = clock();
	double elapsedSeconds = double(endTime - currTime) / FPS;
	
	if (elapsedSeconds >= timer) {
		currTime = endTime;
		cnt = 0;
		index++;
		cout << index << ":[" << cube.position.x << ", " << cube.position.y << "]  " << std::endl;
		if (index >= gCubes.GetSize()) {
			index = 0;
		}
		
		//cube.translate(0.1, 0, 0, program);
		//cube.translate(abs(cube.position.x - gCubes[index].x), abs(cube.position.y - gCubes[index].y), abs(cube.position.z - gCubes[index].z), program);
		cube.position.x = gCubes.GetCopyAt(index).x; 
		cube.position.y = gCubes.GetCopyAt(index).y;
		//cube.translate(abs(cube.position.x - gCubes[index].x), abs(cube.position.y - gCubes[index].y), abs(cube.position.z - gCubes[index].z), program);
		glutPostRedisplay();
	}
	//cube.translate(0.1, 0, 0, program);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebuffer[0]);

	glUseProgram(program);

	int dataSize = numVerts[0] * 4 * sizeof(float);
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vTexCoords = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoords);
	glVertexAttribPointer(vTexCoords, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(dataSize));

	cube.setUpTexture(program);
	/*for (int i = 0; i < gCubes.GetSize(); i++)
	{
		gCubes[i].setUpTexture(program);
	}*/

	glDrawElements(GL_TRIANGLES, numVerts[0], GL_UNSIGNED_SHORT, (void*)0);

	glutSwapBuffers();
}


void createCube() {
	cube = Model(Vector3(0,0,0), Vector3(0.2, 0.2, 0.2), Euler3(45, 30, 0));
	cube.clearModel();

	cube.makeCube();
	//cube.makePlane();
	//cube.makeStrip();

	render();
}

void createGCubes()
{
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
	render();
}

void init()
{
	program = shaderSetup("../assets/shaders/vshader.glsl", "../assets/shaders/fshader.glsl");

	gamelog.log(_RELEASE, "Release Pestilence Started!!");

	if (!program) {
		cerr << "Error setting up shaders - " << errorString(shaderErrorCode) << endl;

		exit(1);
	}
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	createCube();
	createGCubes();
	//createGCubes();
	cube.loadTexture("../assets/textures/dirt.jpg");
	/*for (int i = 0; i < gCubes.GetSize(); i++)
	{
		gCubes[i].loadTexture("../assets/textures/dirt.jpg");
	}*/
}


void keyboard(unsigned char key, int x, int y)
{
	float rotateAmt = 5;
	float translateAmt = 0.1;

	switch (key) {
	case 033:
		exit(0);
		break;

	case '4':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;

	case '5':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;

	case '3':
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;

	case 'w':
		cube.translate(0, translateAmt, 0, program);
		break;
	case 's':
		cube.translate(0, -translateAmt, 0, program);
		break;
	case 'a':
		cube.translate(-translateAmt, 0, 0, program);
		break;
	case 'd':
		cube.translate(translateAmt, 0, 0, program);
		break;
	case 'q':
		cube.translate(0, 0, -translateAmt, program);
		break;
	case 'e':
		cube.translate(0, 0, translateAmt, program);
		break;

	case 'i':
		cube.rotate(0, -rotateAmt, 0, program);
		break;
	case 'k':
		cube.rotate(0, rotateAmt, 0, program);
		break;
	case 'j':
		cube.rotate(rotateAmt, 0, 0, program);
		break;
	case 'l':
		cube.rotate(-rotateAmt, 0, 0, program);
		break;
	case 'u':
		cube.rotate(0, 0, rotateAmt, program);
		break;
	case 'o':
		cube.rotate(0, 0, -rotateAmt, program);
		break;
	}


	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	currTime = clock();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Pesilence Engine 0.01");

	init();

	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
}