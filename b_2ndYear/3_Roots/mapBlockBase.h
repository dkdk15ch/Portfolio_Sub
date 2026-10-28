#pragma once
#include "resource.h"
#include "point.h"
#include "blockType.h"
#include "gameObject.h"
#include "collision.h"
#include "color.h"

class Player;

class BlockBase : public GameObject {
public:
	static float blockSize;
private:
	int m_No;	
	float m_left, m_top, m_right, m_bottom;
	BoxCollision m_collision;
	unsigned int boxColor{ Color::black };	//	“–‚½‚è”»’è‚ÌF
	Resource m_resourceOfMapBlock;			
	bool m_isPublished{ false };		
	bool m_hasPassed{ false };			
	bool m_hasVirus{ false };		

	int m_imgHandle;
public:
	BlockBase(BlockType no, float x, float y, const Resource& res, int imgHandle);
	virtual ~BlockBase() = default;

	bool DetectPlayer(const Player& player);

	virtual void Draw() const override;
	virtual void Run() override;

	//	ƒƒ“ƒo•Ï”‚Ìgetter,setter
	#pragma region SetGetFunctions
	int GetType() const { return m_No; }
	// À•W
	float GetLeft() const { return m_left; }
	float GetTop() const { return m_top; }
	float GetRight() const { return m_right; }
	float GetBottom() const { return m_bottom; }

	Resource GetResouce() const { return m_resourceOfMapBlock; }
	void ToggleIsPublished(){ m_isPublished = !m_isPublished; }
	bool GetIsPublished()const{ return m_isPublished; }
	void ToggleHasPassed(){ m_hasPassed = !m_hasPassed; }
	bool GetHasPassed()const{ return m_hasPassed; }
	void ToggleHasVirus(){ m_hasVirus = !m_hasVirus; }
	bool GetHasVirus()const{ return m_hasVirus; }

	#pragma endregion


};