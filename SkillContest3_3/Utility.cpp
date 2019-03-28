#include "DXUT.h"
#include "Utility.h"


D3DLIGHT9 d3d::GetDirectionLight(Vector3 * dir, D3DXCOLOR * color)
{
	D3DLIGHT9 light;

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = *color * 0.4f;
	light.Diffuse = *color;
	light.Specular = *color * 0.4f;
	light.Direction = *dir;

	return light;
}

int d3d::GetRandomNumber(int low, int high)
{
	static const double fraction = 1.0f / (RAND_MAX + 1.0f);

	return low + static_cast<int>((high - low + 1) * std::rand() * fraction);
}
