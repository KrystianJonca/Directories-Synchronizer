#include "Synchronizer.h"
#include "util/Timer.h"

#include <vector>
#include <filesystem>
#include <future>

const unsigned int BUFFER_SIZE = 1048576;

void Synchronizer::SyncByName(const Directory& syncTo, const Directory& syncFrom)
{
	LOG_DEBUG("Syncing by name {0} and {1}", syncFrom.GetPath(), syncTo.GetPath());

	Timer timer;

	if (syncTo.GetElements().empty())
	{
		LOG_DEBUG("Comperison of folders {0} and {1} completed", syncFrom.GetPath(), syncTo.GetPath());

		LOG_DEBUG("Copying {0} to {1}", syncFrom.GetPath(), syncTo.GetPath());
		std::filesystem::copy(
			syncFrom.GetPath(),
			syncTo.GetPath(),
			std::filesystem::copy_options::recursive
		);
		LOG_DEBUG("Copying folders {0} and {1} completed", syncFrom.GetPath(), syncTo.GetPath());
		return;
	}

	bool addFile = false;

	for (const DirElement& element : syncFrom.GetElements())
	{
		for (const DirElement& refElement : syncTo.GetElements())
		{
			if (element.GetName() == refElement.GetName())
			{
				if (element.isFolder && refElement.isFolder)
				{
					Directory copySubFolder(element.path);
					Directory desSubFolder(refElement.path);

					Synchronizer::Get().SyncByName(desSubFolder, copySubFolder);
				
					addFile = false;
					break;
				}
				addFile = false;
				break;
			}
			else
			{
				addFile = true;
				continue;
			}
		}
		if (addFile)
			Get().Copy(element, syncTo.GetPath());
	}
	LOG_DEBUG("Syncing of folders {0} and {1} completed in {2}s", syncFrom.GetPath(), syncTo.GetPath(), timer.GetDuration());
}
void Synchronizer::SyncByNameAndSize(const Directory& syncTo, const Directory& syncFrom)
{
	LOG_DEBUG("Syncing by name and size {0} and {1}", syncFrom.GetPath(), syncTo.GetPath());

	Timer timer;

	if (syncTo.GetElements().empty())
	{
		LOG_DEBUG("Comperison of folders {} and {1} completed", syncFrom.GetPath(), syncTo.GetPath());

		LOG_DEBUG("Copying {0} to {1}", syncFrom.GetPath(), syncTo.GetPath());
		std::filesystem::copy(
			syncFrom.GetPath(),
			syncTo.GetPath(),
			std::filesystem::copy_options::recursive
		);
		LOG_DEBUG("Copying folders {0} and {1} completed", syncFrom.GetPath(), syncTo.GetPath());
		return;
	}

	bool addFile = false;

	for (const DirElement& element : syncFrom.GetElements())
	{
		for (const DirElement& refElement : syncTo.GetElements())
		{
			if (element.GetName() == refElement.GetName())
			{
				if (element.isFolder && refElement.isFolder)
				{
					Directory copySubFolder(element.path);
					Directory desSubFolder(refElement.path);

					Synchronizer::Get().SyncByName(desSubFolder, copySubFolder);

					addFile = false;
					break;
				}
				if (element.size == refElement.size)
				{
					addFile = false;
					break;
				}
				else
				{
					addFile = true;
					continue;
				}
			}
			else
			{
				addFile = true;
				continue;
			}
		}
		if (addFile)
			Get().Copy(element, syncTo.GetPath());
	}
	LOG_DEBUG("Syncing of folders {0} and {1} completed in {2}s", syncFrom.GetPath(), syncTo.GetPath(), timer.GetDuration());
}
void Synchronizer::SyncByNameAndContent(const Directory& syncTo, const Directory& syncFrom)
{
	LOG_DEBUG("Syncing by name, size and content {0} and {1}", syncFrom.GetPath(), syncTo.GetPath());

	Timer timer;

	if (syncTo.GetElements().empty())
	{
		LOG_DEBUG("Comperison of folders {0} and {1} completed", syncFrom.GetPath(), syncTo.GetPath());

		LOG_DEBUG("Copying {0} to {1}", syncFrom.GetPath(), syncTo.GetPath());
		std::filesystem::copy(
			syncFrom.GetPath(),
			syncTo.GetPath(),
			std::filesystem::copy_options::recursive
		);
		LOG_DEBUG("Copying folders {0} and {1} completed", syncFrom.GetPath(), syncTo.GetPath());
		return;
	}

	bool addFile = false;

	for (const DirElement& element : syncFrom.GetElements())
	{
		for (const DirElement& refElement : syncTo.GetElements())
		{
			if (element.GetName() == refElement.GetName())
			{
				if (element.isFolder && refElement.isFolder)
				{
					Directory copySubFolder(element.path);
					Directory desSubFolder(refElement.path);

					Synchronizer::Get().SyncByName(desSubFolder, copySubFolder);

					addFile = false;
					break;
				}
				if (element.size == refElement.size)
				{
					if (Synchronizer::Get().AreContentsEqual(element, refElement))
					{
						addFile = false;
						break;
					}
					else
					{
						addFile = true;
						continue;
					}
				}
				else
				{
					addFile = true;
					continue;
				}
			}
			else
			{
				addFile = true;
				continue;
			}
		}
		if (addFile)
			Get().Copy(element, syncTo.GetPath());
	}
	LOG_DEBUG("Syncing of folders {0} and {1} completed in {2}s", syncFrom.GetPath(), syncTo.GetPath(), timer.GetDuration());
}

bool Synchronizer::AreContentsEqual(const DirElement& element, const DirElement& refElement)
{
	if (element.isFolder && refElement.isFolder)
		return true;
	else if (element.isFolder || refElement.isFolder)
		return false;

	LOG_DEBUG("Comapring contents of {0} and {1}", element.path, refElement.path);

	Timer timer;

	std::ifstream file(element.path.c_str(), std::ifstream::in | std::ifstream::binary);
	std::ifstream refFile(refElement.path.c_str(), std::ifstream::in | std::ifstream::binary);

	if (!file.is_open() || !refFile.is_open())
	{
		LOG_ERROR("Reading files failed!");
		return true;
	}

	std::unique_ptr<char> elementBuffer(new char[BUFFER_SIZE]);
	std::unique_ptr<char> refElementBuffer(new char[BUFFER_SIZE]);

	auto readFile = [&file, &elementBuffer]() { file.read(elementBuffer.get(), BUFFER_SIZE); };
	auto readRefFile = [&refFile, &refElementBuffer]() { refFile.read(refElementBuffer.get(), BUFFER_SIZE); };

	do
	{
		std::async(std::launch::async, readFile);
		std::async(std::launch::async, readRefFile);

		if (file.gcount() == refFile.gcount() && std::memcmp(elementBuffer.get(), refElementBuffer.get(), (size_t)file.gcount()) != 0)
		{
			LOG_DEBUG("Comparison completed in {0}. Contents are not the same.", timer.GetDuration());

			return false;
		}
	} while (file.good() || refFile.good());
	
	file.close();
	refFile.close();

	LOG_DEBUG("Comparison completed in {0}s. Contents are the same.", timer.GetDuration());

	return true;
}


void Synchronizer::Copy(const DirElement& elementToCopy, const std::string path) const
{
	LOG_DEBUG("Copying {0} to {1}", path, path);

	if (elementToCopy.isFolder)
		std::filesystem::create_directory(path + "\\" + elementToCopy.GetName());

	std::filesystem::copy(
		elementToCopy.path,
		elementToCopy.isFolder ? path + "\\" + elementToCopy.GetName() : path,
		std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing
	);

	LOG_DEBUG("Copying finished.");
	
}
