#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Buffer Definitions: 
//
// cbuffer viewBuffer
// {
//
//   float4x4 view;                     // Offset:    0 Size:    64
//   float4x4 projection;               // Offset:   64 Size:    64
//
// }
//
// cbuffer modelBuffer
// {
//
//   float4x4 model;                    // Offset:    0 Size:    64
//   float4x4 normalMatrix;             // Offset:   64 Size:    64
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// viewBuffer                        cbuffer      NA          NA            cb0      1 
// modelBuffer                       cbuffer      NA          NA            cb3      1 
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// Position3D               0   xyz         0     NONE   float   xyz 
// TexCoord                 0   xy          1     NONE   float   xy  
// Normal                   0   xyz         2     NONE   float   xyz 
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// TexCoord                 0   xy          0     NONE   float   xy  
// VertPos                  0   xyz         1     NONE   float   xyz 
// VertNormal               0   xyz         2     NONE   float   xyz 
// SV_Position              0   xyzw        3      POS   float   xyzw
//
vs_5_0
dcl_globalFlags refactoringAllowed
dcl_constantbuffer CB0[8], immediateIndexed
dcl_constantbuffer CB3[7], immediateIndexed
dcl_input v0.xyz
dcl_input v1.xy
dcl_input v2.xyz
dcl_output o0.xy
dcl_output o1.xyz
dcl_output o2.xyz
dcl_output_siv o3.xyzw, position
dcl_temps 3
mov o0.xy, v1.xyxx
mul r0.xyz, v0.yyyy, cb3[1].xyzx
mad r0.xyz, cb3[0].xyzx, v0.xxxx, r0.xyzx
mad r0.xyz, cb3[2].xyzx, v0.zzzz, r0.xyzx
add o1.xyz, r0.xyzx, cb3[3].xyzx
mul r0.xyz, v2.yyyy, cb3[5].xyzx
mad r0.xyz, cb3[4].xyzx, v2.xxxx, r0.xyzx
mad o2.xyz, cb3[6].xyzx, v2.zzzz, r0.xyzx
mul r0.xyzw, cb0[1].xyzw, cb3[1].yyyy
mad r0.xyzw, cb0[0].xyzw, cb3[1].xxxx, r0.xyzw
mad r0.xyzw, cb0[2].xyzw, cb3[1].zzzz, r0.xyzw
mad r0.xyzw, cb0[3].xyzw, cb3[1].wwww, r0.xyzw
mul r1.xyzw, r0.yyyy, cb0[5].xyzw
mad r1.xyzw, cb0[4].xyzw, r0.xxxx, r1.xyzw
mad r1.xyzw, cb0[6].xyzw, r0.zzzz, r1.xyzw
mad r0.xyzw, cb0[7].xyzw, r0.wwww, r1.xyzw
mul r0.xyzw, r0.xyzw, v0.yyyy
mul r1.xyzw, cb0[1].xyzw, cb3[0].yyyy
mad r1.xyzw, cb0[0].xyzw, cb3[0].xxxx, r1.xyzw
mad r1.xyzw, cb0[2].xyzw, cb3[0].zzzz, r1.xyzw
mad r1.xyzw, cb0[3].xyzw, cb3[0].wwww, r1.xyzw
mul r2.xyzw, r1.yyyy, cb0[5].xyzw
mad r2.xyzw, cb0[4].xyzw, r1.xxxx, r2.xyzw
mad r2.xyzw, cb0[6].xyzw, r1.zzzz, r2.xyzw
mad r1.xyzw, cb0[7].xyzw, r1.wwww, r2.xyzw
mad r0.xyzw, r1.xyzw, v0.xxxx, r0.xyzw
mul r1.xyzw, cb0[1].xyzw, cb3[2].yyyy
mad r1.xyzw, cb0[0].xyzw, cb3[2].xxxx, r1.xyzw
mad r1.xyzw, cb0[2].xyzw, cb3[2].zzzz, r1.xyzw
mad r1.xyzw, cb0[3].xyzw, cb3[2].wwww, r1.xyzw
mul r2.xyzw, r1.yyyy, cb0[5].xyzw
mad r2.xyzw, cb0[4].xyzw, r1.xxxx, r2.xyzw
mad r2.xyzw, cb0[6].xyzw, r1.zzzz, r2.xyzw
mad r1.xyzw, cb0[7].xyzw, r1.wwww, r2.xyzw
mad r0.xyzw, r1.xyzw, v0.zzzz, r0.xyzw
mul r1.xyzw, cb0[1].xyzw, cb3[3].yyyy
mad r1.xyzw, cb0[0].xyzw, cb3[3].xxxx, r1.xyzw
mad r1.xyzw, cb0[2].xyzw, cb3[3].zzzz, r1.xyzw
mad r1.xyzw, cb0[3].xyzw, cb3[3].wwww, r1.xyzw
mul r2.xyzw, r1.yyyy, cb0[5].xyzw
mad r2.xyzw, cb0[4].xyzw, r1.xxxx, r2.xyzw
mad r2.xyzw, cb0[6].xyzw, r1.zzzz, r2.xyzw
mad r1.xyzw, cb0[7].xyzw, r1.wwww, r2.xyzw
add o3.xyzw, r0.xyzw, r1.xyzw
ret 
// Approximately 45 instruction slots used
#endif

const BYTE shader_Lit_vs[] =
{
     68,  88,  66,  67, 249, 220, 
     74,  43, 147, 167, 114, 158, 
    218,  92, 108, 123,   2, 137, 
     77, 245,   1,   0,   0,   0, 
    240,  10,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
     28,   2,   0,   0, 144,   2, 
      0,   0,  40,   3,   0,   0, 
     84,  10,   0,   0,  82,  68, 
     69,  70, 224,   1,   0,   0, 
      2,   0,   0,   0, 148,   0, 
      0,   0,   2,   0,   0,   0, 
     60,   0,   0,   0,   0,   5, 
    254, 255,   0,   1,   0,   0, 
    183,   1,   0,   0,  82,  68, 
     49,  49,  60,   0,   0,   0, 
     24,   0,   0,   0,  32,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
    124,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 135,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0, 118, 105, 
    101, 119,  66, 117, 102, 102, 
    101, 114,   0, 109, 111, 100, 
    101, 108,  66, 117, 102, 102, 
    101, 114,   0, 171, 124,   0, 
      0,   0,   2,   0,   0,   0, 
    196,   0,   0,   0, 128,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 135,   0, 
      0,   0,   2,   0,   0,   0, 
     84,   1,   0,   0, 128,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  20,   1, 
      0,   0,   0,   0,   0,   0, 
     64,   0,   0,   0,   2,   0, 
      0,   0,  36,   1,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  72,   1,   0,   0, 
     64,   0,   0,   0,  64,   0, 
      0,   0,   2,   0,   0,   0, 
     36,   1,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    118, 105, 101, 119,   0, 102, 
    108, 111,  97, 116,  52, 120, 
     52,   0, 171, 171,   3,   0, 
      3,   0,   4,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     25,   1,   0,   0, 112, 114, 
    111, 106, 101,  99, 116, 105, 
    111, 110,   0, 171, 164,   1, 
      0,   0,   0,   0,   0,   0, 
     64,   0,   0,   0,   2,   0, 
      0,   0,  36,   1,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 170,   1,   0,   0, 
     64,   0,   0,   0,  64,   0, 
      0,   0,   2,   0,   0,   0, 
     36,   1,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    109, 111, 100, 101, 108,   0, 
    110, 111, 114, 109,  97, 108, 
     77,  97, 116, 114, 105, 120, 
      0,  77, 105,  99, 114, 111, 
    115, 111, 102, 116,  32,  40, 
     82,  41,  32,  72,  76,  83, 
     76,  32,  83, 104,  97, 100, 
    101, 114,  32,  67, 111, 109, 
    112, 105, 108, 101, 114,  32, 
     49,  48,  46,  49,   0, 171, 
     73,  83,  71,  78, 108,   0, 
      0,   0,   3,   0,   0,   0, 
      8,   0,   0,   0,  80,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      7,   7,   0,   0,  91,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      3,   3,   0,   0, 100,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
      7,   7,   0,   0,  80, 111, 
    115, 105, 116, 105, 111, 110, 
     51,  68,   0,  84, 101, 120, 
     67, 111, 111, 114, 100,   0, 
     78, 111, 114, 109,  97, 108, 
      0, 171,  79,  83,  71,  78, 
    144,   0,   0,   0,   4,   0, 
      0,   0,   8,   0,   0,   0, 
    104,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   3,  12,   0,   0, 
    113,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,   7,   8,   0,   0, 
    121,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,   7,   8,   0,   0, 
    132,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      3,   0,   0,   0,   3,   0, 
      0,   0,  15,   0,   0,   0, 
     84, 101, 120,  67, 111, 111, 
    114, 100,   0,  86, 101, 114, 
    116,  80, 111, 115,   0,  86, 
    101, 114, 116,  78, 111, 114, 
    109,  97, 108,   0,  83,  86, 
     95,  80, 111, 115, 105, 116, 
    105, 111, 110,   0,  83,  72, 
     69,  88,  36,   7,   0,   0, 
     80,   0,   1,   0, 201,   1, 
      0,   0, 106,   8,   0,   1, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,  89,   0, 
      0,   4,  70, 142,  32,   0, 
      3,   0,   0,   0,   7,   0, 
      0,   0,  95,   0,   0,   3, 
    114,  16,  16,   0,   0,   0, 
      0,   0,  95,   0,   0,   3, 
     50,  16,  16,   0,   1,   0, 
      0,   0,  95,   0,   0,   3, 
    114,  16,  16,   0,   2,   0, 
      0,   0, 101,   0,   0,   3, 
     50,  32,  16,   0,   0,   0, 
      0,   0, 101,   0,   0,   3, 
    114,  32,  16,   0,   1,   0, 
      0,   0, 101,   0,   0,   3, 
    114,  32,  16,   0,   2,   0, 
      0,   0, 103,   0,   0,   4, 
    242,  32,  16,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
    104,   0,   0,   2,   3,   0, 
      0,   0,  54,   0,   0,   5, 
     50,  32,  16,   0,   0,   0, 
      0,   0,  70,  16,  16,   0, 
      1,   0,   0,   0,  56,   0, 
      0,   8, 114,   0,  16,   0, 
      0,   0,   0,   0,  86,  21, 
     16,   0,   0,   0,   0,   0, 
     70, 130,  32,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 114,   0, 
     16,   0,   0,   0,   0,   0, 
     70, 130,  32,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      6,  16,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,  10, 114,   0,  16,   0, 
      0,   0,   0,   0,  70, 130, 
     32,   0,   3,   0,   0,   0, 
      2,   0,   0,   0, 166,  26, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,   0,   0,   0,   8, 
    114,  32,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  70, 130, 
     32,   0,   3,   0,   0,   0, 
      3,   0,   0,   0,  56,   0, 
      0,   8, 114,   0,  16,   0, 
      0,   0,   0,   0,  86,  21, 
     16,   0,   2,   0,   0,   0, 
     70, 130,  32,   0,   3,   0, 
      0,   0,   5,   0,   0,   0, 
     50,   0,   0,  10, 114,   0, 
     16,   0,   0,   0,   0,   0, 
     70, 130,  32,   0,   3,   0, 
      0,   0,   4,   0,   0,   0, 
      6,  16,  16,   0,   2,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,  10, 114,  32,  16,   0, 
      2,   0,   0,   0,  70, 130, 
     32,   0,   3,   0,   0,   0, 
      6,   0,   0,   0, 166,  26, 
     16,   0,   2,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   9, 
    242,   0,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  86, 133,  32,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,  50,   0,   0,  11, 
    242,   0,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   6, 128,  32,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,  11, 242,   0,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      2,   0,   0,   0, 166, 138, 
     32,   0,   3,   0,   0,   0, 
      1,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,  11, 242,   0, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
    246, 143,  32,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   8, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  86,   5,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      5,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   6,   0, 
      0,   0, 166,  10,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   7,   0,   0,   0, 
    246,  15,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  56,   0, 
      0,   7, 242,   0,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     86,  21,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   9, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  86, 133,  32,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  50,   0,   0,  11, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   6, 128,  32,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  11, 242,   0,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      2,   0,   0,   0, 166, 138, 
     32,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  11, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
    246, 143,  32,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  56,   0,   0,   8, 
    242,   0,  16,   0,   2,   0, 
      0,   0,  86,   5,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      5,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      2,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,   6,   0, 
     16,   0,   1,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   2,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   6,   0, 
      0,   0, 166,  10,  16,   0, 
      1,   0,   0,   0,  70,  14, 
     16,   0,   2,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   7,   0,   0,   0, 
    246,  15,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,  50,   0, 
      0,   9, 242,   0,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
      6,  16,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   9, 242,   0,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,  86, 133, 
     32,   0,   3,   0,   0,   0, 
      2,   0,   0,   0,  50,   0, 
      0,  11, 242,   0,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   6, 128, 
     32,   0,   3,   0,   0,   0, 
      2,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  11, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
    166, 138,  32,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  11, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0, 246, 143,  32,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  56,   0, 
      0,   8, 242,   0,  16,   0, 
      2,   0,   0,   0,  86,   5, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   5,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   2,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
      6,   0,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      2,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      6,   0,   0,   0, 166,  10, 
     16,   0,   1,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   7,   0, 
      0,   0, 246,  15,  16,   0, 
      1,   0,   0,   0,  70,  14, 
     16,   0,   2,   0,   0,   0, 
     50,   0,   0,   9, 242,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0, 166,  26,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   9, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     86, 133,  32,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
     50,   0,   0,  11, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      6, 128,  32,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  11, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0, 166, 138,  32,   0, 
      3,   0,   0,   0,   3,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  11, 242,   0,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      3,   0,   0,   0, 246, 143, 
     32,   0,   3,   0,   0,   0, 
      3,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     56,   0,   0,   8, 242,   0, 
     16,   0,   2,   0,   0,   0, 
     86,   5,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   5,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   2,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,   6,   0,  16,   0, 
      1,   0,   0,   0,  70,  14, 
     16,   0,   2,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   2,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   6,   0,   0,   0, 
    166,  10,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      7,   0,   0,   0, 246,  15, 
     16,   0,   1,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,   0,   0,   0,   7, 
    242,  32,  16,   0,   3,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     62,   0,   0,   1,  83,  84, 
     65,  84, 148,   0,   0,   0, 
     45,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      7,   0,   0,   0,  43,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
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
      0,   0,   0,   0
};
