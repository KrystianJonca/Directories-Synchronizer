#include "Synchronizer.h"

#include <filesystem>

const unsigned int BUFFER_SIZE = 1048576;

void Synchronizer::SyncByName(const Directory& syncTo, const Directory& syncFrom)
{
	LOG_DEBUG("Comparing by name {0} and {1}", syncFrom.GetPath(), syncTo.GetPath());

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
	LOG_DEBUG("Comperison of folders {0} and {1} completed", syncFrom.GetPath(), syncTo.GetPath());
}
void Synchronizer::SyncByNameAndSize(const Directory& syncTo, const Directory& syncFrom)
{
	LOG_DEBUG("Comparing by name and size {0} and {1}", syncFrom.GetPath(), syncTo.GetPath());

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
	LOG_DEBUG("Comperison of folders {0} and {1} completed", syncFrom.GetPath(), syncTo.GetPath());
}
void Synchronizer::SyncByNameAndContent(const Directory& syncTo, const Directory& syncFrom)
{
	LOG_DEBUG("Comparing by name, size and content {0} and {1}", syncFrom.GetPath(), syncTo.GetPath());

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
	LOG_DEBUG("Comperison of folders {0} and {1} completed", syncFrom.GetPath(), syncTo.GetPath());
}

bool Synchronizer::AreContentsEqual(const DirElement& elementPath, const DirElement& refElementPath)
{
	if (elementPath.isFolder && refElementPath.isFolder)
		return true;
	else if (elementPath.isFolder || refElementPath.isFolder)
		return false;

	LOG_DEBUG("Comapring contents of {0} and {1}", elementPath.path, refElementPath.path);

	std::ifstream file(elementPath.path.c_str(), std::ifstream::in | std::ifstream::binary);
	std::ifstream refFile(refElementPath.path.c_str(), std::ifstream::in | std::ifstream::binary);

	if (!file.is_open() || !refFile.is_open())
	{
		LOG_ERROR("Reading files failed!");
		return true;
	}

	std::unique_ptr<char> elementBuffer(new char[BUFFER_SIZE]);
	std::unique_ptr<char> refElementBuffer(new char[BUFFER_SIZE]);

	do
	{
		file.read(elementBuffer.get(), BUFFER_SIZE);
		refFile.read(refElementBuffer.get(), BUFFER_SIZE);

		if (std::memcmp(elementBuffer.get(), refElementBuffer.get(), file.gcount()) != 0)
		{
			LOG_DEBUG("Comparison completed. Contents are not the same.");

			return false;
		}
	} while (file.good() || refFile.good());
	
	LOG_DEBUG("Comparison completed. Contents are the same.");

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
