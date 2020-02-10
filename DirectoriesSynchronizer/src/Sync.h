#pragma once
#include <memory>

#include "Directory.h"

class Sync
{
public:
	Sync() = default;
	~Sync() = default;

	inline const std::vector<DirElement>& GetElementsToCopy() const { return m_ElementsToCopy; }

	void CompareByName(const Directory& syncTo, const Directory& syncFrom);
	void CompareByNameAndSize(const Directory& syncTo, const Directory& syncFrom);
	void CompareByNameAndContent(const Directory& syncTo, const Directory& syncFrom);
	void CopyElements() const;
private:
	std::vector<DirElement> m_ElementsToCopy;
	std::vector<std::shared_ptr<Sync>> m_SubfoldersToSync;
	std::string m_CopyToPath;
};

