#include "puppet.h"


Puppet::Puppet(ofxraycaster::Mousepicker& _mousepicker, float& _mouthAngle)
	: mouthAngle(_mouthAngle)

{


	for (auto part : partDirs) {
		string path = "./puppets/" + part + "/models/";
		ofDirectory dir(path);
		dir.allowExt("obj");
		dir.listDir();

		//partMeshes.insert({ part, vector<ofMesh>{} });
		//partIndices.insert({ part, 0 });

		puppetParts.parts.insert({ part, {} });
		puppetParts.indices.insert({ part, 0 });
		auto index = 0;

		for (auto f : dir) {

			try {


				if (!modelLoader.loadModel(f.getAbsolutePath().c_str(), true)) {
					cout << "failed to loud: " + f.getAbsolutePath() << endl;
				}

				if (modelLoader.getNumMeshes() > 0) {

					PuppetPart*p = new PuppetPart{ _mousepicker, transform };

					m = modelLoader.getMesh(0);
					p->setModel(m);
					p->setIndex(index);
					p->setName(f.getBaseName());
					
					ofImage im;
					if (!im.load("./puppets/" + part + "/textures/" + f.getBaseName() + ".png")) {
						cout << "failed to load" << endl;
						cout << "./puppets/" + part + "/textures/" + f.getBaseName() + ".png" << endl;
					};
					
					p->setTex(im);
					++index;

					puppetParts.parts.at(part).push_back(p);


				}
			}
			catch (...) {
				cout << "error: " << f.getFileName() << endl;
			}






		}

		puppetParts.selected.insert({ part, puppetParts.parts.at(part).at(0) });

	}

	// !!!!CHANGE TO ENUMS!!!!! 

	auto zDist = 75;

	puppetParts.translations.insert({"headTop", vec3{ 75,0,zDist }});
	puppetParts.translations.insert({"headBottom", vec3{ -150,-20,zDist } });
	puppetParts.translations.insert({"lArm", vec3{ -150,-75,zDist } });
	puppetParts.translations.insert({"rArm", vec3{ 75,-50,zDist } });
	puppetParts.translations.insert({"body", vec3{ 75,-100,zDist } });

	selectSound.load("teleport.mp3");

	// add fail safe if not 0

	setRotation(vec3{ 0,180,0 });


	// font by kiwiSoda https://www.dafont.com/kiwisoda.font
	font.loadFont("Minecraft.ttf", 15,true,false, true);

}


Puppet::~Puppet() {

	for (auto &[name,part] : puppetParts.parts) {
		for (auto* ptr : part) {
			delete ptr;
		}

	}

	for (auto &[name,part] : puppetParts.selected) {
		delete part;
	}


}

void Puppet::setRotation(vec3 _rotation) {
	rotation = _rotation;
}
void Puppet::setLocation(vec3 _location) {
	location = _location;
}


void Puppet::selectPart() {
	

	// add intersecting condition
	auto minPart = *min_element(std::begin(puppetParts.selected), std::end(puppetParts.selected), [](auto& l, auto& r) {return 
		(l.second->getIntersecting() && l.second->getIntersecting()) && (l.second->getDistance() < r.second->getDistance()); });


	for (auto& [name, part] : puppetParts.selected) {
		
		// beautify this

		//cout << minPart.first << endl;

		part->setSelected(minPart.first == name && part->getIntersecting());

	}
	
	

}

void Puppet::draw() {

	transform.setPosition(location);
	transform.setOrientation(rotation);

	if (isSelectable) {
		selectPart();
	};


	

	// select part part2;

	for (auto [name, part] : puppetParts.selected) {
		
		if (isFirstPress && puppetParts.selected.at(name)->getSelected() && ofGetMousePressed() && isSelectable) {

			selectSound.play();

			puppetParts.indices.at(name) = (puppetParts.indices.at(name) + 1) % (puppetParts.parts.at(name).size());

			puppetParts.selected.erase(puppetParts.selected.find(name));
			puppetParts.selected.insert({ name, puppetParts.parts.at(name).at(puppetParts.indices.at(name)) });
			
			isFirstPress = false;

			break;
		}
		

		if (!ofGetMousePressed()) {
			isFirstPress = true;
		}

	}


	transform.setScale(1.2); // figure best scale

	transform.transformGL();
	ofPushStyle(); // remove after adding textures
	//ofSetColor(ofFloatColor(0.5, 0, 0));

		// draw the head
		ofPushMatrix();
			ofRotateX(mouthAngle);
			puppetParts.selected.at("headTop")->draw();
		ofPopMatrix();
		puppetParts.selected.at("headBottom")->draw();

		// draw arms 
		puppetParts.selected.at("lArm")->draw();
		puppetParts.selected.at("rArm")->draw();

		// draw body
		puppetParts.selected.at("body")->draw();
	ofPopStyle();

	//ofDisableDepthTest();
	
	// draw the part names
	if (isTextShowing) {
		for (auto [name, part] : puppetParts.selected) {
			//if (puppetParts.selected.at(name).getSelected()) {


			ofPushStyle();
			ofPushMatrix();

			// translations based on part

			// switch names to enums 


			ofTranslate(puppetParts.translations.at(name));

			bool isSelect = puppetParts.selected.at(name)->getSelected();

			//if (isSelect) { glDisable(GL_CULL_FACE); };

			// add conditional to draw text
			auto txt = name + '\n' + part->getName();
			// add centered location

			ofSetColor(ofColor::black);
			font.drawString(txt, 5, 5);

			ofSetColor(ofColor::white);
			font.drawString(txt, 0, 0);
			//if (isSelect) { glEnable(GL_CULL_FACE); };

			ofPopMatrix();
			ofPopStyle();

			//}

		}
	}

	//ofEnableDepthTest();


	transform.restoreTransformGL();
}
