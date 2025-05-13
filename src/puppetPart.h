#pragma once 

#include "ofxAssimpModelLoader.h"
#include "ofMain.h"
#include "ofxRaycaster.h"
#include "ShaderUtils.h"


using namespace glm;

class PuppetPart
{

	public:
		PuppetPart(ofxraycaster::Mousepicker& _mousepicker, ofNode& _parentTransform); // make default constructor
		~PuppetPart();
		PuppetPart(PuppetPart const &p);

		// setter methods
		void setRotation(vec3 _rotation); // set euler rotation
		void setLocation(vec3 _location); // set location 
		void setModel(ofMesh _model); // set the model to be displayed
		void setSelected(bool isSelected);


		float getDistance();
		bool getSelected();
		bool getIntersecting();

		// drawing method
		void draw();


		// getters and setters
		
		std::string getName() { return name; };
		void setName(std::string _name) { name = _name; };

		int getIndex() { return index; };
		void setIndex(int _index) { index = _index; };

		void setTex(ofImage im) {
			im.resize(1000, 1000);
			tex = im;
		
		};
		ofImage getTex() { return tex; };



	private:

		// transform node
		ofNode transform;


		std::string name;
		int index;
	


		// setting up location
		vec3 rotation;
		vec3 location;

		// setting up model
		//ofxAssimpModelLoader model;
		ofxAssimpModelLoader model;

		ofVboMesh m;
		
		std::string uri;

		ofxraycaster::Mousepicker& mousepicker;
		
		
		// instead of pointers use an initializer list
		ofNode& parentTransform;
		float rayDistance;
		bool isSelected = false;
		bool isIntersecting = false;

		ofMaterial selectedMaterial;
		ofMaterial deselectedMaterial;

		// image of Texture
		ofImage tex;

};

