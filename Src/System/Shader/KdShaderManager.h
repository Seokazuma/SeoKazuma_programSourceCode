#pragma once

#include "EffectShader/KdEffectShader.h"
#include "StandardShader/KdStandardShader.h"
#include "SpriteShader/KdSpriteShader.h"

//==========================================================
//
// シェーダ関係をまとめたクラス
//
//==========================================================
class KdShaderManager
{
public:

	//==============================================================
	//
	// 初期化・解放
	//
	//==============================================================

	// 初期化
	void Init();

	// 解放
	void Release();

	//==========================
	//
	// シェーダ
	//
	//==========================
	KdStandardShader		m_standardShader;		// 3Dモデル描画シェーダ
	KdEffectShader			m_effectShader;			// エフェクト描画シェーダ
	KdSpriteShader			m_spriteShader;			// 2Dテクスチャ描画シェーダ


	//==========================
	//
	// カメラ関係
	//
	//==========================

	// カメラ定数バッファ用構造体
	struct cbCamera
	{
		Math::Matrix		mV;			// ビュー行列
		Math::Matrix		mP;			// 射影行列
		Math::Vector3		CamPos;		// カメラワールド座標

		int					DistanceFogEnable = 0;			// 距離フォグ有効
		Math::Vector3		DistanceFogColor = { 1, 1, 1 };	// 距離フォグ色
		float				DistanceFogDensity = 0.001f;	// 距離フォグ減衰率
	};

	// カメラ定数バッファ
	KdConstantBuffer<cbCamera>	m_cb7_Camera;


	//==========================
	//
	// ライト関係
	//
	//==========================

	struct PointLight
	{
		Math::Vector3		color = { 3,3,3 };  // 光の色
		float				radius = 0;		//効果半径
		//16バイト区切り------------------------------
		Math::Vector3		pos = {};		//光の位置
		float					tmp4;
		//16バイト区切り------------------------------
	};

	struct SpotLighit
	{
		int					enable = 0;
		Math::Vector3		pos = {};	//座標
		//16バイト区切り------------------------------
		Math::Vector3		dir = {};	//方向
		float				radius = 0;	//距離
		//16バイト区切り------------------------------
		float				angle = 0;	//角度
		Math::Vector3		color = {};	//色
		//16バイト区切り------------------------------
	};

	// ライト定数バッファ用構造体
	struct cbLight
	{
		// 環境光
		Math::Vector3		AmbientLight = { 0.0f, 0.0f, 0.0f };
		float					tmp;
		//16バイト区切り------------------------------
			// 平行光
		Math::Vector3		DL_Dir = { 0,-1,0 };    // 光の方向
		float					tmp2;
		//16バイト区切り------------------------------
		Math::Vector3		DL_Color = { 1,1,1 };  // 光の色
		float					tmp3;
		//16バイト区切り------------------------------
		PointLight			PL[100];
		//16バイト区切り------------------------------
		int					PL_Num = 0;
		float					tmp4[3];
		//16バイト区切り------------------------------
		SpotLighit			SL[100];
		//16バイト区切り------------------------------
		int					SL_Num = 0;
		float					tmp5[3];
	};

	// ライト定数バッファ
	KdConstantBuffer<cbLight>		m_cb8_Light;

	void AddPointLight(const Math::Vector3& _color, const Math::Vector3& _pos, const float _radius)
	{
		int index = m_cb8_Light.Work().PL_Num;

		//最高100個まで
		if (index < 100)
		{
			m_cb8_Light.Work().PL[index].color = _color;
			m_cb8_Light.Work().PL[index].pos = _pos;
			m_cb8_Light.Work().PL[index].radius = _radius;

			m_cb8_Light.Work().PL_Num++;
		}
	}

	//スポットライト設定
	void AddSpotLighit(const Math::Vector3 _pos, const Math::Vector3 _dir, const float _radius, const float _angle, const Math::Vector3 _color)
	{
		int index = m_cb8_Light.Work().SL_Num;
		if (index < 100)
		{
			m_cb8_Light.Work().SL[index].pos = _pos;
			m_cb8_Light.Work().SL[index].dir = _dir;
			m_cb8_Light.Work().SL[index].radius = _radius;
			m_cb8_Light.Work().SL[index].angle = _angle;
			m_cb8_Light.Work().SL[index].color = _color;

			m_cb8_Light.Work().SL_Num++;
		}
	}

	//==========================
	//
	// その他
	//
	//==========================





//-------------------------------
// シングルトン
//-------------------------------
private:

	KdShaderManager() {}

public:
	static KdShaderManager& GetInstance() {
		static KdShaderManager instance;
		return instance;
	}
};

#define SHADER KdShaderManager::GetInstance()
