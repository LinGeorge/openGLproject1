#include "main.h"

vec3 camera = vec3(0,0,20);

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitContextVersion(4,3);//�HOpenGL version4.3���������
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);//�O�_�V�U�ۮe,GLUT_FORWARD_COMPATIBLE���䴩(?
	glutInitContextProfile(GLUT_CORE_PROFILE);

	//multisample for golygons smooth
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL 4.3 - Robot");

	glewExperimental = GL_TRUE; //�m��glewInit()���e
	if (glewInit()) {
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;//c error
		exit(EXIT_FAILURE);
	}

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	init(); // ��l�Ƴ̥�
	glutDisplayFunc(display);
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(timer_interval, My_Timer, 0);
	int ActionMenu,ModeMenu,ShaderMenu, BackgroundMenu;

	ActionMenu = glutCreateMenu(ActionMenuEvents);//�إߥk����
	//�[�J�k�䪫��
	glutAddMenuEntry("idle",0);
	glutAddMenuEntry("walk",1);
	glutAddMenuEntry("dance1", 2);
	glutAddMenuEntry("gangnam style", 3);
	glutAddMenuEntry("gangnam style2", 4);
	glutAddMenuEntry("jump", 5);
	glutAddMenuEntry("", 6);
	glutAddMenuEntry("", 7);
	glutAddMenuEntry("", 8);
	glutAddMenuEntry("", 9);
	glutAddMenuEntry("", 10);
	glutAddMenuEntry("", 11);
	glutAddMenuEntry("", 12);
	glutAddMenuEntry("", 13);
	glutAddMenuEntry("", 14);
	glutAddMenuEntry("15", 15);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//�P�k�����p

	ModeMenu = glutCreateMenu(ModeMenuEvents);//�إߥk����
	//�[�J�k�䪫��
	glutAddMenuEntry("Line",0);
	glutAddMenuEntry("Fill", 1);
	glutAddMenuEntry("Reverse", 2);
	glutAddMenuEntry("Gray", 3);
	glutAddMenuEntry("Shine", 4);
	glutAddMenuEntry("Mirrow", 5);
	glutAddMenuEntry("Toon", 6);
	glutAddMenuEntry("noise", 7);
	glutAddMenuEntry("sinWaveNoise", 8);
	glutAddMenuEntry("Laser", 9);
	glutAddMenuEntry("RedBlock", 10);
	glutAddMenuEntry("colorBlock", 11);
	



	glutAttachMenu(GLUT_RIGHT_BUTTON);	//�P�k�����p


	ShaderMenu = glutCreateMenu(ShaderMenuEvents);
	
	glutAddMenuEntry("Normal", 0);
	glutAddMenuEntry("Inverse", 1);
	glutAddMenuEntry("GrayScale", 2);
	glutAddMenuEntry("Blur", 4);
	glutAddMenuEntry("Kernal effect", 3);
	glutAddMenuEntry("Edge detection", 5);
	glutAddMenuEntry("Circle Pixel", 6);
	glutAddMenuEntry("Emboss", 7);
	glutAddMenuEntry("Water Wave", 11);
	glutAddMenuEntry("Triangle Pixel", 8);
	glutAddMenuEntry("Rectangular Pixel", 9);
	glutAddMenuEntry("Sketch", 10);
	glutAddMenuEntry("lightning", 12);
	glutAddMenuEntry("Snow", 13);
	glutAddMenuEntry("RotatingRing", 14); // 2018/11/11new add
	
	glutAddMenuEntry("Laser", 15);// 2018/11/11new add
	glutAddMenuEntry("Gold&GreenIllusion", 16);
	glutAddMenuEntry("Blue&PurpleWave", 17);
	glutAddMenuEntry("ShinyBlueHeart", 18);
	glutAddMenuEntry("ShinyNightParty", 19);
	glutAddMenuEntry("HalfCircle", 20);
	glutAddMenuEntry("VibratingCircle", 21);
	glutAddMenuEntry("22-light spot", 22);
	glutAddMenuEntry("23-blueShootingRay", 23);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	BackgroundMenu = glutCreateMenu(BackgroundMenuEvents); // 2018/11/11new add
	glutAddMenuEntry("Normal", 0);
	glutAddMenuEntry("Minecraft", 1);
	glutAddMenuEntry("CircleIllusion", 2);
	glutAddMenuEntry("RunningLine", 3);
	glutAddMenuEntry("RotatingTriangle", 4);
	glutAddMenuEntry("ColumnsWorld", 5);
	glutAddMenuEntry("ShinyMolecule", 6);
	glutAddMenuEntry("QuantumWorld", 7);
	glutAddMenuEntry("LeafWorld", 8);

	glutCreateMenu(menuEvents);//�إߥk����A�D���
	//�[�J�k�䪫��
	glutAddSubMenu("action",ActionMenu);
	glutAddSubMenu("mode",ModeMenu);
	glutAddSubMenu("robot shader", ShaderMenu);
	glutAddSubMenu("background shader", BackgroundMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//�P�k�����p



	glutMouseFunc(Mouse);
	glutTimerFunc (100, idle, 0); // �p�ƾ� : �C100�@��p�@���A�ñN0�ǤJidle�禡
	glutMainLoop();
	return 0;
}
void My_Timer(int val)
{
	glutPostRedisplay();
	glutTimerFunc(timer_interval, My_Timer, val);
	rotateAngle += timer_interval * rotateSpeed * 0.001;
}
void ChangeSize(int w,int h){
	if(h == 0) h = 1;
	glViewport(0,0,w,h);
	Projection = perspective(80.0f,(float)w/h,0.1f,100.0f);
}
void Mouse(int button,int state,int x,int y){
	if(button == 2) isFrame = false;
}
void idle(int dummy) {

	isFrame = true;
	int out = 0;

	if (action == JUMP) {
		if (isMotionSwitch == -1) {
			resetObj(dummy);
			updateObj5(out);
			isMotionSwitch = JUMP;
		}
		else {
			updateObj5(dummy);
			out = dummy + 1;
			if (out > 12) out = 1;
		}
	}

	else if (action == GANGNAM2) {
		if (isMotionSwitch == -1) {
			resetObj(dummy);
			updateObj4(out);
			isMotionSwitch = GANGNAM2;
		}
		else {
			updateObj4(dummy);
			out = dummy + 1;
			if (out > 12) out = 1;
		}
	}
	else if (action == GANGNAM) {
		if (isMotionSwitch == -1) { // isMotionSwitch == -1 �ɡA�N��W�@��O��L�ʧ@�A�n���ʧ@�q�Y�}�l
			resetObj(dummy);// ���ʧ@�q�Y�}�l(��idle���A)
			updateObj3(out);// ���ۡA���ʧ@�}�l�Ĥ@�Ӫ��A
			isMotionSwitch = 3; // isMotionSwitch = 3�A�Ʀr0,1,2,3�N���P���A(idle, walk, dance, gangnam)�A�]�����|�A�i�J�o�Ӱj��
		}
		else {
			updateObj3(dummy);
			out = dummy + 1;
			if (out > 12) out = 1;
		}

	}
	else if (action == DANCE) {
		if (isMotionSwitch == -1) {
			resetObj(dummy);
			updateObj2(out);
			isMotionSwitch = 2;
		}
		else {
			updateObj2(dummy);
			out = dummy + 1;
			if (out > 12) out = 1;
		}

	}
	else if (action == WALK) {
		if (isMotionSwitch == -1) {
			resetObj(dummy);
			updateObj(out);
			isMotionSwitch = 1;
		}
		else {
			updateObj(dummy);
			out = dummy + 1;
			if (out > 12) out = 1;
		}

	}
	else if (action == IDLE) {
		if (isMotionSwitch == -1) {
			isMotionSwitch = 0;
		}
		resetObj(dummy);
		out = 0;

	}
	glutPostRedisplay();

	glutTimerFunc(150, idle, out);
}
void resetObj(int f) {
	for (int i = 0; i < PARTSNUM; i++) {
		angles[i] = 0.0f;
		zangles[i] = 0.0f;
		yangles[i] = 0.0f;
	}
	//zangles[1] = 10;
	//zangles[6] = -10;
	//zangles[12] = 10;
	//zangles[15] = -10;
	position = 0;
}
void updateObj(int frame) {
	switch (frame) {
	case 0:

		//����
		//angles[2] = -45;//�e�u�e�s
		//�k��

		//�L
		angles[13] = 45;//�p�L���s	
		angles[16] = 45;
		//angles[13] = 90;

		break;
	case 1:
	case 2:
	case 3:
		angles[1] += 10;//���W���u������
		angles[6] -= 10;//�k�W���u�����e
		angles[12] -= 15;//���j�L�����e
		angles[15] += 15;//�k�j�L������
		angles[16] -= 15;
		position += 0.1;//���֥H�W�V��y�b����
		break;
	case 4:
	case 5:
	case 6:
		angles[1] -= 10;
		angles[6] += 10;
		angles[12] += 15;//���j�L����
		angles[15] -= 15;//�k�j�L���e
		angles[13] -= 15;//���p�L�e�s
		angles[16] = 0;
		position -= 0.1;
		break;
	case 7:
	case 8:
	case 9:
		angles[1] -= 10;
		angles[6] += 10;
		angles[12] += 15;
		angles[15] -= 15;
		angles[13] = 0;//�p�L����
		angles[16] += 15;
		position += 0.1;
		break;
	case 10:
	case 11:
	case 12:
		angles[1] += 10;
		angles[6] -= 10;
		angles[12] -= 15;
		angles[15] += 15;
		angles[13] += 15;//�p�L���s

		position -= 0.1;
		break;
	}
}

void updateObj2(int frame) {


	switch (frame) {
	case 0:

		resetObj(0);

		//����
		//angles[2] = -45;//�e�u�e�s
		//�k��

		//�L
		angles[13] = 30;//�p�L���s	
		angles[16] = 30;
		//angles[13] = 90;

		break;
	case 1:
	case 2:
	case 3:
		zangles[1] += 10;//���W���u�V����
		zangles[6] -= 10;//�k�W���u�V�k��
		zangles[12] += 10;
		zangles[15] -= 10;
		position += 0.1;//���֥H�W�V��y�b����
		break;
	case 4:
	case 5:
	case 6:
		zangles[1] += 10;
		zangles[6] -= 10;
		zangles[12] += 10;
		zangles[15] -= 10;
		position -= 0.1;
		break;
	case 7:
	case 8:
	case 9:
		zangles[1] -= 10;
		zangles[6] += 10;
		zangles[12] -= 10;
		zangles[15] += 10;
		position += 0.1;
		break;
	case 10:
	case 11:
	case 12:
		zangles[1] -= 10;
		zangles[6] += 10;
		zangles[12] -= 10;
		zangles[15] += 10;
		position -= 0.1;
		break;
	}
}

void updateObj3(int frame) {
	switch (frame) {
	case 0:
		angles[1] = -45;//���W���u�e�s(�t��)
		angles[6] = -50;//�k�W���u�e�s
						//angles[2] = -45;//���e�u�e�s
		yangles[1] = -90;//���W���u����
		yangles[6] = 90;//�k�W���u����

		zangles[12] = 30;//���j�L�~�}
		zangles[15] = -30;
		yangles[12] = 30;//���j�L�~�K(�~�ۨϥ�y�b���)
		yangles[15] = -30;
		angles[12] = -30;//���j�L�V�e
		angles[15] = -30;

		angles[13] = 45;//�p�L���s
		angles[16] = 45;

		angles[14] = 45;//���}�x�A�}
		angles[17] = 45;


		break;
	case 1:
	case 2:
	case 3:
		zangles[1] -= 5;
		zangles[6] -= 5;//���⥪�k�̰�
		angles[1] -= 5;
		angles[6] -= 5;
		angles[12] -= 6;//���j�L�W��
		angles[13] += 8;//���p�L�W��
		position += 0.1;//���֥H�W�V��y�b����
		break;
	case 4:
	case 5:
	case 6:
		zangles[1] += 5;
		zangles[6] += 5;
		angles[1] += 5;
		angles[6] += 5;
		angles[12] += 6;
		angles[13] -= 8;
		position -= 0.1;
		break;
	case 7:
	case 8:
	case 9:
		zangles[1] += 5;
		zangles[6] += 5;
		angles[1] -= 5;
		angles[6] -= 5;
		angles[15] -= 6;
		angles[16] += 8;
		position += 0.1;
		break;
	case 10:
	case 11:
	case 12:
		zangles[1] -= 5;
		zangles[6] -= 5;
		angles[1] += 5;
		angles[6] += 5;
		angles[15] += 6;
		angles[16] -= 8;
		position -= 0.1;
		break;
	}
}

void updateObj4(int frame) {
	switch (frame) {
	case 0:
		//angles�t�dx�b(�e-��+)����
		//yangles�t�dy�b(������-�f����+(�H�����H������))����
		//zangles�t�dz�b(������+�f����-(�H�����H������))����
		//1,6 : �W���u
		//2,7 : �U���u
		//3,8 : ��x
		//12,15 : �j�L
		//13,16 : �p�L
		//14,17 : �}�x
		//5 : �Y�` //0 : ����֤�(�ϥ�angle�ܼ�) //11 : �y��


		//�Y
		zangles[5] = -60;
		yangles[5] = -90;


		//��
		yangles[1] = -90;
		yangles[6] = 90;
		zangles[1] = 50;
		zangles[6] = -50;

		angles[2] = -70;
		angles[7] = -70;

		zangles[3] = 30;
		zangles[8] = -30;

		//�}
		angles[14] = 60;
		angles[17] = 60;
		break;
	case 1:
	case 2:
	case 3:

		zangles[12] += 15; //���j�L
		zangles[13] += 10; //���p�L

		zangles[0] += 3; //�k��
		position += 0.1;//���֥H�W�V��y�b����
		break;
	case 4:
	case 5:
	case 6:


		zangles[12] -= 15;
		zangles[13] -= 10;




		zangles[0] -= 3;
		position -= 0.1;
		break;
	case 7:
	case 8:
	case 9:

		zangles[15] -= 15;
		zangles[16] -= 10;


		zangles[0] -= 3;
		position += 0.1;
		break;
	case 10:
	case 11:
	case 12:


		zangles[15] += 15;
		zangles[16] += 10;


		zangles[0] += 3;
		position -= 0.1;
		break;
	}
}

void updateObj5(int frame) {
	switch (frame) {
	case 0:
		//angles�t�dx�b(�e-��+)����
		//yangles�t�dy�b(������-�f����+(�H�����H������))����
		//zangles�t�dz�b(������+�f����-(�H�����H������))����
		//1,6 : �W���u
		//2,7 : �U���u
		//3,8 : ��x
		//12,15 : �j�L
		//13,16 : �p�L
		//14,17 : �}�x
		//5 : �Y�` //0 : ����֤�(�ϥ�angle�ܼ�) //11 : �y��

		angles[2] = 30;
		angles[7] = 30;


		break;
	case 1:
	case 2:
	case 3:

		angles[12] -= 30;
		angles[13] += 30;
		angles[16] += 30;

		angles[6] += 10;
		angles[7] -= 25;

		yangles[0] += 15;
		position -= 0.1;//���֥H�W�V��y�b����
		break;
	case 4:
	case 5:
	case 6:

		angles[12] += 30;
		angles[13] -= 30;
		angles[16] -= 30;

		angles[6] -= 10;
		angles[7] += 25;

		angles[1] -= 60;

		yangles[0] -= 15;
		position += 2.0;
		break;
	case 7:
	case 8:
	case 9:



		position += 2.0;
		break;
	case 10:
	case 11:
	case 12:

		angles[1] += 60;

		position -= 3.9;
		break;
	}
}
//�x�smaterial ���l�ܼƪ�ID
 GLuint M_KaID;
 GLuint M_KdID;
 GLuint M_KsID;

void init(){
	isFrame = false;
	pNo = 0;
	mode = 0;
	backgroundMode = 0;
	isMotionSwitch = 0;
	for (int i = 0; i < PARTSNUM; i++) {
		//��l�ƨ��װ}�C
		angles[i] = 0.0;
		zangles[i] = 0.0;
		yangles[i] = 0.0;
	}
	//VAO
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "DSPhong_Material.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER, "DSPhong_Material.fp" },//fragment shader
		{ GL_NONE, NULL }};
	program = LoadShaders(shaders);//Ū��shader

	glUseProgram(program);//uniform�ѼƼƭȫe������use shader
	
	MatricesIdx = glGetUniformBlockIndex(program,"MatVP");//matvp �� view �M projection �x�}
	ModelID =  glGetUniformLocation(program,"Model");//�x�}
    M_KaID = glGetUniformLocation(program,"Material.Ka");
	M_KdID = glGetUniformLocation(program,"Material.Kd");
	M_KsID = glGetUniformLocation(program,"Material.Ks");
	//or
	M_KdID = M_KaID+1;
	M_KsID = M_KaID+2;
	ViewPosID = glGetUniformLocation(program, "ViewPos");//�N�۾���m�ǤJ

	Projection = glm::perspective(80.0f,4.0f/3.0f,0.1f,10000.0f);
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
	
	// Camera matrix
	View       = glm::lookAt(
		glm::vec3(0,10,25) , // Camera is at (0,10,25), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,1,0 to look upside-down)
		);

	Obj2Buffer();// load�iobj�A�ñN�䳻�I, ����, �k�V�q�˦b�T��VBO��

	//UBO
	glGenBuffers(1,&UBO);
	glBindBuffer(GL_UNIFORM_BUFFER,UBO);
	glBufferData(GL_UNIFORM_BUFFER,sizeof(mat4)*2,NULL,GL_DYNAMIC_DRAW);
	//get uniform struct size
	int UBOsize = 0;
	glGetActiveUniformBlockiv(program, MatricesIdx, GL_UNIFORM_BLOCK_DATA_SIZE, &UBOsize);//���o�bshader�ŧi��uniform "MatVP" ���j�p�A�s�JUBOsize
	//bind UBO to its idx
	glBindBufferRange(GL_UNIFORM_BUFFER,0,UBO,0,UBOsize);//�M�O����n�DUBOsize���Ŷ��A�NUBO�j�w��GL_UNIFORM_BUFFER�ݩ�
	glUniformBlockBinding(program, MatricesIdx,0);

	modeID = glGetUniformLocation(program, "mode"); // ��robot�������S�Ī��A���n�칺�@�Ѽƪ���m�A�H�Q����ǻ��ƭ�
	timeID = glGetUniformLocation(program, "time");

	CubemapID = glGetUniformLocation(program, "skybox");//�������H�[�蹳�S��
	glUniform1i(glGetUniformLocation(program, "skybox"), 0);


	//skybox zone
	ShaderInfo shadersky[] = {
		{ GL_VERTEX_SHADER, "skybox.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "skybox.fp" },//fragment shader
	{ GL_NONE, NULL } };
	programs = LoadShaders(shadersky);//Ū��shader

	glUseProgram(programs);//uniform�ѼƼƭȫe������use shader

						   // skybox VAO

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);

	vector<std::string> faces
	{
		"skybox/right.jpg",
		"skybox/left.jpg",
		"skybox/top.jpg",
		"skybox/bottom.jpg",
		"skybox/front.jpg",
		"skybox/back.jpg"
	};
	cubemapTexture = loadCubemap(faces);

	// shader configuration
	// --------------------
	glUseProgram(programs);

	ViewskyID = glGetUniformLocation(programs, "view");
	ProjectionskyID = glGetUniformLocation(programs, "projection");
	backgroundModeID = glGetUniformLocation(programs, "mode");
	backgroundTimeID = glGetUniformLocation(programs, "time");
	//skyboxShader.setInt("skybox", 0);
	glUniform1i(glGetUniformLocation(programs, "skybox"), 0);


	//framebuffer zone : �N�ù��e�����U�A�g�L��s�S�ġA�A�Ǧ^���覡
	ShaderInfo shaderframe[] = {
		{ GL_VERTEX_SHADER, "framebuffer.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "framebuffer.fp" },//fragment shader
	{ GL_NONE, NULL } };
	programf = LoadShaders(shaderframe);//Ū��shader

	glUseProgram(programf);//uniform�ѼƼƭȫe������use shader

	// screen quad VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	

	glUseProgram(programf);
	glUniform1i(glGetUniformLocation(programf, "screenTexture"), 0);

	// framebuffer configuration
	// -------------------------
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// create a color attachment texture
	
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
																								  // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	pNoID = glGetUniformLocation(programf, "mode");
	lenID = glGetUniformLocation(programf, "len");
	frametimeID = glGetUniformLocation(programf, "time");
	

	glClearColor(0.0,0.0,0.0,1);//black screen
}

#define DOR(angle) (angle*3.1415/180);
void display(){
	
	
	glUniform1i(pNoID, pNo);
	glUniform1f(lenID, sin(rotateAngle*1.0f)/100.0f);
	glUniform1f(frametimeID, rotateAngle);

	

	// bind to framebuffer and draw scene as we normally would to color texture 
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Debug 

	glBindVertexArray(VAO);//VAO�w�g�binit()���ŧi
	glUseProgram(program);//uniform�ѼƼƭȫe������use shader
	float eyey = DOR(eyeAngley);//eyeAngley�����u����ߡA�����V���k��¶�����׭�
	camera = vec3(eyedistance*sin(eyey), 5, eyedistance*cos(eyey));//�b�o�̪�����s�۾�����m
	View       = lookAt(
		               vec3(eyedistance*sin(eyey),5,eyedistance*cos(eyey)) , // Camera is at (0,0,20), in World Space
		               vec3(0,5,0), // and looks at the origin
		               vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
		                );
	updateModels();//�N���骺�C�@�ӳ���̾�angles[]�Mangle(�x�ި��饪�k����)�����װ�����[�W�첾��A���m�A�N�x�}���n�x�s�b�U��Models[]�̭�
	//updated data to UBO for MVP
	glBindBuffer(GL_UNIFORM_BUFFER,UBO);
	glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(mat4),&View);//�N�۾����[�ݩM�`��(�V���V�p)�x�}�ǤJ
	glBufferSubData(GL_UNIFORM_BUFFER,sizeof(mat4),sizeof(mat4),&Projection);
	glBindBuffer(GL_UNIFORM_BUFFER,0);

	glUniform3fv(ViewPosID, 1, &camera[0]); // �o�̱N�۾���m�ǤJ
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

	GLuint offset[3] = {0,0,0};//offset for vertices , uvs , normals
	for(int i = 0;i < PARTSNUM ;i++){//�ǤJ�x�}�A�ǤJVBO��ơA�ǤJ�C��(ambient, diffuse, specular)�A�e�X���u
		glUniformMatrix4fv(ModelID,1,GL_FALSE,&Models[i][0][0]);//�N�bupdateModels();�s�@�n��Models[]�x�}�̧ǶǤJ

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,				//location
							  3,				//vec3
							  GL_FLOAT,			//type
							  GL_FALSE,			//not normalized
							  0,				//strip
							  (void*)offset[0]);//buffer offset
		//(location,vec3,type,�T�w�I,�s���I�������q,buffer point)�е{���p��ϥ�VBO�̪���ơA�p����q�A�ñN�L�Ƕishader�̭����B��(�Hlayout(location = 0)�A�Ǹ�0�����ѭn�ǤJ���@���ܼ�)
		offset[0] +=  vertices_size[i]*sizeof(vec3);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);//location 1 :vec2 UV
		glBindBuffer(GL_ARRAY_BUFFER, uVBO);
		glVertexAttribPointer(1, 
							  2, 
							  GL_FLOAT, 
							  GL_FALSE, 
							  0,
							  (void*)offset[1]);
		//(location,vec2,type,�T�w�I,�s���I�������q,point)
		offset[1] +=  uvs_size[i]*sizeof(vec2);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);//location 2 :vec3 Normal
		glBindBuffer(GL_ARRAY_BUFFER, nVBO);
		glVertexAttribPointer(2,
							  3, 
							  GL_FLOAT, 
							  GL_FALSE, 
							  0,
							  (void*)offset[2]);
		//(location,vec3,type,�T�w�I,�s���I�������q,point)
		offset[2] +=  normals_size[i]*sizeof(vec3);

		int vertexIDoffset = 0;//glVertexID's offset 
		string mtlname;//material name
		vec3 Ks = vec3(1,1,1);//because .mtl excluding specular , so give it here.
		for(int j = 0;j <mtls[i].size() ;j++){//
			mtlname = mtls[i][j];	
			//find the material diffuse color in map:KDs by material name.
			glUniform3fv(M_KdID,1,&KDs[mtlname][0]);
			glUniform3fv(M_KsID,1,&Ks[0]);
			//          (primitive   , glVertexID base , vertex count    )
			glDrawArrays(GL_TRIANGLES, vertexIDoffset  , faces[i][j+1]*3);
			//we draw triangles by giving the glVertexID base and vertex count is face count*3
			vertexIDoffset += faces[i][j+1]*3;//glVertexID's base offset is face count*3
		}//end for loop for draw one part of the robot	
		
	}//end for loop for updating and drawing model

	
	 //try placing in the front
	glUniform1i(modeID, mode); // �N�ƭȶǻ��i�Jshader
	glUniform1f(timeID, rotateAngle); // �N�ɶ��ǤJ

	eyey = DOR(eyeAngley);
	View = lookAt(
		vec3(eyedistance*sin(eyey), 2, eyedistance*cos(eyey)), // Camera is at (0,0,20), in World Space
		vec3(0, 0, 0), // and looks at the origin
		vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// draw skybox as last
	
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	glUseProgram(programs);
	View = glm::mat4(glm::mat3(View)); // remove translation from the view matrix
	glUniformMatrix4fv(ViewskyID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(ProjectionskyID, 1, GL_FALSE, &Projection[0][0]);
	//skyboxShader.setMat4("view", ViewskyID);
	//skyboxShader.setMat4("projection", ProjectionskyID);
	// skybox cube

	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default

	//�ǤJskybox��uniform���������DrawArrays(skybox)�᭱
	glUniform1i(backgroundModeID, backgroundMode);
	glUniform1f(backgroundTimeID, rotateAngle);

	
	// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
							  // clear all relevant buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	//screenShader.use();
	glUseProgram(programf);
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);



	glFlush();//�j�����W����OpenGL commands
	glutSwapBuffers();//�մ��e�x�M��xbuffer ,���Obuffer�e���M�e�xbuffer�洫�ϧڭ̬ݨ���
}

void Obj2Buffer(){


	//vector���}�C���@�ءA�i�H�s��U�خ榡(vec3, string)�����
	std::vector<vec3> Kds;
	std::vector<vec3> Kas;
	std::vector<vec3> Kss;
	std::vector<std::string> Materials;//mtl-name
	std::string texture; // �Τ���
	loadMTL("Obj/ironmantest.mtl",Kds,Kas,Kss,Materials,texture);//�Ѷ}mtl�A�N���誺�ݩ�(�C��P���������)Ū�J(ambient, diffuse, specular���s��V�q���}�C)
	//printf("%d\n",texture);
	for(int i = 0;i<Materials.size();i++){
		string mtlname = Materials[i];
		//  name            vec3
		KDs[mtlname] = Kds[i];
	}

	//load2Buffer("Obj/ironmantest.obj", 0);

	load2Buffer("Obj/ibody.obj", 0);

	load2Buffer("Obj/ileftu8.obj", 1);//�W���u
	load2Buffer("Obj/ileftd2.obj",2);//�U���u
	load2Buffer("Obj/ilefthand2.obj",3);//��
	load2Buffer("Obj/ileftshoulder2.obj",4);//�ӻH
	
	load2Buffer("Obj/ihead4.obj",5);

	load2Buffer("Obj/irightu3.obj",6);
	load2Buffer("Obj/irightd2.obj",7);
	load2Buffer("Obj/irighthand2.obj",8);
	load2Buffer("Obj/irightshoulder2.obj",9);


	load2Buffer("Obj/ibodyd.obj",11);
	//load2Buffer("Obj/back2.obj",10);

	load2Buffer("Obj/ileftlegu2.obj",12);
	load2Buffer("Obj/ileftlegd2.obj",13);
	load2Buffer("Obj/ileftfoot2.obj",14);

	load2Buffer("Obj/irightlegu2.obj",15);	
	load2Buffer("Obj/irightlegd2.obj",16);	
	load2Buffer("Obj/irightfoot2.obj",17);
	//�W��load2Buffer��, �}�CVBOs, uVBOs, nVBOs �Q��J���, �q0-17

	GLuint totalSize[3] = {0,0,0};
	GLuint offset[3] = {0,0,0};
	for(int i = 0;i < PARTSNUM ;i++){//���F�ХߤT��VBO�ӭp�⪺�j�p(glBufferData�n�j�p��)
		totalSize[0] += vertices_size[i]*sizeof(vec3);
		totalSize[1] += uvs_size[i] * sizeof(vec2);
		totalSize[2] += normals_size[i] * sizeof(vec3);
	}
	//generate vbo
	glGenBuffers(1,&VBO);//���I�y�нw�ľ�
	glGenBuffers(1,&uVBO);//����y�Ъ����I�w�ľ�
	glGenBuffers(1,&nVBO);//�k�V�q�����I�w�ľ�
	//bind vbo ,�Ĥ@��bind�]�P���� create vbo 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//VBO��target�OGL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER,totalSize[0],NULL,GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uVBO);//VBO��target�OGL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER,totalSize[1],NULL,GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, nVBO);//VBO��target�OGL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER,totalSize[2],NULL,GL_STATIC_DRAW);
	
	
	for(int i = 0;i < PARTSNUM ;i++){//�o��for�j��D�n�b������bVBOs, uVBOs, nVBOs����ƦX�@
		glBindBuffer(GL_COPY_WRITE_BUFFER,VBO);
		glBindBuffer(GL_COPY_READ_BUFFER, VBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,offset[0],vertices_size[i]*sizeof(vec3));//�ⷽ�Y��ƽƻs����w�a(���Y, ���w�a, ���Y������, ���w�a������)
		offset[0] += vertices_size[i]*sizeof(vec3);//�����ȧ�s
		glInvalidateBufferData(VBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER,0);

		glBindBuffer(GL_COPY_WRITE_BUFFER,uVBO);
		glBindBuffer(GL_COPY_READ_BUFFER, uVBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,offset[1],uvs_size[i]*sizeof(vec2));
		offset[1] += uvs_size[i]*sizeof(vec2);//offset(������)�O�j�p�T���}�C(���I, ����, �k�V�q)
		glInvalidateBufferData(uVBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER,0);

		glBindBuffer(GL_COPY_WRITE_BUFFER,nVBO);
		glBindBuffer(GL_COPY_READ_BUFFER, nVBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,offset[2],normals_size[i]*sizeof(vec3));
		offset[2] += normals_size[i] * sizeof(vec3);
		glInvalidateBufferData(uVBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER,0);
	}
	glBindBuffer(GL_COPY_WRITE_BUFFER,0);


}

void updateModels() {
	mat4 Rotatation[PARTSNUM];
	mat4 Translation[PARTSNUM];
	for (int i = 0; i < PARTSNUM; i++) {
		Models[i] = mat4(1.0f);
		Rotatation[i] = mat4(1.0f);
		Translation[i] = mat4(1.0f);
	}
	float r, pitch, yaw, roll;
	float alpha, beta, gamma; // alpha���e���\��(x�b),beta���Hy�b����,gamma�������\��(z�b)

							  //Body
	alpha = angles[0];
	//beta = angle; // angle ���ܪ��O���}�H�W���骺����(���k���)
	beta = yangles[0];
	gamma = zangles[0];
	Rotatation[0] = rotate(beta, 0, 1, 0)*rotate(alpha, 1, 0, 0)*rotate(gamma, 0, 0, 1);// �Hy�b������b���beta��(beta�brotate�禡���|�ର����)
	Translation[0] = translate(0, 2.9 + position, 0);
	Models[0] = Translation[0] * Rotatation[0];//Models[0]�x�s�ۨ��骺�@�ɮy�Яx�}���n

											   //����=======================================================
											   //���W���u
	yaw = DOR(beta); r = 3.7;
	alpha = angles[1]; // angles[1]���ӻH�P���鶡�����`
	gamma = zangles[1]; // �i�H����gamma�������u�����u�V���k���|�A gamma = 50 , ���u�ܦ�T�r��
	beta = yangles[1]; //��ʤ��u
	Rotatation[1] = rotate(alpha, 1, 0, 0)*rotate(gamma, 0, 0, 1)*rotate(beta, 0, 1, 0);//�V�e����*�V�k����
																						//Translation[1] = translate(2.7, 5.8, -0.25);//ileftu7
	Translation[1] = translate(2.7, 6.4, -0.25);
	//Translation[1] = translate(0, 0, 0);

	Models[1] = Models[0] * Translation[1] * Rotatation[1];//���F���u�������@�ɮy�Яx�}�A�ѩ�����ʤ��u�|��ۡA�ҥH����x�}�]�n���i�h

														   //���ӻH
	Rotatation[4] = rotate(alpha, 1, 0, 0)*rotate(gamma, 0, 0, 1);//�V�e����*�V�k����
																  //Translation[4] =translate(3.7,1,-0.5);//�첾�쥪�W���u�B
	Translation[4] = translate(2.7, 6.3, 0);
	//Models[4] =Models[0]*Translation[1]*Rotatation[1];//���ӻH�x�}�M���W���u�x�}�ۦP
	Models[4] = Models[0] * Translation[4] * Rotatation[4];




	//���U���u
	pitch = DOR(alpha); r = 3;//pitch�x�ޤ��u���e���\�ʨ���
	roll = DOR(gamma);//roll�x�ޤ��u�����|����
	static int i = 0;
	i += 5;
	//alpha = angles[2]-20;//���U���u�V�e�s20��
	alpha = angles[2];
	beta = yangles[2];
	gamma = zangles[2];
	//�W���u+�U���u�V�e����*�V�k����
	Rotatation[2] = rotate(alpha, 1, 0, 0)*rotate(gamma, 0, 0, 1)*rotate(beta, 0, 1, 0);
	//��x�b�첾�H�W���u���b�|����P��:translate(0,r*cos,r*sin)
	//��z�b�첾�H�W���u���b�|����:translate(r*sin,-rcos,0)
	//Translation[2] = translate(0,-3,0);
	Translation[2] = translate(0.3, -2.4, 0);

	Models[2] = Models[1] * Translation[2] * Rotatation[2];//�����ۤv���������ʡA�]�n�[���W���u(�W�e�|�ɡA�U�]��ۦV�e)

														   //�����x
	pitch = DOR(alpha);
	//b = DOR(angles[2]);
	roll = DOR(gamma);
	//��x���׻P�U���u�ۦP
	//Rotatation[3] = Rotatation[2];
	//��x�b�첾�H�W���u���b�|����P��:translate(0,r*cos,r*sin) ,���׬��W���u+�U���u
	alpha = angles[3];
	gamma = zangles[3];
	beta = yangles[3];
	Translation[3] = translate(0.75, -2, 0.5);
	Rotatation[3] = rotate(alpha, 1, 0, 0)*rotate(gamma, 0, 0, 1)*rotate(beta, 0, 1, 0);
	//Translation[3] = translate(0, 0, 0);
	Models[3] = Models[2] * Translation[3] * Rotatation[3];//�]���w�����W(�~��)�U���u���x�}(�֦��U���u������)�A�ҥH���Χ��rotation�x�}�A�Y�Q�������A�i�H���Rotation[3]
														   //============================================================
														   //�Y==========================================================
	alpha = angles[5] + 0;
	beta = yangles[5];
	Translation[5] = translate(-0.1, 8.2, 0.3);// ���� : initial z = -0.5
	Rotatation[5] = rotate(alpha, 1, 0, 0)*rotate(beta, 0, 1, 0);
	Models[5] = Models[0] * Translation[5] * Rotatation[5];
	//============================================================
	//�k��=========================================================
	//�k�W���u
	//gamma = zangles[6] = -zangles[1]; 
	//alpha = angles[6] = -angles[1]; 
	//beta = yangles[6] = -yangles[1];
	gamma = zangles[6];
	alpha = angles[6];
	beta = yangles[6];
	Rotatation[6] = rotate(alpha, 1, 0, 0)*rotate(gamma, 0, 0, 1)*rotate(beta, 0, 1, 0);
	//Translation[6] = translate(-2.9, 5.8, -0.25); // irightu2
	Translation[6] = translate(-2.9, 6.4, -0.25);
	Models[6] = Models[0] * Translation[6] * Rotatation[6];

	//�k�ӻH
	Rotatation[9] = rotate(alpha, 1, 0, 0)*rotate(gamma, 0, 0, 1);
	Translation[9] = translate(-2.9, 6.3, 0);
	Models[9] = Models[0] * Translation[9] * Rotatation[9];

	//�k�U���u
	//angles[7] = angles[2];
	pitch = DOR(alpha); r = -3;
	roll = DOR(gamma);
	//alpha = angles[7]-20;
	alpha = angles[7];
	beta = yangles[7];
	gamma = zangles[7];
	Rotatation[7] = rotate(alpha, 1, 0, 0)*rotate(gamma, 0, 0, 1)*rotate(beta, 0, 1, 0);
	//Translation[7] = translate(0,-3,0);
	Translation[7] = translate(-0.3, -2.4, 0);
	Models[7] = Models[6] * Translation[7] * Rotatation[7];

	//�k��x
	pitch = DOR(alpha);
	//b = DOR(angles[7]);
	roll = DOR(gamma);
	//Translation[8] =translate(0,-6,0);
	alpha = angles[8];
	gamma = zangles[8];
	beta = yangles[8];
	Rotatation[8] = rotate(alpha, 1, 0, 0)*rotate(gamma, 0, 0, 1)*rotate(beta, 0, 1, 0);
	Translation[8] = translate(-0.75, -2.5, 1.5);
	Models[8] = Models[7] * Translation[8] * Rotatation[8];
	//=============================================================
	//back&DBody===================================================
	//�I�]
	Translation[10] = translate(0, 2, -4.5);
	Models[10] = Models[0] * Translation[10] * Rotatation[10];

	//����
	Translation[11] = translate(0, 0, 0); // -5.3
	Models[11] = Models[0] * Translation[11] * Rotatation[11];
	//=============================================================
	//���}
	//���j�L
	alpha = angles[12]; gamma = zangles[12]; beta = yangles[12];
	Rotatation[12] = rotate(alpha, 1, 0, 0)*rotate(gamma, 0, 0, 1)*rotate(beta, 0, 1, 0);
	//Translation[12] =translate(1.8,-4.5,0);
	//Translation[12] = translate(1.25, 3.7, 0.4); // ���ʧ@�|�M���ֲ�`
	Translation[12] = translate(1.25, 0.8, 0.4);
	Models[12] = Models[11] * Translation[12] * Rotatation[12];

	//���p�L
	pitch = DOR(alpha); r = -7;
	roll = DOR(gamma);
	alpha = angles[13] + 0; gamma = zangles[13]; beta = yangles[13];
	//alpha = angles[13]+angles[12];//�N�~�Ӫ��F��Ķitranslate and rotate����
	//Translation[13] = translate(-r*sin(roll),r*cos(pitch),r*sin(pitch))*Translation[12];
	//Rotatation[13] = rotate(alpha,1,0,0) * rotate(gamma,0,0,1);
	//Models[13] = Translation[13]*Rotatation[13]*Models[13];
	Translation[13] = translate(1, -5, -0.36);//self
											  //Translation[13] = translate(0, 0, 0);
	Rotatation[13] = rotate(alpha, 1, 0, 0)*rotate(gamma, 0, 0, 1)*rotate(beta, 0, 1, 0);
	Models[13] = Models[12] * Translation[13] * Rotatation[13];



	//���}�x
	pitch = DOR(alpha); r = -5;
	//b = DOR(angles[13]);
	roll = DOR(gamma);
	alpha = angles[14] + 0;
	//Translation[14] = translate(-(r+2)*sin(roll),r*cos(pitch),r*sin(pitch)-1)*Translation[13];
	//Rotatation[14] = rotate(alpha,1,0,0) * rotate(gamma, 0, 0, 1);
	//Models[14] = Translation[14]*Rotatation[14]*Models[14];
	//Translation[14] = translate(0, -4.5, 0);//self
	Translation[14] = translate(1, -5.25, 0);
	Rotatation[14] = rotate(alpha, 1, 0, 0);
	Models[14] = Models[13] * Translation[14] * Rotatation[14];

	//=============================================================
	//�k�}
	//alpha = angles[15] = -angles[12];
	//gamma = zangles[15] = -zangles[12];
	alpha = angles[15];
	gamma = zangles[15];
	beta = yangles[15];
	Rotatation[15] = rotate(alpha, 1, 0, 0)*rotate(gamma, 0, 0, 1)*rotate(beta, 0, 1, 0);
	//Translation[15] =translate(-1.8,-4.5,0);
	//Translation[15] = translate(-1.5, 3.7, 0.4);
	Translation[15] = translate(-1.5, 0.8, 0.4);
	Models[15] = Models[11] * Translation[15] * Rotatation[15];

	//angles[16] = angles[13];
	pitch = DOR(alpha); r = -7;
	roll = DOR(gamma);


	alpha = angles[16] + 0;
	gamma = zangles[16];
	beta = yangles[16];
	//alpha = angles[16]+angles[15];
	//Rotatation[16] = rotate(alpha,1,0,0) * rotate(gamma, 0, 0, 1);
	//Translation[16] = translate(-r*sin(roll),r*cos(pitch),r*sin(pitch))*Translation[15];
	//Models[16] = Translation[16]*Rotatation[16]*Models[16];
	//Translation[16] = translate(0, -7.5, 0);
	Translation[16] = translate(-1, -5, -0.36);
	Rotatation[16] = rotate(alpha, 1, 0, 0)*rotate(gamma, 0, 0, 1)*rotate(beta, 0, 1, 0);
	Models[16] = Models[15] * Translation[16] * Rotatation[16];



	pitch = DOR(alpha); r = -5;
	//b = DOR(angles[16]);
	roll = DOR(gamma);
	alpha = angles[17] + 0;
	//alpha = angles[15]+angles[16];
	//Translation[17] = translate(-(r+2)*sin(roll),r*cos(pitch),r*sin(pitch)-0.5)*Translation[16];
	//Rotatation[17] = rotate(alpha,1,0,0) * rotate(gamma, 0, 0, 1);
	//Models[17] = Translation[17]*Rotatation[17]*Models[17];
	//Translation[17] = translate(0, -4.5, 0);//self
	Translation[17] = translate(-0.9, -5.25, 0);
	Rotatation[17] = rotate(alpha, 1, 0, 0);
	Models[17] = Models[16] * Translation[17] * Rotatation[17];
	//=============================================================
}


void load2Buffer(char* obj,int i){
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals; // Won't be used at the moment.
	std::vector<unsigned int> materialIndices;

	bool res = loadOBJ(obj, vertices, uvs, normals,faces[i],mtls[i]);//mtls(materials)�@17�Ӧ�l�A�C�Ӧ�l�sŪ�Jobj���U�ا���W��
	if(!res) printf("load failed\n");

	//glUseProgram(program);

	glGenBuffers(1,&VBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
	glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(vec3),&vertices[0],GL_STATIC_DRAW);
	vertices_size[i] = vertices.size();

	//(buffer type,data�_�l��m,data size,data first ptr)
	//vertices_size[i] = glm_model->numtriangles;
	
	//printf("vertices:%d\n",vertices_size[);

	glGenBuffers(1,&uVBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, uVBOs[i]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW);
	uvs_size[i] = uvs.size();

	glGenBuffers(1,&nVBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, nVBOs[i]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);
	normals_size[i] = normals.size();
}
mat4 translate(float x,float y,float z){
	vec4 t = vec4(x,y,z,1);//w = 1 ,�hx,y,z=0�ɤ]��translate
	vec4 c1 = vec4(1,0,0,0);
	vec4 c2 = vec4(0,1,0,0);
	vec4 c3 = vec4(0,0,1,0);
	mat4 M = mat4(c1,c2,c3,t);
	return M;
} 
mat4 scale(float x,float y,float z){
	vec4 c1 = vec4(x,0,0,0);
	vec4 c2 = vec4(0,y,0,0);
	vec4 c3 = vec4(0,0,z,0);
	vec4 c4 = vec4(0,0,0,1);
	mat4 M = mat4(c1,c2,c3,c4);
	return M;
}

mat4 rotate(float angle,float x,float y,float z){
	float r = DOR(angle);
	mat4 M = mat4(1);

	vec4 c1 = vec4(cos(r)+(1-cos(r))*x*x,(1-cos(r))*y*x+sin(r)*z,(1-cos(r))*z*x-sin(r)*y,0);
	vec4 c2 = vec4((1-cos(r))*y*x-sin(r)*z,cos(r)+(1-cos(r))*y*y,(1-cos(r))*z*y+sin(r)*x,0);
	vec4 c3 = vec4((1-cos(r))*z*x+sin(r)*y,(1-cos(r))*z*y-sin(r)*x,cos(r)+(1-cos(r))*z*z,0);
	vec4 c4 = vec4(0,0,0,1);
	M = mat4(c1,c2,c3,c4);
	return M;
}
void Keyboard(unsigned char key, int x, int y){
	switch(key){
	case '1':
		angle += 5;//angle����ֻP���}�������`(���k���)
		if(angle>=360) angle = 0;
		printf("beta:%f\n",angle);
		break;
	case '2':
		angle -= 5;
		if(angle<=0) angle = 360;
		printf("beta:%f\n",angle);
		break;
	case 'w':
		eyedistance -= 0.2;//eyedistance�O�۾��M���u���Z��
		break;
	case 's':
		eyedistance += 0.2;
		break;
	case 'a':
		eyeAngley -=10;// eyeAngley �O �H���u����ߡA�۾����k���ʮɪ�����
		break;
	case 'd':
		eyeAngley +=10;
		break;
	case 'r':
		angles[1] -= 5; // angles[1]����ӻH�P���餧�������`����(�e���\��)
		angles[6] -= 5; // �k��
		if(angles[1] == -360) angles[1] = 0;
		movey = 0;
		movex = 0;
		break;
	case 't':
		angles[2] -= 5;// angles[2]�����y�����`(�e���\��)
		if(angles[2] == -360) angles[2] = 0;
		movey = 0;
		movex = 0;
		break;
	case 'q':
		break;
	case 'e':
		break;
	}
	glutPostRedisplay();
}
void menuEvents(int option){}
void ActionMenuEvents(int option) {

	if (isMotionSwitch != option) {
		isMotionSwitch = -1;
	}

	switch (option) {

	case 0:
		action = IDLE;
		break;
	case 1:
		action = WALK;
		break;
	case 2:
		action = DANCE;
		break;
	case 3:
		action = GANGNAM;
		break;
	case 4:
		action = GANGNAM2;
		break;
	case 5:
		action = JUMP;
		break;
	case 6:
		action = WALK;
		break;
	case 7:
		action = DANCE;
		break;
	case 8:
		action = GANGNAM;
		break;
	case 9:
		action = GANGNAM2;
		break;
	case 10:
		action = IDLE;
		break;
	case 11:
		action = WALK;
		break;
	case 12:
		action = DANCE;
		break;
	case 13:
		action = GANGNAM;
		break;
	case 14:
		action = GANGNAM2;
		break;
	case 15:
		action = GANGNAM2;
		break;
	}
}
void ModeMenuEvents(int option){
	mode = option;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	
}
void ShaderMenuEvents(int option){
	//pNo = option;
	//new added
	pNo = option;
}
void BackgroundMenuEvents(int option) {
	//pNo = option;
	//new added
	backgroundMode = option;
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}