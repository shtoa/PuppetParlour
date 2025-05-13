#pragma once

#include "ofMain.h"
#include <cstdlib>

class handTrack : public ofThread
{
	public:


		void threadedFunction() override;
		void stopTracking();

	private:
};

