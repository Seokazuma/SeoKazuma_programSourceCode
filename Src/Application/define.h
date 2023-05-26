#pragma once

struct Vertex
{
	Math::Vector3 pos;
	Math::Vector2 uv;
	Math::Vector4 color;
};
struct ModelVertex
{
	Math::Vector3 pos;
	Math::Vector2 uv;
	Math::Vector3 normal;
	UINT color;
};

static const int ScrW = 1280;
static const int ScrH = 720;
