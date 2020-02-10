#include <iostream>
#include "Sync.h"
#include "util/Logger.h"

int main()
{
	Logger::Init();
	LOG_INFO("Log initialiazed");

	Directory dir1("D:/Testy/folder");
	Directory dir2("D:/Testy/Folderdocelowy");

	Sync synced;

	synced.CompareByNameAndContent(dir2, dir1);
	LOG_DEBUG(synced.GetElementsToCopy().size());
	synced.CopyElements();
	std::cin.get();
	return 0;
}