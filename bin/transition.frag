#version 150

// author: bobylito
// license: MIT
const float SQRT_2 = 1.414213562373;
uniform float dots;// = 20.0;
uniform vec2 center;// = vec2(0, 0);
uniform float progress;
uniform vec2 


out vec4 FragColor;
in vec4 vertexColor;


// vec4 transition(vec2 uv) {
//  bool nextImage = distance(fract(uv * dots), vec2(0.5, 0.5)) < ( progress / distance(uv, center));
//  return nextImage ? getToColor(uv) : getFromColor(uv);
//}


main(){

	FragColor = vertexColor;
	
}