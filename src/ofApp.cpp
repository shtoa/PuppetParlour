#include "ofApp.h"

//--------------------------------------------------------------
static float getRollTime(float t) {

	return ((t < 0.5) ? (-2*t+1) : (2*t-1));

}

//--------------------------------------------------------------

void ofApp::buildShader() {
	build(transition, R"(
		// Vertex program
		uniform mat4 modelViewProjectionMatrix; // from OF programmable renderer
		
		// Seperate modelViewProjectionMatrices

		uniform mat4 projectionMatrix;
		uniform mat4 viewMatrix;
		uniform mat4 modelMatrix; // gives position in world space

		// position of the camera eye
		uniform vec3 eye;

		uniform vec2 mouseUnitPos;
		uniform float appTime;

		in vec4 position;	// position (vertex attribute from mesh) (gives local position in world space)
		in vec3 normal;		// normal (vertex attribute from mesh)
		in vec3 color;      // color (vertex attribute form mesh)
		in vec2 texcoord; // texture coordinate ( vertex attribute from mesh )

		out vec2 vtexcoord;	// passed to fragment shader
		out vec3 vcolor;	// passed to fragment shader

		out vec3 vposition; // passed to fragment shader
		out vec3 vnormal; // passed to fragment shader



		void main(){

			vcolor = color;
			vnormal = normal;
			vtexcoord = texcoord;
			vposition = ( modelMatrix * position ).xyz;
			gl_Position = projectionMatrix * viewMatrix * vec4( vposition , 1.);
				
		}
	)", glslNoise() + R"(
		// Fragment program

		// texture vars
		uniform sampler2D prevScene; // texture (ID) passed in from the CPU
		uniform sampler2D curScene;
	

		uniform float t;
		in vec2 vtexcoord; // interpolant from vertex shader
	
		// light vars
		uniform vec3 eye;
		in vec3 vposition;
		in vec3 vnormal;
		in vec3 vcolor;			// interpolant from vertex shader
		out vec4 fragColor;		// output pixel color (RGBA)

		void main(){
			
			vec4 sceneCol = mix(texture(prevScene,vtexcoord+t*snoise(vec3(5*vtexcoord,5*t))),texture(curScene,vtexcoord+(1-t)*snoise(vec3(5*vtexcoord,-5*t))),t);
			

	
			fragColor = sceneCol;

			
		}
	)");


	build(sceneShader, R"(
		// Vertex program
		uniform mat4 modelViewProjectionMatrix; // from OF programmable renderer
		
		// Seperate modelViewProjectionMatrices

		uniform mat4 projectionMatrix;
		uniform mat4 viewMatrix;
		uniform mat4 modelMatrix; // gives position in world space

	

		// position of the camera eye
		uniform vec3 eye;

		uniform vec2 mouseUnitPos;
		uniform float appTime;
		uniform bool isTutorial;
		uniform float rollTime;

		in vec4 position;	// position (vertex attribute from mesh) (gives local position in world space)
		in vec3 normal;		// normal (vertex attribute from mesh)
		in vec3 color;      // color (vertex attribute form mesh)
		in vec2 texcoord; // texture coordinate ( vertex attribute from mesh )

		out vec2 vtexcoord;	// passed to fragment shader
		out vec3 vcolor;	// passed to fragment shader

		out vec3 vposition; // passed to fragment shader
		out vec3 vnormal; // passed to fragment shader

	

		void main(){

			vcolor = color;
			vnormal = normal;
			vtexcoord = texcoord;
			
			vec4 pos = position;
			
			if(isTutorial){
				float wave = rollTime*rollTime*rollTime*rollTime*sin((2*(355./133.)*0.01*appTime*100+pos.y))*0.5*(texcoord.y);
				float wrap = 2.f;
				
				// pass height of plane via shader

				pos.z = texcoord.y*sin((1-rollTime)*wrap*3.14*2.*texcoord.y)*1.5*(rollTime+0.5) + wave;
				pos.y = -texcoord.y*cos((1-rollTime)*wrap*3.14*2.*texcoord.y)*1.5*(rollTime+0.5);
			}

			vposition = ( modelMatrix * pos ).xyz;
			gl_Position = projectionMatrix * viewMatrix * vec4( vposition , 1.);


				
		}
	)", glslWorley() + glslNoise() + glslLighting() + R"(
		// Fragment program

		// texture vars
		uniform sampler2D tex;

		uniform float appTime;

		uniform float t;
		in vec2 vtexcoord; // interpolant from vertex shader


		uniform bool isNoise;
		uniform bool isTex;
		uniform bool isCover;
		uniform bool isRod;
		uniform bool isText;
		uniform bool isSelected;

		uniform bool hasColor;
		uniform vec3 setColor;
	
		// light vars
		uniform vec3 eye;
		in vec3 vposition;
		in vec3 vnormal;
		in vec3 vcolor;			// interpolant from vertex shader
		out vec4 fragColor;		// output pixel color (RGBA)

		void main(){

			// Lighting code from Graphics 2 Module

			
			// ---- Lighting Using Structs and Functions ----
			vec3 pos = vposition;

			vec3 normal = normalize(vnormal);

			if(isRod){
				// Z is up
				// vec3(0,0,1) is the normal 

				float delta = 0.1;
				float scale = 0.05f;

				vec2 dx1 =  2*worley(scale*vec3(pos.x-delta/2.f,pos.y+20*appTime,pos.z), 1.5f, false)-1;
				vec2 dx2 =	2*worley(scale*vec3(pos.x+delta/2.f,pos.y+20*appTime,pos.z), 1.5f, false)-1;
				float dx = ((dx2.y-dx2.x)-(dx1.y-dx1.x));
				
				vec3 v1 = normalize(vec3(delta,0,dx));


				vec2 dy1 =  2*worley(scale*vec3(pos.x,pos.y+20*appTime-delta/2.f,pos.z), 1.5f, false)-1;
				vec2 dy2 =	2*worley(scale*vec3(pos.x,pos.y+20*appTime+delta/2.f,pos.z), 1.5f, false)-1;
				float dy = ((dy2.y-dy2.x)-(dy1.y-dy1.x));

				vec3 v2 = normalize(vec3(0,delta, dy));

				
				vec3 n = cross(v1,v2);

				//vec2 w = worley(scale*vec3(pos.x,pos.y+20*appTime,pos.z), 1.5f, false);
				//normal = (w.y-w.x)*normal

				//normal = normalize((n*2-1)+(normal*2-1))*0.5+0.5;


				vec3 n1 = normal.xyz*2 - 1;
				vec3 n2 = n.xyz*2-1;
				
				mat3 nBasis = mat3(
				n1.z,n1.y,-n1.x,
				n1.x,n1.z,-n1.y,
				n1.x, n1.y, n1.z
				);
				
				vec3 r = normalize(n2.x*vec3(n1.z,n1.y,-n1.x) + n2.y*vec3(n1.x,n1.z,-n1.y) + n2.z*vec3(n1.x, n1.y, n1.z));

				//normal = normalize(r*0.5+0.5);
				normal = normalize(n*0.5 + 0.5);
			}

			Light light1;

			light1.pos = vec3(-500., 500. , -500.f);
			light1.strength = 2.;
			light1.halfDist = 750.;
			light1.ambient = 0.2;
			light1.diffuse = vec3(0.8) ;
			light1.specular = light1.diffuse ;

			Light light2 = light1 ;
			//light2.pos = light1.pos ;	
			light2.pos = vec3(500., 500,500.);
			light2.diffuse = vec3(0.8) ;
			light2.specular = light2.diffuse ;

			Material mtrl;
			

			if(isTex){
				
				// mtrl.diffuse = texture(tex,vtexcoord).rgb;
				if(isRod){
					float scale = 0.05f;
					vec2 w = worley(scale*vec3(pos.x,pos.y+20*appTime,pos.z), 1.5f, false);

					mtrl.diffuse = vec3(0,0.5+0.5*(w.y-w.x),0);
				} else if (isSelected){
					float scale = 0.05f;
				
					vec2 w = worley(scale*vec3(vtexcoord.x, vtexcoord.y, 20*appTime), 1.5f, false);
					mtrl.diffuse = mix(texture(tex, vtexcoord), vec4(1.),0.3).rgb;
				}

				else {
					mtrl.diffuse = texture(tex,vtexcoord).rgb;
				}
			}
	
			else if(isNoise){
				vec4 col1 = vec4(1.,1.,1.,abs(snoise(vec3(0.08*vposition+vec3(0,appTime,0)))));
				vec4 col2 = vec4(0.5,0,0,1);
				mtrl.diffuse = col1.rgb*(col1.a) + col2.rgb*(1-col1.a); 
			} else if (isCover){
				mtrl.diffuse = vec3(0.8);
			}
			else if (hasColor) {
				mtrl.diffuse = setColor;
			}
			else {			
				mtrl.diffuse = vcolor;
			}

		


			mtrl.specular = vec3 (1.0);
			mtrl.shine = 60.;
			LightFall fall = computeLightFall ( pos , normal , eye , light1 , mtrl );
			addTo ( fall , computeLightFall ( pos , normal , eye , light2 , mtrl ));

			vec3 col = lightColor ( fall , mtrl );
			
			if(isCover){
				fragColor = vec4(col, 0.3);
			} else if (hasColor){
				fragColor = vec4(setColor, 1.);
			} else {
				fragColor = vec4(col, 1.);
			}
			
			if(isText){
				fragColor = texture(tex, vtexcoord);
			}

			if(isRod){
				//fragColor = vec4(normal,1.);
			}
			
		}
	)");
}


//--------------------------------------------------------------
void ofApp::setup() {

	spike.loader.load("spike.obj");
	spike.mesh = spike.loader.getMesh(0);
	spike.tex.load("spike.png");

	backTempTex.load("skyBoxTexture.png");

	transitionSound.load("ds_bonfire.mp3");

	ambientSound.load("Formula.mp3");
	ambientSound.setVolume(0.1);
	ambientSound.setLoop(true);
	ambientSound.play();

	


	ofSetWindowTitle("~PuppetParlour~ :3");
	ofSetFrameRate(60); // run at 60 fps
	ofSetVerticalSync(true);


	curMode = Mode::select;

	cam2.setPosition(0, 0, -1200);
	cam2.lookAt(vec3{ 0,0,0 });


	// cam.setFov(90.f); // try different fov

	//cam.setNearClip(0.05);
	//cam.setFarClip(100.);

	// listen on the given port
	ofLog() << "listening for osc messages on port " << PORT;
	receiver.setup(PORT);

	testSphere1.setScale(50);
	testSphere2.setScale(50);
	testSphere3.setScale(50);


	boxModel.loadModel("./box/frame.obj", true);
	boxMesh = boxModel.getMesh(0);


	backModel.loadModel("./box/back.obj", true);
	backMesh = backModel.getMesh(0);

	backFBO.allocate(2 * 800, 2 * 600, GL_RGBA);

	backFBO.begin();
	ofClear(0);
	backFBO.end();

	keyModel.loadModel("key.obj", true);
	keyMesh = keyModel.getMesh(0);

	posThumb = ofVec3f(0.f, 0.f, 0.f);
	posMiddle = ofVec3f(0.f, 0.f, 0.f);

	testSphere1.rotateDeg(180, ofVec3f{ 0,1,0 });
	testSphere2.rotateDeg(180, ofVec3f{ 0,1,0 });
	testSphere3.rotateDeg(180, ofVec3f{ 0,1,0 });

	testSphere3.setPosition(0, -12.5, 0);

	puppetMat.setAmbientColor(ofFloatColor(0.8f, 0.3f, 0.8f));
	puppetMat.setSpecularColor(ofFloatColor(1.0f));
	puppetMat.setDiffuseColor(ofFloatColor(0.f));
	puppetMat.setShininess(10.f);

	testLight.lookAt(ofVec3f{ 0,0,0 });

	ofEnableNormalizedTexCoords(); // use [0,1] range for UVs
	ofDisableArbTex(); // disable use of legacy rect textures (required for wrapping modes)


	ofEnableAlphaBlending();

	for (int i = 0; i < nParticles; i++) {

		particles.push_back(Particle{ ofRandom(0,10),ofRandom(5,10),testSphere3.getGlobalPosition() - vec3{ofRandom(-10,10),180,ofRandom(-10,10)},vec3{0,ofRandom(-1,-0.1),0} });

	}

	curSceneFBO.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	curSceneFBO.begin();
	ofClear(255, 255, 255, 0);
	curSceneFBO.end();


	prevSceneFBO.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	prevSceneFBO.begin();
	ofClear(255, 255, 255, 0);
	prevSceneFBO.end();


	ofFbo::Settings settings;
	settings.useDepth = true;

	screen = ofMesh::plane(ofGetWidth(), ofGetHeight(), 2, 2, OF_PRIMITIVE_TRIANGLES);


	mat2 rotM = {
			{-1,0},
			{0,1}
	};

	for (auto& t : screen.getTexCoords()) { t = (rotM * (t - vec2(0.5, 0.5))) + vec2(0.5, 0.5); };

	buildShader();


	// load the box models + textures

	string path = "./box/textures/";
	ofDirectory dir(path);
	dir.allowExt("png");
	dir.allowExt("jpg");

	dir.listDir();

	/*
	
		----- REWRITE INTO FUNCTION -----
	
	*/
	for (auto part : keyParts) {
		TexMesh t;
		ofImage im;

		t.loader.load("./box/models/key/" + part + ".obj");
		t.mesh = t.loader.getMesh(0);
		// or try matching usig the directory
		// do this better
		for (auto f : dir.getFiles()) {
			if (f.getBaseName() == part) {
				im.load(f.getAbsolutePath());
	
			}
		}

		/*if (!im.load("./box/textures/" + part + ".png")) {
			im.load("./box/textures/" + part + ".jpg");
		}*/

		t.tex = im;

		key[part].mesh = t.mesh;
		key[part].tex = t.tex;
	}

	for (auto part: boxParts) {

		TexMesh t;
		ofImage im;
		
		t.loader.load("./box/models/" + part + ".obj");
		t.mesh = t.loader.getMesh(0);
		// or try matching usig the directory
		// do this better
		for (auto f : dir.getFiles()) {
			if (f.getBaseName() == part) {
				im.load(f.getAbsolutePath());
				
			}
		}

		/*if (!im.load("./box/textures/" + part + ".png")) {
			im.load("./box/textures/" + part + ".jpg");
		}*/

		t.tex = im;

		box[part].mesh = t.mesh;
		box[part].tex = t.tex;

	}

	boxCover.loader.load("./box/models/boxCover.obj");
	boxCover.mesh = boxCover.loader.getMesh(0);


	auto t = box.at("base").tex;
	boxBaseFbo.allocate(t.getWidth(), t.getHeight(), GL_RGBA);

	boxBaseFbo.begin();
		ofClear(0);
	boxBaseFbo.end();

	boxBaseTex = t;

	// font for drawing puppet name
	fontText.load("Minecraft.ttf", 20, true, true, true, 0, 0);

	auto ratio = 5.5;

	nameFbo.allocate(ratio * 5 * 50, 5 * 50, GL_RGBA);
	nameFbo.begin();
	ofClear(1, 0, 0, 1);
	nameFbo.end();


	for (auto s : { "prev", "cur", "next" }) {

		ofFbo fbo;
		
		fbo.allocate(ratio * 5 * 50, 5 * 50, GL_RGBA);
		fbo.begin();
			ofClear(1, 0, 0, 1);
		fbo.end();

		tutorialFbo.insert({ s, fbo });

	}
	




	/*
	string path = part;
	ofDirectory dir(path);
	dir.allowExt("obj");
	dir.listDir();*/

	nameMesh = ofMesh::plane(5.5, 1);
	

	tutorial.mesh = ofMesh::plane(10, 3., 500,500, OF_PRIMITIVE_TRIANGLES);
}
//--------------------------------------------------------------
void ofApp::selectMode() {
	puppet.selectable(true);
	cam.canRotate(true);
		ofDisableArbTex();
		//sceneShader.setUniformTexture("tex", backFBO.getTextureReference(), 0);

	

		puppet.setRotation(vec3{ 0,180,0 }); // reset rotation

		//ofEnableAlphaBlending();
		ofNode boxTransform;
		ofNode keyTransform;
		boxTransform.setScale(50.0f);

		//boxCoverMaterial.setAmbientColor(ofFloatColor(1.f, 0.09f));
		//boxCoverMaterial.setColors(ofFloatColor(1.f, 0.02f), ofFloatColor(1.f, 0.02f), ofFloatColor(1.f, 0.02f), ofFloatColor(1.f, 0.02f));

		boxMaterial.setAmbientColor(ofFloatColor(1.f, 0, 0));
		boxMaterial.setDiffuseColor(ofFloatColor(0.0f));
		boxMaterial.setSpecularColor(ofFloatColor(1.0f));
		boxMaterial.setShininess(30.f);
	
		keyTransform.setPosition(vec3{ -4.5f,-5.75f,0 });

		keyTransform.rotate(ofGetFrameNum() % 360, vec3{1,0,0});

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // look into what this does
		ofEnableLighting();
		

		boxTransform.transformGL();
					
					vec2 bCoords;
					float dist;
					vec3 sNormal;

					ofPushMatrix();
					for (auto& [key, value] : box) {
						
						if (value.tex.isAllocated()) {
							sceneShader.setUniform1f("isTex", true);
							sceneShader.setUniformTexture("tex", value.tex, 0);

							
							value.tex.bind();
								sceneShader.setUniform1f("isRod", key=="rods");
												
										sceneShader.setUniform1f("appTime", float(ofGetElapsedTimeMillis() / 1000.f));
										
										// rewrite
										if (key == "base") { 
											if (mousepicker.getRay().intersectsMesh(value.mesh, boxTransform.getGlobalTransformMatrix(), bCoords, dist, sNormal)) {
												if (!isClicked && ofGetMousePressed()) {
													isTyping = !isTyping;
													isClicked = true;
													
												}
												else if (!ofGetMousePressed()) {
											
													isClicked = false;
												
												}

										
												
												
												sceneShader.setUniform1f("isSelected", true);
												
											}
											
											
											
										
										};
										value.mesh.draw();

										sceneShader.setUniform1f("isSelected", false);
								
								sceneShader.setUniform1f("isRod", false);

								

							value.tex.unbind();

							sceneShader.setUniform1f("isTex", false);
						}
					}
					ofPopMatrix();

					keyTransform.transformGL();
						//puppetMat.begin();
						sceneShader.setUniform1f("isTex", true);

							for (auto &[name,part] : key) {
								
								if(part.tex.isAllocated()) sceneShader.setUniformTexture("tex", part.tex.getTexture(), 0);
								sceneShader.setUniform1f("isRod", name == "rod");
									part.mesh.draw();
								sceneShader.setUniform1f("isRod", false);
							}
							
						sceneShader.setUniform1f("isTex", false);

					
						//puppetMat.end();
					keyTransform.restoreTransformGL();


					


				
				sceneShader.setUniform1f("isTex", true);


				ofPushMatrix();
				

					ofRotateY(180);
					ofTranslate(vec3{ -0.25,0,-4.0 });

					sceneShader.setUniformTexture("tex", backFBO.getTexture(), 0);
					
					//backFBO.getTextureReference().bind();
						backMesh.draw();
					//backFBO.getTextureReference().unbind();



				//backFBO.getTextureReference().unbind();
				ofPopMatrix();
				
				sceneShader.setUniform1f("isTex", false);
				//boxCoverMaterial.begin();
				
				ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
				sceneShader.setUniform1f("isCover", true);
					boxCover.mesh.draw();
				sceneShader.setUniform1f("isCover", false);
				ofEnableAlphaBlending();

				//boxCoverMaterial.end();
				
				glDisable(GL_CULL_FACE);
				ofPushMatrix();
					ofPushStyle();
						ofRotateY(180);
						ofTranslate(0.5f, -5.75, 2.0);
						

						//ofDisableArbTex();
						//ofEnableNormalizedTexCoords();

						sceneShader.setUniform1f("isTex", true);
						sceneShader.setUniform1f("isText", true);
							sceneShader.setUniformTexture("tex", nameFbo.getTexture(), 0);
							nameMesh.draw();
							//ofDrawPlane(5.5, 1);

						sceneShader.setUniform1f("isText", false);
						sceneShader.setUniform1f("isTex", false);

						//ofScale(0.1);
						//

						//// !!!! use rectangle and dynamically set size
						//sceneShader.setUniform1f("hasColor", true);
						//	// this is wrong fix later
						//
						//	auto txtCol = isTyping ? vec3(0.8 + 0.2 *sin(float(ofGetElapsedTimeMillis()) / 1000.f)) : vec3(1); // use differnet function (make it blink)
	


						//	sceneShader.setUniform3f("setColor", txtCol);

						//	// add the cursor differently
						//	fontText.drawStringAsShapes(isTyping ? (name + "|") : name, 0, 0); // make cursor be positioned inline with text 
						//	
						//
						//
						//sceneShader.setUniform1f("hasColor", false);

					ofPopStyle();
				ofPopMatrix();
				glEnable(GL_CULL_FACE);
				
				
				glDisable(GL_CULL_FACE);
				ofPushMatrix();
				{
					vec2 bCoords;
					float dist;
					vec3 sNormal;

					ofNode transform;
					transform.setGlobalPosition(0, -8, -4);

					
					sceneShader.setUniform1f("isTutorial", true);
					//sceneShader.setUniform1f("appTime", float(ofGetElapsedTimeMillis()) / 10.f);


				/*	if (mousepicker.getRay().intersectsMesh(tutorial.mesh, transform.getGlobalTransformMatrix(), bCoords, dist, sNormal)) {
						cout << "over" << endl;
						sceneShader.setUniform1f("isSelected", true);
					}*/

					rollTime = ofClamp(rollStep + rollTime, 0, 1);


					sceneShader.setUniform1f("rollTime", getRollTime(rollTime));
					sceneShader.setUniform1f("isTex", true);
					sceneShader.setUniformTexture("tex", tutorialFbo.at("cur").getTexture(), 0);

					transform.transformGL();
					tutorial.mesh.draw();
				
					sceneShader.setUniform1f("isTex", false);

					sceneShader.setUniform1f("isTutorial", false);

					sceneShader.setUniform1f("isTex", true);
					sceneShader.setUniform1f("isRod", true);

					ofTranslate(0, 0.25, 0);
					ofRotateZ(90);
					sceneShader.setUniformTexture("tex", box.at("rods").tex.getTextureReference(), 0);
					ofDrawCylinder(0.25f, 10.5f);

					


					transform.restoreTransformGL();

					sceneShader.setUniform1f("isRod", false);

					sceneShader.setUniformTexture("tex", spike.tex.getTexture(), 0);
						// draw two spikes either side of the cylinder
						
						ofPushMatrix();
							ofPushMatrix();
								ofTranslate(vec3(6.25f,-7.75f,-4));
								spike.mesh.draw();
							ofPopMatrix();
							ofPushMatrix();
								ofTranslate(vec3(-6.25f,-7.75f,-4));
								ofRotateY(180);
								spike.mesh.draw();
							ofPopMatrix();

						ofPopMatrix();
					sceneShader.setUniform1f("isTex", false);
					//sceneShader.setUniform1f("isSelected", false);
				}
				ofPopMatrix();
				glEnable(GL_CULL_FACE);
		
	
		boxTransform.restoreTransformGL();


		
		ofDisableLighting();

		ofSetWindowPosition(ofGetScreenWidth() / 2.0f - ofGetWindowWidth() / 2.0f, ofGetScreenHeight() / 2.0f - ofGetWindowHeight() / 2.0f);
}

void ofApp::spaceMode() {
	
	puppet.selectable(false);
	cam.canRotate(true);
	puppet.setRotation(vec3{ 180,0,180 }); // reset the rotation

	ofSetWindowPosition(ofGetScreenWidth()/2.0f - ofGetWindowWidth() / 2.0f, ofGetScreenHeight()/2.0f - ofGetWindowHeight() / 2.0f);
	//glEnable(GL_CULL_FACE);
	// figure out how to cull

	//glDisable(GL_CULL_FACE);
	//glDisable(GL_CULL_FACE);
	world.draw();
	
	ofEnableAlphaBlending();
	glDisable(GL_CULL_FACE);
	ofPushMatrix();
	ofPushStyle();
	

	//ofDisableArbTex();
	//ofEnableNormalizedTexCoords();

	sceneShader.setUniform1f("isTex", true);
	sceneShader.setUniform1f("isText", true);
	sceneShader.setUniformTexture("tex", nameFbo.getTexture(), 0);



	// ofTranslate(100.f * nameMesh.getCentroid().x*100.f, 100.f + 100.f * nameMesh.getCentroid().y, 0);
	ofScale(100.f);
	ofRotateY(180);
	ofTranslate(0.f, 1.5f, 0);
	
	//cout << "centroid: " << nameMesh.getCentroid().x << endl;
	//ofTranslate(-0.5*fontText.getStringBoundingBox(name, 0, 0).getWidth()/220.f, 1.f, 0);


	//cout << "length of name: " << name.length() << endl;
	//cout << fontText.getStringBoundingBox(name,0,0).getWidth() << endl;
	ofSetColor(ofColor::red);
	nameMesh.draw();
	//ofDrawPlane(5.5, 1);

	sceneShader.setUniform1f("isText", false);
	sceneShader.setUniform1f("isTex", false);

	

	ofPopStyle();
	ofPopMatrix();
	glEnable(GL_CULL_FACE);
	//glEnable(GL_CULL_FACE);

}

void ofApp::desktopMode() {

	puppet.selectable(false);
	cam.canRotate(false);

	
	//glDisable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // look into what this does


	
	HWND hwnd = ofGetWin32Window();
	//set the window always-on-top 
	// if clicked throws you out 


	//::SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT);
	//::SetWindowPos(hwnd, HWND_TOPMOST, NULL, NULL, NULL, NULL, SWP_NOREPOSITION | SWP_NOSIZE);
	//::SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	
	GetCursorPos(&p);

	float d = 0.99f;

	ofSetWindowPosition((float(p.x) * (1.0f - d) + prevX * d) - ofGetWindowWidth() / 2.0f, (float(p.y) * (1.0f - d) + prevY * d) - ofGetWindowHeight() / 2.0f);


	prevX = (float(p.x) * (1.0f - d) + prevX * d);
	prevY = (float(p.y) * (1.0f - d) + prevY * d);


	vec3 rotTest = vec3{ p.x - (float(p.x) * (1.0f - d) + prevX * d),p.y - (float(p.y) * (1.0f - d) + prevY * d),500 };

	ofNode n;
	n.lookAt(rotTest);

	puppet.setRotation(n.getOrientationEuler());
	
	glDisable(GL_CULL_FACE);
	ofPushMatrix();
	ofPushStyle();


	//ofDisableArbTex();
	//ofEnableNormalizedTexCoords();

	sceneShader.setUniform1f("isTex", true);
	sceneShader.setUniform1f("isText", true);
	sceneShader.setUniformTexture("tex", nameFbo.getTexture(), 0);



	// ofTranslate(100.f * nameMesh.getCentroid().x*100.f, 100.f + 100.f * nameMesh.getCentroid().y, 0);
	ofScale(100.f);
	ofRotateY(180);
	ofTranslate(0.f, 1.5f, 0);

	//cout << "centroid: " << nameMesh.getCentroid().x << endl;
	//ofTranslate(-0.5*fontText.getStringBoundingBox(name, 0, 0).getWidth()/220.f, 1.f, 0);


	//cout << "length of name: " << name.length() << endl;
	//cout << fontText.getStringBoundingBox(name,0,0).getWidth() << endl;
	ofSetColor(ofColor::red);
	nameMesh.draw();
	//ofDrawPlane(5.5, 1);

	sceneShader.setUniform1f("isText", false);
	sceneShader.setUniform1f("isTex", false);



	ofPopStyle();
	ofPopMatrix();
	glEnable(GL_CULL_FACE);


	
	// breaks the mouth functionality 
	testSphere1.lookAt(rotTest);
	testSphere2.lookAt(rotTest);
	testSphere3.lookAt(rotTest);

}

float ofApp::getMouthAngle() {
	// check for waiting messages
	if (receiver.hasWaitingMessages()) {

		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(m);



		if (m.getAddress() == "/PID") {

			PID = m.getArgAsInt(1);

		}
		else if (m.getAddress() == "/mediapipe/hands"){
		// using 4n+1 for x
		// landmark locations via https://developers.google.com/mediapipe/solutions/vision/hand_landmarker

		if (m.getArgAsFloat(0)) {

			// if hands are detected 

			posThumb = ofVec3f{ m.getArgAsFloat(17), m.getArgAsFloat(18), m.getArgAsFloat(19) };
			posThumbKnuckle = ofVec3f{ m.getArgAsFloat(13), m.getArgAsFloat(14), m.getArgAsFloat(15) };
			posMiddle = ofVec3f{ m.getArgAsFloat(12 * 4 + 1), m.getArgAsFloat(12 * 4 + 2), m.getArgAsFloat(12 * 4 + 3) };
			posMiddleKnuckle = ofVec3f{ m.getArgAsFloat(9 * 4 + 1), m.getArgAsFloat(9 * 4 + 2), m.getArgAsFloat(9 * 4 + 3) };
			posPalm = ofVec3f{ m.getArgAsFloat(1), m.getArgAsFloat(2), m.getArgAsFloat(3) };
			return (-float((posThumb - posPalm).angle((posMiddle - posPalm))));
		}
		else {
		//	// return 0 if no hands are detected
			cout << "nothing detected" << endl;
			return 0;
		}
	}
	}
	else {
		return currAngle;
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofPushStyle();
		nameFbo.begin();


		

			ofClear(1,0,0,1.); // change alpha to 0
			//backTempTex.draw(0,0);
			//backTempTex.draw(0,0);
			ofSetColor(255);
			ofPushMatrix();
				auto s = 5.0f;
				//ofRotateY(180);
				ofTranslate(5.f*50.f*5.5/2.f - s*float(fontText.stringWidth(name))/2.f, 5.f * 50.f / 2.f + float(fontText.stringHeight(name)) / 2.f, 0.f);
				ofScale(s);
				//ofScale(s);
				//ofTranslate(5.5, 0, 0);
				//ofTranslate(-5*s*5.5*fontText.stringWidth(name)/220.f-5*s, 20, 0);
				//ofTranslate(-5*5.5*45/s+5, 20, 0);
				//ofTranslate(5 * 50*5.5, 0, 0);
				fontText.drawString(name, 0, 0);
				if (isTyping) {
					ofTranslate(fontText.stringWidth(name), 0, 0);
					ofSetColor(ofFloatColor(1, 8*fmod((float(ofGetElapsedTimeMillis()) / 1000.f),1.f)));
					fontText.drawString("|", 0, 0);
				}
			ofPopMatrix();
		nameFbo.end();
	ofPopStyle();


	ofPushStyle();
		tutorialFbo.at("next").begin();
		
			ofClear(0, 0, 0, 0);
			//backTempTex.draw(0,0);
			//backTempTex.draw(0,0);
			ofPushMatrix();
			ofRotateY(180);
			s = 2.5f;
			ofScale(s);
			ofTranslate(-5 * 5.5 * 45 / s + 5, 30, 0);
			box.at("spikes").tex.draw(-100, -100);
			sceneShader.setUniform1f("isText", true);
			ofPushMatrix();
				ofSetColor(0);
				ofTranslate(1, 1, 0);
				fontText.drawString(tutorialInfo.at(tutorialInfoIndex), 0, 0);
			ofPopMatrix();
			
			ofSetColor(255);
			fontText.drawString(tutorialInfo.at(tutorialInfoIndex), 0, 0);
			ofPopMatrix();
		sceneShader.setUniform1f("isText", false);
		tutorialFbo.at("next").end();
	ofPopStyle();

	ofPushStyle();


	

	transition.begin();

	tutorialFbo.at("cur").begin();
		ofClear(0, 0, 0, 0);
		cam2.begin();
		ofClear(0, 0, 0, 0);

		transition.setUniformTexture("prevScene", tutorialFbo.at("prev").getTexture(),0);
		transition.setUniformTexture("curScene", tutorialFbo.at("next").getTexture(),1);
		transition.setUniform1f("t", rollTime);

		ofPushMatrix();
		ofScale(800.f, 500.f);
		ofRotateY(180);
		tutorial.mesh.draw();
		ofPopMatrix();
		cam2.end();

	tutorialFbo.at("cur").end();
	ofPopStyle();

	transition.end();


	//glm::quat q = glm::quatLookAt(glm::normalize(glm::vec3{ p.x - (float(p.x) * (1.0f - 0.99f) + prevX * 0.99f),p.y - (float(p.y) * (1.0f - 0.99f) + prevY * 0.99f),-500 }), glm::vec3{ 0,1,0 }); // add as a constant

	//glm::vec3 rotT = glm::eulerAngles(q);


	for (auto &p : particles) {

		p.size = ofClamp(float(p.size-0.1), 0.f, 10.f);
		p.life -= 0.05;
		
		
		// use a transformation matrix (Figure out how to rotate particles)

	
		p.location += p.velocity;



		if (p.life < 0) {
			p = Particle{ ofRandom(5,10),ofRandom(5,10),testSphere3.getGlobalPosition() - vec3{ofRandom(-10,10),180,ofRandom(-10,10)},vec3{0,ofRandom(-1,-0.1),0}};
		}

		
		//else {
			//particles.erase(std::remove(particles.begin(), particles.end(), p.life), particles.end());
		//}

	}


	// figure out how to reset mouth angle
	// somehow sync this up 

	
	currAngle = getMouthAngle();

	cout << currAngle << endl;
	/*
	
		// --- Puppet Only Mouth Controlls ---

	*/

	//testSphere1.rotateDeg(-currAngle - prevAngle, ofVec3f(1.f, 0.f, 0.f));

	// make puppet look at mouse pointer (figur otu how to combine with mouse)

	float d = 0.99f;
	vec3 rotTest = vec3{ p.x - (float(p.x) * (1.0f - d) + prevX * d),p.y - (float(p.y) * (1.0f - d) + prevY * d),500 };
	
	// rotating from a different point
	testSphere1.rotateAroundDeg(-currAngle - prevAngle, ofVec3f(1.f, 0.f, 0.f), ofVec3f{0,0,50.f});
	testSphere1.rotateDeg(-currAngle - prevAngle, ofVec3f(1.f, 0.f, 0.f));
	
	// change this
	if (currAngle != 0) {
		prevAngle = currAngle; //float((posThumb-posPalm).angle(posMiddle-posPalm));
	}

	ofEnableLighting();
	testLight.enable();
	
	// back texture
		backFBO.begin();
		ofClear(ofFloatColor(0,0.8f,0.f,1.f));




			cam2.begin();
			ofPushMatrix();
				ofRotateZ(-90);
				ofTranslate(vec3{ 165,30,0 });


				backTempTex.bind();
				ofDrawPlane(800, 600);
				backTempTex.unbind();

				ofScale(0.8);
				
				// fix depth test maybe use stencil buffer
				
				puppet.showText(true);
				ofPushStyle();
				ofSetColor(0.f);
				puppet.draw();
				ofPopStyle();
				puppet.showText(false);
				
			ofPopMatrix();
			cam2.end();
			
		
				

		backFBO.end();

		ofDisableLighting();
		testLight.disable();

		ofPushStyle();

		boxBaseFbo.begin();

			boxBaseTex.draw(0, 0);
			ofSetColor(ofColor::black);
			fontText.drawString(name, 5, 5);
			ofSetColor(ofColor::white);
			fontText.drawString(name, 0, 0);

		boxBaseFbo.end();
		ofPopStyle();

		ofPixels p;

		boxBaseFbo.readToPixels(p);
		box.at("base").tex.setFromPixels(p);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofEnableNormalizedTexCoords();
	
	ofClear(0, 0, 0, 0);


	
	//glEnable(GL_CULL_FACE);


	curSceneFBO.begin();

	sceneShader.begin();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	ofEnableLighting();

	ofEnableAlphaBlending();
	
	ofEnableDepthTest();
	testLight.enable();


	
		
		cam.cam().begin();
		ofClear(0, 0, 0, 0);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_DEPTH_BUFFER);


		ofLight* lptr = world.getSunLightPtr();

		lptr->enable();
		 
	
		
		// have in puppet class
		for (auto& p : particles) {
			if (p.life > 0) {
				
				particleModel = ofMesh::sphere(p.size, 10);

					ofPushMatrix();
						sceneShader.setUniform1f("isTex", true);
						
						sceneShader.setUniformTexture("tex", box.at("rods").tex, 0);
						ofTranslate(p.location);
						particleModel.drawFaces();
						sceneShader.setUniform1f("isTex", false);

					ofPopMatrix();

			}
			
		}


		


		// !!!! ---- Maybe Keep this Enabled for Lighting ---- !!!!!
		//glEnable(GL_COLOR_MATERIAL);
		//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

		//---- Working With Puppet via Classes ---- //
		
			if (!(Mode::space == curMode)) {
				//ofClear(0.0f, 0.0f, 0.0f, 0.0f);
				//glClear(GL_COLOR_BUFFER_BIT);
			}
			ofPushStyle();
			

				
				puppet.draw();

				// 
				/*ofPushMatrix();
				glDisable(GL_CULL_FACE);
				ofTranslate(0, 0, -200.f);
					sceneShader.setUniform1f("isTex", false);
					sceneShader.setUniform1f("isRod", true);
					ofSetColor(0, 1.f, 0);
					ofDrawSphere(100.f);
					sceneShader.setUniform1f("isRod", false);
				glEnable(GL_CULL_FACE);
				ofPopMatrix();*/
			ofPopStyle();

		vec2 baricentricCoords;
		vec3 surfaceNormal;
		float distance;

		switch (curMode) {
		case Mode::select:
			selectMode(); // figure out better way to do transparency
			break;
		case Mode::space:
			spaceMode();
			break;
		case Mode::desktop:
			desktopMode();
			break;
		}

	 //glDisable(GL_COLOR_MATERIAL);
	
		lptr->disable();

		cam.cam().end();
	
		glDisable(GL_CULL_FACE);


		sceneShader.end();

	curSceneFBO.end();
		

	glDisable(GL_CULL_FACE);

	cam2.begin();


	cam2.end();


	cam2.begin();

	transition.begin();
	ofPushMatrix();
		//ofTranslate(-curSceneFBO.getWidth() / 2, -curSceneFBO.getHeight() / 2);
		

	

		ofTexture curTex = curSceneFBO.getTexture();
		ofTexture prevTex = prevSceneFBO.getTexture();

		transition.setUniformTexture("curScene", curTex, 0);
		transition.setUniformTexture("prevScene", prevTex, 1);

		// original at 0.01
		transitionTime = ofClamp(transitionTime + 0.02, 0, 1);

		transition.setUniform1f("t", transitionTime);

		prevTex.bind();
			curTex.bind();
				// figure out correct scaling
				ofTranslate(0, 0,-350);
				screen.draw();
			curTex.unbind();
		prevTex.unbind();

	ofPopMatrix();
	transition.end();


	cam2.end();



	testLight.disable();

	ofDisableLighting();

	ofDisableDepthTest();

	



	

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//cout << key << endl;

	//if (key == 49) {

	//


	//	transitionSound.play();

	//	prevSceneFBO.begin();
	//		ofClear(0, 0, 0, 0);
	//		curSceneFBO.draw(0, 0);
	//	prevSceneFBO.end();

	//	
	//	transitionTime = 0;
	//	
	//	//cam.removeInteraction(ofEasyCam::TRANSFORM_SCALE,0);
	//	

	//	
	//	
	//	// figure out way to not allow zoom
	//	curMode = Mode::select;
	//	
	//	cam.reset();
	//	
	//
	//}

	/*if (key == '2') {
		isTyping = false;

		prevSceneFBO.begin();
		ofClear(0, 0, 0, 0);
		curSceneFBO.draw(0, 0);
		prevSceneFBO.end();

		transitionTime = 0;

		curMode = Mode::space;

		cam.reset();

	}*/

	if (curMode == Mode::select && key == 13 && (tutorialInfoIndex == (tutorialInfo.size()-1)) && (rollTime == 1)) {
		/*
		
			---- Transition to Space Mode ----
		
		*/

		
		track.startThread();
		

		transitionSound.play();
		isTyping = false;

		prevSceneFBO.begin();
			ofClear(0, 0, 0, 0);
			curSceneFBO.draw(0, 0);
		prevSceneFBO.end();

		transitionTime = 0;
		
		curMode = Mode::space;
		
		cam.reset();
	}

	if (key == 51) {

		/*
		
			---- Transition to Desktop Mode ----
		
		*/


		if (PID && track.isThreadRunning()) {
			ofSystem("taskkill /F /PID " + std::to_string(PID));
		}

		track.stopThread();

		transitionSound.play();
		isTyping = false;

		prevSceneFBO.begin();
			ofClear(0, 0, 0, 0);
			curSceneFBO.draw(0,0);
		prevSceneFBO.end();

		transitionTime = 0;

		// fix glitchy window
		prevX = ofGetScreenWidth() / 2.0f;
		prevY = ofGetScreenHeight() / 2.0f;


		//p.x = ofGetScreenWidth() / 2.0f - ofGetWindowWidth() / 2.0f;
		//p.y = ofGetScreenHeight() / 2.0f - ofGetWindowHeight() / 2.0f;

		cam.cam().setPosition(vec3{ 0,0,-1200 });
		cam.cam().lookAt(vec3{ 0,0,0 });
		curMode = Mode::desktop;

		//puppetVoice.speak("you have awakened me!");

		cam.reset();

	}

	if (key == 13 && rollTime == 1) {
		rollTime = 0;
		tutorialFbo.at("prev").begin();
			tutorialFbo.at("cur").draw(0,0);
		tutorialFbo.at("prev").end();
		tutorialInfoIndex = std::min(tutorialInfoIndex + 1, int(tutorialInfo.size()-1));
		//cout << tutorialInfoIndex << endl;
	}

	
	if (isTyping) {

		// stop shift and other special keys from working
		if (key != ofKey:: OF_KEY_LEFT_SHIFT) {
			if (key == 8 && name.length() > 0) {
				name.pop_back();
			}
			else if (name.length() < 12 && key <= 255) {
				name += char(key);
			}
		}

		//cout << name << endl;
	}




}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

	mousepicker.setFromCamera(glm::vec2(x, y), cam.cam());

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

	
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
//-------------------------------------------------------------
void ofApp::exit() {

	//SystemParametersInfo(SPI_SETCURSORS, 0, nullptr, 0); // changes cursors to defaults
	;

	// add this to stopTracking
	
	if (PID && track.isThreadRunning()) {
		ofSystem("taskkill /F /PID " + std::to_string(PID));
	}

	track.stopThread(); // figure out how to stop thread
	track.stopTracking();

	std::exit(0);

}

