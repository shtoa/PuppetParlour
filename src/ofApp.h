#pragma once

#include "ofMain.h"


// external libraries
#include "ofxOsc.h"
#include "ofxRaycaster.h"

// drawing classes
#include "world.h"
#include "puppet.h"

// utilities
#include "handTrack.h"
#include "orbitCam.h"
#include "ofGraphicsUtil.h"
#include "ShaderUtils.h"

// model loading
#include "ofxAssimpModelLoader.h"

// for doing windows specific WINDOW operations
#include <WinUser.h>

/*

	---- OSC Setup ----

*/


// listening port
#define PORT 7500

// max number of strings to display
#define NUM_MSG_STRINGS 20

//ofShader sceneShader;


// https://stackoverflow.com/questions/3386861/converting-a-variable-name-to-a-string-in-c
#define GET_VARIABLE_NAME(Variable) (#Variable)


class ofApp : public ofBaseApp{
	public:

		void setup();
		void update();
		void draw();
		void drawScene();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void exit();

		//void removeParticle();

		void buildShader();


		enum Mode{select, space, desktop};

		void selectMode();
		void spaceMode();
		void desktopMode();


		// get the rotaiton angle from the handracking information
		float getMouthAngle();


	private:

		Mode curMode; 

		ofTrueTypeFont font;
		ofxOscReceiver receiver;

		int currentMsgString;
		string msgStrings[NUM_MSG_STRINGS];
		float timers[NUM_MSG_STRINGS];

		float mouseXf = 0;
		float mouseYf = 0;
		int mouseButtonInt = 0;
		string mouseButtonState = "";

		ofImage receivedImage;
		ofSpherePrimitive testSphere1;
		ofSpherePrimitive testSphere2;
		ofSpherePrimitive testSphere3;
		ofVboMesh particleModel;

		//ofEasyCam cam;
		OrbitCam cam;
		ofCamera cam2;

		ofVec3f posThumb;
		ofVec3f posMiddle;
		ofVec3f posPalm;
		ofVec3f posMiddleKnuckle;
		ofVec3f posThumbKnuckle;


		ofVec3f vHeadingTop;
		ofVec3f vHeadingBottom;

		ofMaterial puppetMat;

		float prevAngle = 0;
		float currAngle = 0;

		ofLight testLight;

		ofxAssimpModelLoader boxModel;
		ofVboMesh boxMesh;
		ofMaterial boxMaterial;

		ofxAssimpModelLoader keyModel;
		ofVboMesh keyMesh;

		ofxAssimpModelLoader backModel;
		ofVboMesh backMesh;
		ofMaterial backMaterial;

		ofFbo backFBO;
		ofImage backTempTex;


		//ofTexture topText;

		ofVboMesh headTopMesh;
		ofVboMesh headBottomMesh;
		ofVboMesh bodyMesh;

		World world{ ofVec3f{0.f,0.f,0.f},500 };

		


		// Create Particles

		typedef struct particle {
			
			float life;
			float size;
			vec3 location;
			vec3 velocity;
		
		} Particle;


		// manipulating particles to array
		int nParticles = 10;
	

		vector<Particle> particles;

		float prevX = 0.0f;
		float prevY = 0.0f;

		POINT p;
		ofxraycaster::Mousepicker mousepicker;

		Puppet puppet{ mousepicker , currAngle};
		
		handTrack track;



		// making transition shader

		ofShader transition;

		ofFbo curSceneFBO;
		ofFbo prevSceneFBO;

		float transitionTime = 1;

		ofMesh screen;

		

		ofSoundPlayer transitionSound;
		ofSoundPlayer ambientSound;



		// redo with references
		struct {
			vec2 prev = vec2{ 0,0 };
			vec2 cur = vec2{ 0,0 };
			vec2 delta;
			void update(int x,int y) {
				
				prev = cur;
				cur = vec2{ x,y };

				delta = cur - prev;
			}
		} mousePos;

		// store in camera
		float xAng = 0;
		float yAng = 0;

		int PID = 0;


		// structures to load in textured box

		typedef struct TexMesh {
			ofxAssimpModelLoader loader;
			ofImage tex;
			ofMesh mesh;
		};


		// "backBoard",

		array<std::string, 8> boxParts = {
			"accent",
			"base",
			"eyeballs",
			"eyeWhites",
			"main",
			"rods",
			"spikes"
		};

		array < std::string, 6> keyParts = {
			"accent",
			"eyeballs",
			"eyeWhites",
			"balls",
			"rod"
		};

		map<std::string, TexMesh> box;
		map<std::string, TexMesh> key;

		TexMesh boxCover;

		ofFbo boxBaseFbo; 
		ofImage boxBaseTex;

		// for typing the puppets name
		std::string name = "click2name";
		



		ofFbo nameFbo;

		ofMesh nameMesh;
		bool isTyping;
		bool isClicked;
		//ofTrueTypeFont fontText;
		
		ofTrueTypeFont fontText;

		bool isTutorial = true;
		TexMesh tutorial;

		float rollTime = 0.5f;
		float rollStep = 0.02f;

		std::string skipStr = "... (ENTER)";

		vector<std::string> tutorialInfo = {
			"Welcome to PuppetParlour ~ :> \n " + skipStr,
			"Please begin to design your puppet ;) \n by clicking on the parts " + skipStr,
			"Name ur puppet by clicking \n the base of the box ..." + skipStr,
			"In the next stage try using your hands \n to control the puppet " + skipStr,
			"press enter when ur ready 2 start ;3 \n try " + skipStr
		};

		int tutorialInfoIndex = 0;
		map<std::string, ofFbo> tutorialFbo;
		
		TexMesh spike;
};
