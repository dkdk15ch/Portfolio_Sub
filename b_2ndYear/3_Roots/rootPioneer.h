#pragma once
#include "rootBase.h"

class IRoot;
class MapManager;

class Pioneer : public RootBase {
public:
	Pioneer(const MapManager& mgr,BoxRects start, BoxRects end, std::shared_ptr<IRoot> prev);

	void Draw() const override;
	void Run() override;
};