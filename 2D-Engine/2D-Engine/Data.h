#pragma once

#include "Globals.h"
#include <SFML\System.hpp>
#include <fstream>
#include <iostream>
#include "cereal-1.2.2\include\cereal\archives\xml.hpp"
#include "cereal-1.2.2\include\cereal\archives\json.hpp"
#include "cereal-1.2.2\include\cereal\archives\binary.hpp"
#include "cereal-1.2.2\include\cereal\types\string.hpp"
#include "cereal-1.2.2\include\cereal\types\vector.hpp"

using namespace std;

class Data
{
public:
	Data();
	~Data();

	void SaveAsXML(string path);
	bool LoadXML(string path);
	void SaveAsJSON(string path);
	bool LoadJSON(string path);
	void SaveAsBinary(string path);
	bool LoadBinary(string path);

	void ClearData();

	void CreateSection(string sectionName);
	void CloseSection();
	bool EnterSection(string sectionName);
	void LeaveSection();

	bool GetBool(string valueName);
	int GetInt(string valueName);
	uint GetUInt(string valueName);
	double GetDouble(string valueName);
	float GetFloat(string valueName);
	string GetString(string valueName);
	sf::Vector2f GetVector2Float(string valueName);
	sf::Vector3f GetVector3Float(string valueName);
	sf::Vector2i GetVector2Int(string valueName);
	sf::Vector3i GetVector3Int(string valueName);

	void AddBool(string valueName, bool value);
	void AddInt(string valueName, int value);
	void AddUInt(string valueName, uint value);
	void AddDouble(string valueName, double value);
	void AddFloat(string valueName, float value);
	void AddString(string valueName, string value);
	void AddVector2Float(string valueName, sf::Vector2f value);
	void AddVector3Float(string valueName, sf::Vector3f value);
	void AddVector2Int(string valueName, sf::Vector2i value);
	void Addvector3Int(string valueName, sf::Vector3i);

private:

	friend class cereal::access;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(dataNames, dataValues);
	}

private:
	vector<string> dataNames;
	vector<string> dataValues;

	vector<string> inSectionNames;
	vector<string> inSectionValues;
	int sectionsOpen = 0;
	int currentIndex = 0;
	list<int> lastIndex;
	list<string> lastIndexName;

	vector<string> outSectionNames;
	vector<string> outSectionValues;

	bool gettingFromSection = false;
};

