#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ShaderUtils.h"

using namespace glm;



class World
{
	public:
		World(ofVec3f _location, int _size);
		~World();
		void draw();
		int getSize();
		ofLight* getSunLightPtr();

	private:
		
		int size; // diameter of the world
		int height;
		ofVec3f location;


		// world variables
		ofCylinderPrimitive world;
		ofxAssimpModelLoader worldModel;
		ofVboMesh worldMesh;
		ofImage worldTexture;

		ofSpherePrimitive sun;
		ofxAssimpModelLoader sunModel;
		ofVboMesh sunMesh;
		ofImage sunTexture;
		ofMaterial sunMaterial;
		ofLight sunLight;
		int sunRotation = 0;

		// skyBox variables 
		ofMesh skyBox;
		ofImage skyBoxTexture;
};

