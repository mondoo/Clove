//
//  File.metal
//  Bullet2FileLoader
//
//  Created by Alex Charlwood on 23/11/2019.
//

#include <metal_stdlib>
using namespace metal;

vertex float4 vertex_shader(const device packed_float3* vertices [[ buffer(0) ]], uint vertexID [[ vertex_id ]]){
	return float4(vertices[vertexID], 1);
}

fragment half4 fragment_shader(){
	return half4(1, 0, 0, 1);
}
