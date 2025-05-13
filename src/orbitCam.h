#include "ofMain.h"

using namespace glm;

class OrbitCam {

	public:

		OrbitCam();
		~OrbitCam();
	
		void mouseDragged(ofMouseEventArgs& e);
		void mousePressed(ofMouseEventArgs& e);

		void canRotate(bool _canCamRotate) {
			canCamRotate = _canCamRotate;
		}

		void reset();

		ofCamera& cam() { return orbitCam; }

	
	private:

		ofCamera orbitCam;


		

		// redo with references
		struct {
			vec2 prev = vec2{ 0,0 };
			vec2 cur = vec2{ 0,0 };
			vec2 delta;
			void update(int x, int y) {

				prev = cur;
				cur = vec2{ x,y };

				delta = cur - prev;
			}
		} mousePos;

		// store in camera
		float xAng = 0;
		float yAng = 0;

		bool canCamRotate = true;

};