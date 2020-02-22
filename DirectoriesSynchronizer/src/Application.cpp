#include <iostream>
#include <conio.h>

#include "util/Logger.h"
#include "pfd/portable-file-dialogs.h"
#include "Synchronizer.h"

enum SYNC_METHOD
{
	SYNC_BY_NAME = 1,
	SYNC_BY_NAME_AND_SIZE = 2,
	SYNC_BY_NAME_AND_CONTENT = 3,
};

int main()
{
	Logger::Init();
	LOG_INFO("Log initialiazed");

	std::cout << "-----Witaj w programie synchronizujacym foldery!-----" << std::endl;
	std::cout << "Kliknij jakikoliwek klawisz aby zaczac." << std::endl;
	_getch();

	for (;;)
	{
		auto syncFolder = pfd::select_folder("Wybierz folder do synchronizacji").result();

		if (!syncFolder.empty())
		{
			LOG_INFO("Destination folder: {0}", syncFolder);

			std::cout << "Folder do synchronizacji: " << syncFolder << std::endl;
		}
		Directory syncDir(syncFolder);

		auto syncFromFolder = pfd::select_folder("Wybierz folder z ktorym bedziemy synchronizowac").result();

		if (!syncFromFolder.empty())
		{
			LOG_INFO("Refrence folder: {0}", syncFromFolder);

			std::cout << "Folder z ktorego bedziemy synchronizowac" << syncFromFolder << std::endl;
		}
		Directory syncFromDir(syncFromFolder);

		std::string confirmation;
		bool sync = false;
		int method = 0;
		
		for (;;)
		{
			std::cout << "Wybierz metode synchronizacji: (1/2/3)" << std::endl;
			std::cout << "1. Synchronizacja po nazwie plikow" << std::endl;
			std::cout << "2. Synchronizacja po nazwie i rozmiarze plikow" << std::endl;
			std::cout << "3. Synchronizacja po nazwie i zawartosci plikow (najwolniejsza)" << std::endl;

			std::cin >> method;

			if (method != 1 && method != 2 && method != 3)
			{
				std::cout << "Prosze wprowadz poprawna metode (1/2/3)!" << std::endl;
				continue;
			}
			break;
		}

		for (;;)
		{
			std::cout << "Czy napewno chcesz zsynchronizowac wybrane foldery (t/n)?" << std::endl;

			std::cin >> confirmation;
			
			if (confirmation == "T" || confirmation == "t")
				sync = true;
			else if (confirmation == "N" || confirmation == "n")
				sync = false;
			else
				continue;

			break;
		}
		
		if (sync)
		{
			switch (method)
			{
			case 1:
				Synchronizer::Get().SyncByName(syncDir, syncFromFolder);
				break;
			case 2:
				Synchronizer::Get().SyncByNameAndSize(syncDir, syncFromFolder);
				break;
			case 3:
				Synchronizer::Get().SyncByNameAndContent(syncDir, syncFromFolder);
				break;
			default:
				break;
			}
			std::cout << "Synchronizacja zakonczona" << std::endl;
		}

		std::cout << "Zaczac jeszcze raz?" << std::endl;
		_getch();
	}
	return 0;
}