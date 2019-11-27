//
//  File.metal
//  Bullet2FileLoader
//
//  Created by Alex Charlwood on 23/11/2019.
//

#include <metal_stdlib>
using namespace metal;

struct Constants{
	float animateBy;
};

struct VertexIn{
	float4 position [[ attribute(0) ]]; //Attribute = the element in the descriptor array
	float4 colour [[ attribute(1) ]];
};

struct VertexOut{
	float4 position [[ position ]];
	float4 colour;
};

vertex VertexOut vertex_shader(const VertexIn vertexIn [[ stage_in ]], constant Constants& constants [[ buffer(1) ]]){
	VertexOut vertexOut;
	vertexOut.position = vertexIn.position;
	vertexOut.colour = vertexIn.colour;
	
	vertexOut.position.x += constants.animateBy;
	
	return vertexOut;
}

fragment half4 fragment_shader(VertexOut vertexIn [[ stage_in ]]){
	return half4(vertexIn.colour);
}
