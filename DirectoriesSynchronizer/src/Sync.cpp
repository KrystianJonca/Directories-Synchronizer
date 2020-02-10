#include "Sync.h"

#include <filesystem>

void Sync::CompareByName(const Directory& syncTo, const Directory& syncFrom)
{
	m_ElementsToCopy.clear();
	m_SubfoldersToSync.clear();

	m_CopyToPath = syncTo.GetPath();
	LOG_DEBUG("Comparing by name {0} and {1}", syncFrom.GetPath(), syncTo.GetPath());
	if (syncTo.GetElements().empty())
	{
		m_ElementsToCopy.insert(m_ElementsToCopy.end(), syncFrom.GetElements().begin(), syncFrom.GetElements().end());

		LOG_DEBUG("Comperison completed");
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

					Sync syncedSubfolders;

					syncedSubfolders.CompareByName(desSubFolder, copySubFolder);

					m_SubfoldersToSync.push_back(std::make_shared<Sync>(syncedSubfolders));

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
			m_ElementsToCopy.push_back(element);
	}
	LOG_DEBUG("Comperison completed");
}
void Sync::CompareByNameAndSize(const Directory& syncTo, const Directory& syncFrom)
{
	m_ElementsToCopy.clear();
	m_SubfoldersToSync.clear();
	m_CopyToPath = syncTo.GetPath();
	LOG_DEBUG("Comparing by name and size {0} and {1}", syncFrom.GetPath(), syncTo.GetPath());
	if (syncTo.GetElements().empty())
	{
		m_ElementsToCopy.insert(m_ElementsToCopy.end(), syncFrom.GetElements().begin(), syncFrom.GetElements().end());

		LOG_DEBUG("Comperison completed");
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

					Sync syncedSubfolders;

					syncedSubfolders.CompareByNameAndSize(desSubFolder, copySubFolder);

					syncedSubfolders.CopyElements();

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
			m_ElementsToCopy.push_back(element);
	}
	LOG_DEBUG("Comperison completed");
}
void Sync::CompareByNameAndContent(const Directory& syncTo, const Directory& syncFrom)
{
	m_ElementsToCopy.clear();
	m_SubfoldersToSync.clear();
	m_CopyToPath = syncTo.GetPath();
	LOG_DEBUG("Comparing by name and content {0} and {1}", syncFrom.GetPath(), syncTo.GetPath());
	if (syncTo.GetElements().empty())
	{
		m_ElementsToCopy.insert(m_ElementsToCopy.end(), syncFrom.GetElements().begin(), syncFrom.GetElements().end());

		LOG_DEBUG("Comperison completed");
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

					Sync syncedSubfolders;

					syncedSubfolders.CompareByNameAndContent(desSubFolder, copySubFolder);

					m_SubfoldersToSync.push_back(std::make_shared<Sync>(syncedSubfolders));

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
			m_ElementsToCopy.push_back(element);
	}
	LOG_DEBUG("Comperison completed");
}

void Sync::CopyElements() const
{
	LOG_DEBUG("Copying subfolders...");
	for (int i = 0; i < m_SubfoldersToSync.size(); i++)
	{
		m_SubfoldersToSync[i]->CopyElements();
	}
	LOG_DEBUG("Finished.");
	for (const DirElement& elementToCopy : m_ElementsToCopy)
	{
		LOG_DEBUG("Copying {0} to {1}...", elementToCopy.path, m_CopyToPath);

		if (elementToCopy.isFolder)
			std::filesystem::create_directory(m_CopyToPath + "/" + elementToCopy.GetName());
		LOG_DEBUG(m_CopyToPath);
		std::filesystem::copy(
			elementToCopy.path,
			elementToCopy.isFolder ? m_CopyToPath + "/" + elementToCopy.GetName() : m_CopyToPath,
			std::filesystem::copy_options::recursive
		);
		
		LOG_DEBUG("Finished.");
	}
}
