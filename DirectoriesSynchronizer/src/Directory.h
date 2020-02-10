#pragma once
#include <vector>

#include "DirElement.h"

class Directory
{
public:
	Directory(const std::string& directoryPath);
	~Directory() = default;

	inline const std::string& GetPath() const { return m_Path; }
	inline const std::vector<DirElement>& GetElements() const { return m_Elements; }
private:
	std::string m_Path;
	std::vector<DirElement> m_Elements;
};

