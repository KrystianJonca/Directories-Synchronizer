#include "Directory.h"
#include <iostream>
int main()
{
	Directory dir("D:/Testy/folder");
	std::cout << dir.GetPath() << std::endl;
	for (const auto& ele : dir.GetElements())
	{
		std::cout << ele.path << std::endl;
		std::cout << ele.size << std::endl;
		std::cout << ele.isFolder << std::endl;
		std::cout << ele.GetName() << std::endl;
		std::cout << ele.GetContent() << std::endl;
	}
	std::cin.get();
	return 0;
}