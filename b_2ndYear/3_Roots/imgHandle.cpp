#include "imgHandle.h"
#include "DxLib.h"

std::unordered_map<ImgName, int> ImageHandles::m_handles;

void ImageHandles::Load(ImgName name, std::string& filepath) {
	int handle = LoadGraph(filepath.c_str());
	m_handles[name] = handle;
}

int ImageHandles::Get(ImgName name) {
	auto it = m_handles.find(name);
	if (it != m_handles.end()) return it->second;
	return -1;
}

void ImageHandles::ReleaseAll() {
	for (auto& pair : m_handles) {
		DeleteGraph(pair.second);
	}
	m_handles.clear();
}
