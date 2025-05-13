#include "puppetPart.h"


// find a better way to do this
PuppetPart::PuppetPart(ofxraycaster::Mousepicker& _mousepicker, ofNode& _parentTransform)
	: mousepicker(_mousepicker), parentTransform(_parentTransform)



{
	location = vec3{ 0,0,0 };
	rotation = vec3{ 0,0,0 };

	
	selectedMaterial.setDiffuseColor(ofFloatColor{ 0.1f });
	selectedMaterial.setAmbientColor(ofFloatColor{ 0.2f ,0.0f,0.f });
	selectedMaterial.setSpecularColor(ofFloatColor{ 0.8f });
	selectedMaterial.setShininess(200);

	deselectedMaterial.setDiffuseColor(ofFloatColor{ 0.2f });
	deselectedMaterial.setAmbientColor(ofFloatColor{ 0.5f ,0,0});
	deselectedMaterial.setSpecularColor(ofFloatColor{ 0.8f });
	deselectedMaterial.setShininess(200);

	ofEnableNormalizedTexCoords();
	ofDisableArbTex();
	
}

PuppetPart::PuppetPart(const PuppetPart &p) : mousepicker(p.mousepicker), parentTransform(p.parentTransform) {
	
	// keep this updated or use pointer
	
	p.deselectedMaterial;
	p.index;
	isIntersecting = p.isIntersecting;
	isSelected = p.isSelected;
	location = p.location;
	m = p.m;
	name = p.name;
	rayDistance = p.rayDistance;
	rotation = p.rotation;
	selectedMaterial = p.selectedMaterial;
	transform = p.transform;
	uri = p.uri;
	deselectedMaterial = p.deselectedMaterial;
	tex = p.tex;
	
}

PuppetPart::~PuppetPart() {

}

void PuppetPart::setLocation(vec3 _location) {
	location = _location;
}

void PuppetPart::setRotation(vec3 _rotation) {
	rotation = _rotation;
}

void PuppetPart::setModel(ofMesh _mesh) {
	//model.clear();
	m = _mesh;
}

void PuppetPart::setSelected(bool _isSelected) {
	isSelected = _isSelected;
}

float PuppetPart::getDistance() {
	return rayDistance;
}

bool PuppetPart::getSelected() {
	return isSelected;
}

bool PuppetPart::getIntersecting() {

	return isIntersecting;
}

void PuppetPart::draw(){

	transform.setGlobalPosition(location);
	transform.setGlobalOrientation(rotation);
	transform.setScale(40);

	transform.transformGL();

	
	vec2 baricentricCoords;
	vec3 surfaceNormal;
	float distance;

	

	/*for (int i = 0; i < model.getMeshCount(); i++) {
		m = model.getMesh(i)*/;

		ofPushStyle();

		isIntersecting = mousepicker.getRay().intersectsMesh(m, parentTransform.getGlobalTransformMatrix() * transform.getLocalTransformMatrix(), baricentricCoords, distance, surfaceNormal);

		if (isSelected) {
	
			// code to do when selected
		
			//selectedMaterial.begin();
			sceneShader.setUniform1f("isTex", true);

			if (tex.bAllocated()) {

				sceneShader.setUniformTexture("tex", tex.getTexture(), 0);
			}

				sceneShader.setUniform1f("isSelected", true);
				sceneShader.setUniform1f("appTime", float(ofGetElapsedTimeMillis() / 1000.f));
					m.draw();
				sceneShader.setUniform1f("isSelected", false);
				sceneShader.setUniform1f("isTex", false);
			//selectedMaterial.end();


			

			
		}
		else {
			
			//deselectedMaterial.begin();
			sceneShader.setUniform1f("isTex", true);
			ofDisableArbTex();
			ofEnableNormalizedTexCoords();

			if (tex.bAllocated()) {
				
				sceneShader.setUniformTexture("tex", tex.getTexture(), 0);
			}
	
				m.draw();
			sceneShader.setUniform1f("isTex", false);
			//deselectedMaterial.end();
			
		}
		

		rayDistance = distance;
		
	
		ofPopStyle();


		
		
	
	//}


	transform.restoreTransformGL();
	
	
}