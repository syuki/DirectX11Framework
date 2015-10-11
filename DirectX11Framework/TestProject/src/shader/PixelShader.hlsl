
struct PsIn {
	float4 Position : SV_POSITION;
  float4 col : COLOR;
};

float4 main(PsIn arg) : SV_Target0 {
  return arg.col;
}