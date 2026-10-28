#pragma once
#include "gameObject.h"
#include "IRoot.h"
#include <memory>

class Player : public GameObject{
private:
	MutaPoint m_center;			
	void UpdatePlayerPos();		
	bool m_isSeleting = false;	
	std::weak_ptr<IRoot> m_isSeletedRoot;	//	‘I‘ğ’†‚Ìª
public:
	Player(float x, float y) : m_center(x,y){}
	~Player() {}
	void Draw() const override;		
	void Run() override;
	
	//	ƒƒ“ƒo•Ï”‚Ìgetter,setter
	MutaPoint GetCenter() const { return m_center; }
	void SetCenter(float x, float y) { m_center.set(x, y); }
	bool GetIsSelecting() const { return m_isSeleting; }
	void SetIsSelecting(bool isSelecting) { m_isSeleting = isSelecting; }
	std::weak_ptr<IRoot> GetSelectedRoot() const { return m_isSeletedRoot; }
	void SetSelectedRoot(std::shared_ptr<IRoot> root) { m_isSeletedRoot = root; }
};

