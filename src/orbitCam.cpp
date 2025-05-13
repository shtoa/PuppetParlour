#include "orbitCam.h"

OrbitCam::OrbitCam() {

	reset();

	orbitCam.setPosition(1200 * vec3{
	cos(xAng) * sin(yAng),
	cos(yAng),
	sin(xAng) * sin(yAng)
		});

	orbitCam.lookAt(vec3{ 0,0,0 });
	orbitCam.setFov(60.f);

	
	ofAddListener(ofEvents().mouseDragged, this, &OrbitCam::mouseDragged);
	ofAddListener(ofEvents().mousePressed, this, &OrbitCam::mousePressed);

}


OrbitCam::~OrbitCam() {
	ofRemoveListener(ofEvents().mouseDragged, this, &OrbitCam::mouseDragged);
	ofRemoveListener(ofEvents().mousePressed, this, &OrbitCam::mousePressed);
}


void OrbitCam::reset() {
	yAng = PI / 2;
	xAng = -PI / 2;

	orbitCam.lookAt(vec3{ 0,0,0 });
}

void OrbitCam::mouseDragged(ofMouseEventArgs & e) {
	if (canCamRotate) {
		auto x = e.x;
		auto y = e.y;

		mousePos.update(x, y);

		// change to struct
		auto dist = glm::length(orbitCam.getPosition());

		// change from lambda function
		auto calcFlip = [&] {

			if (yAng > 0) {
				return (std::fmod(abs(glm::degrees(yAng)), 360) > 180);
			}
			else {
				return (std::fmod(abs(glm::degrees(yAng)), 360) < 180);
			}

			};


		// fix this to correctly rotate when going past threshold 
		yAng += mousePos.delta.y / 400;


		// change to deg
		bool isFlip = calcFlip();


		// only do this if the starting loc doesnt flip
		xAng += mousePos.delta.x / 400;

		// ((isFlip) ? -1 : 1) * 

		orbitCam.setPosition(dist * vec3{
			cos(xAng) * sin(yAng),
			-cos(yAng),
			sin(xAng) * sin(yAng)
			});



		// can set up vector inside camera
		orbitCam.lookAt(vec3{ 0,0,0 }, vec3{ 0,((isFlip) ? -1 : 1),0 });
	}
};

void OrbitCam::mousePressed(ofMouseEventArgs& e) {
	
	auto x = e.x;
	auto y = e.y;


	mousePos.cur = vec2{ x, y };

};

