#ifndef _RAYCAST
#define _RAYCAST

#include "value.fx"
#include "func.fx"

RWStructuredBuffer<tRaycastOut> OUTPUT : register(u0);

// Laycast Vertex Pos Buffer
StructuredBuffer<float4> g_arrVtx : register(t25);

Texture2D g_naviTex : register(t1);

#define CAM_POS         g_vec4_0
#define CAM_DIR         g_vec4_1

#define POS             g_vec4_2

#define FACE_X          g_int_0
#define FACE_Z          g_int_1

#define MAX_X_UV        g_float_0
#define MAX_Z_UV        g_float_1

#define X_MIN_MAX       g_vec2_0
#define Y_MIN_MAX       g_vec2_1
#define Z_MIN_MAX       g_vec2_2


[numthreads(32, 32, 1)]
void CS_Raycast(int3 _iThreadID : SV_DispatchThreadID)
{
    int2 id = _iThreadID.xy;

    float3 vPos[3] = { (float3) 0.f, (float3) 0.f, (float3) 0.f };

    if (0 == id.x % 2)
    {
        // �Ʒ��� �ﰢ��        
        // 2
        // | \
        // 0--1        
        vPos[0].x = id.x / 2;
        vPos[0].z = id.y;

        vPos[1].x = vPos[0].x + 1;
        vPos[1].z = id.y;

        vPos[2].x = vPos[0].x;
        vPos[2].z = id.y + 1;
    }
    else
    {
        // ���� �ﰢ��
        // 1--0
        //  \ |
        //    2  
        vPos[0].x = (id.x / 2) + 1;
        vPos[0].z = id.y + 1;

        vPos[1].x = vPos[0].x - 1;
        vPos[1].z = id.y + 1;

        vPos[2].x = vPos[0].x;
        vPos[2].z = id.y;
    }
    
    //vPos[0].xyz *= 10;
    //vPos[1].xyz *= 10;
    //vPos[2].xyz *= 10;

    float3 vCrossPoint = (float3) 0.f;
    float fDist = 0.f;

    if (IntersectsLay(vPos, CAM_POS.xyz, CAM_DIR.xyz, vCrossPoint, fDist))
    {
        vCrossPoint.x -= g_vec4_2.x;
        vCrossPoint.z -= g_vec4_2.z;
        
        if ((vCrossPoint.x / (float) FACE_X) < 0.f)
            vCrossPoint.x = 0.f;
        if ((vCrossPoint.z / (float) FACE_Z) < 0.f)
            vCrossPoint.z = 0.f;       
        if ((vCrossPoint.x / (float) FACE_X) > MAX_X_UV)
            vCrossPoint.x = MAX_X_UV * (float) FACE_X;
        if ((vCrossPoint.z / (float) FACE_Z) > MAX_Z_UV)
            vCrossPoint.z = MAX_Z_UV * (float) FACE_Z;
        
        OUTPUT[0].vUV = float2(vCrossPoint.x / (float) FACE_X, vCrossPoint.z / (float) FACE_Z);
        OUTPUT[0].vUV = float2(vCrossPoint.x , vCrossPoint.z);
        
        OUTPUT[0].fDist = fDist;
        OUTPUT[0].success = 1;
    }
    else
    {
        OUTPUT[0].vRGB.rgb = float3(1.0f, 1.0f, 1.0f);

    }
    
    //if (OUTPUT[0].success)
    //{
    //    float2 vUV = float2(vCrossPoint.x / (float) FACE_X, MAX_Z_UV - (vCrossPoint.z / (float) FACE_Z));
    //    vUV.x = vUV.x / (float) MAX_X_UV;
    //    vUV.y = vUV.y / (float) MAX_Z_UV;
    //    //vUV.x += (0.001 * MAX_X_UV);
    //    //vUV.y -= (0.0002 * MAX_Z_UV);
    //    
    //    float4 color = g_naviTex.SampleLevel(g_sam_0, vUV,0);
    //    if (color.a == 0.f)
    //        OUTPUT[0].success = 0;
    //    
    //    OUTPUT[0].vRGB.r = (abs(X_MIN_MAX.x) + abs(X_MIN_MAX.y)) * vUV.x;
    //    OUTPUT[0].vRGB.g = (abs(Z_MIN_MAX.x) + abs(Z_MIN_MAX.y)) * 0.f;
    //    OUTPUT[0].vRGB.b = (abs(Y_MIN_MAX.x) + abs(Y_MIN_MAX.y)) * vUV.y;
    //}
        
}

#endif