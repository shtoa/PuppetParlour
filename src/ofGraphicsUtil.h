#include <string>
#include "ofShader.h"

/*
This file contains a collection of utilities to extend/simplify OF graphics.

Lance Putnam, 2023
*/

/// Push matrix, call function, then pop matrix
template <class Func>
static void matrixScope(const Func& f){
	ofPushMatrix();
	f();
	ofPopMatrix();
}

/// For any object with begin/end, calls its begin, then a function, then its end
template <class T, class Func>
static void scope(T& obj, const Func& f){
	obj.begin();
	f();
	obj.end();
}

/// Compiles shader vertex and fragment source and then links them

/// @param[in] s			Shader to build
/// @param[in] vs		Vertex program source
/// @param[in] fs		Fragment program source
/// @param[in] version	GLSL version to use for both programs
static bool build(ofShader& s, const std::string& vs, const std::string& fs, const std::string& version = "150"){
	auto preamble = "#version " + version + "\n";
	return
		s.setupShaderFromSource(GL_VERTEX_SHADER, (preamble + vs).c_str()) &&
		s.setupShaderFromSource(GL_FRAGMENT_SHADER, (preamble + fs).c_str()) &&
		s.bindDefaults() &&
		s.linkProgram()
	;
}
