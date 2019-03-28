#pragma once

namespace d3d
{
	template<typename T>
	T Lerp(T p1, T p2, float d)
	{
		return p1 + (p2 - p1) * d;
	}
	D3DLIGHT9 GetDirectionLight(Vector3 *dir, D3DXCOLOR *color);

	int GetRandomNumber(int low, int high);
}