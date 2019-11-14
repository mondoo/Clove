#version 460 core

layout(location = 0) in vec2 Position2D;
layout(location = 1) in vec2 TexCoord;

out vec2 vertTexCoord;

void main(){
	gl_Position = vec4(Position2D.x, Position2D.y, 0.0f, 1.0f);

	vertTexCoord = TexCoord;
}