#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Buffer Definitions: 
//
// cbuffer cbSprite
// {
//
//   row_major float4x4 g_mTransform;   // Offset:    0 Size:    64 [unused]
//   float4 g_color;                    // Offset:   64 Size:    16
//   float g_texel;                     // Offset:   80 Size:     4
//   float g_value;                     // Offset:   84 Size:     4
//   float g_pow;                       // Offset:   88 Size:     4
//   int g_EdgeParamEnable;             // Offset:   92 Size:     4
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// g_ss                              sampler      NA          NA             s0      1 
// g_inputTex                        texture  float4          2d             t0      1 
// cbSprite                          cbuffer      NA          NA            cb0      1 
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_Position              0   xyzw        0      POS   float       
// TEXCOORD                 0   xy          1     NONE   float   xy  
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_Target                0   xyzw        0   TARGET   float   xyzw
//
ps_5_0
dcl_globalFlags refactoringAllowed
dcl_constantbuffer CB0[6], immediateIndexed
dcl_sampler s0, mode_default
dcl_resource_texture2d (float,float,float,float) t0
dcl_input_ps linear v1.xy
dcl_output o0.xyzw
dcl_temps 4
if_nz cb0[5].w
  resinfo_indexable(texture2d)(float,float,float,float) r0.xy, l(0), t0.xyzw
  div r0.xy, cb0[5].xxxx, r0.xyxx
  sample_indexable(texture2d)(float,float,float,float) r1.x, v1.xyxx, t0.xyzw, s0
  mov r2.xy, -r0.xyxx
  add r1.yz, r2.xxyx, v1.xxyx
  sample_indexable(texture2d)(float,float,float,float) r1.y, r1.yzyy, t0.yxzw, s0
  mov r2.z, l(0)
  add r2.xyzw, r2.zyxz, v1.xyxy
  sample_indexable(texture2d)(float,float,float,float) r1.z, r2.xyxx, t0.yzxw, s0
  mov r0.z, -r0.y
  add r3.xyzw, r0.xzxy, v1.xyxy
  sample_indexable(texture2d)(float,float,float,float) r0.z, r3.xyxx, t0.yzxw, s0
  sample_indexable(texture2d)(float,float,float,float) r1.w, r2.zwzz, t0.yzwx, s0
  mov r0.w, l(0)
  add r2.xyzw, r0.xwwy, v1.xyxy
  sample_indexable(texture2d)(float,float,float,float) r0.w, r2.xyxx, t0.yzwx, s0
  mad r0.xy, r0.xyxx, l(-1.000000, 1.000000, 0.000000, 0.000000), v1.xyxx
  sample_indexable(texture2d)(float,float,float,float) r0.x, r0.xyxx, t0.xyzw, s0
  sample_indexable(texture2d)(float,float,float,float) r0.y, r2.zwzz, t0.yxzw, s0
  sample_indexable(texture2d)(float,float,float,float) r2.x, r3.zwzz, t0.xyzw, s0
  add r1.yz, -r1.xxxx, r1.yyzy
  max r1.yz, r1.yyzy, l(0.000000, 0.000000, 0.000000, 0.000000)
  div_sat r1.yz, r1.yyzy, cb0[5].yyyy
  add r1.y, r1.z, r1.y
  add r0.xyzw, -r1.xxxx, r0.xyzw
  max r0.xyzw, r0.xyzw, l(0.000000, 0.000000, 0.000000, 0.000000)
  div_sat r0.xyzw, r0.xyzw, cb0[5].yyyy
  add r0.z, r0.z, r1.y
  add r1.y, -r1.x, r1.w
  max r1.y, r1.y, l(0.000000)
  div_sat r1.y, r1.y, cb0[5].y
  add r0.z, r0.z, r1.y
  add r0.z, r0.w, r0.z
  add r0.x, r0.x, r0.z
  add r0.x, r0.y, r0.x
  add r0.y, -r1.x, r2.x
  max r0.y, r0.y, l(0.000000)
  div_sat r0.y, r0.y, cb0[5].y
  add r0.x, r0.y, r0.x
  log r0.x, r0.x
  mul r0.x, r0.x, cb0[5].z
  exp o0.w, r0.x
  mov o0.xyz, l(0,0,0,0)
  ret 
else 
  sample_indexable(texture2d)(float,float,float,float) r0.xyzw, v1.xyxx, t0.xyzw, s0
  lt r1.x, r0.w, l(0.100000)
  discard_nz r1.x
  mul o0.xyzw, r0.xyzw, cb0[4].xyzw
  ret 
endif 
ret 
// Approximately 53 instruction slots used
#endif

const BYTE compiledBuffer[] =
{
     68,  88,  66,  67, 153,  63, 
    215, 255, 148, 165,  69, 211, 
    176, 223, 138,  67, 217,  32, 
     22, 213,   1,   0,   0,   0, 
    172,  10,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
     16,   3,   0,   0, 104,   3, 
      0,   0, 156,   3,   0,   0, 
     16,  10,   0,   0,  82,  68, 
     69,  70, 212,   2,   0,   0, 
      1,   0,   0,   0, 184,   0, 
      0,   0,   3,   0,   0,   0, 
     60,   0,   0,   0,   0,   5, 
    255, 255,   0,   1,   0,   0, 
    172,   2,   0,   0,  82,  68, 
     49,  49,  60,   0,   0,   0, 
     24,   0,   0,   0,  32,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
    156,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 161,   0,   0,   0, 
      2,   0,   0,   0,   5,   0, 
      0,   0,   4,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     13,   0,   0,   0, 172,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
    103,  95, 115, 115,   0, 103, 
     95, 105, 110, 112, 117, 116, 
     84, 101, 120,   0,  99,  98, 
     83, 112, 114, 105, 116, 101, 
      0, 171, 171, 171, 172,   0, 
      0,   0,   6,   0,   0,   0, 
    208,   0,   0,   0,  96,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 192,   1, 
      0,   0,   0,   0,   0,   0, 
     64,   0,   0,   0,   0,   0, 
      0,   0, 216,   1,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 252,   1,   0,   0, 
     64,   0,   0,   0,  16,   0, 
      0,   0,   2,   0,   0,   0, 
     12,   2,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     48,   2,   0,   0,  80,   0, 
      0,   0,   4,   0,   0,   0, 
      2,   0,   0,   0,  64,   2, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 100,   2, 
      0,   0,  84,   0,   0,   0, 
      4,   0,   0,   0,   2,   0, 
      0,   0,  64,   2,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 108,   2,   0,   0, 
     88,   0,   0,   0,   4,   0, 
      0,   0,   2,   0,   0,   0, 
     64,   2,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    114,   2,   0,   0,  92,   0, 
      0,   0,   4,   0,   0,   0, 
      2,   0,   0,   0, 136,   2, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 103,  95, 
    109,  84, 114,  97, 110, 115, 
    102, 111, 114, 109,   0, 102, 
    108, 111,  97, 116,  52, 120, 
     52,   0, 171, 171,   2,   0, 
      3,   0,   4,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    205,   1,   0,   0, 103,  95, 
     99, 111, 108, 111, 114,   0, 
    102, 108, 111,  97, 116,  52, 
      0, 171,   1,   0,   3,   0, 
      1,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   4,   2, 
      0,   0, 103,  95, 116, 101, 
    120, 101, 108,   0, 102, 108, 
    111,  97, 116,   0, 171, 171, 
      0,   0,   3,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  56,   2,   0,   0, 
    103,  95, 118,  97, 108, 117, 
    101,   0, 103,  95, 112, 111, 
    119,   0, 103,  95,  69, 100, 
    103, 101,  80,  97, 114,  97, 
    109,  69, 110,  97,  98, 108, 
    101,   0, 105, 110, 116,   0, 
      0,   0,   2,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 132,   2,   0,   0, 
     77, 105,  99, 114, 111, 115, 
    111, 102, 116,  32,  40,  82, 
     41,  32,  72,  76,  83,  76, 
     32,  83, 104,  97, 100, 101, 
    114,  32,  67, 111, 109, 112, 
    105, 108, 101, 114,  32,  49, 
     48,  46,  49,   0,  73,  83, 
     71,  78,  80,   0,   0,   0, 
      2,   0,   0,   0,   8,   0, 
      0,   0,  56,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  15,   0, 
      0,   0,  68,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   0,   0,   3,   3, 
      0,   0,  83,  86,  95,  80, 
    111, 115, 105, 116, 105, 111, 
    110,   0,  84,  69,  88,  67, 
     79,  79,  82,  68,   0, 171, 
    171, 171,  79,  83,  71,  78, 
     44,   0,   0,   0,   1,   0, 
      0,   0,   8,   0,   0,   0, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,   0,   0,   0, 
     83,  86,  95,  84,  97, 114, 
    103, 101, 116,   0, 171, 171, 
     83,  72,  69,  88, 108,   6, 
      0,   0,  80,   0,   0,   0, 
    155,   1,   0,   0, 106,   8, 
      0,   1,  89,   0,   0,   4, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   6,   0,   0,   0, 
     90,   0,   0,   3,   0,  96, 
     16,   0,   0,   0,   0,   0, 
     88,  24,   0,   4,   0, 112, 
     16,   0,   0,   0,   0,   0, 
     85,  85,   0,   0,  98,  16, 
      0,   3,  50,  16,  16,   0, 
      1,   0,   0,   0, 101,   0, 
      0,   3, 242,  32,  16,   0, 
      0,   0,   0,   0, 104,   0, 
      0,   2,   4,   0,   0,   0, 
     31,   0,   4,   4,  58, 128, 
     32,   0,   0,   0,   0,   0, 
      5,   0,   0,   0,  61,   0, 
      0, 137, 194,   0,   0, 128, 
     67,  85,  21,   0,  50,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  70, 126,  16,   0, 
      0,   0,   0,   0,  14,   0, 
      0,   8,  50,   0,  16,   0, 
      0,   0,   0,   0,   6, 128, 
     32,   0,   0,   0,   0,   0, 
      5,   0,   0,   0,  70,   0, 
     16,   0,   0,   0,   0,   0, 
     69,   0,   0, 139, 194,   0, 
      0, 128,  67,  85,  21,   0, 
     18,   0,  16,   0,   1,   0, 
      0,   0,  70,  16,  16,   0, 
      1,   0,   0,   0,  70, 126, 
     16,   0,   0,   0,   0,   0, 
      0,  96,  16,   0,   0,   0, 
      0,   0,  54,   0,   0,   6, 
     50,   0,  16,   0,   2,   0, 
      0,   0,  70,   0,  16, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   7, 
     98,   0,  16,   0,   1,   0, 
      0,   0,   6,   1,  16,   0, 
      2,   0,   0,   0,   6,  17, 
     16,   0,   1,   0,   0,   0, 
     69,   0,   0, 139, 194,   0, 
      0, 128,  67,  85,  21,   0, 
     34,   0,  16,   0,   1,   0, 
      0,   0, 150,   5,  16,   0, 
      1,   0,   0,   0,  22, 126, 
     16,   0,   0,   0,   0,   0, 
      0,  96,  16,   0,   0,   0, 
      0,   0,  54,   0,   0,   5, 
     66,   0,  16,   0,   2,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   7, 242,   0,  16,   0, 
      2,   0,   0,   0, 102,   8, 
     16,   0,   2,   0,   0,   0, 
     70,  20,  16,   0,   1,   0, 
      0,   0,  69,   0,   0, 139, 
    194,   0,   0, 128,  67,  85, 
     21,   0,  66,   0,  16,   0, 
      1,   0,   0,   0,  70,   0, 
     16,   0,   2,   0,   0,   0, 
    150, 124,  16,   0,   0,   0, 
      0,   0,   0,  96,  16,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   6,  66,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16, 128,  65,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   7, 242,   0,  16,   0, 
      3,   0,   0,   0, 134,   4, 
     16,   0,   0,   0,   0,   0, 
     70,  20,  16,   0,   1,   0, 
      0,   0,  69,   0,   0, 139, 
    194,   0,   0, 128,  67,  85, 
     21,   0,  66,   0,  16,   0, 
      0,   0,   0,   0,  70,   0, 
     16,   0,   3,   0,   0,   0, 
    150, 124,  16,   0,   0,   0, 
      0,   0,   0,  96,  16,   0, 
      0,   0,   0,   0,  69,   0, 
      0, 139, 194,   0,   0, 128, 
     67,  85,  21,   0, 130,   0, 
     16,   0,   1,   0,   0,   0, 
    230,  10,  16,   0,   2,   0, 
      0,   0, 150, 115,  16,   0, 
      0,   0,   0,   0,   0,  96, 
     16,   0,   0,   0,   0,   0, 
     54,   0,   0,   5, 130,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   7, 
    242,   0,  16,   0,   2,   0, 
      0,   0, 198,   7,  16,   0, 
      0,   0,   0,   0,  70,  20, 
     16,   0,   1,   0,   0,   0, 
     69,   0,   0, 139, 194,   0, 
      0, 128,  67,  85,  21,   0, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  70,   0,  16,   0, 
      2,   0,   0,   0, 150, 115, 
     16,   0,   0,   0,   0,   0, 
      0,  96,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  12, 
     50,   0,  16,   0,   0,   0, 
      0,   0,  70,   0,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   0,   0, 128, 191, 
      0,   0, 128,  63,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     70,  16,  16,   0,   1,   0, 
      0,   0,  69,   0,   0, 139, 
    194,   0,   0, 128,  67,  85, 
     21,   0,  18,   0,  16,   0, 
      0,   0,   0,   0,  70,   0, 
     16,   0,   0,   0,   0,   0, 
     70, 126,  16,   0,   0,   0, 
      0,   0,   0,  96,  16,   0, 
      0,   0,   0,   0,  69,   0, 
      0, 139, 194,   0,   0, 128, 
     67,  85,  21,   0,  34,   0, 
     16,   0,   0,   0,   0,   0, 
    230,  10,  16,   0,   2,   0, 
      0,   0,  22, 126,  16,   0, 
      0,   0,   0,   0,   0,  96, 
     16,   0,   0,   0,   0,   0, 
     69,   0,   0, 139, 194,   0, 
      0, 128,  67,  85,  21,   0, 
     18,   0,  16,   0,   2,   0, 
      0,   0, 230,  10,  16,   0, 
      3,   0,   0,   0,  70, 126, 
     16,   0,   0,   0,   0,   0, 
      0,  96,  16,   0,   0,   0, 
      0,   0,   0,   0,   0,   8, 
     98,   0,  16,   0,   1,   0, 
      0,   0,   6,   0,  16, 128, 
     65,   0,   0,   0,   1,   0, 
      0,   0,  86,   6,  16,   0, 
      1,   0,   0,   0,  52,   0, 
      0,  10,  98,   0,  16,   0, 
      1,   0,   0,   0,  86,   6, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  14,  32,   0,   8, 
     98,   0,  16,   0,   1,   0, 
      0,   0,  86,   6,  16,   0, 
      1,   0,   0,   0,  86, 133, 
     32,   0,   0,   0,   0,   0, 
      5,   0,   0,   0,   0,   0, 
      0,   7,  34,   0,  16,   0, 
      1,   0,   0,   0,  42,   0, 
     16,   0,   1,   0,   0,   0, 
     26,   0,  16,   0,   1,   0, 
      0,   0,   0,   0,   0,   8, 
    242,   0,  16,   0,   0,   0, 
      0,   0,   6,   0,  16, 128, 
     65,   0,   0,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  52,   0, 
      0,  10, 242,   0,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  14,  32,   0,   8, 
    242,   0,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  86, 133, 
     32,   0,   0,   0,   0,   0, 
      5,   0,   0,   0,   0,   0, 
      0,   7,  66,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   1,   0, 
      0,   0,   0,   0,   0,   8, 
     34,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16, 128, 
     65,   0,   0,   0,   1,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,  52,   0, 
      0,   7,  34,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  14,  32,   0,   8, 
     34,   0,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,  26, 128, 
     32,   0,   0,   0,   0,   0, 
      5,   0,   0,   0,   0,   0, 
      0,   7,  66,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   1,   0, 
      0,   0,   0,   0,   0,   7, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
      0,   0,   0,   7,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   7,  18,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   0,   0,   0,   8, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16, 128, 
     65,   0,   0,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      2,   0,   0,   0,  52,   0, 
      0,   7,  34,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  14,  32,   0,   8, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  26, 128, 
     32,   0,   0,   0,   0,   0, 
      5,   0,   0,   0,   0,   0, 
      0,   7,  18,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  47,   0,   0,   5, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   8,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     42, 128,  32,   0,   0,   0, 
      0,   0,   5,   0,   0,   0, 
     25,   0,   0,   5, 130,  32, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  54,   0,   0,   8, 
    114,  32,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  62,   0, 
      0,   1,  18,   0,   0,   1, 
     69,   0,   0, 139, 194,   0, 
      0, 128,  67,  85,  21,   0, 
    242,   0,  16,   0,   0,   0, 
      0,   0,  70,  16,  16,   0, 
      1,   0,   0,   0,  70, 126, 
     16,   0,   0,   0,   0,   0, 
      0,  96,  16,   0,   0,   0, 
      0,   0,  49,   0,   0,   7, 
     18,   0,  16,   0,   1,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0, 205, 204, 204,  61, 
     13,   0,   4,   3,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     56,   0,   0,   8, 242,  32, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,  62,   0,   0,   1, 
     21,   0,   0,   1,  62,   0, 
      0,   1,  83,  84,  65,  84, 
    148,   0,   0,   0,  53,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     10,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0
};