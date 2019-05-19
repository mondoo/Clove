cbuffer Cbuff{
	matrix model;
	matrix view;
	matrix projection;

	//matrix transform;
};

float4 main(float3 pos : Position) : SV_Position{
	//return mul(float4(pos, 1.0f),  model * /*view **/ projection);
	//matrix mvp = mul(model, projection);
	//matrix mvp = model * /*view **/ projection;
	//matrix mvp = projection * model;
	//matrix mvp = mul(model, mul(view, projection));

	matrix mvp = mul(projection, mul(view, model));
	return mul(mvp, float4(pos, 1.0f));

	//return mul(float4(pos, 1.0f), mvp);
	//return mul(transform, float4(pos, 1.0f));
}