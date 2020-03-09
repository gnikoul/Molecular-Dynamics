#version 410 core
//FLAME SHADER
#ifdef GL_ES
precision mediump float;
#endif
uniform float iTime;
out vec4 color;

void main()
{
	
	color = vec4(1.0, 1.0, 1.0, 1.0);
	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}