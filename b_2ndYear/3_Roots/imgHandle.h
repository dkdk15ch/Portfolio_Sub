#pragma once
#include <unordered_map>
#include <string>

enum class ImgName {
	BLOCK_SAND,
	BLOCK_NUTRIENT,
	BLOCK_WATER,
	BLOCK_OXYGEN,
	TREE,
};

class ImageHandles {
private:
	static std::unordered_map<ImgName, int> m_handles;
public:
	static void Load(ImgName name, std::string& filepath);
	static int Get(ImgName name) ;
	static void ReleaseAll();
	ImageHandles() = default;
	~ImageHandles() { ReleaseAll(); };
};