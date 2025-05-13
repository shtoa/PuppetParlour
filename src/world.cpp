#include "world.h"

using namespace glm;

World::World(ofVec3f _location, int _size)

	:location{ _location }, 
	size{ _size } // member initialzie list

{
	ofEnableNormalizedTexCoords();
	ofDisableArbTex();
	// world

	world.setResolution(100, 100, 10);
	world.setRadius(1);
	world.setHeight(height);
	world.setGlobalPosition(location);
	world.setScale(size);



	// skyBox

	skyBox = ofMesh::sphere(size, 100);

	for (auto& n : skyBox.getNormals()) {
		n *= -1;
	}
	

	

	skyBoxTexture.load("skyBoxTexture.png");

	//sun

	sun.setResolution(100);
	sun.setRadius(1);
	sun.setScale(50);
	sun.setGlobalPosition(location + ofVec3f{ float(size/2.f),float(size/2.f)/1.3f,float(size/2.f) });

	sunModel.loadModel("sun.obj", true);
	sunTexture.load("fire.png");

	sunLight.setParent(sun);
	sunLight.setPointLight(); // set sun as a pointlight

	//sunMaterial.setEmissiveColor(ofFloatColor(0.5f)); // set suns material to emissive
	sunLight.setDiffuseColor(ofFloatColor(0.f));
	sunLight.setAmbientColor(ofFloatColor(0.5f));
	sunLight.setAttenuation(0.5f, 0, 1. / (500.f*500.f));

	sunMaterial.setDiffuseColor(ofFloatColor(0.0f));
	sunMaterial.setSpecularColor(ofFloatColor(1.0f));
	sunMaterial.setShininess(40.0f);

	//skyBoxTexture.setTextureWrap(GL_REPEAT, GL_REPEAT);

}

World::~World() {
	cout << "destroy the world" << endl;
}

int World::getSize() {
	return size;
}

ofLight* World::getSunLightPtr(){
	return &sunLight;
}

void World::draw() {

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);


	sun.rotateAroundDeg((sunRotation + 1)%(360), ofVec3f{0,1,0}, location);
	sun.lookAt(location); // look at center of the world
	
	// draw skybox



	sceneShader.setUniform1f("isTex", true);
	sceneShader.setUniformTexture("tex", skyBoxTexture.getTexture(), 0);
	
	skyBoxTexture.getTexture().bind();
		skyBox.draw();
	skyBoxTexture.getTexture().unbind();

	

	// draw sun
	
	sceneShader.setUniformTexture("tex", sunTexture.getTexture(), 0);
	sun.transformGL();
	
	//sunTexture.bind();
		sunModel.getMesh(0).drawFaces();
	//sunTexture.unbind();


	sun.restoreTransformGL();
	sceneShader.setUniform1f("isTex", false);

	glDisable(GL_COLOR_MATERIAL);

}