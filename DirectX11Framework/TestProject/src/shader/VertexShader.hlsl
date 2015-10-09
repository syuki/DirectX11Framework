
struct VsIn {
  float4 pos : POSITION;
};

struct VsOut {
  float4 pos : SV_POSITION;
};

VsOut main(VsIn arg) {

  VsOut ret;
  ret.pos = arg.pos;

  return ret;
}