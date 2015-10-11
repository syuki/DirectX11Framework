
struct VsIn {
  float4 pos : POSITION;
  float4 col : COLOR0;
};

struct VsOut {
  float4 pos : SV_POSITION;
  float4 col : COLOR;
};

cbuffer ConstantBuffer {
  row_major matrix World;
  row_major matrix View;
  row_major matrix Projection;
};

VsOut main(VsIn arg) {

  VsOut ret;

  matrix wvp = mul(World, mul(View, Projection));
  ret.pos = mul(arg.pos, wvp);
  ret.col = arg.col;

  return ret;
}