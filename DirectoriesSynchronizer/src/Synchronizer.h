#pragma once
#include <memory>

#include "Directory.h"

class Synchronizer
{
public:
	static Synchronizer& Get() { 
		static Synchronizer instance;
		return instance;
	}

	void SyncByName(const Directory& syncTo, const Directory& syncFrom);
	void SyncByNameAndSize(const Directory& syncTo, const Directory& syncFrom);
	void SyncByNameAndContent(const Directory& syncTo, const Directory& syncFrom);

	

	bool AreContentsEqual(const DirElement& elementPath, const DirElement& refElementPath);
private:
	Synchronizer() = default;

	void Copy(const DirElement& elementToCopy, const std::string path) const;
};
