#pragma once

//===========================================
//
// 便利機能
//
//===========================================

// ComPtr短縮名
using Microsoft::WRL::ComPtr;

// 算術系短縮名
namespace Math = DirectX::SimpleMath;

// 角度変換
inline constexpr float KdToRadians(float fDegrees) { return fDegrees * (3.141592654f / 180.0f); }
inline constexpr float KdToDegrees(float fRadians) { return fRadians * (180.0f / 3.141592654f); }

// 安全にReleaseするための関数
template<class T>
void KdSafeRelease(T*& p)
{
	if (p)
	{
		p->Release();
		p = nullptr;
	}
}

// 安全にDeleteするための関数
template<class T>
void KdSafeDelete(T*& p)
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}

// ファイルパスから、親ディレクトリまでのパスを取得
inline std::string KdGetDirFromPath(const std::string &path)
{
	const std::string::size_type pos = std::max<signed>(path.find_last_of('/'), path.find_last_of('\\'));
	return (pos == std::string::npos) ? std::string() : path.substr(0, pos + 1);
}

//=====================================================
//
// テクスチャ
//
//=====================================================

// 画像ファイルを読み込み、リソースデータを返す
// ・filepath		… 画像ファイル名
// ・bindFlags		… リソースバインドフラグ
// ・generateMipmap	… ミップマップ生成する？
// ・usage			… 使用法
// ・cpuAccessFlags	… CPUからアクセス可能にするか ※注意※ 使用可能かはusageの設定による
// ・miscFlags		… その他フラグ
// 戻り値：成功…2D画像リソースのアドレス　失敗…nullptr
ID3D11Texture2D* KdLoadTexture2D(const std::string& filepath, UINT bindFlags = D3D11_BIND_SHADER_RESOURCE, bool generateMipmap = false, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT cpuAccessFlags = 0, UINT miscFlags = 0);

// 2D画像(resource)リソースから、最適なビューを作成する
// ・resource		… 2D画像リソース
// ・ppSRV			… 作成されたShaderResourceViewを受け取るための変数のアドレス
// ・ppRTV			… 作成されたRenderTargetViewを受け取るための変数のアドレス
// ・ppDSV			… 作成されたDepthStencilViewを受け取るための変数のアドレス
bool KdCreateViewsFromTexture2D(ID3D11Texture2D* resource, ID3D11ShaderResourceView** ppSRV, ID3D11RenderTargetView** ppRTV, ID3D11DepthStencilView** ppDSV);

// viewから画像情報を取得する
void KdGetTextureInfo(ID3D11View* view, D3D11_TEXTURE2D_DESC& outDesc);
