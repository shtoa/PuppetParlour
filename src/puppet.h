#pragma once

#include "puppetPart.h"
#include "ofMain.h"

using namespace glm;

class Puppet
{
	public:
		
		Puppet(ofxraycaster::Mousepicker & _MousePicker, float& _mouthAngle);
		~Puppet();

		void selectPart();

		void setRotation(vec3 _rotation);
		void setLocation(vec3 _location);

		void showText(bool _isTextShowing) {
			isTextShowing = _isTextShowing;
		}

		void selectable(bool _isSelectable) { isSelectable = _isSelectable;}
		bool selectable() { return isSelectable; };

		void draw();

	private:

		// add set transform function
		ofNode transform;

		// setting up location
		vec3 rotation;
		vec3 location;

		float& mouthAngle;

		// find better way to do this

		// preload models and get meshes into seperate objects


		vector<std::string>partDirs = { "headTop", "headBottom", "lArm", "rArm","body" };


		// not safe change maybe use smart pointer

		// std::map<std::string, vector<PuppetPart>> puppetParts{};


		// redo with smart pointers !!!! 
		struct {
			std::map<std::string, vector<PuppetPart*>> parts;
			std::map<std::string, int> indices;
			std::map<std::string, vec3> translations;
			std::map<std::string, PuppetPart*> selected;
		} puppetParts;



		bool isFirstPress = true;
		ofxAssimpModelLoader modelLoader;
		ofMesh m;

		ofSoundPlayer selectSound;


		ofTrueTypeFont font;
		bool isTextShowing = false;
		bool isSelectable = true;
};

