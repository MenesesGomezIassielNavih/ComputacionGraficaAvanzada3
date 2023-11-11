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

// Include Terrain
#include "Headers/Terrain.h"

#include "Headers/AnimationUtils.h"

// Include Colision headers functions
#include "Headers/Colisiones.h"


#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;



GLFWwindow *window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;



std::shared_ptr<FirstPersonCamera> camera1(new FirstPersonCamera());

std::shared_ptr<ThirdPersonCamera> camera(new ThirdPersonCamera());
float distanceFromTarget = 7.0;

glm::mat4 view;
	glm::vec3 axis;
	glm::vec3 target;
	float angleTarget;

Sphere skyboxSphere(20, 20);

Box boxCesped;
Box boxWalls;
Box boxHighway;
Box boxLandingPad;
Sphere esfera1(10, 10);

Box boxCollider;
Sphere sphereCollider(10,10);

Box boxViewDepth;
Box boxLightViewBox;

Cylinder rayModel(10,10,1.0f, 1.0f, 1.0f);
Model modelRayo1; 

// Models complex instances
Model modelRock;
Model modelAircraft;
Model modelEclipseChasis;
Model modelEclipseRearWheels;
Model modelEclipseFrontalWheels;
Model modelHeliChasis;
Model modelHeliHeli;
Model modelHeliHeliBack;
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

// Buzz
Model modelBuzzTorso;
Model modelBuzzHead;
Model modelBuzzLeftArm;
Model modelBuzzLeftForeArm;
Model modelBuzzLeftHand;

//Modelos de lamparas

Model modelLamp1;
Model modelLamp2;
Model modelLamp2Post;
Model modelLamp3;
Model modelLamp4;


// Modelos animados
// Mayow
Model mayowModelAnimate;
// Cowboy
Model cowboyModelAnimate;
// Guardian con lampara
Model guardianModelAnimate;
// Cybog
Model cyborgModelAnimate;
// Terrain model instance
//Terrain terrain(-1, -1, 200, 8, "../Textures/heightmap.png");
Terrain terrain(-1, -1, 200, 8, "../Textures/Terrain2024-1.png");



GLuint textureCespedID, textureWallID, textureWindowID, textureHighwayID, textureLandingPadID;
GLuint textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID;
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




bool isThirdCamera=true,changingCamera=false;

// Model matrix definitions
glm::mat4 modelMatrixEclipse = glm::mat4(1.0f);
glm::mat4 matrixModelRock = glm::mat4(1.0);
glm::mat4 modelMatrixHeli = glm::mat4(1.0f);
glm::mat4 modelMatrixLambo = glm::mat4(1.0);
glm::mat4 modelMatrixAircraft = glm::mat4(1.0);
glm::mat4 modelMatrixDart = glm::mat4(1.0f);
glm::mat4 modelMatrixBuzz = glm::mat4(1.0f);
glm::mat4 modelMatrixMayow = glm::mat4(1.0f);
glm::mat4 modelMatrixCowboy = glm::mat4(1.0f);
glm::mat4 modelMatrixGuardian = glm::mat4(1.0f);
glm::mat4 modelMatrixCyborg = glm::mat4(1.0f);
glm::mat4 modelMatrixRayo = glm::mat4(1.0f);



int esperar = 3; //6
int saltar = 1;
int correr = 2;
int golpe = 0; 
float golpe1 = 0.0;
int animationMayowIndex = 1;
int animationCyborgIndex = esperar;
//int animationMayowIndex = 1;
//int animationCyborgIndex = 5;

float rotDartHead = 0.0, rotDartLeftArm = 0.0, rotDartLeftHand = 0.0, rotDartRightArm = 0.0, rotDartRightHand = 0.0, rotDartLeftLeg = 0.0, rotDartRightLeg = 0.0;
float rotBuzzHead = 0.0, rotBuzzLeftarm = 0.0, rotBuzzLeftForeArm = 0.0, rotBuzzLeftHand = 0.0;
int modelSelected = 0;
bool enableCountSelected = true;

// Variables to animations keyframes
bool saveFrame = false, availableSave = true;
std::ofstream myfile;
std::string fileName = "";
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

// Joints interpolations Buzz
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
int maxNumPasosBuzz = 100;
int numPasosBuzz = 0;

// Var animate helicopter
float rotHelHelY = 0.0;
float rotHelHelBack = 0.0;



// Var animate lambo dor
int stateDoor = 0;
float dorRotCount = 0.0;

// lamps position
std::vector<glm::vec3> lamp1Position =  {
	glm :: vec3 ( -7.03 , 0 , -19.14 ) ,
	glm :: vec3 ( 24.41 , 0 , -34.57 ) ,
	glm :: vec3 ( -10.15 , 0 , -54.10 )
};

std :: vector <float> lamp1Orientation = {
    -17.0 , -82.67 , 23.7
} ;


std::vector<glm::vec3> lamp2Position =  {
	glm::vec3(-36.52, 0, -23.24),
	glm::vec3(-52.73,0,-3.90)
};

std :: vector <float> lamp2Orientation = {
	21.37,-65.0
} ;

std::vector<glm::vec3> lamp3Position =  {
	glm::vec3(10.00, 0, 15),
	glm::vec3(38.00,0,-23)
};

std :: vector <float> lamp3Orientation = {
	240,30
} ;

std::vector<glm::vec3> lamp4Position =  {
	glm::vec3(36, 0, -43.24),
	glm::vec3(56.00,0,-28)
};

std :: vector <float> lamp4Orientation = {
	21.37,-65.0
} ;
                                     
 


double deltaTime;
double currTime, lastTime;

// Jump variables
bool isJump = false;
bool isPunch = false;
float GRAVITY = 1.81;
double tmv = 0;
double tmv2 = 0;
double startTimeJump = 0;
double startTimePunch = 0;


float maxDistanceRay=10.0;//variable que contiene el tamano del rayo

// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;



// Variables animacion maquina de estados eclipse
const float avance = 0.2;
const float giroEclipse = 0.5f;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
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
	glfwSetScrollCallback(window, scrollCallback);
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

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation.vs", "../Shaders/multipleLights.fs");
	shaderTerrain.initialize("../Shaders/terrain.vs", "../Shaders/terrain.fs");

	
	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

	boxCollider.init();
	boxCollider.setShader(&shader);
	boxCollider.setColor(glm::vec4(1.0));

	sphereCollider.init();
	sphereCollider.setShader(&shader);
	sphereCollider.setColor(glm::vec4(1.0));


	boxCesped.init();
	boxCesped.setShader(&shaderMulLighting);

	boxWalls.init();
	boxWalls.setShader(&shaderMulLighting);

	boxHighway.init();
	boxHighway.setShader(&shaderMulLighting);

	boxLandingPad.init();
	boxLandingPad.setShader(&shaderMulLighting);

	esfera1.init();
	esfera1.setShader(&shaderMulLighting);

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
	modelHeliHeliBack.loadModel("../models/Helicopter/Mi_24_heli_rear.obj");
	modelHeliHeliBack.setShader(&shaderMulLighting);
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

	
	// Buzz
	modelBuzzTorso.loadModel("../models/buzz/buzzlightyTorso.obj");
	modelBuzzTorso.setShader(&shaderMulLighting);
	modelBuzzHead.loadModel("../models/buzz/buzzlightyHead.obj");
	modelBuzzHead.setShader(&shaderMulLighting);
	modelBuzzLeftArm.loadModel("../models/buzz/buzzlightyLeftArm.obj");
	modelBuzzLeftArm.setShader(&shaderMulLighting);
	modelBuzzLeftForeArm.loadModel("../models/buzz/buzzlightyLeftForearm.obj");
	modelBuzzLeftForeArm.setShader(&shaderMulLighting);
	modelBuzzLeftHand.loadModel("../models/buzz/buzzlightyLeftHand.obj");
	modelBuzzLeftHand.setShader(&shaderMulLighting);


	//Lamparas
	modelLamp1.loadModel("../models/Street-Lamp-Black/objLamp.obj");
	modelLamp1.setShader(&shaderMulLighting);

	modelLamp2.loadModel("../models/Street_Light/Lamp.obj");
	modelLamp2.setShader(&shaderMulLighting);
	modelLamp2Post.loadModel("../models/Street_Light/LampPost.obj");
	modelLamp2Post.setShader(&shaderMulLighting);

	modelLamp3.loadModel("../models/Luminarias/Poste1/Poste1.obj");
	modelLamp3.setShader(&shaderMulLighting);
	modelLamp4.loadModel("../models/Luminarias/Poste2/Poste2.obj");
	modelLamp4.setShader(&shaderMulLighting);



	// Mayow
	mayowModelAnimate.loadModel("../models/mayow/personaje2.fbx");
	mayowModelAnimate.setShader(&shaderMulLighting);
	
	// Cowboy
	cowboyModelAnimate.loadModel("../models/cowboy/Character Running.fbx");
	cowboyModelAnimate.setShader(&shaderMulLighting);

	// Guardian
	guardianModelAnimate.loadModel("../models/boblampclean/boblampclean.md5mesh");
	guardianModelAnimate.setShader(&shaderMulLighting);

	// Cyborg
	//cyborgModelAnimate.loadModel("../models/cyborg/cyborg.fbx");
	//cyborgModelAnimate.loadModel("../models/mona/caminando7.fbx");
	//cyborgModelAnimate.loadModel("../models/Luffy/esperar_correr_1.fbx");
	cyborgModelAnimate.loadModel("../models/Luffy/golpe_saltar_correr_esperar.fbx");
	cyborgModelAnimate.setShader(&shaderMulLighting);

	// Terreno
	terrain.init();
	terrain.setShader(&shaderTerrain);

	rayModel.init();
	rayModel.setShader(&shader);
	rayModel.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	
	

	if(isThirdCamera==true)
	{
		camera->setPosition(glm::vec3(0.0,0.0, 10.0));
		camera->setDistanceFromTarget(distanceFromTarget);
		camera->setSensitivity(1.0);
	} else
	{
		camera1->setPosition(camera->getPosition());
		
	}
	

		
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

	// Defininiendo texturas del mapa de mezclas
	// Definiendo la textura

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

	
	// Defininiendo texturas del mapa de mezclas
	// Definiendo la textura
	Texture textureR("../Textures/mud.png");
	textureR.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainRID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureR.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureR.getChannels() == 3 ? GL_RGB : GL_RGBA, textureR.getWidth(), textureR.getHeight(), 0,
		textureR.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureR.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureR.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureG("../Textures/grassFlowers.png");
	textureG.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainGID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureG.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureG.getChannels() == 3 ? GL_RGB : GL_RGBA, textureG.getWidth(), textureG.getHeight(), 0,
		textureG.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureG.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureG.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureB("../Textures/path.png");
	textureB.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainBID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureB.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureB.getChannels() == 3 ? GL_RGB : GL_RGBA, textureB.getWidth(), textureB.getHeight(), 0,
		textureB.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureB.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureB.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureBlendMap("../Textures/blendMap2.png");
	textureBlendMap.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainBlendMapID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureBlendMap.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureBlendMap.getChannels() == 3 ? GL_RGB : GL_RGBA, textureBlendMap.getWidth(), textureBlendMap.getHeight(), 0,
		textureBlendMap.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureBlendMap.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureBlendMap.freeImage(); // Liberamos memoria


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
	shaderTerrain.destroy();

	boxCollider.destroy();
	sphereCollider.destroy();

	rayModel.destroy();

	

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCesped.destroy();
	boxWalls.destroy();
	boxHighway.destroy();
	boxLandingPad.destroy();
	esfera1.destroy();

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
	modelHeliHeliBack.destroy();
	modelLambo.destroy();
	modelLamboFrontLeftWheel.destroy();
	modelLamboFrontRightWheel.destroy();
	modelLamboLeftDor.destroy();
	modelLamboRearLeftWheel.destroy();
	modelLamboRearRightWheel.destroy();
	modelLamboRightDor.destroy();
	modelRock.destroy();
	modelBuzzHead.destroy();
	modelBuzzLeftArm.destroy();
	modelBuzzLeftForeArm.destroy();
	modelBuzzLeftHand.destroy();
	modelBuzzTorso.destroy();
	mayowModelAnimate.destroy();
	cowboyModelAnimate.destroy();
	guardianModelAnimate.destroy();
	cyborgModelAnimate.destroy();

	//Lamparas
	modelLamp1.destroy();
	modelLamp2.destroy();
	modelLamp2Post.destroy();
	modelLamp3.destroy();
	modelLamp4.destroy();

	// Terrains objects Delete
	terrain.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureCespedID);
	glDeleteTextures(1, &textureWallID);
	glDeleteTextures(1, &textureWindowID);
	glDeleteTextures(1, &textureHighwayID);
	glDeleteTextures(1, &textureLandingPadID);
	glDeleteTextures(1, &textureTerrainBID);
	glDeleteTextures(1, &textureTerrainGID);
	glDeleteTextures(1, &textureTerrainRID);
	glDeleteTextures(1, &textureTerrainBlendMapID);

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

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	distanceFromTarget -= yoffset;
	camera->setDistanceFromTarget(distanceFromTarget);
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

	
	if (glfwJoystickPresent(GLFW_JOYSTICK_1))
	{
		std::cout << "está conectado el joystick" << std::endl;
		int axisCount = 0, buttonCount = 0;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axisCount);
		const unsigned char * buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
		
		std::cout << "Joystick Name " << glfwGetJoystickName(GLFW_JOYSTICK_1) << "." << std::endl;
		std::cout << "Axis count: " << axisCount << "." << std::endl;
		std::cout << "Axis LX: " << axes[0] << std::endl;
		std::cout << "Axis LY: " << axes[1] << std::endl;
		std::cout << "Axis RX: " << axes[2] << std::endl;
		std::cout << "Axis RY: " << axes[3] << std::endl;
		std::cout << "Axis LT: " << axes[4] << std::endl;
		std::cout << "Axis RT: " << axes[5] << std::endl;
		

		for (int i = 0; i < buttonCount; i++)
		{
			std::cout << "Button "<< i <<": " << (int)buttons[i] << std::endl;

		}
		
		if (fabs(axes[0]) >= 0.1f || fabs(axes[1]) >=0.1f )
		{
			modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(-axes[0],0,axes[1]));
			animationMayowIndex = 0;
		}
		if (fabs(axes[2]) >= 0.1f)
		{
			modelMatrixMayow = glm::rotate(modelMatrixMayow, axes[2]*0.1f, glm::vec3(0, -1, 0));	
			animationMayowIndex = 1;
		}
		if (fabs(axes[4]) >= -1.0f || fabs(axes[5]>=-1.0f) )
		{
			camera->mouseMoveCamera( (axes[4]-1.0f) - (axes[5]-1.0f), 0, deltaTime);
		}
	}



if (isThirdCamera)
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			camera->mouseMoveCamera(offsetX, offsetY, deltaTime);
		if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			camera->mouseMoveCamera(0.0, offsetY, deltaTime);
		offsetX = 0;
		offsetY = 0;
	}
	else
	{
		
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera1->moveFrontCamera(true, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera1->moveFrontCamera(false, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera1->moveRightCamera(false, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera1->moveRightCamera(true, deltaTime);
		if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)== GLFW_PRESS)
			camera1->mouseMoveCamera(offsetX, offsetY, deltaTime);
		
	}

	
//cambio de CAMARA 

if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)==GLFW_PRESS )
	{
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		{
			changingCamera = true;
			camera1->setPosition(glm::vec3(camera->getPosition()));
		}
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE)
		{
			if (changingCamera)
			{
				isThirdCamera = !isThirdCamera;
				std::cout << "Changed Camera" << std::endl;
			}
			changingCamera = false;
		}
		
	}

	
	//seleccion de modelo 

	if(modelSelected == 5){
		axis = glm::axis(glm::quat_cast(modelMatrixCyborg));
		angleTarget = glm::angle(glm::quat_cast(modelMatrixCyborg));
		target = modelMatrixCyborg[3];
	}
	
	else{
		axis = glm::axis(glm::quat_cast(modelMatrixMayow));
		angleTarget = glm::angle(glm::quat_cast(modelMatrixMayow));
		target = modelMatrixMayow[3];
	}

	if(std::isnan(angleTarget))
		angleTarget = 0.0;
	if(axis.y < 0)
		angleTarget = -angleTarget;
	if(modelSelected == 1)
		angleTarget -= glm::radians(90.0f);

		if (isThirdCamera)
		{
			camera->setAngleTarget(angleTarget);
			camera->setCameraTarget(target);
			camera->updateCamera();
			view = camera->getViewMatrix();
		}
		else
		{
			
			view = camera1->getViewMatrix();
		}
	



		// Seleccionar modelo
	if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS){
		enableCountSelected = false;
		modelSelected++;
		if(modelSelected > 6)
			modelSelected = 0;
		if(modelSelected == 1)
			fileName = "../animaciones/animation_dart_joints.txt";
		if (modelSelected == 2)
			fileName = "../animaciones/animation_dart.txt";
		if(modelSelected == 3)
			fileName = "../animaciones/animation_buzz_joints.txt";
		if (modelSelected == 4)
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
	
	// Movimientos de buzz
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotBuzzHead += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotBuzzHead -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotBuzzLeftarm += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotBuzzLeftarm -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotBuzzLeftForeArm += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotBuzzLeftForeArm -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotBuzzLeftHand += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotBuzzLeftHand -= 0.02;
	if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		modelMatrixBuzz = glm::rotate(modelMatrixBuzz, 0.02f, glm::vec3(0, 1, 0));
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		modelMatrixBuzz = glm::rotate(modelMatrixBuzz, -0.02f, glm::vec3(0, 1, 0));
	if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		modelMatrixBuzz = glm::translate(modelMatrixBuzz, glm::vec3(0.0, 0.0, 0.02));
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		modelMatrixBuzz = glm::translate(modelMatrixBuzz, glm::vec3(0.0, 0.0, -0.02));

	// Controles de mayow
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		modelMatrixMayow = glm::rotate(modelMatrixMayow, 0.04f, glm::vec3(0, 1, 0));
		animationMayowIndex = 0;
	} else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
		modelMatrixMayow = glm::rotate(modelMatrixMayow, -0.04f, glm::vec3(0, 1, 0));
		animationMayowIndex = 0;
	}
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0.0, 0.0, 0.3));
		animationMayowIndex = 0;
	}
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0.0, 0.0, -0.3));
		animationMayowIndex = 0;
	}

	if (modelSelected == 5 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		modelMatrixCyborg = glm::rotate(modelMatrixCyborg, 0.04f, glm::vec3(0, 1, 0));
		animationCyborgIndex = correr;
		
	} else if (modelSelected == 5 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
		modelMatrixCyborg = glm::rotate(modelMatrixCyborg, -0.04f, glm::vec3(0, 1, 0));
		animationCyborgIndex = correr;
	}
	if (modelSelected == 5 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		modelMatrixCyborg = glm::translate(modelMatrixCyborg, glm::vec3(0.0, 0.0, 0.2));
		animationCyborgIndex = correr;
	}
	else if (modelSelected == 5 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		modelMatrixCyborg = glm::translate(modelMatrixCyborg, glm::vec3(0.0, 0.0, -0.2));
		animationCyborgIndex = correr;
	}

	if (modelSelected == 5 && glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS){
		//animationCyborgIndex = golpe;
		isPunch= true;	
		golpe1 = 20.0f;
	}
	else if (modelSelected == 5 && glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE){
		golpe1 = 0.0f;
	}

	if(isPunch==true){
		animationCyborgIndex = golpe;
		startTimePunch += 1;
		if(startTimePunch > 45){
			isPunch= false;
			startTimePunch=0;
		}
	}


	bool keySpaceStatus = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
		if(!isJump && keySpaceStatus){
			isJump = true;
			startTimeJump = currTime;
			tmv = 0;
			
		}
		if(isJump==true){
			animationCyborgIndex = saltar;
		}
		
	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;



	modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(27.5, 0, 30.0));
	modelMatrixEclipse = glm::rotate(modelMatrixEclipse, glm::radians(180.0f), glm::vec3(0, 1, 0));
	int state = 0;
	float advanceCount = 0.0;
	float rotCount = 0.0;
	float rotWheelsX = 0.0;
	float rotWheelsY = 0.0;
	int numberAdvance = 0;
	int maxAdvance = 0.0;

	int state2 = 0;
	float advanceCount2 = 0.0;
	float rotCount2 = 0.0;
	float rotWheelsX2 = 0.0;
	float rotWheelsY2 = 0.0;
	int numberAdvance2 = 0; 
	int maxAdvance2 = 0.0;
	int maxAdvance3 = 0.0;

	matrixModelRock = glm::translate(matrixModelRock, glm::vec3(-3.0, 0.0, 2.0));

	modelMatrixRayo = glm::translate(modelMatrixRayo, glm::vec3(0.0, 2.0, -5.0));
	modelMatrixRayo = glm::scale(modelMatrixRayo, glm::vec3(0.02f,2.5f,0.02f));

	
	modelMatrixHeli = glm::translate(modelMatrixHeli, glm::vec3(4.0, 10.0, -3.0));

	modelMatrixAircraft = glm::translate(modelMatrixAircraft, glm::vec3(10.0, 2.0, -17.5));

	modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(23.0, 1.0, 0.0));

	modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(3.0, 0.0, 20.0));

	modelMatrixBuzz = glm::translate(modelMatrixBuzz, glm::vec3(15.0, 0.0, -10.0));

	modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(13.0f, 0.05f, -5.0f));
	modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(-90.0f), glm::vec3(0, 1, 0));

	modelMatrixCowboy = glm::translate(modelMatrixCowboy, glm::vec3(13.0, 0.05, 0.0));

	modelMatrixGuardian = glm::translate(modelMatrixGuardian, glm::vec3(15, 0.05, 0.0));
	modelMatrixGuardian = glm::rotate(modelMatrixGuardian, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));

	modelMatrixCyborg = glm::translate(modelMatrixCyborg, glm::vec3(4.0f, 0.7, -1.0f));

	// Variables to interpolation key frames
	fileName = "../animaciones/animation_dart_joints.txt";
	keyFramesDartJoints = getKeyRotFrames(fileName);
	keyFramesDart = getKeyFrames("../animaciones/animation_dart.txt");
	keyFramesBuzzJoints = getKeyRotFrames("../animaciones/animation_buzz_joints.txt");
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

		std::map<std::string, bool> collisionDetection;

		// Variables donde se guardan las matrices de cada articulacion por 1 frame
		std::vector<float> matrixDartJoints;
		std::vector<glm::mat4> matrixDart;
		std::vector<float> matrixBuzzJoints;
		std::vector<glm::mat4> matrixBuzz;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) screenWidth / (float) screenHeight, 0.01f, 100.0f);


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
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false,
				glm::value_ptr(view));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		/*if(isThirdCamera==true)
		{
			shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		} else
		{
			shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera1->getPosition()));
		}*/

		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));//0.7
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));//0.9
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));


		/*if(isThirdCamera==true)
		{
			shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		} else{
			shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera1->getPosition()));
		}*/

		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/
		
		shaderMulLighting.setInt("spotLightCount", 1);
		shaderTerrain.setInt("spotLightCount", 1);
		glm::vec3 spotPosition = glm:: vec3(modelMatrixHeli *  glm::vec4(0.0, 0.19, 1.693, 1.0));

		shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.45, 0.3, 0.01)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.6, 0.4, 0.01)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.7, 0.5, 0.01)));
		shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotPosition));
		shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0.0, -1.0, 0.0)));
		shaderMulLighting.setFloat("spotLights[0].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[0].linear", 0.09);
		shaderMulLighting.setFloat("spotLights[0].quadratic", 0.02);
		shaderMulLighting.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5f)));
		shaderMulLighting.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0f)));

		shaderTerrain.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.45, 0.3, 0.01)));
		shaderTerrain.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.6, 0.4, 0.01)));
		shaderTerrain.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.7, 0.5, 0.01)));
		shaderTerrain.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotPosition));
		shaderTerrain.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0.0, -1.0, 0.0)));
		shaderTerrain.setFloat("spotLights[0].constant", 1.0);
		shaderTerrain.setFloat("spotLights[0].linear", 0.09);
		shaderTerrain.setFloat("spotLights[0].quadratic", 0.02);
		shaderTerrain.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5f)));
		shaderTerrain.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0f)));

		/*
		LUCES LAMBO
		*/
		shaderMulLighting.setInt("spotLightCount", 2);
		shaderTerrain.setInt("spotLightCount", 2);
		glm::vec3 spotPositionLambo1 = glm:: vec3(modelMatrixLambo *  glm::vec4(0.76, 4.66, 5.2, 1.0));
		
		shaderMulLighting.setVectorFloat3("spotLights[1].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
		shaderMulLighting.setVectorFloat3("spotLights[1].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
		shaderMulLighting.setVectorFloat3("spotLights[1].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.03)));
		shaderMulLighting.setVectorFloat3("spotLights[1].position", glm::value_ptr(spotPositionLambo1));
		shaderMulLighting.setVectorFloat3("spotLights[1].direction", glm::value_ptr(spotPositionLambo1*glm::vec3(0.0,-1.0,0.0)));
		shaderMulLighting.setFloat("spotLights[1].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[1].linear", 0.09);
		shaderMulLighting.setFloat("spotLights[1].quadratic", 0.02);
		shaderMulLighting.setFloat("spotLights[1].cutOff", cos(glm::radians(20.0f)));
		shaderMulLighting.setFloat("spotLights[1].outerCutOff", cos(glm::radians(25.0f)));

		shaderTerrain.setVectorFloat3("spotLights[1].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
		shaderTerrain.setVectorFloat3("spotLights[1].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
		shaderTerrain.setVectorFloat3("spotLights[1].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.03)));
		shaderTerrain.setVectorFloat3("spotLights[1].position", glm::value_ptr(spotPositionLambo1));
		shaderTerrain.setVectorFloat3("spotLights[1].direction", glm::value_ptr(spotPositionLambo1*glm::vec3(0.0,-1.0,0.0)));
		shaderTerrain.setFloat("spotLights[1].constant", 1.0);
		shaderTerrain.setFloat("spotLights[1].linear", 0.09);
		shaderTerrain.setFloat("spotLights[1].quadratic", 0.02);
		shaderTerrain.setFloat("spotLights[1].cutOff", cos(glm::radians(20.0f)));
		shaderTerrain.setFloat("spotLights[1].outerCutOff", cos(glm::radians(25.0f)));

		shaderMulLighting.setInt("spotLightCount", 3);
		shaderTerrain.setInt("spotLightCount", 3);
		glm::vec3 spotPositionLambo2 = glm:: vec3(modelMatrixLambo *  glm::vec4(-0.76,4.66, 5.2, 1.0));
		
		shaderMulLighting.setVectorFloat3("spotLights[2].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
		shaderMulLighting.setVectorFloat3("spotLights[2].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
		shaderMulLighting.setVectorFloat3("spotLights[2].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.03)));
		shaderMulLighting.setVectorFloat3("spotLights[2].position", glm::value_ptr(spotPositionLambo2));
		shaderMulLighting.setVectorFloat3("spotLights[2].direction", glm::value_ptr(spotPositionLambo2*glm::vec3(0.0,-1.0,0.0)));
		shaderMulLighting.setFloat("spotLights[2].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[2].linear", 0.09);
		shaderMulLighting.setFloat("spotLights[2].quadratic", 0.02);
		shaderMulLighting.setFloat("spotLights[2].cutOff", cos(glm::radians(20.0f)));
		shaderMulLighting.setFloat("spotLights[2].outerCutOff", cos(glm::radians(25.0f)));

		shaderTerrain.setVectorFloat3("spotLights[2].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
		shaderTerrain.setVectorFloat3("spotLights[2].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
		shaderTerrain.setVectorFloat3("spotLights[2].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.03)));
		shaderTerrain.setVectorFloat3("spotLights[2].position", glm::value_ptr(spotPositionLambo2));
		shaderTerrain.setVectorFloat3("spotLights[2].direction", glm::value_ptr(spotPositionLambo2*glm::vec3(0.0,-1.0,0.0)));
		shaderTerrain.setFloat("spotLights[2].constant", 1.0);
		shaderTerrain.setFloat("spotLights[2].linear", 0.09);
		shaderTerrain.setFloat("spotLights[2].quadratic", 0.02);
		shaderTerrain.setFloat("spotLights[2].cutOff", cos(glm::radians(20.0f)));
		shaderTerrain.setFloat("spotLights[2].outerCutOff", cos(glm::radians(25.0f)));

		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		shaderMulLighting.setInt("pointLightCount", lamp1Position.size()+ lamp2Position.size()+lamp3Position.size()+lamp4Position.size());
		shaderTerrain.setInt("pointLightCount", lamp1Position.size()+ lamp2Position.size()+lamp3Position.size()+lamp4Position.size());
		
		for (int i = 0; i < lamp1Position.size(); i++){
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp1Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp1Orientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(0.5, 0.5, 0.5));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0, 10.3585, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.01);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.02);
		}

		
		for (int i = 0; i < lamp2Position.size(); i++){
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp2Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp2Orientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.0, 1.0, 1.0));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.759521, 5.00174, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].quadratic", 0.01);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].quadratic", 0.02);
		}
		//shaderMulLighting.setInt("pointLightCount", 3);
		//shaderTerrain.setInt("pointLightCount", 3);
		for (int i = 0; i < lamp3Position.size(); i++){
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp3Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp3Orientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.0, 1.0, 1.0));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.759521, 5.00174, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].quadratic", 0.01);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].quadratic", 0.02);
		}
		//shaderMulLighting.setInt("pointLightCount", 4);
		//shaderTerrain.setInt("pointLightCount", 4);
		for (int i = 0; i < lamp4Position.size(); i++){
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp4Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp4Orientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.0, 1.0, 1.0));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.759521, 5.00174, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].quadratic", 0.01);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].quadratic", 0.02);
		}

		/*******************************************
		 * Terrain Cesped
		 *******************************************/
		// Se activa la textura del agua

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureCespedID);
		shaderTerrain.setInt("backgroundTexture", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
		shaderTerrain.setInt("textureR", 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
		shaderTerrain.setInt("textureG", 2);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
		shaderTerrain.setInt("textureB", 3);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
		shaderTerrain.setInt("textureBlendMap", 4);
		shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(80, 80)));
		terrain.setPosition(glm::vec3(100, 0, 100));
		terrain.render();
		shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		/*******************************************
		 * Custom objects obj
		 *******************************************/
		//Rock render
		matrixModelRock[3][1] = terrain.getHeightTerrain(matrixModelRock[3][0], matrixModelRock[3][2]);
		modelRock.render(matrixModelRock);


		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		// Render for the aircraft model
		modelMatrixAircraft[3][1] = terrain.getHeightTerrain(modelMatrixAircraft[3][0], modelMatrixAircraft[3][2]) + 2.0;
		modelAircraft.render(modelMatrixAircraft);

		// Render for the eclipse car
		modelMatrixEclipse[3][1] = terrain.getHeightTerrain(modelMatrixEclipse[3][0], modelMatrixEclipse[3][2]);
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
		glm::mat4 modelMatrixHeliHeliBack = glm::mat4(modelMatrixHeliChasis);
		modelMatrixHeliHeliBack = glm::translate(modelMatrixHeliHeliBack, glm::vec3(0.400524, 2.0928, -5.64124));
		modelMatrixHeliHeliBack = glm::rotate(modelMatrixHeliHeliBack, rotHelHelBack, glm::vec3(1.0, 0.0, 0.0));
		modelMatrixHeliHeliBack = glm::translate(modelMatrixHeliHeliBack, glm::vec3(-0.400524, -2.0928, 5.64124));
		modelHeliHeliBack.render(modelMatrixHeliHeliBack);



// Lambo car
		glDisable(GL_CULL_FACE);
		glm::mat4 modelMatrixLamboChasis = glm::mat4(modelMatrixLambo);
		modelMatrixLamboChasis = glm::scale(modelMatrixLamboChasis, glm::vec3(1.3, 1.3, 1.3));
		modelMatrixLamboChasis[3][1]=
		terrain.getHeightTerrain(modelMatrixLamboChasis[3][0],modelMatrixLamboChasis[3][2]);//hacer la interpolacion en base a la posicion x y z del modelo
		//cambio con angulo de inclinacion
		glm::vec3 ejeyLamboChasis=glm::normalize(
			terrain.getNormalTerrain(modelMatrixLamboChasis[3][0],modelMatrixLamboChasis[3][2]));
		glm::vec3 ejezLamboChasis=glm::normalize(modelMatrixLamboChasis[2]);
		glm::vec3 ejexLamboChasis=glm::normalize(glm::cross(ejeyLamboChasis,ejezLamboChasis));
		 ejezLamboChasis=glm::normalize(glm::cross(ejexLamboChasis,ejeyLamboChasis));
		modelMatrixLamboChasis[0]=glm::vec4(ejexLamboChasis,0.0f);
		modelMatrixLamboChasis[1]=glm::vec4(ejeyLamboChasis,0.0f);
		modelMatrixLamboChasis[2]=glm::vec4(ejezLamboChasis,0.0f);
		modelLambo.render(modelMatrixLamboChasis);
		
		glActiveTexture(GL_TEXTURE0);
		glm::mat4 modelMatrixLamboLeftDor = glm::mat4(modelMatrixLamboChasis);
		modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(1.08676, 0.707316, 0.982601));
		modelMatrixLamboLeftDor = glm::rotate(modelMatrixLamboLeftDor, glm::radians(dorRotCount), glm::vec3(1.0, 0, 0));
		modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(-1.08676, -0.707316, -0.982601));
		modelLamboLeftDor.render(modelMatrixLamboLeftDor);

		glm::mat4 modelMatrixLamboRightDor = glm::mat4(modelMatrixLamboChasis);
		modelMatrixLamboRightDor = glm::translate(modelMatrixLamboRightDor, glm::vec3(-1.076, 0.6992, 0.9766));
		modelMatrixLamboRightDor = glm::rotate(modelMatrixLamboRightDor, glm::radians(dorRotCount), glm::vec3(1.0, 0, 0));
		modelMatrixLamboRightDor = glm::translate(modelMatrixLamboRightDor, glm::vec3(1.076, -0.6992, -0.9766));
		modelLamboRightDor.render(modelMatrixLamboRightDor);


		glm::mat4 modelMatrixLamboFrontLeftWheel = glm::mat4(modelMatrixLamboChasis);
		modelMatrixLamboFrontLeftWheel[3][1]=
		terrain.getHeightTerrain(modelMatrixLamboFrontLeftWheel[3][0],modelMatrixLamboFrontLeftWheel[3][2]);//hacer la interpolacion en base a la posicion x y z del modelo
		//cambio con angulo de inclinacion
		glm::vec3 ejeyLamboFrontLeftWheel=glm::normalize(
			terrain.getNormalTerrain(modelMatrixLamboFrontLeftWheel[3][0],modelMatrixLamboFrontLeftWheel[3][2]));
		glm::vec3 ejezLamboFrontLeftWheel=glm::normalize(modelMatrixLamboFrontLeftWheel[2]);
		glm::vec3 ejexLamboFrontLeftWheel=glm::normalize(glm::cross(ejeyLamboFrontLeftWheel,ejezLamboFrontLeftWheel));
		 ejezLamboFrontLeftWheel=glm::normalize(glm::cross(ejexLamboFrontLeftWheel,ejeyLamboFrontLeftWheel));
		modelMatrixLamboFrontLeftWheel[0]=glm::vec4(ejexLamboFrontLeftWheel,0.0f);
		modelMatrixLamboFrontLeftWheel[1]=glm::vec4(ejeyLamboFrontLeftWheel,0.0f);
		modelMatrixLamboFrontLeftWheel[2]=glm::vec4(ejezLamboFrontLeftWheel,0.0f);

		modelMatrixLamboFrontLeftWheel = glm::translate(modelMatrixLamboFrontLeftWheel, glm::vec3(0.0f,0.37f,1.4f));
		modelMatrixLamboFrontLeftWheel = glm::rotate(modelMatrixLamboFrontLeftWheel, -rotWheelsY2, glm::vec3(0, 1, 0)); //giro en Y contrario al del Eclipse
		modelMatrixLamboFrontLeftWheel = glm::rotate(modelMatrixLamboFrontLeftWheel, rotWheelsX2, glm::vec3(1, 0, 0));
		modelMatrixLamboFrontLeftWheel = glm::translate(modelMatrixLamboFrontLeftWheel, glm::vec3(0.0, -0.37f, -1.4f));
		modelLamboFrontLeftWheel.render(modelMatrixLamboFrontLeftWheel);
		modelLamboFrontRightWheel.render(modelMatrixLamboFrontLeftWheel);
		
		glm::mat4 modelMatrixLamboRearLeftWheel = glm::mat4(modelMatrixLamboChasis);
		modelMatrixLamboRearLeftWheel[3][1]=
		terrain.getHeightTerrain(modelMatrixLamboRearLeftWheel[3][0],modelMatrixLamboRearLeftWheel[3][2]);//hacer la interpolacion en base a la posicion x y z del modelo
		//cambio con angulo de inclinacion
		glm::vec3 ejeyLamboRearLeftWheel=glm::normalize(
			terrain.getNormalTerrain(modelMatrixLamboRearLeftWheel[3][0],modelMatrixLamboRearLeftWheel[3][2]));
		glm::vec3 ejezLamboRearLeftWheel=glm::normalize(modelMatrixLamboRearLeftWheel[2]);
		glm::vec3 ejexLamboRearLeftWheel=glm::normalize(glm::cross(ejeyLamboRearLeftWheel,ejezLamboRearLeftWheel));
		 ejezLamboRearLeftWheel=glm::normalize(glm::cross(ejexLamboRearLeftWheel,ejeyLamboRearLeftWheel));
		modelMatrixLamboRearLeftWheel[0]=glm::vec4(ejexLamboRearLeftWheel,0.0f);
		modelMatrixLamboRearLeftWheel[1]=glm::vec4(ejeyLamboRearLeftWheel,0.0f);
		modelMatrixLamboRearLeftWheel[2]=glm::vec4(ejezLamboRearLeftWheel,0.0f);


		modelMatrixLamboRearLeftWheel = glm::translate(modelMatrixLamboRearLeftWheel, glm::vec3(0.0f,0.397f,-1.598f));
		modelMatrixLamboRearLeftWheel = glm::rotate(modelMatrixLamboRearLeftWheel, rotWheelsX2, glm::vec3(1, 0, 0));
		modelMatrixLamboRearLeftWheel = glm::translate(modelMatrixLamboRearLeftWheel, glm::vec3(0.0, -0.397f, 1.598f));
		modelLamboRearLeftWheel.render(modelMatrixLamboRearLeftWheel);
		modelLamboRearRightWheel.render(modelMatrixLamboRearLeftWheel);
		glEnable(GL_CULL_FACE);

		//Render lamp1
		for(int i=0; i<lamp1Position.size(); i++){
			lamp1Position[i].y = terrain.getHeightTerrain(lamp1Position[i].x,lamp1Position[i].z);
			modelLamp1.setPosition(lamp1Position[i]);
			modelLamp1.setScale(glm::vec3(0.5));
			modelLamp1.setOrientation(glm::vec3(0, lamp1Orientation[i], 0));
			modelLamp1.render();
		}
		//Render lamp2
		for(int i=0; i<lamp2Position.size(); i++){
			lamp2Position[i].y = terrain.getHeightTerrain(lamp2Position[i].x,lamp2Position[i].z);
			modelLamp2.setPosition(lamp2Position[i]);
			modelLamp2.setScale(glm::vec3(1.0));
			modelLamp2.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
			modelLamp2.render();
			modelLamp2Post.setPosition(lamp2Position[i]);
			modelLamp2Post.setScale(glm::vec3(1.0));
			modelLamp2Post.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
			modelLamp2Post.render();
		}

		for(int i=0; i<lamp3Position.size(); i++){
			lamp3Position[i].y = terrain.getHeightTerrain(lamp3Position[i].x,lamp3Position[i].z);
			modelLamp3.setPosition(lamp3Position[i]);
			modelLamp3.setScale(glm::vec3(0.05));
			modelLamp3.setOrientation(glm::vec3(0, lamp3Orientation[i], 0));
			modelLamp3.render();
			
		}

		for(int i=0; i<lamp4Position.size(); i++){
			lamp4Position[i].y = terrain.getHeightTerrain(lamp4Position[i].x,lamp4Position[i].z);
			modelLamp4.setPosition(lamp4Position[i]);
			modelLamp4.setScale(glm::vec3(0.05));
			modelLamp4.setOrientation(glm::vec3(0, lamp4Orientation[i], 0));
			modelLamp4.render();
			
		}

		// Dart lego
		// Se deshabilita el cull faces IMPORTANTE para la capa
		glDisable(GL_CULL_FACE);
		modelMatrixDart[3][1] = terrain.getHeightTerrain(modelMatrixDart[3][0], modelMatrixDart[3][2]);
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

		
		glm::mat4 modelMatrixTorso = glm::mat4(modelMatrixBuzz);
		modelMatrixTorso = glm::scale(modelMatrixTorso, glm::vec3(3.0));
		modelBuzzTorso.render(modelMatrixTorso);
		glm::mat4 modelMatrizHead = glm::mat4(modelMatrixTorso);
		modelMatrizHead = glm::translate(modelMatrizHead, glm::vec3(0, 0, -0.017506));
		modelMatrizHead = glm::rotate(modelMatrizHead, rotBuzzHead, glm::vec3(0, 1, 0));
		modelMatrizHead = glm::translate(modelMatrizHead, glm::vec3(0, 0, 0.017506));
		modelBuzzHead.render(modelMatrizHead);

		glm::mat4 modelMatrixLeftArm = glm::mat4(modelMatrixTorso);
		modelMatrixLeftArm = glm::translate(modelMatrixLeftArm, glm::vec3(0.179974, 0.577592, -0.022103));
		modelMatrixLeftArm = glm::rotate(modelMatrixLeftArm, glm::radians(-65.0f), glm::vec3(0, 0, 1));
		modelMatrixLeftArm = glm::rotate(modelMatrixLeftArm, rotBuzzLeftarm, glm::vec3(0, 1, 0));
		modelMatrixLeftArm = glm::translate(modelMatrixLeftArm, glm::vec3(-0.179974, -0.577592, 0.022103));
		modelBuzzLeftArm.render(modelMatrixLeftArm);

		glm::mat4 modelMatrixLeftForeArm = glm::mat4(modelMatrixLeftArm);
		modelMatrixLeftForeArm = glm::translate(modelMatrixLeftForeArm, glm::vec3(0.298368, 0.583773, 0.008465));
		modelMatrixLeftForeArm = glm::rotate(modelMatrixLeftForeArm, rotBuzzLeftForeArm, glm::vec3(0, 1, 0));
		modelMatrixLeftForeArm = glm::translate(modelMatrixLeftForeArm, glm::vec3(-0.298368, -0.583773, -0.008465));
		modelBuzzLeftForeArm.render(modelMatrixLeftForeArm);

		glm::mat4 modelMatrixLeftHand = glm::mat4(modelMatrixLeftForeArm);
		modelMatrixLeftHand = glm::translate(modelMatrixLeftHand, glm::vec3(0.416066, 0.587046, 0.076258));
		modelMatrixLeftHand = glm::rotate(modelMatrixLeftHand, glm::radians(45.0f), glm::vec3(0, 1, 0));
		modelMatrixLeftHand = glm::rotate(modelMatrixLeftHand, rotBuzzLeftHand, glm::vec3(1, 0, 0));
		modelMatrixLeftHand = glm::rotate(modelMatrixLeftHand, glm::radians(-45.0f), glm::vec3(0, 1, 0));
		modelMatrixLeftHand = glm::translate(modelMatrixLeftHand, glm::vec3(-0.416066, -0.587046, -0.076258));
		modelBuzzLeftHand.render(modelMatrixLeftHand);

		/*****************************************
		 * Objetos animados por huesos
		 * **************************************/
		//salto de Mayow
		if(modelSelected==0){
			modelMatrixMayow[3][1] = -GRAVITY * tmv * tmv + 3.5 * tmv + terrain.getHeightTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2]);
			tmv = currTime - startTimeJump;
			if(modelMatrixMayow[3][1] < terrain.getHeightTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2])){
				isJump = false;
				modelMatrixMayow[3][1] = terrain.getHeightTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2]);
			}
		}
		

		glm::vec3 ejey = glm::normalize(terrain.getNormalTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2]));
		glm::vec3 ejex = glm::vec3(modelMatrixMayow[0]);
		glm::vec3 ejez = glm::normalize(glm::cross(ejex, ejey));
		ejex = glm::normalize(glm::cross(ejey, ejez));
		modelMatrixMayow[0] = glm::vec4(ejex, 0.0);
		modelMatrixMayow[1] = glm::vec4(ejey, 0.0);
		modelMatrixMayow[2] = glm::vec4(ejez, 0.0);
		//modelMatrixMayow[3][1] = terrain.getHeightTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2]);
		glm::mat4 modelMatrixMayowBody = glm::mat4(modelMatrixMayow);
		modelMatrixMayowBody = glm::scale(modelMatrixMayowBody, glm::vec3(0.021f));
		mayowModelAnimate.setAnimationIndex(animationMayowIndex);
		mayowModelAnimate.render(modelMatrixMayowBody);
		animationMayowIndex = 1;

		modelMatrixCowboy[3][1] = terrain.getHeightTerrain(modelMatrixCowboy[3][0], modelMatrixCowboy[3][2]);
		glm::mat4 modelMatrixCowboyBody = glm::mat4(modelMatrixCowboy);
		modelMatrixCowboyBody = glm::scale(modelMatrixCowboyBody, glm::vec3(0.0021f));
		cowboyModelAnimate.render(modelMatrixCowboyBody);

		modelMatrixGuardian[3][1] = terrain.getHeightTerrain(modelMatrixGuardian[3][0], modelMatrixGuardian[3][2]);
		glm::mat4 modelMatrixGuardianBody = glm::mat4(modelMatrixGuardian);
		modelMatrixGuardianBody = glm::scale(modelMatrixGuardianBody, glm::vec3(0.04f));
		guardianModelAnimate.render(modelMatrixGuardianBody);

// esto controla el muneco anaranjado que corre y se detiene
		if(modelSelected==5){
			modelMatrixCyborg[3][1] = -GRAVITY * tmv * tmv + 3.5 * tmv + terrain.getHeightTerrain(modelMatrixCyborg[3][0], modelMatrixCyborg[3][2]);
			tmv = currTime - startTimeJump;
			if(modelMatrixCyborg[3][1] < terrain.getHeightTerrain(modelMatrixCyborg[3][0], modelMatrixCyborg[3][2])){
				isJump = false;
				modelMatrixCyborg[3][1] = terrain.getHeightTerrain(modelMatrixCyborg[3][0], modelMatrixCyborg[3][2]);
			}
		}
		

		glm::vec3 ejeyCyborg = glm::normalize(terrain.getNormalTerrain(modelMatrixCyborg[3][0], modelMatrixCyborg[3][2]));
		glm::vec3 ejexCyborg = glm::vec3(modelMatrixCyborg[0]);
		glm::vec3 ejezCyborg = glm::normalize(glm::cross(ejexCyborg, ejeyCyborg));
		ejex = glm::normalize(glm::cross(ejeyCyborg, ejezCyborg));
		modelMatrixCyborg[0] = glm::vec4(ejexCyborg, 0.0);
		modelMatrixCyborg[1] = glm::vec4(ejeyCyborg, 0.0);
		modelMatrixCyborg[2] = glm::vec4(ejezCyborg, 0.0);

		//modelMatrixCyborg[3][1] = terrain.getHeightTerrain(modelMatrixCyborg[3][0], modelMatrixCyborg[3][2]);
		glm::mat4 modelMatrixCyborgBody = glm::mat4(modelMatrixCyborg);
		modelMatrixCyborgBody = glm::scale(modelMatrixCyborgBody, glm::vec3(0.009f));
		cyborgModelAnimate.setAnimationIndex(animationCyborgIndex);
		cyborgModelAnimate.render(modelMatrixCyborgBody);
		animationCyborgIndex = esperar;

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

/*******************************************
		 * Creacion de colliders
		 * IMPORTANT do this before interpolations
		 *******************************************/
		// Collider del dart vader lego
		glm::mat4 modelMatrixColliderDart = glm::mat4(modelMatrixDart);
		AbstractModel::OBB dartLegoBodyCollider;
		// Colocar la orientacion del collider antes de hacer la escalacion
		dartLegoBodyCollider.u = glm::quat_cast(modelMatrixColliderDart);
		modelMatrixColliderDart = glm::scale(modelMatrixColliderDart, glm::vec3(0.5));
		modelMatrixColliderDart = glm::translate(modelMatrixColliderDart,
				glm::vec3(modelDartLegoBody.getObb().c));              //coordenadas del centro del modelo de Dart
		dartLegoBodyCollider.c = glm::vec3(modelMatrixColliderDart[3]);  //columna 3 de la caja de colision
		dartLegoBodyCollider.e = modelDartLegoBody.getObb().e * glm::vec3(0.5); //dimensiones originales por su escalamiento dado de 0.5 
		addOrUpdateColliders(collidersOBB, "dart", dartLegoBodyCollider, modelMatrixDart);

		// Collider del aricraft
		glm::mat4 modelMatrixColliderAircraft = glm::mat4(modelMatrixAircraft);
		AbstractModel::OBB aircraftCollider;
		// Colocar la orientacion del collider antes de hacer la escalacion
		aircraftCollider.u = glm::quat_cast(modelMatrixColliderAircraft);
		modelMatrixColliderAircraft = glm::scale(modelMatrixColliderAircraft,
				glm::vec3(1.0));
		modelMatrixColliderAircraft = glm::translate(
				modelMatrixColliderAircraft, modelAircraft.getObb().c);  
		aircraftCollider.c = glm::vec3(modelMatrixColliderAircraft[3]);
		aircraftCollider.e = modelAircraft.getObb().e * glm::vec3(1.0);
		addOrUpdateColliders(collidersOBB, "aircraft", aircraftCollider, modelMatrixAircraft);

		


		//Collider del la rock
		AbstractModel::SBB rockCollider;
		glm::mat4 modelMatrixColliderRock= glm::mat4(matrixModelRock);
		modelMatrixColliderRock = glm::scale(modelMatrixColliderRock, glm::vec3(1.0));
		modelMatrixColliderRock = glm::translate(modelMatrixColliderRock, modelRock.getSbb().c);
		rockCollider.c = glm::vec3(modelMatrixColliderRock[3]);
		rockCollider.ratio = modelRock.getSbb().ratio * 1.0;
		addOrUpdateColliders(collidersSBB, "rock", rockCollider, matrixModelRock);




		// Lamps1 colliders
		for (int i = 0; i < lamp1Position.size(); i++){
			AbstractModel::OBB lampCollider;
			glm::mat4 modelMatrixColliderLamp = glm::mat4(1.0);
			modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, lamp1Position[i]);
			modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(lamp1Orientation[i]),
					glm::vec3(0, 1, 0));
			
			// Set the orientation of collider before doing the scale
			lampCollider.u = glm::quat_cast(modelMatrixColliderLamp);
			modelMatrixColliderLamp = glm::scale(modelMatrixColliderLamp, glm::vec3(0.5));
			modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, modelLamp1.getObb().c);
			lampCollider.c = glm::vec3(modelMatrixColliderLamp[3]);
			lampCollider.e = modelLamp1.getObb().e * glm::vec3(0.5);
			addOrUpdateColliders(collidersOBB, "lamp1-" + std::to_string(i), lampCollider, modelMatrixColliderLamp);

			
		}

		//Lamp2 Colliders

		for (int i = 0; i < lamp2Position.size(); i++)
		{
			AbstractModel::OBB modelCollider;
			glm::mat4 modelMatrixColliderLamp = glm::mat4(1.0f);
			modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, lamp2Position[i]);
			modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp,
									glm::radians(lamp2Orientation[i]), 
									glm::vec3(0, 1, 0));
			
			
			modelCollider.u = glm::quat_cast(modelMatrixColliderLamp);
			modelMatrixColliderLamp = glm::scale(modelMatrixColliderLamp, glm::vec3(1.0f));
			modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, modelLamp2Post.getObb().c);
			modelCollider.c = modelMatrixColliderLamp[3];
			modelCollider.e = modelLamp2Post.getObb().e * glm::vec3(1.0f);
			addOrUpdateColliders(collidersOBB, "lamp2-" + std::to_string(i),modelCollider,modelMatrixColliderLamp);
				
		}

		// Lamps3 colliders
		for (int i = 0; i < lamp3Position.size(); i++){
			AbstractModel::OBB lampCollider3;
			glm::mat4 modelMatrixColliderLamp3 = glm::mat4(1.0);
			modelMatrixColliderLamp3 = glm::translate(modelMatrixColliderLamp3, lamp3Position[i]);
			modelMatrixColliderLamp3 = glm::rotate(modelMatrixColliderLamp3, glm::radians(lamp3Orientation[i]),
					glm::vec3(0, 1, 0));
			
			// Set the orientation of collider before doing the scale
			lampCollider3.u = glm::quat_cast(modelMatrixColliderLamp3);
			modelMatrixColliderLamp3 = glm::scale(modelMatrixColliderLamp3, glm::vec3(0.05));
			modelMatrixColliderLamp3 = glm::translate(modelMatrixColliderLamp3, modelLamp3.getObb().c);
			lampCollider3.c = modelMatrixColliderLamp3[3];
			lampCollider3.e = modelLamp3.getObb().e * glm::vec3(0.05);
			addOrUpdateColliders(collidersOBB, "lamp3-" + std::to_string(i), lampCollider3, modelMatrixColliderLamp3);
			
		}

		// Lamps4 colliders
		for (int i = 0; i < lamp4Position.size(); i++){
			AbstractModel::OBB lampCollider4;
			glm::mat4 modelMatrixColliderLamp4 = glm::mat4(1.0);
			modelMatrixColliderLamp4 = glm::translate(modelMatrixColliderLamp4, lamp4Position[i]);
			modelMatrixColliderLamp4 = glm::rotate(modelMatrixColliderLamp4, glm::radians(lamp4Orientation[i]),
					glm::vec3(0, 1, 0));
			
			// Set the orientation of collider before doing the scale
			lampCollider4.u = glm::quat_cast(modelMatrixColliderLamp4);
			modelMatrixColliderLamp4 = glm::scale(modelMatrixColliderLamp4, glm::vec3(0.05));
			modelMatrixColliderLamp4 = glm::translate(modelMatrixColliderLamp4, modelLamp4.getObb().c);
			lampCollider4.c = modelMatrixColliderLamp4[3];
			lampCollider4.e = modelLamp4.getObb().e * glm::vec3(0.05);
			addOrUpdateColliders(collidersOBB, "lamp4-" + std::to_string(i), lampCollider4, modelMatrixColliderLamp4);
			
		}

		// Collider de mayow
		AbstractModel::OBB mayowCollider;
		glm::mat4 modelmatrixColliderMayow = glm::mat4(modelMatrixMayow);
		modelmatrixColliderMayow = glm::rotate(modelmatrixColliderMayow,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		mayowCollider.u = glm::quat_cast(modelmatrixColliderMayow);
		modelmatrixColliderMayow = glm::scale(modelmatrixColliderMayow, glm::vec3(0.021));
		modelmatrixColliderMayow = glm::translate(modelmatrixColliderMayow,
				glm::vec3(mayowModelAnimate.getObb().c.x,
						mayowModelAnimate.getObb().c.y,
						mayowModelAnimate.getObb().c.z));
		mayowCollider.e = mayowModelAnimate.getObb().e * glm::vec3(0.021) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
		mayowCollider.c = glm::vec3(modelmatrixColliderMayow[3]);
		addOrUpdateColliders(collidersOBB, "mayow", mayowCollider, modelMatrixMayow);


		


		// Collider de muneco naranja  cyborg
		AbstractModel::OBB cyborgCollider;
		glm::mat4 modelmatrixColliderCyborg = glm::mat4(modelMatrixCyborg);
		modelmatrixColliderCyborg = glm::rotate(modelmatrixColliderCyborg,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		cyborgCollider.u = glm::quat_cast(modelmatrixColliderCyborg);
		modelmatrixColliderCyborg = glm::scale(modelmatrixColliderCyborg, glm::vec3(1.0, 1.0, 1.0));
		modelmatrixColliderCyborg = glm::translate(modelmatrixColliderCyborg,
				glm::vec3(cyborgModelAnimate.getObb().c.x,
						cyborgModelAnimate.getObb().c.y-golpe1*0.3,
						cyborgModelAnimate.getObb().c.z));
		cyborgCollider.e = cyborgModelAnimate.getObb().e * glm::vec3(0.6, 0.6+golpe1*0.4, 0.9) ;
		cyborgCollider.c = glm::vec3(modelmatrixColliderCyborg[3]);
		addOrUpdateColliders(collidersOBB, "cyborg", cyborgCollider, modelMatrixCyborg);


		/*******************************************
		 * Render de colliders
		 *******************************************/
		std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it;  //se saco fuera del for para mejor lectura
		for ( it = collidersOBB.begin(); it != collidersOBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c); //centro de la caja
			matrixCollider = matrixCollider * glm::mat4(std::get<0>(it->second).u); //para la rotacion
			matrixCollider = glm::scale(matrixCollider, std::get<0>(it->second).e * 2.0f);
			//boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0)); ya se ha definido el color linea 384
			boxCollider.enableWireMode();  //para visualizar el alambrado
			boxCollider.render(matrixCollider);
		}

		std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it_s;
		for ( it_s =collidersSBB.begin(); it_s != collidersSBB.end(); it_s++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it_s->second).c);
			matrixCollider = glm::scale(matrixCollider, glm::vec3(std::get<0>(it_s->second).ratio * 2.0f));
			//sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0)); ya se ha definido el color en la linea 388
			sphereCollider.enableWireMode();
			sphereCollider.render(matrixCollider);
		}

		// Test Collisions
		
		for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
				collidersOBB.begin(); it != collidersOBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
					std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
					collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (it != jt
						&& testOBBOBB(std::get<0>(it->second),
								std::get<0>(jt->second))) {
					std::cout << "Colision " << it->first << " with "
							<< jt->first << std::endl;
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
					std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator jt =
					collidersSBB.begin(); jt != collidersSBB.end(); jt++) {
				if (it != jt
						&& testSphereSphereIntersection(std::get<0>(it->second),
								std::get<0>(jt->second))) {
					std::cout << "Colision " << it->first << " with "
							<< jt->first << std::endl;
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			std::map<std::string,
					std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
					collidersOBB.begin();
			for (; jt != collidersOBB.end(); jt++) {
				if (testSphereOBox(std::get<0>(it->second),
								std::get<0>(jt->second))) {
					std::cout << "Colision " << it->first << " with "
							<< jt->first << std::endl;
					isCollision = true;
					addOrUpdateCollisionDetection(collisionDetection, jt->first, isCollision);
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		std::map<std::string, bool>::iterator colIt;
		for (colIt = collisionDetection.begin(); colIt != collisionDetection.end();
				colIt++) {
			std::map<std::string,
					std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
					collidersSBB.find(colIt->first);
			std::map<std::string,
					std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
					collidersOBB.find(colIt->first);
			if (it != collidersSBB.end()) {
				if (!colIt->second)
					addOrUpdateColliders(collidersSBB, it->first);
			}
			if (jt != collidersOBB.end()) {
				if (!colIt->second)
					addOrUpdateColliders(collidersOBB, jt->first);
				else {
					if (jt->first.compare("mayow") == 0)
						modelMatrixMayow = std::get<1>(jt->second);
					if (jt->first.compare("dart") == 0)
						modelMatrixDart = std::get<1>(jt->second);
					if (jt->first.compare("cyborg") == 0)
						modelMatrixCyborg = std::get<1>(jt->second);
				}
			}
		}


		glm::mat4 modelMatrixRayMay=glm::mat4(modelMatrixMayow);
		modelMatrixRayMay=glm::translate(modelMatrixRayMay,glm::vec3(0,1,0));
		// float maxDistanceRay=10.0;
		glm::vec3 rayDirection=modelMatrixRayMay[2];
		glm::vec3 ori=modelMatrixRayMay[3];
		glm::vec3 rmd=ori+rayDirection*(maxDistanceRay/2.0f);
		glm::vec3 targetRay=ori+rayDirection*maxDistanceRay;
		modelMatrixRayMay[3]=glm::vec4(rmd,1.0);
		modelMatrixRayMay=glm::rotate(modelMatrixRayMay,glm::radians(90.0f),glm::vec3(1,0,0));
		modelMatrixRayMay=glm::scale(modelMatrixRayMay,glm::vec3(0.05,maxDistanceRay,0.05));
		rayModel.render(modelMatrixRayMay);

		for(std::map<std::string, std::tuple<AbstractModel::SBB,glm::mat4,glm::mat4>>::iterator jt=collidersSBB.begin();jt!=collidersSBB.end();jt++){
			float tRint;
			if(raySphereIntersect(ori,targetRay,rayDirection,std::get<0>(jt->second),tRint)){
				std::cout<<"Colision del rayo con el modelo"<< jt->first<<std::endl;
			}

		}




		//*********************************************************
		//*** Interpolacion key frames  with disconect objects
		
		// Animaciones por keyframes dart Vader
		// Para salvar los keyframes
		if(record && modelSelected == 1){
			matrixDartJoints.push_back(rotDartHead);
			matrixDartJoints.push_back(rotDartLeftArm);
			matrixDartJoints.push_back(rotDartLeftHand);
			matrixDartJoints.push_back(rotDartRightArm);
			matrixDartJoints.push_back(rotDartRightHand);
			matrixDartJoints.push_back(rotDartLeftLeg);
			matrixDartJoints.push_back(rotDartRightLeg);
			if(saveFrame){
				saveFrame = false;
				appendFrame(myfile, matrixDartJoints);
			}
		}
		else if(keyFramesDartJoints.size() > 0){
			// Para reproducir el frame
			interpolationDartJoints = numPasosDartJoints / (float) maxNumPasosDartJoints;
			numPasosDartJoints++;
			if(interpolationDartJoints > 1.0){
				interpolationDartJoints = 0;
				numPasosDartJoints = 0;
				indexFrameDartJoints = indexFrameDartJointsNext;
				indexFrameDartJointsNext++;
			}
			if(indexFrameDartJointsNext > keyFramesDartJoints.size() -1)
				indexFrameDartJointsNext = 0;
			rotDartHead = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 0, interpolationDartJoints);
			rotDartLeftArm = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 1, interpolationDartJoints);
			rotDartLeftHand = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 2, interpolationDartJoints);
			rotDartRightArm = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 3, interpolationDartJoints);
			rotDartRightHand = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 4, interpolationDartJoints);
			rotDartLeftLeg = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 5, interpolationDartJoints);
			rotDartRightLeg = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 6, interpolationDartJoints);
		}
		if(record && modelSelected == 2){
			matrixDart.push_back(modelMatrixDart);
			if(saveFrame){
				saveFrame = false;
				appendFrame(myfile, matrixDart);
			}
		}
		else if(keyFramesDart.size() > 0){
			interpolationDart = numPasosDart / (float) maxNumPasosDart;
			numPasosDart++;
			if(interpolationDart > 1.0){
				numPasosDart = 0;
				interpolationDart = 0;
				indexFrameDart = indexFrameDartNext;
				indexFrameDartNext++;
			}
			if(indexFrameDartNext > keyFramesDart.size() - 1)
				indexFrameDartNext = 0;
			modelMatrixDart = interpolate(keyFramesDart, indexFrameDart, indexFrameDartNext, 0, interpolationDart);
		}
		// Animaciones por keyframes buzz
		// Para salvar los keyframes
		if(record && modelSelected == 3){
			matrixBuzzJoints.push_back(rotBuzzHead);
			matrixBuzzJoints.push_back(rotBuzzLeftarm);
			matrixBuzzJoints.push_back(rotBuzzLeftForeArm);
			matrixBuzzJoints.push_back(rotBuzzLeftHand);
			if(saveFrame){
				saveFrame = false;
				appendFrame(myfile, matrixBuzzJoints);
			}
		}
		else if(keyFramesBuzzJoints.size() > 0){
			// Para reproducir el frame
			interpolationBuzzJoints = numPasosBuzzJoints / (float) maxNumPasosBuzzJoints;
			numPasosBuzzJoints++;
			if(interpolationBuzzJoints > 1.0){
				interpolationBuzzJoints = 0;
				numPasosBuzzJoints = 0;
				indexFrameBuzzJoints = indexFrameBuzzJointsNext;
				indexFrameBuzzJointsNext++;
			}
			if(indexFrameBuzzJointsNext > keyFramesBuzzJoints.size() -1)
				indexFrameBuzzJointsNext = 0;
			rotBuzzHead = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 0, interpolationBuzzJoints);
			rotBuzzLeftarm = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 1, interpolationBuzzJoints);
			rotBuzzLeftForeArm = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 2, interpolationBuzzJoints);
			rotBuzzLeftHand = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 3, interpolationBuzzJoints);
		}
		if(record && modelSelected == 4){
			matrixBuzz.push_back(modelMatrixBuzz);
			if(saveFrame){
				saveFrame = false;
				appendFrame(myfile, matrixBuzz);
			}
		}
		else if(keyFramesBuzz.size() > 0){
			interpolationBuzz = numPasosBuzz / (float) maxNumPasosBuzz;
			numPasosBuzz++;
			if(interpolationBuzz > 1.0){
				numPasosBuzz = 0;
				interpolationBuzz = 0;
				indexFrameBuzz = indexFrameBuzzNext;
				indexFrameBuzzNext++;
			}
			if(indexFrameBuzzNext > keyFramesBuzz.size() - 1)
				indexFrameBuzzNext = 0;
			modelMatrixBuzz = interpolate(keyFramesBuzz, indexFrameBuzz, indexFrameBuzzNext, 0, interpolationBuzz);
		}
		
		/**********Maquinas de estado*************/
		// Maquina de estados para el carro eclipse
		switch (state){
		case 0:
			if(numberAdvance == 0)
				maxAdvance = 65.0;
			else if(numberAdvance == 1)
				maxAdvance = 49.0;
			else if(numberAdvance == 2)
				maxAdvance = 44.5;
			else if(numberAdvance == 3)
				maxAdvance = 49.0;
			else if(numberAdvance == 4)
				maxAdvance = 44.5;
			state = 1;
			break;
		case 1:
			modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(0.0f, 0.0f, avance));
			advanceCount += avance;
			rotWheelsX += 0.05;
			rotWheelsY -= 0.02;
			if(rotWheelsY < 0)
				rotWheelsY = 0;
			if(advanceCount > maxAdvance){
				advanceCount = 0;
				numberAdvance++;
				state = 2;
			}
			break;
		case 2:
			modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(0.0, 0.0, 0.025f));
			modelMatrixEclipse = glm::rotate(modelMatrixEclipse, glm::radians(giroEclipse), glm::vec3(0, 1, 0));
			rotCount += giroEclipse;
			rotWheelsX += 0.05;
			rotWheelsY += 0.02;
			if(rotWheelsY > 0.25)
				rotWheelsY = 0.25;
			if(rotCount >= 90.0f){
				rotCount = 0;
				state = 0;
				if(numberAdvance > 4)
					numberAdvance = 1;
			}
			break;
		
		default:
			break;
		}

		

		
		//Maquina de estado de Auto Lambo
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
					maxAdvance2 = 36.0f;
				state2 = 1;
				break;

			case 1:
				modelMatrixLambo = glm::translate(modelMatrixLambo,glm::vec3(0.0f, 0.0f,0.2f));
				advanceCount2 += 0.2f;
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
					if(numberAdvance2 > 4)
						numberAdvance2 = 1;
				}	
				break;
		}

		//Maquina de estados (Lambo) de la puerta
	/*
		switch(stateDoor){
			case 0:
				dorRotCount += 0.5;
				if(dorRotCount > 75.0f)
					stateDoor = 1;
				break;
			case 1:
				dorRotCount  -= 0.5;
				if(dorRotCount <0) {
					dorRotCount = 0.0f;
					stateDoor = 0;
				}
			
		
		}*/

		// Constantes de animaciones
		rotHelHelY += 0.5;
		rotHelHelBack += 0.5;
		//maxDistanceRay+= 0.02;//cambio size del ray

		glfwSwapBuffers(window);
	}
}



int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}