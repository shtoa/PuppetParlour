#include "handTrack.h"

void handTrack::threadedFunction() {
	std::string command = "py -3.9 handTest.py --input 0";
	ofSystem(command.c_str());
}

// figure out way to stop file thread

void handTrack::stopTracking() {
	std::string command = "";
	
	
	// figure out how to find PID 
	//ofSystem("taskkill /F /IM python.exe");

	//cout << "stopped tracking (test)" << endl;

}