
struct GSPSInput {
	float4 Position : SV_POSITION;
};

float4 main(GSPSInput output) : SV_Target0 {
  return float4(1.0f, 1.0f, 1.0f, 1.0f);
}


