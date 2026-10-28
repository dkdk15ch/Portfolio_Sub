#pragma once
#include "point.h"

class Player;	//	前方宣言

class GameObject {
protected:

public :
	GameObject() {}
	virtual ~GameObject() = default;

	virtual void Draw() const = 0;	//	描画
	virtual void Run() = 0;		//	実行
};

//class IPlayerRunable {
//public:
//	virtual void Run() = 0;	//	実行
//};
//
//class IRunable {
//private:
//	virtual void Run() = 0;	//	実行
//};