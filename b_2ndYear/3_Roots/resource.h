#pragma once

class Resource {
private:
	int m_nutrient;	//	栄養素
	int m_water;	//	水分
	int m_oxygen;	//	酸素
	bool m_isLocked = false;	//	固定リソース用

	void SetNutrient(int n) { m_nutrient = n; }
	void SetWater(int w) { m_water = w; }
	void SetOxygen(int o) { m_oxygen = o; }
public:
	//	デフォルトコンストラクタ
	Resource() : m_nutrient{ 0 }, m_water{ 0 }, m_oxygen{ 0 } {}
	//	固定リソース用コンストラクタ
	Resource(int n, int w, int o) : m_nutrient{ n }, m_water{ w }, m_oxygen{ o }, m_isLocked{true} {}

	//	getter/setter
	void SetResource(int n, int w, int o) {
		if (m_isLocked)
			return;
		SetNutrient(n);
		SetWater(w);
		SetOxygen(o);
	}

	int GetNutrient() const{ return m_nutrient; }
	int GetWater() const{ return m_water; }
	int GetOxygen() const{ return m_oxygen; }

	// Util
	static Resource AddResouce(Resource a, Resource b) {
		int n = a.m_nutrient + b.m_nutrient;
		int w = a.m_water + b.m_water;
		int o = a.m_oxygen + b.m_oxygen;
		return Resource(n, w, o);
	}
};