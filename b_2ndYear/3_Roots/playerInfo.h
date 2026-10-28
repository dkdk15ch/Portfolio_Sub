#pragma once
#include "resource.h"

class PlayerInfo {
private:
	Resource m_playerResource;	//	リソース情報（水、栄養、酸素)
	int m_skillPoint{ 0 };				//	スキルポイント
	int m_elevation{ 0 };			//	木の標高
	int m_mainRootDepth{ 0 };			//	主根の深さ
public:
	//	スキルポイントをセット・取得
	void AddSkillPoint(const int add) { m_skillPoint += add; }
	void SubSkillPoint(const int sub) { 
		if (m_skillPoint < sub)
			return;
		m_skillPoint -= sub; }
	int GetSkillPoint() const{ return m_skillPoint; }
	//	木の標高のセット・取得
	void AddElevation(const int add) { m_elevation += add; }
};