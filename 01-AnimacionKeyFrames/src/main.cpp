#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"

#include "Headers/ThirdPersonCamera.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

#include "Headers/AnimationUtils.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

GLFWwindow *window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;

std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());



Sphere skyboxSphere(20, 20);
Box boxCesped;
Box boxWalls;
Box boxHighway;
Box boxLandingPad;   //para area de aterrizaje
Sphere esfera1(20,20);
// Models complex instances
Model modelRock;
Model modelAircraft;
Model modelEclipseChasis;
Model modelEclipseRearWheels;
Model modelEclipseFrontalWheels;
Model modelHeliChasis;
Model modelHeliHeli;
Model modelHeliRear;

Model modelLambo;
Model modelLamboLeftDor;
Model modelLamboRightDor;
Model modelLamboFrontLeftWheel;
Model modelLamboFrontRightWheel;
Model modelLamboRearLeftWheel;
Model modelLamboRearRightWheel;
// Dart lego
Model modelDartLegoBody;
Model modelDartLegoHead;
Model modelDartLegoMask;
Model modelDartLegoLeftArm;
Model modelDartLegoRightArm;
Model modelDartLegoLeftHand;
Model modelDartLegoRightHand;
Model modelDartLegoLeftLeg;
Model modelDartLegoRightLeg;

//Buzz
Model modelBuzzHead;
Model modelBuzzTorso;
Model modelBuzzHip;

Model modelBuzzLeftArm;
Model modelBuzzLeftForeArm;
Model modelBuzzLeftHand;
Model modelBuzzLeftThigh;
Model modelBuzzLeftCalf;
Model modelBuzzLeftFoot;

Model modelBuzzRightArm;
Model modelBuzzRightForeArm;
Model modelBuzzRightHand;
Model modelBuzzRightThigh;
Model modelBuzzRightCalf;
Model modelBuzzRightFoot;

//
//Model MC1 
Model modelMC1;
//Model MC2
Model modelMC2;




GLuint textureCespedID, textureWallID, textureWindowID, textureHighwayID, textureLandingPadID;
GLuint skyboxTextureID;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/mp_bloodvalley/blood-valley_ft.tga",
		"../Textures/mp_bloodvalley/blood-valley_bk.tga",
		"../Textures/mp_bloodvalley/blood-valley_up.tga",
		"../Textures/mp_bloodvalley/blood-valley_dn.tga",
		"../Textures/mp_bloodvalley/blood-valley_rt.tga",
		"../Textures/mp_bloodvalley/blood-valley_lf.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

// Model matrix definitions
glm::mat4 modelMatrixEclipse = glm::mat4(1.0f);
glm::mat4 matrixModelRock = glm::mat4(1.0);
glm::mat4 modelMatrixHeli = glm::mat4(1.0f);
glm::mat4 modelMatrixLambo = glm::mat4(1.0);
glm::mat4 modelMatrixAircraft = glm::mat4(1.0);
glm::mat4 modelMatrixDart = glm::mat4(1.0f);
//
glm::mat4 modelMatrixMC1 = glm::mat4(1.0f);
glm::mat4 modelMatrixMC2 = glm::mat4(1.0f);

glm::mat4 modelMatrixBuzz = glm::mat4(1.0f); //matriz con diagonal principal, suyos elementos son 1.0f

int animationIndex = 1;
float 	rotDartHead = 0.0f, rotDartLeftArm = 0.0f, rotDartLeftHand = 0.0f, 
		rotDartRightArm = 0.0f, rotDartRightHand = 0.0f, rotDartLeftLeg = 0.0f, 
		rotDartRightLeg = 0.0f;

float 	rotBuzzHead = 0.0f, rotBuzzLeftArm = 0.0f, rotBuzzLeftHand = 0.0f, 
		rotBuzzRightArm = 0.0f, rotBuzzRightHand = 0.0f, rotBuzzLeftThigh = 0.0f, 
		rotBuzzLeftCalf = 0.0f, rotBuzzRightCalf = 0.0f, rotBuzzLeftFoot = 0.0f,
		rotBuzzRightFoot = 0.0f, rotBuzzRightThigh = 0.0;

int modelSelected = 0;
bool enableCountSelected = true;

// Variables to animations keyframes
bool saveFrame = false, availableSave = true;
std::ofstream myfile;
std::string fileName = "";
std::string fileName2 = "";
bool record = false;

// Joints interpolations Dart Lego
std::vector<std::vector<float>> keyFramesDartJoints;
std::vector<std::vector<glm::mat4>> keyFramesDart;
int indexFrameDartJoints = 0;
int indexFrameDartJointsNext = 1;
float interpolationDartJoints = 0.0;
int maxNumPasosDartJoints = 20;
int numPasosDartJoints = 0;
int indexFrameDart = 0;
int indexFrameDartNext = 1;
float interpolationDart = 0.0;
int maxNumPasosDart = 200;
int numPasosDart = 0;

//para el Buzz
std::vector<std::vector<float>> keyFramesBuzzJoints;
std::vector<std::vector<glm::mat4>> keyFramesBuzz;
int indexFrameBuzzJoints = 0;
int indexFrameBuzzJointsNext = 1;
float interpolationBuzzJoints = 0.0;
int maxNumPasosBuzzJoints = 20;
int numPasosBuzzJoints = 0;
int indexFrameBuzz = 0;
int indexFrameBuzzNext = 1;
float interpolationBuzz = 0.0;
int maxNumPasosBuzz = 200;
int numPasosBuzz = 0;

// Var animate helicopter
float rotHelHelY = 0.0;
float rotHelRearY = 0.0;

// Var animate lambo dor
int stateDoor = 3;
float dorRotCount = 0.0;
float dorRotCountRight = 0.0;

double deltaTime;
double currTime, lastTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
				glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
				nullptr);

	if (window == nullptr) {
		std::cerr
				<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
				<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// InicializaciÃ³n de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox.fs");
	//shaderMulLighting.initialize("../Shaders/iluminacion_texture_res.vs", "../Shaders/multipleLights.fs");
	//para poder renderizar los objetos
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation.vs", "../Shaders/multipleLights.fs");
	
	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

	esfera1.init();  //**clase01
	esfera1.setShader(&shaderMulLighting); //**clase01
	esfera1.setPosition(glm::vec3(0.0f,2.0f,-5.0f));

	boxCesped.init();
	boxCesped.setShader(&shaderMulLighting);

	boxWalls.init();
	boxWalls.setShader(&shaderMulLighting);

	boxHighway.init();
	boxHighway.setShader(&shaderMulLighting);

	boxLandingPad.init();
	boxLandingPad.setShader(&shaderMulLighting);

	modelRock.loadModel("../models/rock/rock.obj");
	modelRock.setShader(&shaderMulLighting);

	modelAircraft.loadModel("../models/Aircraft_obj/E 45 Aircraft_obj.obj");
	modelAircraft.setShader(&shaderMulLighting);

	// Eclipse
	modelEclipseChasis.loadModel("../models/Eclipse/2003eclipse_chasis.obj");
	modelEclipseChasis.setShader(&shaderMulLighting);
	modelEclipseFrontalWheels.loadModel("../models/Eclipse/2003eclipse_frontal_wheels.obj");
	modelEclipseFrontalWheels.setShader(&shaderMulLighting);
	modelEclipseRearWheels.loadModel("../models/Eclipse/2003eclipse_rear_wheels.obj");
	modelEclipseRearWheels.setShader(&shaderMulLighting);
	// Helicopter
	modelHeliChasis.loadModel("../models/Helicopter/Mi_24_chasis.obj");
	modelHeliChasis.setShader(&shaderMulLighting);
	modelHeliHeli.loadModel("../models/Helicopter/Mi_24_heli.obj");
	modelHeliHeli.setShader(&shaderMulLighting);
	modelHeliRear.loadModel("../models/Helicopter/Mi_24_heli_rear.obj");
	modelHeliRear.setShader(&shaderMulLighting);

	// Lamborginhi
	modelLambo.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_chasis.obj");
	modelLambo.setShader(&shaderMulLighting);
	modelLamboLeftDor.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_left_dor.obj");
	modelLamboLeftDor.setShader(&shaderMulLighting);
	modelLamboRightDor.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_right_dor.obj");
	modelLamboRightDor.setShader(&shaderMulLighting);
	modelLamboFrontLeftWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_front_left_wheel.obj");
	modelLamboFrontLeftWheel.setShader(&shaderMulLighting);
	modelLamboFrontRightWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_front_right_wheel.obj");
	modelLamboFrontRightWheel.setShader(&shaderMulLighting);
	modelLamboRearLeftWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_rear_left_wheel.obj");
	modelLamboRearLeftWheel.setShader(&shaderMulLighting);
	modelLamboRearRightWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_rear_right_wheel.obj");
	modelLamboRearRightWheel.setShader(&shaderMulLighting);

	// Dart Lego
	modelDartLegoBody.loadModel("../models/LegoDart/LeoDart_body.obj");
	modelDartLegoBody.setShader(&shaderMulLighting);
	modelDartLegoMask.loadModel("../models/LegoDart/LeoDart_mask.obj");
	modelDartLegoMask.setShader(&shaderMulLighting);
	modelDartLegoHead.loadModel("../models/LegoDart/LeoDart_head.obj");
	modelDartLegoHead.setShader(&shaderMulLighting);
	modelDartLegoLeftArm.loadModel("../models/LegoDart/LeoDart_left_arm.obj");
	modelDartLegoLeftArm.setShader(&shaderMulLighting);
	modelDartLegoRightArm.loadModel("../models/LegoDart/LeoDart_right_arm.obj");
	modelDartLegoRightArm.setShader(&shaderMulLighting);
	modelDartLegoLeftHand.loadModel("../models/LegoDart/LeoDart_left_hand.obj");
	modelDartLegoLeftHand.setShader(&shaderMulLighting);
	modelDartLegoRightHand.loadModel("../models/LegoDart/LeoDart_right_hand.obj");
	modelDartLegoRightHand.setShader(&shaderMulLighting);
	modelDartLegoLeftLeg.loadModel("../models/LegoDart/LeoDart_left_leg.obj");
	modelDartLegoLeftLeg.setShader(&shaderMulLighting);
	modelDartLegoRightLeg.loadModel("../models/LegoDart/LeoDart_right_leg.obj");
	modelDartLegoRightLeg.setShader(&shaderMulLighting);

	//Buzz
	modelBuzzHead.loadModel("../models/Buzz/buzzlightyHead.obj");
	modelBuzzHead.setShader(&shaderMulLighting);
	modelBuzzTorso.loadModel("../models/Buzz/buzzlightyTorso.obj");
	modelBuzzTorso.setShader(&shaderMulLighting);

	modelBuzzLeftArm.loadModel("../models/Buzz/buzzlightyLeftarm.obj");
	modelBuzzLeftArm.setShader(&shaderMulLighting);
	modelBuzzLeftForeArm.loadModel("../models/Buzz/buzzlightyLeftForearm.obj");
	modelBuzzLeftForeArm.setShader(&shaderMulLighting);
	modelBuzzLeftHand.loadModel("../models/Buzz/buzzlightyLeftHand.obj");
	modelBuzzLeftHand.setShader(&shaderMulLighting);
	modelBuzzLeftThigh.loadModel("../models/Buzz/buzzlightyLeftThigh.obj");
	modelBuzzLeftThigh.setShader(&shaderMulLighting);
	modelBuzzLeftCalf.loadModel("../models/Buzz/buzzlightyLeftCalf.obj");
	modelBuzzLeftCalf.setShader(&shaderMulLighting);
	modelBuzzLeftFoot.loadModel("../models/Buzz/buzzlightyLeftFoot.obj");
	modelBuzzLeftFoot.setShader(&shaderMulLighting);
	
	modelBuzzHip.loadModel("../models/Buzz/buzzlightyHip.obj");
	modelBuzzHip.setShader(&shaderMulLighting);

	modelBuzzRightArm.loadModel("../models/Buzz/buzzlightyRightarm.obj");
	modelBuzzRightArm.setShader(&shaderMulLighting);
	modelBuzzRightForeArm.loadModel("../models/Buzz/buzzlightyRightForearm.obj");
	modelBuzzRightForeArm.setShader(&shaderMulLighting);
	modelBuzzRightHand.loadModel("../models/Buzz/buzzlightyRightHand.obj");
	modelBuzzRightHand.setShader(&shaderMulLighting);
	modelBuzzRightThigh.loadModel("../models/Buzz/buzzlightyRightThigh.obj");
	modelBuzzRightThigh.setShader(&shaderMulLighting);
	modelBuzzRightCalf.loadModel("../models/Buzz/buzzlightyRightCalf.obj");
	modelBuzzRightCalf.setShader(&shaderMulLighting);
	modelBuzzRightFoot.loadModel("../models/Buzz/buzzlightyRightFoot.obj");
	modelBuzzRightFoot.setShader(&shaderMulLighting);

	//practica
	//char1
	// modelMC1.loadModel("../models/onichan_verde/GIRL_DANCER.fbx");DANCER_GIRL_1
	// modelMC1.loadModel("../models/onichan_verde/DANCER_GIRL_1.obj");
	modelMC1.loadModel("../models/DANCER_GIRL_1/DANCER_GIRL_1.obj");
	modelMC1.setShader(&shaderMulLighting);
	//char2
	// modelMC2.loadModel("../models/chica_rosa/baby_young.fbx");
	modelMC2.loadModel("../models/chica_rosa_1/model.obj");
	modelMC2.setShader(&shaderMulLighting);
	


	camera->setPosition(glm::vec3(0.0, 3.0, 4.0));
	
	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		skyboxTexture.loadImage(true);
		if (skyboxTexture.getData()) {
			glTexImage2D(types[i], 0, skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, skyboxTexture.getWidth(), skyboxTexture.getHeight(), 0,
			skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, skyboxTexture.getData());
		} else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage();
	}

	// Definiendo la textura a utilizar
	Texture textureCesped("../Textures/grassy2.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureCesped.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureCespedID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureCespedID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureCesped.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		std::cout << "Numero de canales :=> " << textureCesped.getChannels() << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, textureCesped.getChannels() == 3 ? GL_RGB : GL_RGBA, textureCesped.getWidth(), textureCesped.getHeight(), 0,
		textureCesped.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureCesped.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureCesped.freeImage();

	// Definiendo la textura a utilizar
	Texture textureWall("../Textures/whiteWall.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureWall.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureWallID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureWallID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureWall.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, textureWall.getChannels() == 3 ? GL_RGB : GL_RGBA, textureWall.getWidth(), textureWall.getHeight(), 0,
		textureWall.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureWall.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureWall.freeImage();

	// Definiendo la textura a utilizar
	Texture textureWindow("../Textures/ventana.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureWindow.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureWindowID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureWindowID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureWindow.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, textureWindow.getChannels() == 3 ? GL_RGB : GL_RGBA, textureWindow.getWidth(), textureWindow.getHeight(), 0,
		textureWindow.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureWindow.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureWindow.freeImage();

	// Definiendo la textura a utilizar
	Texture textureHighway("../Textures/highway.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureHighway.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureHighwayID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureHighwayID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureHighway.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, textureHighway.getChannels() == 3 ? GL_RGB : GL_RGBA, textureHighway.getWidth(), textureHighway.getHeight(), 0,
		textureHighway.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureHighway.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureHighway.freeImage();

	// Definiendo la Textura a utilizar
	Texture textureLandingPad("../Textures/landingPad.jpg");
	//cargando el mapa de BITS
	textureLandingPad.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureLandingPadID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureLandingPadID);

	// configurando  the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// configurando texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	
	// Verifica si se pudo abrir la textura
	if (textureLandingPad.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, textureLandingPad.getChannels() == 3 ? GL_RGB : GL_RGBA, textureLandingPad.getWidth(), textureLandingPad.getHeight(), 0,
		textureLandingPad.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureLandingPad.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureLandingPad.freeImage();


}





void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
	shaderSkybox.destroy();

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCesped.destroy();
	boxWalls.destroy();
	boxHighway.destroy();
	boxLandingPad.destroy();

	// Custom objects Delete
	modelAircraft.destroy();
	modelDartLegoBody.destroy();
	modelDartLegoHead.destroy();
	modelDartLegoLeftArm.destroy();
	modelDartLegoLeftHand.destroy();
	modelDartLegoLeftLeg.destroy();
	modelDartLegoMask.destroy();
	modelDartLegoRightArm.destroy();
	modelDartLegoRightHand.destroy();
	modelDartLegoRightLeg.destroy();
	modelEclipseChasis.destroy();
	modelEclipseFrontalWheels.destroy();
	modelEclipseRearWheels.destroy();
	modelHeliChasis.destroy();
	modelHeliHeli.destroy();
	modelHeliRear.destroy();
	modelLambo.destroy();
	modelLamboFrontLeftWheel.destroy();
	modelLamboFrontRightWheel.destroy();
	modelLamboLeftDor.destroy();
	modelLamboRightDor.destroy();
	modelLamboRearLeftWheel.destroy();
	modelLamboRearRightWheel.destroy();
	modelLamboRightDor.destroy();
	modelRock.destroy();
	modelBuzzHead.destroy();
	modelBuzzTorso.destroy();
	modelBuzzLeftArm.destroy();
	modelBuzzLeftForeArm.destroy();
	modelBuzzLeftHand.destroy();
	modelBuzzLeftThigh.destroy();
	modelBuzzLeftCalf.destroy();
	modelBuzzLeftFoot.destroy();

	modelBuzzHip.destroy();

	modelBuzzRightArm.destroy();
	modelBuzzRightForeArm.destroy();
	modelBuzzRightHand.destroy();
	modelBuzzRightThigh.destroy();
	modelBuzzRightCalf.destroy();
	modelBuzzRightFoot.destroy();

	
	//practica destroy
	modelMC1.destroy();
	modelMC2.destroy();
	

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureCespedID);
	glDeleteTextures(1, &textureWallID);
	glDeleteTextures(1, &textureWindowID);
	glDeleteTextures(1, &textureHighwayID);
	glDeleteTextures(1, &textureLandingPadID);

	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);
}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveFrontCamera(true, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveFrontCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveRightCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRightCamera(true, deltaTime);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, offsetY, deltaTime);
	offsetX = 0;
	offsetY = 0;

	// Seleccionar modelo
	if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS){
		enableCountSelected = false;
		modelSelected++;
		if(modelSelected > 4)
			modelSelected = 0;
		if(modelSelected == 1)
			fileName = "../animaciones/animation_dart_joints.txt";
		if (modelSelected == 2)
			fileName = "../animaciones/animation_dart.txt";
		if (modelSelected == 3)
			fileName = "../animaciones/animation_buzz_joints.txt";
		if(modelSelected == 4)
			fileName = "../animaciones/animation_buzz.txt";
		std::cout << "modelSelected:" << modelSelected << std::endl;
	}
	else if(glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
		enableCountSelected = true;

	// Guardar key frames
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
			&& glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
		record = true;
		if(myfile.is_open())
			myfile.close();
		myfile.open(fileName);
	}
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE
			&& glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
		record = false;
		myfile.close();
		if(modelSelected == 1)
			keyFramesDartJoints = getKeyRotFrames(fileName);
		if (modelSelected == 2)
			keyFramesDart = getKeyFrames(fileName);
		if(modelSelected == 3)
			keyFramesBuzzJoints = getKeyRotFrames(fileName);
		if (modelSelected == 4)
			keyFramesBuzz = getKeyFrames(fileName);
	}
	if(availableSave && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
		saveFrame = true;
		availableSave = false;
	}if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
		availableSave = true;

	// Dart Lego model movements
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotDartHead += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotDartHead -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotDartLeftArm += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotDartLeftArm -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotDartRightArm += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotDartRightArm -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotDartLeftHand += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotDartLeftHand -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotDartRightHand += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotDartRightHand -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotDartLeftLeg += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotDartLeftLeg -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotDartRightLeg += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotDartRightLeg -= 0.02;
	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		modelMatrixDart = glm::rotate(modelMatrixDart, 0.02f, glm::vec3(0, 1, 0));
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		modelMatrixDart = glm::rotate(modelMatrixDart, -0.02f, glm::vec3(0, 1, 0));
	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(-0.02, 0.0, 0.0));
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(0.02, 0.0, 0.0));

//BUZZ CONTROLES
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotBuzzHead += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotBuzzHead -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotBuzzLeftArm += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotBuzzLeftArm -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotBuzzLeftThigh += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotBuzzLeftThigh -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotBuzzRightArm += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotBuzzRightArm -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotBuzzRightThigh += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotBuzzRightThigh -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotBuzzLeftCalf += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotBuzzLeftCalf -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotBuzzRightCalf += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotBuzzRightCalf -= 0.02;




	if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		modelMatrixBuzz = glm::rotate(modelMatrixBuzz, 0.02f, glm::vec3(0, 1, 0));
	else if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		modelMatrixBuzz = glm::rotate(modelMatrixBuzz, -0.02f, glm::vec3(0, 1, 0));
	if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		modelMatrixBuzz = glm::translate(modelMatrixBuzz, glm::vec3(0.0, 0.0, 0.02));
	else if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		modelMatrixBuzz = glm::translate(modelMatrixBuzz, glm::vec3(0.0, 0.0, -0.02));


	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;

	modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(27.5, 0, 30.0));
	modelMatrixEclipse = glm::rotate(modelMatrixEclipse, glm::radians(180.0f), glm::vec3(0, 1, 0));
	int state = 0;
	int state2 = 0;		// LAMBO
	int state3 = 0;
	float helicopterHeightCounter=0.5f;
	float helicopterRotationCounter = 0.0f;
	float advanceCount = 0.0;
	float advanceCount2 = 0.0;		// LAMBO 
	float rotCount = 0.0;
	float rotCount2 = 0.0;			// LAMBO
	float rotWheelsX = 0.0;
	float rotWheelsX2 = 0.0;		// LAMBO
	float rotWheelsY = 0.0;
	float rotWheelsY2 = 0.0;		// LAMBO
	int numberAdvance = 0;
	int numberAdvance2 = 0;			// LAMBO 
	int maxAdvance = 0.0;
	int maxAdvance2 = 0.0;			// LAMBO
	int maxAdvance3 = 0.0;

	matrixModelRock = glm::translate(matrixModelRock, glm::vec3(-3.0, 0.0, 2.0));

	modelMatrixHeli = glm::translate(modelMatrixHeli, glm::vec3(5.0, 0.0, -5.0));

	modelMatrixAircraft = glm::translate(modelMatrixAircraft, glm::vec3(10.0, 2.0, -17.5));

	modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(23.0, 0.0, 0.0));

	modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(3.0, 0.0, -14.0));

	modelMatrixBuzz = glm::translate(modelMatrixBuzz, glm::vec3(3.0f, 0.0f, -16.0));

	//practica
	modelMatrixMC1 = glm::translate(modelMatrixMC1,glm::vec3(-5.0f, 0.05f, -11.0f));
	modelMatrixMC1 = glm::rotate(modelMatrixMC1,glm::radians(0.0f),glm::vec3(1,0,0));
	modelMatrixMC1 = glm::scale(modelMatrixMC1, glm::vec3(0.012f,0.012f,0.012f));
	modelMatrixMC2 = glm::translate(modelMatrixMC2,glm::vec3(-5.0f, 5.05f, -10.0f));
	modelMatrixMC2 = glm::rotate(modelMatrixMC2,glm::radians(0.0f),glm::vec3(1,0,0));
	modelMatrixMC2 = glm::scale(modelMatrixMC2, glm::vec3(0.012f,0.012f,0.012f));
	// Variables to interpolation key frames
	fileName = "../animaciones/animation_dart_joints.txt";
	keyFramesDartJoints = getKeyRotFrames(fileName);
	keyFramesDart = getKeyFrames("../animaciones/animation_dart.txt");
	fileName2 = "../animaciones/animation_buzz_joints.txt";
	keyFramesBuzzJoints = getKeyRotFrames(fileName2);
	keyFramesBuzz = getKeyFrames("../animaciones/animation_buzz.txt");

	lastTime = TimeManager::Instance().GetTime();

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if(currTime - lastTime < 0.016666667){
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		// Variables donde se guardan las matrices de cada articulacion por 1 frame
		std::vector<float> matrixDartJoints;
		std::vector<glm::mat4> matrixDart;

		std::vector<float> matrixBuzzJoints;
		std::vector<glm::mat4> matrixBuzz;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //limpiado los bufferes de Color y de profundidad

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) screenWidth / (float) screenHeight, 0.01f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
				glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
				glm::value_ptr(view));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/
		shaderMulLighting.setInt("spotLightCount", 0);

		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		shaderMulLighting.setInt("pointLightCount", 0);

		/*******************************************
		 * Cesped
		 *******************************************/
		glm::mat4 modelCesped = glm::mat4(1.0);
		modelCesped = glm::translate(modelCesped, glm::vec3(0.0, 0.0, 0.0));
		modelCesped = glm::scale(modelCesped, glm::vec3(200.0, 0.001, 200.0));
		// Se activa la textura del agua
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureCespedID);
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(200, 200)));
		boxCesped.render(modelCesped);
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		/*******************************************ww
		 * Casa
		 *******************************************/
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureWallID);
		// Frontal wall
		boxWalls.setScale(glm::vec3(10.0, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-10.0, 2.5, 0.0));
		boxWalls.setOrientation(glm::vec3(0.0, 0.0, 0.0));
		boxWalls.render();
		// Left wall
		boxWalls.setScale(glm::vec3(10.0, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-15.0, 2.5, -5.0));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Right walls
		// First wall
		boxWalls.setScale(glm::vec3(3.75, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 2.5, -1.875));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Second wall
		boxWalls.setScale(glm::vec3(3.75, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 2.5, -8.125));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Third wall
		boxWalls.setScale(glm::vec3(2.5, 1.25, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 4.375, -5.0));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Four wall
		boxWalls.setScale(glm::vec3(2.5, 1.25, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 0.625, -5.0));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Back wall
		boxWalls.setScale(glm::vec3(10.0, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-10.0, 2.5, -10.0));
		boxWalls.setOrientation(glm::vec3(0.0, 0.0, 0.0));
		boxWalls.render();
		// Ceilling
		boxWalls.setScale(glm::vec3(12.0, 12.0, 0.05));
		boxWalls.setPosition(glm::vec3(-10.0, 5, -5.0));
		boxWalls.setOrientation(glm::vec3(90.0, 0.0, 0.0));
		boxWalls.render();
		// Window
		glBindTexture(GL_TEXTURE_2D, textureWindowID);
		boxWalls.setScale(glm::vec3(2.5, 2.5, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 2.5, -5.0));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Highway 0
		glBindTexture(GL_TEXTURE_2D, textureHighwayID);
		boxHighway.setScale(glm::vec3(40.0, 0.05, 10.0));
		boxHighway.setPosition(glm::vec3(0.0, 0.05, 10.0));
		boxHighway.setOrientation(glm::vec3(0.0, 0.0, 0.0));
		boxHighway.render();
		// Highway 1
		boxHighway.setScale(glm::vec3(80.0, 0.05, 10.0));
		boxHighway.setPosition(glm::vec3(25.0, 0.05, 0.0));
		boxHighway.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2, 1)));
		boxHighway.render();
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		// Highway 2
		boxHighway.setScale(glm::vec3(80.0, 0.05, 10.0));
		boxHighway.setPosition(glm::vec3(-25.0, 0.05, 0.0));
		boxHighway.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2, 1)));
		boxHighway.render();
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		// Highway 3
		boxHighway.setScale(glm::vec3(40.0, 0.05, 10.0));
		boxHighway.setPosition(glm::vec3(0.0, 0.05, -35.0));
		boxHighway.setOrientation(glm::vec3(0.0, 0.0, 0.0));
		boxHighway.render();
		//LandingPad
		glBindTexture(GL_TEXTURE_2D, textureLandingPadID);
		boxLandingPad.setScale(glm::vec3(10.0f, 0.05f, 10.0f));
		boxLandingPad.setPosition(glm::vec3(5.0f, 0.05f,-5.0f));
		boxLandingPad.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		boxLandingPad.render();
		glBindTexture(GL_TEXTURE_2D,0);
		
		/************************
		*  renderizado de las esferas
		ademas de aplicar una Textura
		***********************************/
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, textureWindowID);
		//esfera1.setScale(glm::vec3(2.5f,2.5f,2.5f));
		//.setPosition(glm::vec3(0.0f, 2.0f, -5.0));
		//esfera1.render();

		//(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, textureHighwayID);
		//esfera1.setScale(glm::vec3(3.5f,3.5f,3.5f));
		//esfera1.setPosition(glm::vec3(12.0f, 2.0f, -5.0));
		//esfera1.render();



		/*******************************************
		 * Custom objects obj
		 *******************************************/
		//Rock render
		modelRock.render(matrixModelRock);
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		// Render for the aircraft model
		modelAircraft.render(modelMatrixAircraft);

		// Render for the eclipse car
		glm::mat4 modelMatrixEclipseChasis = glm::mat4(modelMatrixEclipse);
		modelMatrixEclipseChasis = glm::scale(modelMatrixEclipse, glm::vec3(0.5, 0.5, 0.5));
		modelEclipseChasis.render(modelMatrixEclipseChasis);

		glm::mat4 modelMatrixFrontalWheels = glm::mat4(modelMatrixEclipseChasis);
		modelMatrixFrontalWheels = glm::translate(modelMatrixFrontalWheels, glm::vec3(0.0, 1.05813, 4.11483 ));
		modelMatrixFrontalWheels = glm::rotate(modelMatrixFrontalWheels, rotWheelsY, glm::vec3(0, 1, 0));
		modelMatrixFrontalWheels = glm::rotate(modelMatrixFrontalWheels, rotWheelsX, glm::vec3(1, 0, 0));
		modelMatrixFrontalWheels = glm::translate(modelMatrixFrontalWheels, glm::vec3(0.0, -1.05813, -4.11483));
		modelEclipseFrontalWheels.render(modelMatrixFrontalWheels);

		glm::mat4 modelMatrixRearWheels = glm::mat4(modelMatrixEclipseChasis);
		modelMatrixRearWheels = glm::translate(modelMatrixRearWheels, glm::vec3(0.0, 1.05813, -4.35157 ));
		modelMatrixRearWheels = glm::rotate(modelMatrixRearWheels, rotWheelsX, glm::vec3(1, 0, 0));
		modelMatrixRearWheels = glm::translate(modelMatrixRearWheels, glm::vec3(0.0, -1.05813, 4.35157));
		modelEclipseRearWheels.render(modelMatrixRearWheels);

		// Helicopter
		glm::mat4 modelMatrixHeliChasis = glm::mat4(modelMatrixHeli);
		modelHeliChasis.render(modelMatrixHeliChasis);

		glm::mat4 modelMatrixHeliHeli = glm::mat4(modelMatrixHeliChasis);
		modelMatrixHeliHeli = glm::translate(modelMatrixHeliHeli, glm::vec3(0.0, 0.0, -0.249548));
		modelMatrixHeliHeli = glm::rotate(modelMatrixHeliHeli, rotHelHelY, glm::vec3(0, 1, 0));
		modelMatrixHeliHeli = glm::translate(modelMatrixHeliHeli, glm::vec3(0.0, 0.0, 0.249548));
		modelHeliHeli.render(modelMatrixHeliHeli);

		glm::mat4 modelMatrixHeliRear = glm::mat4(modelMatrixHeliChasis);
		modelMatrixHeliRear = glm::translate(modelMatrixHeliRear, glm::vec3(0.0, 2.095, -5.635));
		modelMatrixHeliRear = glm::rotate(modelMatrixHeliRear, rotHelRearY, glm::vec3(1, 0, 0));
		modelMatrixHeliRear = glm::translate(modelMatrixHeliRear, glm::vec3(0.0, -2.095, 5.635));
		modelHeliRear.render(modelMatrixHeliRear);

		// Lambo car
		glDisable(GL_CULL_FACE);
		glm::mat4 modelMatrixLamboChasis = glm::mat4(modelMatrixLambo);
		modelMatrixLamboChasis = glm::scale(modelMatrixLamboChasis, glm::vec3(1.3, 1.3, 1.3));
		modelLambo.render(modelMatrixLamboChasis);
		
		glActiveTexture(GL_TEXTURE0);
		glm::mat4 modelMatrixLamboLeftDor = glm::mat4(modelMatrixLamboChasis);
		modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(1.08676, 0.707316, 0.982601));
		modelMatrixLamboLeftDor = glm::rotate(modelMatrixLamboLeftDor, glm::radians(dorRotCount), glm::vec3(1.0, 0, 0));
		modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(-1.08676, -0.707316, -0.982601));
		modelLamboLeftDor.render(modelMatrixLamboLeftDor);

		glm::mat4 modelMatrixLamboRightDor = glm::mat4(modelMatrixLamboChasis);
		modelMatrixLamboRightDor = glm::translate(modelMatrixLamboRightDor, glm::vec3(-1.076, 0.6992, 0.9766));
		modelMatrixLamboRightDor = glm::rotate(modelMatrixLamboRightDor, glm::radians(dorRotCountRight), glm::vec3(1.0, 0, 0));
		modelMatrixLamboRightDor = glm::translate(modelMatrixLamboRightDor, glm::vec3(1.076, -0.6992, -0.9766));
		modelLamboRightDor.render(modelMatrixLamboRightDor);


		glm::mat4 modelMatrixLamboFrontLeftWheel = glm::mat4(modelMatrixLamboChasis);
		modelMatrixLamboFrontLeftWheel = glm::translate(modelMatrixLamboFrontLeftWheel, glm::vec3(0.0f,0.37f,1.4f));
		modelMatrixLamboFrontLeftWheel = glm::rotate(modelMatrixLamboFrontLeftWheel, -rotWheelsY2, glm::vec3(0, 1, 0)); //giro en Y contrario al del Eclipse
		modelMatrixLamboFrontLeftWheel = glm::rotate(modelMatrixLamboFrontLeftWheel, rotWheelsX2, glm::vec3(1, 0, 0));
		modelMatrixLamboFrontLeftWheel = glm::translate(modelMatrixLamboFrontLeftWheel, glm::vec3(0.0, -0.37f, -1.4f));
		modelLamboFrontLeftWheel.render(modelMatrixLamboFrontLeftWheel);
		modelLamboFrontRightWheel.render(modelMatrixLamboFrontLeftWheel);
		
		glm::mat4 modelMatrixLamboRearLeftWheel = glm::mat4(modelMatrixLamboChasis);
		modelMatrixLamboRearLeftWheel = glm::translate(modelMatrixLamboRearLeftWheel, glm::vec3(0.0f,0.397f,-1.598f));
		modelMatrixLamboRearLeftWheel = glm::rotate(modelMatrixLamboRearLeftWheel, rotWheelsX2, glm::vec3(1, 0, 0));
		modelMatrixLamboRearLeftWheel = glm::translate(modelMatrixLamboRearLeftWheel, glm::vec3(0.0, -0.397f, 1.598f));
		modelLamboRearLeftWheel.render(modelMatrixLamboRearLeftWheel);
		modelLamboRearRightWheel.render(modelMatrixLamboRearLeftWheel);

		
		// Se regresa el cull faces IMPORTANTE para las puertas
		glEnable(GL_CULL_FACE);

		// Dart lego
		// Se deshabilita el cull faces IMPORTANTE para la capa
		glDisable(GL_CULL_FACE);
		glm::mat4 modelMatrixDartBody = glm::mat4(modelMatrixDart);
		modelMatrixDartBody = glm::scale(modelMatrixDartBody, glm::vec3(0.5, 0.5, 0.5));
		modelDartLegoBody.render(modelMatrixDartBody);
		glm::mat4 modelMatrixDartHead = glm::mat4(modelMatrixDartBody);
		modelMatrixDartHead = glm::rotate(modelMatrixDartHead, rotDartHead, glm::vec3(0, 1, 0));
		modelDartLegoHead.render(modelMatrixDartHead);
		modelDartLegoMask.render(modelMatrixDartHead);
		glm::mat4 modelMatrixDartLeftArm = glm::mat4(modelMatrixDartBody);
		modelMatrixDartLeftArm = glm::translate(modelMatrixDartLeftArm, glm::vec3(-0.023515, 2.43607, 0.446066));
		modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, rotDartLeftArm, glm::vec3(0, 0, 1));
		modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftArm = glm::translate(modelMatrixDartLeftArm, glm::vec3(0.023515, -2.43607, -0.446066));
		modelDartLegoLeftArm.render(modelMatrixDartLeftArm);
		glm::mat4 modelMatrixDartLeftHand = glm::mat4(modelMatrixDartLeftArm);
		modelMatrixDartLeftHand = glm::translate(modelMatrixDartLeftHand, glm::vec3(0.201343, 1.68317, 0.99774));
		modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, rotDartLeftHand, glm::vec3(0, 1, 0));
		modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftHand = glm::translate(modelMatrixDartLeftHand, glm::vec3(-0.201343, -1.68317, -0.99774));
		modelDartLegoLeftHand.render(modelMatrixDartLeftHand);
		glm::mat4 modelMatrixDartRightArm = glm::mat4(modelMatrixDartBody);
		modelMatrixDartRightArm = glm::translate(modelMatrixDartRightArm, glm::vec3(-0.023515, 2.43607, -0.446066));
		modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, rotDartRightArm, glm::vec3(0, 0, 1));
		modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightArm = glm::translate(modelMatrixDartRightArm, glm::vec3(0.023515, -2.43607, 0.446066));
		modelDartLegoRightArm.render(modelMatrixDartRightArm);
		glm::mat4 modelMatrixDartRightHand = glm::mat4(modelMatrixDartRightArm);
		modelMatrixDartRightHand = glm::translate(modelMatrixDartRightHand, glm::vec3(0.201343, 1.68317, -0.99774));
		modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, rotDartRightHand, glm::vec3(0, 1, 0));
		modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightHand = glm::translate(modelMatrixDartRightHand, glm::vec3(-0.201343, -1.68317, 0.99774));
		modelDartLegoRightHand.render(modelMatrixDartRightHand);
		glm::mat4 modelMatrixDartLeftLeg = glm::mat4(modelMatrixDartBody);
		modelMatrixDartLeftLeg = glm::translate(modelMatrixDartLeftLeg, glm::vec3(0, 1.12632, 0.423349));
		modelMatrixDartLeftLeg = glm::rotate(modelMatrixDartLeftLeg, rotDartLeftLeg, glm::vec3(0, 0, 1));
		modelMatrixDartLeftLeg = glm::translate(modelMatrixDartLeftLeg, glm::vec3(0, -1.12632, -0.423349));
		modelDartLegoLeftLeg.render(modelMatrixDartLeftLeg);
		glm::mat4 modelMatrixDartRightLeg = glm::mat4(modelMatrixDartBody);
		modelMatrixDartRightLeg = glm::translate(modelMatrixDartRightLeg, glm::vec3(0, 1.12632, -0.423349));
		modelMatrixDartRightLeg = glm::rotate(modelMatrixDartRightLeg, rotDartRightLeg, glm::vec3(0, 0, 1));
		modelMatrixDartRightLeg = glm::translate(modelMatrixDartRightLeg, glm::vec3(0, -1.12632, 0.423349));
		modelDartLegoRightLeg.render(modelMatrixDartRightLeg);
		// Se regresa el cull faces IMPORTANTE para la capa
		glEnable(GL_CULL_FACE);

		glDisable(GL_CULL_FACE);
		//Buzz
		glm::mat4 modelMatrixBuzzBody = glm::mat4(modelMatrixBuzz);
		modelMatrixBuzzBody = glm::scale(modelMatrixBuzzBody, glm::vec3(3.0f));
		modelBuzzTorso.render(modelMatrixBuzzBody);
		glm::mat4 modelMatrixBuzzLeftArm = glm::mat4(modelMatrixBuzzBody);
		modelMatrixBuzzLeftArm = glm::translate(modelMatrixBuzzLeftArm,glm::vec3(0.1837,0.5798, -0.02744));
		modelMatrixBuzzLeftArm = glm::rotate(modelMatrixBuzzLeftArm, rotBuzzLeftArm, glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrixBuzzLeftArm = glm::rotate(modelMatrixBuzzLeftArm, glm::radians(-65.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrixBuzzLeftArm = glm::translate(modelMatrixBuzzLeftArm,glm::vec3(-0.1837,-0.5798, 0.02744));
		modelBuzzLeftArm.render(modelMatrixBuzzLeftArm);
		glm::mat4 modelMatrixBuzzLeftForeArm = glm::mat4(modelMatrixBuzzLeftArm);
		modelBuzzLeftForeArm.render(modelMatrixBuzzLeftForeArm);
		glm::mat4 modelMatrixBuzzLeftHand = glm::mat4(modelMatrixBuzzLeftForeArm);
		modelBuzzLeftHand.render(modelMatrixBuzzLeftHand);

		glm::mat4 modelMatrixBuzzHead = glm::mat4(modelMatrixBuzzBody);
		modelMatrixBuzzHead = glm::rotate(modelMatrixBuzzHead, rotBuzzHead, glm::vec3(0.0f, 1.0f, 0.0f));
		modelBuzzHead.render(modelMatrixBuzzHead);

		glm::mat4 modelMatrixBuzzHip = glm::mat4(modelMatrixBuzzBody);
		modelBuzzHip.render(modelMatrixBuzzHip);
		
		glm::mat4 modelMatrixBuzzRightArm = glm::mat4(modelMatrixBuzzBody);
		modelMatrixBuzzRightArm = glm::translate(modelMatrixBuzzRightArm,glm::vec3(-0.1837,0.5798, 0.02744));
		modelMatrixBuzzRightArm = glm::rotate(modelMatrixBuzzRightArm, rotBuzzRightArm, glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrixBuzzRightArm = glm::rotate(modelMatrixBuzzRightArm, glm::radians(65.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrixBuzzRightArm = glm::translate(modelMatrixBuzzRightArm,glm::vec3(0.1837,-0.5798, -0.02744));
		modelBuzzRightArm.render(modelMatrixBuzzRightArm);
		glm::mat4 modelMatrixBuzzRightForeArm = glm::mat4(modelMatrixBuzzRightArm);
		modelBuzzRightForeArm.render(modelMatrixBuzzRightForeArm);
		glm::mat4 modelMatrixBuzzRightHand = glm::mat4(modelMatrixBuzzRightForeArm);
		modelBuzzRightHand.render(modelMatrixBuzzRightHand);

		
		glm::mat4 modelMatrixBuzzLeftThigh = glm::mat4(modelMatrixBuzzBody);
		modelMatrixBuzzLeftThigh = glm::translate(modelMatrixBuzzLeftThigh, glm::vec3(-0.05f, 0.36f, 0.0f));
		modelMatrixBuzzLeftThigh = glm::rotate(modelMatrixBuzzLeftThigh, rotBuzzLeftThigh, glm::vec3(1,0,0));
		modelMatrixBuzzLeftThigh = glm::translate(modelMatrixBuzzLeftThigh, glm::vec3(0.05f, -0.36f, -0.0f));
		modelBuzzLeftThigh.render(modelMatrixBuzzLeftThigh);

		glm::mat4 modelMatrixBuzzLeftCalf = glm::mat4(modelMatrixBuzzLeftThigh);
		modelMatrixBuzzLeftCalf = glm::translate(modelMatrixBuzzLeftCalf, glm::vec3(0.063f, 0.202f, 0.0f));
		modelMatrixBuzzLeftCalf = glm::rotate(modelMatrixBuzzLeftCalf, rotBuzzLeftCalf, glm::vec3(1,0,0));
		modelMatrixBuzzLeftCalf = glm::translate(modelMatrixBuzzLeftCalf, glm::vec3(-0.063f, -0.202f, 0.0f));
		modelBuzzLeftCalf.render(modelMatrixBuzzLeftCalf);
		glm::mat4 modelMatrixBuzzLeftFoot = glm::mat4(modelMatrixBuzzLeftCalf);
		modelBuzzLeftFoot.render(modelMatrixBuzzLeftFoot);
		
		glm::mat4 modelMatrixBuzzRightThigh = glm::mat4(modelMatrixBuzzBody);
		modelMatrixBuzzRightThigh = glm::translate(modelMatrixBuzzRightThigh, glm::vec3(0.05f, 0.36f, 0.0f));
		modelMatrixBuzzRightThigh = glm::rotate(modelMatrixBuzzRightThigh, rotBuzzRightThigh, glm::vec3(1,0,0));
		modelMatrixBuzzRightThigh = glm::translate(modelMatrixBuzzRightThigh, glm::vec3(-0.05f, -0.36f, -0.0f));
		modelBuzzRightThigh.render(modelMatrixBuzzRightThigh);
		
		glm::mat4 modelMatrixBuzzRightCalf = glm::mat4(modelMatrixBuzzRightThigh);
		modelMatrixBuzzRightCalf = glm::translate(modelMatrixBuzzRightCalf, glm::vec3(-0.063f, 0.202f, 0.0f));
		modelMatrixBuzzRightCalf = glm::rotate(modelMatrixBuzzRightCalf, rotBuzzRightCalf, glm::vec3(1,0,0));
		modelMatrixBuzzRightCalf = glm::translate(modelMatrixBuzzRightCalf, glm::vec3(0.063f, -0.202f, 0.0f));
		modelBuzzRightCalf.render(modelMatrixBuzzRightCalf);
		glm::mat4 modelMatrixBuzzRightFoot = glm::mat4(modelMatrixBuzzRightCalf);
		modelBuzzRightFoot.render(modelMatrixBuzzRightFoot);
		
		



		glEnable(GL_CULL_FACE);

		/*******************************
		 * Custom Anim objects obj
		*****************************/

		//practica
		modelMC1.setAnimationIndex(0);
		modelMC2.setAnimationIndex(0);
		
		modelMC1.render(modelMatrixMC1);
		modelMC2.render(modelMatrixMC2);

		/*******************************************
		 * Skybox
		 *******************************************/
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		shaderSkybox.setFloat("skybox", 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		glActiveTexture(GL_TEXTURE0);
		skyboxSphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);

	//Para guardar los frames
		if(record && modelSelected == 1){
			matrixDartJoints.push_back(rotDartHead);
			matrixDartJoints.push_back(rotDartLeftArm);
			matrixDartJoints.push_back(rotDartLeftHand);
			matrixDartJoints.push_back(rotDartRightArm);
			matrixDartJoints.push_back(rotDartRightHand);
			matrixDartJoints.push_back(rotDartLeftLeg);
			matrixDartJoints.push_back(rotDartRightLeg);
			if(saveFrame) {
				appendFrame(myfile, matrixDartJoints);
				saveFrame = false;
			}
		} 
		else if(keyFramesDartJoints.size() > 0) {
			//Para reproducir frames
			interpolationDartJoints = numPasosDartJoints /(float) maxNumPasosDartJoints; //0/20, 1/20, 2/20, 3/20...
			numPasosDartJoints++;
			if(interpolationDartJoints > 1.0){
				numPasosDartJoints = 0;
				interpolationDartJoints = 0;
				indexFrameDartJoints = indexFrameDartJointsNext;
				indexFrameDartJointsNext++;
			}
			if(indexFrameDartJointsNext > keyFramesDartJoints.size() - 1)
				indexFrameDartJointsNext = 0;  //regresa al primer frame
			rotDartHead = interpolate(keyFramesDartJoints,indexFrameDartJoints, indexFrameDartJointsNext,0, interpolationDartJoints);
			rotDartLeftArm = interpolate(keyFramesDartJoints,indexFrameDartJoints, indexFrameDartJointsNext,1, interpolationDartJoints);
			rotDartLeftHand = interpolate(keyFramesDartJoints,indexFrameDartJoints, indexFrameDartJointsNext,2, interpolationDartJoints);
			rotDartRightArm = interpolate(keyFramesDartJoints,indexFrameDartJoints, indexFrameDartJointsNext,3, interpolationDartJoints);
			rotDartRightHand = interpolate(keyFramesDartJoints,indexFrameDartJoints, indexFrameDartJointsNext,4, interpolationDartJoints);
			rotDartLeftLeg = interpolate(keyFramesDartJoints,indexFrameDartJoints, indexFrameDartJointsNext,5, interpolationDartJoints);
			rotDartRightLeg = interpolate(keyFramesDartJoints,indexFrameDartJoints, indexFrameDartJointsNext,6, interpolationDartJoints);
		}

			if(record && modelSelected ==2){
				matrixDart.push_back(modelMatrixDart);
				if(saveFrame) {
					appendFrame(myfile,matrixDart);
					saveFrame = false;
				}
			}
			else if(keyFramesDart.size() > 0){
				//para reproducir frames
				interpolationDart = numPasosDart / (float)maxNumPasosDart;
				numPasosDart++;
				if(interpolationDart > 1.0){
					numPasosDart = 0;
					interpolationDart = 0;
					indexFrameDart = indexFrameDartNext;
					indexFrameDartNext++;
				}
				if(indexFrameDartNext > keyFramesDart.size() - 1)
					indexFrameDartNext = 0;
				modelMatrixDart = interpolate(keyFramesDart,indexFrameDart, indexFrameDartNext, 0, interpolationDart );
			}

		if(record && modelSelected ==3 ){
			matrixBuzzJoints.push_back(rotBuzzHead);
			matrixBuzzJoints.push_back(rotBuzzLeftArm);
			matrixBuzzJoints.push_back(rotBuzzLeftHand);
			matrixBuzzJoints.push_back(rotBuzzRightArm);
			matrixBuzzJoints.push_back(rotBuzzRightHand);
			matrixBuzzJoints.push_back(rotBuzzLeftThigh);
			matrixBuzzJoints.push_back(rotBuzzLeftCalf);
			matrixBuzzJoints.push_back(rotBuzzLeftFoot);
			matrixBuzzJoints.push_back(rotBuzzRightThigh);
			matrixBuzzJoints.push_back(rotBuzzRightCalf);
			matrixBuzzJoints.push_back(rotBuzzRightFoot);
			if(saveFrame) {
				appendFrame(myfile, matrixBuzzJoints);
				saveFrame = false;
			}
		} 
		else if(keyFramesBuzzJoints.size() > 0){
			//Para reproducir frames
			interpolationBuzzJoints = numPasosBuzzJoints /(float) maxNumPasosBuzzJoints; //0/20, 1/20, 2/20, 3/20...
			numPasosBuzzJoints++;
			if(interpolationBuzzJoints > 1.0){
				numPasosBuzzJoints = 0;
				interpolationBuzzJoints = 0;
				indexFrameBuzzJoints = indexFrameBuzzJointsNext;
				indexFrameBuzzJointsNext++;
			}
			if(indexFrameBuzzJointsNext > keyFramesBuzzJoints.size() - 1)
				indexFrameBuzzJointsNext = 0;  //regresa al primer frame
			rotBuzzHead = interpolate(keyFramesBuzzJoints,indexFrameBuzzJoints, indexFrameBuzzJointsNext,0, interpolationBuzzJoints);
			rotBuzzLeftArm = interpolate(keyFramesBuzzJoints,indexFrameBuzzJoints, indexFrameBuzzJointsNext,1, interpolationBuzzJoints);
			rotBuzzLeftHand = interpolate(keyFramesBuzzJoints,indexFrameBuzzJoints, indexFrameBuzzJointsNext,2, interpolationBuzzJoints);
			rotBuzzRightArm = interpolate(keyFramesBuzzJoints,indexFrameBuzzJoints, indexFrameBuzzJointsNext,3, interpolationBuzzJoints);
			rotBuzzRightHand = interpolate(keyFramesBuzzJoints,indexFrameBuzzJoints, indexFrameBuzzJointsNext,4, interpolationBuzzJoints);	
			rotBuzzLeftThigh = interpolate(keyFramesBuzzJoints,indexFrameBuzzJoints, indexFrameBuzzJointsNext,5, interpolationBuzzJoints);
			rotBuzzLeftCalf = interpolate(keyFramesBuzzJoints,indexFrameBuzzJoints, indexFrameBuzzJointsNext,6, interpolationBuzzJoints);
			rotBuzzLeftFoot = interpolate(keyFramesBuzzJoints,indexFrameBuzzJoints, indexFrameBuzzJointsNext,7, interpolationBuzzJoints);	
			rotBuzzRightThigh = interpolate(keyFramesBuzzJoints,indexFrameBuzzJoints, indexFrameBuzzJointsNext,8, interpolationBuzzJoints);
			rotBuzzRightCalf = interpolate(keyFramesBuzzJoints,indexFrameBuzzJoints, indexFrameBuzzJointsNext,9, interpolationBuzzJoints);
			rotBuzzRightFoot = interpolate(keyFramesBuzzJoints,indexFrameBuzzJoints, indexFrameBuzzJointsNext,10, interpolationBuzzJoints);
			}

			if(record && modelSelected ==4){
				matrixBuzz.push_back(modelMatrixBuzz);
				if(saveFrame) {
					appendFrame(myfile,matrixBuzz);
					saveFrame = false;
				}
			}
			else if(keyFramesBuzz.size() > 0){
				//para reproducir frames
				interpolationBuzz = numPasosBuzz / (float)maxNumPasosBuzz;
				numPasosBuzz++;
				if(interpolationBuzz > 1.0){
					numPasosBuzz = 0;
					interpolationBuzz = 0;
					indexFrameBuzz = indexFrameBuzzNext;
					indexFrameBuzzNext++;
				}
				if(indexFrameBuzzNext > keyFramesBuzz.size() - 1)
					indexFrameBuzzNext = 0;
				modelMatrixBuzz = interpolate(keyFramesBuzz,indexFrameBuzz, indexFrameBuzzNext, 0, interpolationBuzz );
			}


		// Constantes de animaciones
		rotHelHelY += helicopterRotationCounter;
		rotHelRearY += 5.0f*helicopterRotationCounter;

		/*************************
		Maquinas de estados
			
		*****************/
		//Maquina de estado de Auto Eclipse
		switch(state){
			case 0:
				if(numberAdvance == 0)
					maxAdvance = 65.0f;
				else if(numberAdvance == 1)
					maxAdvance = 49.0f;
				else if(numberAdvance == 2)
					maxAdvance = 44.5;
				else if(numberAdvance == 3)
					maxAdvance = 49.0f;
				else if(numberAdvance == 4)
					maxAdvance = 44.5f;
				state = 1;
				break;

			case 1:
				modelMatrixEclipse = glm::translate(modelMatrixEclipse,glm::vec3(0.0f, 0.0f,0.1f));
				advanceCount += 0.1f;
				rotWheelsX += 0.05f;
				rotWheelsY -= 0.02f;
				if(rotWheelsY < 0)
					rotWheelsY = 0;
				if(advanceCount> maxAdvance){
					advanceCount = 0;
					numberAdvance++;
					state = 2;					
				}
			 	break;
			case 2:
				modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(0.0f,0.0f,0.025f)); //para gira de llantas en eje Y
				modelMatrixEclipse = glm::rotate(modelMatrixEclipse, glm::radians(0.5f),glm::vec3(0.0f,1.0f,0.0f));
				rotCount += 0.5f;
				rotWheelsX += 0.05f;
				rotWheelsY += 0.02f;
				if(rotWheelsY > 0.24f)
					rotWheelsY = 0.24f;
				if(rotCount >= 90.0f){
					rotCount = 0;
					state = 0;
					if(numberAdvance > 4)
						numberAdvance = 1;
				}	
				break;
		}


		//Maquina de estado del Auto Lambo
		switch(state2){  
			case 0:
				if(numberAdvance2 == 0)
					maxAdvance2 = 5.0f;
				else if(numberAdvance2 == 1)
					maxAdvance2 = 40.0f;
				else if(numberAdvance2 == 2)
					maxAdvance2 = 36.0f;
				else if(numberAdvance2 == 3)
					maxAdvance2 = 40.0f;
				else if(numberAdvance2 == 4)
					maxAdvance2 = 31.0f;
				else if(numberAdvance2 == 5)
					break;
				printf("state circuit -: %d \n", numberAdvance2);
				state2 = 1;
				break;

			case 1:
				modelMatrixLambo = glm::translate(modelMatrixLambo,glm::vec3(0.0f, 0.0f,0.1f));
				advanceCount2 += 0.1f;
				rotWheelsX2 += 0.05f;
				rotWheelsY2 -= 0.02f;
				if(rotWheelsY2 < 0.0f)
					rotWheelsY2 = 0.0f;
				if(advanceCount2> maxAdvance2){
					advanceCount2 = 0;
					numberAdvance2++;
					state2 = 2;					
				}
				
				break;
			case 2:
				if(numberAdvance2 == 5){
						stateDoor  = 0;
						if(dorRotCountRight >= 75.0f)
							state2=3;
						break;
					}
				modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0.0f,0.0f,0.025f)); //para gira de llantas en eje Y
				modelMatrixLambo = glm::rotate(modelMatrixLambo, glm::radians(-0.5f),glm::vec3(0.0f,1.0f,0.0f)); //+
				rotCount2 -= 0.5f; //+
				rotWheelsX2 += 0.05f;
				rotWheelsY2 += 0.02f; //+
				if(rotWheelsY2 > 0.24f)    //*****
					rotWheelsY2 = 0.24f;
				if(rotCount2 <= -90.0f){
					rotCount2 = 0;
					state2 = 0;
				}	
				break;
			case 3:
				break;
		}

		//Maquina de estados (Lambo) de la puerta
		switch(stateDoor){
			case 0:
				if (numberAdvance2 == 5 && stateDoor == 0){
					stateDoor = 1;
				} else {
					break;
				}
			
			case 1:
				dorRotCountRight += 0.5f;
				if(dorRotCountRight >= 75.0f)
					dorRotCountRight = 75.0f;//stateDoor = 2;
					stateDoor = 3;
				break;
			case 2:
				dorRotCount  -= 0.5f;
				if(dorRotCount < 0) {
					dorRotCount = 0.0f;
					stateDoor = 3;
				break;
				}
			case 3:
				break;
		}
	
		
		//Maquina de estados Helicoptero de la puerta
		switch(state3){
			case 0: //acelera
				helicopterRotationCounter += 0.001f;
				if(helicopterRotationCounter >= 0.5f)
				{
					helicopterRotationCounter = 0.5f;
					state3 = 1;
				}
				break;
			case 1: //ascenso
				helicopterHeightCounter +=0.1f;
				maxAdvance3 = 60.0f;
				//advanceCount3 += 0.05;
				modelMatrixHeli = glm::translate(modelMatrixHeli, glm::vec3(0.0, 0.01, 0.0));
				if(helicopterHeightCounter >= maxAdvance3){
					helicopterHeightCounter = 0.0f;
					state3 = 2;
				}
				break;
			case 2: //descenso
				maxAdvance3 = 60.0f;
				helicopterHeightCounter += 0.1;
				modelMatrixHeli = glm::translate(modelMatrixHeli, glm::vec3(0.0, -0.01, 0.0));
				if(helicopterHeightCounter >= maxAdvance3){
					helicopterHeightCounter = 0.0f;
					state3 = 3;
				}
				break;
			case 3:  //desacelera
				helicopterRotationCounter -= 0.001f;
				if(helicopterRotationCounter <= 0.0 ){
					helicopterRotationCounter = 0.0f;
					state3 = 0;
				}
				break;
			default:
				break;
		}

		glfwSwapBuffers(window);
	}
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}