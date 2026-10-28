#pragma once
#include "rootBase.h"

class IRoot;

class Scout : public RootBase {
public:
	Scout(BoxRects start, BoxRects end, std::shared_ptr<IRoot> prev);

	void Draw() const override;
	void Run() override;

	//std::unique_ptr<Scout> Create(Point start, Point end ,RootBase* prev)
};