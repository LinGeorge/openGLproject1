#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <windows.h>
using namespace std;
//#include <cmath>

#include "vgl.h"
#include "LoadShaders.h"
#include "objloader.hpp"
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define PARTSNUM 18
#define BODY 0
#define LEFTSHOUDER 1
#define ULEFTARM 2
#define DLEFTARM 3
#define LEFTHAND 4

void updateModels();

void init();

void ChangeSize(int w,int h);
void display();
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button,int state,int x,int y);

void My_Timer(int val);
float	timer_interval = 30;
float	rotateAngle = 0.0f;
double	rotateSpeed = 1.0f;

void menuEvents(int option);
void ActionMenuEvents(int option);
void ModeMenuEvents(int option);
void ShaderMenuEvents(int option);
void BackgroundMenuEvents(int option);// 20181111 new added

void idle(int dummy);

mat4 translate(float x,float y,float z);
mat4 scale(float x,float y,float z);
mat4 rotate(float angle,float x,float y,float z);

void Obj2Buffer();
void load2Buffer( char* obj,int);

void updateObj5(int);
void updateObj4(int);
void updateObj3(int); // new added gangnam function
void updateObj2(int); // new added dance function
void updateObj(int);
void resetObj(int);

//the function for the skybox
unsigned int loadTexture(char const * path);
unsigned int loadCubemap(vector<std::string> faces);



bool isFrame;
GLint isMotionSwitch; // �ഫ�ʧ@���Ǧ����L���]��0�A���|���ʧ@�ഫ���~�ӤW�@���ʧ@

GLuint VAO;
GLuint VBO;
GLuint uVBO;
GLuint nVBO;
GLuint mVBO;
GLuint UBO;
GLuint VBOs[PARTSNUM];
GLuint uVBOs[PARTSNUM];
GLuint nVBOs[PARTSNUM];
unsigned int skyboxVAO, skyboxVBO; // this set is for skybox
unsigned int quadVAO, quadVBO; // ��framebuffer���ù���
GLuint program;
unsigned int programs; // this program is for skybox
unsigned int programf;// �s�@��s�S�Ī�shader-program
int backgroundMode;
int pNo;
int mode;
GLuint modeID;
GLuint timeID;
GLuint CubemapID;
GLuint pNoID;
GLuint lenID;
float frametimeID;
GLuint backgroundModeID;
double backgroundTimeID;

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
float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
};
unsigned int framebuffer; // �o�T��buffer�O�n����i�e�����S�Ĩϥ�
unsigned int textureColorbuffer;
unsigned int rbo;

float yangles[PARTSNUM]; // new add for ���u����
float zangles[PARTSNUM]; // new add for ���u���k���|
float angles[PARTSNUM];
float position = 0.0;
float angle = 0.0;
float eyeAngley = 0.0;
float eyedistance = 20.0;
float size = 1;
GLfloat movex,movey;
GLint MatricesIdx;
GLuint ModelID;
GLuint ViewPosID;// new added

int vertices_size[PARTSNUM];
int uvs_size[PARTSNUM];
int normals_size[PARTSNUM];
int materialCount[PARTSNUM];

std::vector<std::string> mtls[PARTSNUM];//use material
std::vector<unsigned int> faces[PARTSNUM];//face count
map<string,vec3> KDs;//mtl-name&Kd
map<string,vec3> KSs;//mtl-name&Ks

mat4 Projection;
mat4 View;
mat4 Model;
mat4 Models[PARTSNUM];

GLuint ProjectionskyID; // this both matrix(�x�}) ID is for loading into skybox
GLuint ViewskyID;
GLuint cubemapTexture;

#define leftHand 0
#define rightHand 1
#define leftFoot 2
#define rightFoot 3

#define JUMP 5
#define GANGNAM2 4
#define GANGNAM 3 // new add gangnam keyword
#define DANCE 2 // new add dance keyword
#define WALK 1
#define IDLE 0
//int mode;
int action;

