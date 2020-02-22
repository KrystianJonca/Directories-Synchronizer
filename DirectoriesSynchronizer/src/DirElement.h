#include <string>
#include <fstream>
#include <sstream>

#include "util/Logger.h"

struct DirElement
{
	std::string path;
	unsigned int size;
	bool isFolder;

	const std::string GetName() const
	{
		return path.substr(path.find_last_of("\\") + 1);
	}
};
