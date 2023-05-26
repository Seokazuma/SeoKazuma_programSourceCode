//=====================================================
//
// 全シェーダ共通
//
//=====================================================

//------------------------------
// 定数バッファ(カメラ)
//------------------------------
cbuffer cbCamera : register(b7)
{
    // カメラ情報
    row_major float4x4 g_mV; // ビュー変換行列
    row_major float4x4 g_mP; // 射影変換行列
    float3 g_CamPos; // カメラ座標

	// フォグ
    int g_DistanceFogEnable; // フォグ有効/無効フラグ
    float3 g_DistanceFogColor; // フォグ色
    float g_DistanceFogDensity; // フォグ減衰率
};

struct PointLight
{
    float3 color; //色
    float radius; //効果半径
    float3 pos; //光の位置
    float tmp4;
};

struct SpotLight
{
    int enable; //有効化
    float3 pos; //座標
    float3 dir; //方向
    float radius; //距離（効果範囲）
    float angle; //角度
    float3 color; //色
};

//------------------------------
// 定数バッファ(ライト)
//------------------------------
cbuffer cbLight : register(b8)
{
    // 環境光
    float3 g_AmbientLight;

    // 平行光
    float3 g_DL_Dir; // 光の方向
    float3 g_DL_Color; // 光の色

	// 点光源
    PointLight g_PL[100]; //各ポイントライトの情報
    int g_PL_Num; //ポイントライトの数

    //スポットライト
    SpotLight g_SL[100];
    int g_SL_Num;
    
};
