#pragma once
#include "../../SDK/SDK.h"

enum struct EItemType {
	DEFAULT,
	INT,
	FLOAT,
	BOOL
};

class CItemBase
{
public:
	std::string m_Name = {};
	EItemType e_Type = EItemType::DEFAULT;
	CItemBase(EItemType type, const std::string &name) : e_Type(type), m_Name(name) {};
};

class CItemInt : public CItemBase
{
public:
	int *m_Ptr = nullptr;
	std::vector<std::pair<int, std::string>> m_Aliases = {};
	int m_Min = std::numeric_limits<int>::min();
	int m_Max = std::numeric_limits<int>::max();

	int m_AliasIdx = 0;

	CItemInt(const std::string &name,
		int *ptr,
		std::vector<std::pair<int, std::string>> aliases = {},
		int min = std::numeric_limits<int>::min(),
		int max = std::numeric_limits<int>::max())
		: CItemBase(EItemType::INT, name), m_Ptr(ptr), m_Aliases(aliases), m_Min(min), m_Max(max)
	{
		if (!m_Aliases.empty())
		{
			bool Found = false;

			for (const auto &Alias : this->m_Aliases)
			{
				if (*reinterpret_cast<int *>(this->m_Ptr) == Alias.first) {
					this->m_AliasIdx = Alias.first;
					Found = true;
					break;
				}
			}

			if (!Found)
				*reinterpret_cast<int *>(this->m_Ptr) = this->m_Aliases.at(0).first;
		}
	}
};

class CItemFloat : public CItemBase
{
public:
	float *m_Ptr = nullptr;
	float m_Step = 1.0f;
	float m_Min = std::numeric_limits<float>::min();
	float m_Max = std::numeric_limits<float>::max();

	CItemFloat(const std::string &name,
		float *ptr,
		float step = 1.0f,
		float min = std::numeric_limits<float>::min(),
		float max = std::numeric_limits<float>::max())
		: CItemBase(EItemType::FLOAT, name), m_Ptr(ptr), m_Step(step), m_Min(min), m_Max(max) {}
};

class CItemBool : public CItemBase
{
public:
	bool *m_Ptr = nullptr;
	CItemBool(const std::string &name, bool *ptr) : CItemBase(EItemType::BOOL, name), m_Ptr(ptr) {}
};

class CItemGroup
{
public:
	std::string m_Name = {};
	std::vector<CItemBase *> m_Items = {};
	bool m_Open = true;
};

class CItemList
{
public:
	std::string m_Name = {};
	std::vector<CItemGroup *> m_ItemGroups = {};
};

class CMenu
{
private:
	std::vector<CItemList *> m_ItemLists = {};
	void CreateLists();

public:
	bool m_Open = false;
	bool m_OnAimbotFOV = false;

	~CMenu();
	void Run();
};

extern CMenu gMenu;