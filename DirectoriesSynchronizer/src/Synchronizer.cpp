#include "Synchronizer.h"

#include <filesystem>

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
		for (const DirElement& desElement : syncTo.GetElements())
		{
			if (element.GetName() == desElement.GetName())
			{
				if (element.isFolder && desElement.isFolder)
				{
					Directory copySubFolder(element.path);
					Directory desSubFolder(desElement.path);

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
		for (const DirElement& desElement : syncTo.GetElements())
		{
			if (element.GetName() == desElement.GetName())
			{
				if (element.isFolder && desElement.isFolder)
				{
					Directory copySubFolder(element.path);
					Directory desSubFolder(desElement.path);

					Synchronizer::Get().SyncByName(desSubFolder, copySubFolder);

					addFile = false;
					break;
				}
				if (element.size == desElement.size)
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
		for (const DirElement& desElement : syncTo.GetElements())
		{
			if (element.GetName() == desElement.GetName())
			{
				if (element.isFolder && desElement.isFolder)
				{
					Directory copySubFolder(element.path);
					Directory desSubFolder(desElement.path);

					Synchronizer::Get().SyncByName(desSubFolder, copySubFolder);

					addFile = false;
					break;
				}
				if (element.size == desElement.size)
				{
					if (element.GetContent() == desElement.GetContent())
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

void Synchronizer::Copy(const DirElement& elementToCopy, const std::string path) const
{
	LOG_DEBUG("Copying {0} to {1}", path, path);

	if (elementToCopy.isFolder)
		std::filesystem::create_directory(path + "\\" + elementToCopy.GetName());

	std::filesystem::copy(
		elementToCopy.path,
		elementToCopy.isFolder ? path + "\\" + elementToCopy.GetName() : path,
		std::filesystem::copy_options::recursive
	);

	LOG_DEBUG("Copying finished.");
	
}
