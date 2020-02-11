#pragma once
#include <memory>

#include "Directory.h"

class Synchronizer
{
public:
	Synchronizer(const Synchronizer&) = delete;

	static Synchronizer& Get() { 
		static Synchronizer instance;
		return instance;
	}

	void SyncByName(const Directory& syncTo, const Directory& syncFrom);
	void SyncByNameAndSize(const Directory& syncTo, const Directory& syncFrom);
	void SyncByNameAndContent(const Directory& syncTo, const Directory& syncFrom);
private:
	Synchronizer() = default;

	void Copy(const DirElement& elementToCopy, const std::string path) const;
};
