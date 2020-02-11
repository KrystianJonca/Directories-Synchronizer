#include <iostream>
#include "Synchronizer.h"
#include "util/Logger.h"
#include "pfd/portable-file-dialogs.h"


int main()
{
	Logger::Init();
	LOG_INFO("Log initialiazed");

	//auto selection = pfd::select_folder("Select a folder").result();

	//if (!selection.empty())
	//	LOG_INFO("User did not select any folder!");
	//else
	//	LOG_INFO("User selected folder {0}", selection);

	Directory dir1("D:/Testy/folder");
	Directory dir2("D:/Testy/Folderdocelowy");

	Synchronizer::Get().SyncByName(dir2, dir1);

	std::cin.get();
	return 0;
}