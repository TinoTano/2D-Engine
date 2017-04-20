#include "Data.h"

Data::Data()
{
}


Data::~Data()
{
}

void Data::SaveAsXML(string path)
{
	if (path.find(".xml") == string::npos) { //xml extension is not especified
		path += ".xml";
	}
	std::ofstream file(path);
	cereal::XMLOutputArchive archive(file);
	for (int i = 0; i < dataNames.size(); i++) {
		if (dataNames[i] == "New_Section") {
			replace(dataNames[i + 1].begin(), dataNames[i + 1].end(), ' ', '_');
			archive.setNextName(dataNames[i + 1].c_str());
			archive.startNode();
			i++;
		}
		else if (dataNames[i] == "Section_Close") {
			archive.finishNode();
		}
		else {
			archive(cereal::make_nvp(dataNames[i], dataValues[i]));
		}
	}
}

bool Data::LoadXML(string path)
{
	bool ret = false;
	bool outsideSection = true;
	int sectionsOpen = 0;
	dataNames.clear();
	dataValues.clear();
	std::ifstream file(path);
	if (file.is_open()) {
		cereal::XMLInputArchive archive(file);
		while (true) {
			string nodeName;
			string nodeValue;
			if (archive.getNodeName() != NULL) {
				nodeName = archive.getNodeName();
			}
			else {
				if (sectionsOpen == 1) {
					outsideSection = true;
				}
				if (archive.isLastNode()) {
					break;
				}
				else {
					dataNames.push_back("Section_Close");
					dataValues.push_back("");
					archive.finishNode();
					sectionsOpen--;
					continue;
				}
			}
			archive.startNode();
			archive.loadValue(nodeValue);
			dataNames.push_back(nodeName);
			if (nodeValue != "") {
				dataValues.push_back(nodeValue);
				if (outsideSection) {
					outSectionValues.push_back(nodeValue);
					outSectionNames.push_back(nodeName);
				}
			}
			else {
				dataValues.push_back("Section");
				outsideSection = false;
				sectionsOpen++;
				continue;
			}
			archive.finishNode();
		}
		ret = true;
	}
	return ret;
}

void Data::SaveAsJSON(string path)
{
	if (path.find(".json") == string::npos) { //json extension is not especified
		path += ".json";
	}
	std::ofstream file(path);
	cereal::JSONOutputArchive archive(file);
	for (int i = 0; i < dataNames.size(); i++) {
		if (dataNames[i] == "New_Section") {
			replace(dataNames[i + 1].begin(), dataNames[i + 1].end(), ' ', '_');
			archive.setNextName(dataNames[i + 1].c_str());
			archive.startNode();
			i++;
		}
		else if (dataNames[i] == "Section_Close") {
			archive.finishNode();
		}
		else {
			archive(cereal::make_nvp(dataNames[i], dataValues[i]));
		}
	}
}

bool Data::LoadJSON(string path)
{
	bool ret = false;
	bool outsideSection = true;
	int sectionsOpen = 0;
	dataNames.clear();
	dataValues.clear();
	std::ifstream file(path);
	if (file.is_open()) {
		cereal::JSONInputArchive archive(file);
		while (true) {
			string nodeName;
			string nodeValue;
			if (archive.isObject()) {
				if (archive.isEmptyObject()) {
					archive.startNode();
					archive.finishNode();
					continue;
				}
				else {
					if (archive.getNodeName() != NULL) {
						nodeName = archive.getNodeName();
						dataNames.push_back(nodeName);
						dataValues.push_back("Section");
						archive.startNode();
						outsideSection = false;
						sectionsOpen++;
					}
					else {
						dataNames.push_back("Section_Close");
						dataValues.push_back("");
						sectionsOpen--;
						if (sectionsOpen == 0) {
							outsideSection = true;
						}
						if (archive.finishNode())
						{
							break;
						}
					}
				}
			}
			else {
				if (archive.getNodeName() != NULL) {
					nodeName = archive.getNodeName();
					archive.loadValue(nodeValue);
					dataNames.push_back(nodeName);
					dataValues.push_back(nodeValue);
					if (outsideSection) {
						outSectionValues.push_back(nodeValue);
						outSectionNames.push_back(nodeName);
					}
				}
				else {
					dataNames.push_back("Section_Close");
					sectionsOpen--;
					dataValues.push_back("");
					if (sectionsOpen == 0) {
						outsideSection = true;
					}
					if (archive.finishNode())
					{
						break;
					}
				}
			}
		}
		ret = true;
	}
	return ret;
}

void Data::SaveAsBinary(string path)
{
	std::ofstream file(path);
	cereal::BinaryOutputArchive archive(file);
	archive(*this);
}

bool Data::LoadBinary(string path)
{
	bool ret = false;
	bool outsideSection = true;
	int sectionsOpen = 0;
	dataNames.clear();
	dataValues.clear();
	std::ifstream file(path);
	if (file.is_open()) {
		cereal::BinaryInputArchive archive(file);
		archive(*this);
		for (int i = 0; i < dataNames.size(); i++) {
			if (dataNames[i] == "New_Section") {
				dataNames.erase(dataNames.begin() + i);
				dataValues.erase(dataValues.begin() + i);
				outsideSection = false;
				sectionsOpen++;
			}
			else if (dataNames[i] == "Section_Close") {
				sectionsOpen--;
				if (sectionsOpen == 0) {
					outsideSection = true;
				}
			}
			else {
				if (outsideSection) {
					outSectionNames.push_back(dataNames[i]);
					outSectionValues.push_back(dataValues[i]);
				}
			}
		}
		ret = true;
	}
	return ret;
}

void Data::ClearData()
{
	dataNames.clear();
	dataValues.clear();
}

void Data::CreateSection(string sectionName)
{
	dataNames.push_back("New_Section");
	dataValues.push_back("");
	dataNames.push_back(sectionName);
	dataValues.push_back("Section");
}

bool Data::EnterSection(string sectionName)
{
	bool ret = false;
	gettingFromSection = true;
	vector<string>::iterator it = find(dataNames.begin() + currentIndex, dataNames.end(), sectionName);
	if (it != dataNames.end()) {
		int index = it - dataNames.begin();
		if (dataValues[index] == "Section") {
			lastIndex.push_back(index);
			lastIndexName.push_back(sectionName);
			currentIndex = index;
			inSectionValues.clear();
			inSectionNames.clear();
			sectionsOpen++;
			int internalSectionsOpen = 1; //Avoid getting data from other sections inside this one
			bool isVector = false;
			for (int i = index + 1; internalSectionsOpen > 0; i++) {
				if (dataValues[i] == "Section") {
					if (dataNames[i + 1] != "value_X" && dataNames[i + 2] != "value_Y") { //vectors are not new sections
						internalSectionsOpen++;
					}
					else {
						isVector = true;
					}
				}
				if (dataNames[i] == "Section_Close") {
					if (!isVector) {
						internalSectionsOpen--;
					}
					else {
						isVector = false;
					}
				}
				else if (internalSectionsOpen < 2) {
					inSectionValues.push_back(dataValues[i]);
					inSectionNames.push_back(dataNames[i]);
				}
			}
			ret = true;
		}
	}
	return ret;
}

void Data::LeaveSection() {
	inSectionValues.clear();
	inSectionNames.clear();
	sectionsOpen--;
	if (sectionsOpen == 0) {
		gettingFromSection = false;
	}
	else {
		if (!lastIndex.empty() && !lastIndexName.empty()) {
			lastIndex.pop_back();
			lastIndexName.pop_back();
			currentIndex = lastIndex.back();
			EnterSection(lastIndexName.back());
			//remove again because we are adding those during "EnterSection"
			lastIndex.pop_back();
			lastIndexName.pop_back();
			sectionsOpen--;
		}
	}
}

void Data::CloseSection()
{
	dataNames.push_back("Section_Close");
	dataValues.push_back("");
}

bool Data::GetBool(string valueName)
{
	replace(valueName.begin(), valueName.end(), ' ', '_');
	vector<string> vecNames;
	vector<string> vecValues;

	if (gettingFromSection) {
		vecNames = inSectionNames;
		vecValues = inSectionValues;
	}
	else {
		vecNames = outSectionNames;
		vecValues = outSectionValues;
	}

	vector<string>::iterator it = find(vecNames.begin(), vecNames.end(), valueName);
	if (it != vecNames.end()) {
		int index = it - vecNames.begin();
		if (vecValues[index] == "1") return true;
	}

	return false;
}

int Data::GetInt(string valueName)
{
	replace(valueName.begin(), valueName.end(), ' ', '_');
	vector<string> vecNames;
	vector<string> vecValues;

	if (gettingFromSection) {
		vecNames = inSectionNames;
		vecValues = inSectionValues;
	}
	else {
		vecNames = outSectionNames;
		vecValues = outSectionValues;
	}

	vector<string>::iterator it = find(vecNames.begin(), vecNames.end(), valueName);
	if (it != vecNames.end()) {
		int index = it - vecNames.begin();
		return stoi(vecValues[index]);
	}
	return -1;
}

uint Data::GetUInt(string valueName)
{
	replace(valueName.begin(), valueName.end(), ' ', '_');
	vector<string> vecNames;
	vector<string> vecValues;

	if (gettingFromSection) {
		vecNames = inSectionNames;
		vecValues = inSectionValues;
	}
	else {
		vecNames = outSectionNames;
		vecValues = outSectionValues;
	}

	vector<string>::iterator it = find(vecNames.begin(), vecNames.end(), valueName);
	if (it != vecNames.end()) {
		int index = it - vecNames.begin();
		return stoul(vecValues[index]);
	}
	return 0;
}

double Data::GetDouble(string valueName)
{
	replace(valueName.begin(), valueName.end(), ' ', '_');
	vector<string> vecNames;
	vector<string> vecValues;

	if (gettingFromSection) {
		vecNames = inSectionNames;
		vecValues = inSectionValues;
	}
	else {
		vecNames = outSectionNames;
		vecValues = outSectionValues;
	}

	vector<string>::iterator it = find(vecNames.begin(), vecNames.end(), valueName);
	if (it != vecNames.end()) {
		int index = it - vecNames.begin();
		return stod(vecValues[index]);
	}
	return -1.0;
}

float Data::GetFloat(string valueName)
{
	replace(valueName.begin(), valueName.end(), ' ', '_');
	vector<string> vecNames;
	vector<string> vecValues;

	if (gettingFromSection) {
		vecNames = inSectionNames;
		vecValues = inSectionValues;
	}
	else {
		vecNames = outSectionNames;
		vecValues = outSectionValues;
	}

	vector<string>::iterator it = find(vecNames.begin(), vecNames.end(), valueName);
	if (it != vecNames.end()) {
		int index = it - vecNames.begin();
		return stof(vecValues[index]);
	}
	return -1.0f;
}

string Data::GetString(string valueName)
{
	replace(valueName.begin(), valueName.end(), ' ', '_');
	vector<string> vecNames;
	vector<string> vecValues;

	if (gettingFromSection) {
		vecNames = inSectionNames;
		vecValues = inSectionValues;
	}
	else {
		vecNames = outSectionNames;
		vecValues = outSectionValues;
	}

	vector<string>::iterator it = find(vecNames.begin(), vecNames.end(), valueName);
	if (it != vecNames.end()) {
		int index = it - vecNames.begin();
		return vecValues[index];
	}
	return "value not found";
}

sf::Vector2f Data::GetVector2Float(string valueName)
{
	sf::Vector2f ret;
	replace(valueName.begin(), valueName.end(), ' ', '_');

	vector<string> vecNames;
	vector<string> vecValues;

	if (gettingFromSection) {
		vecNames = inSectionNames;
		vecValues = inSectionValues;
	}
	else {
		vecNames = outSectionNames;
		vecValues = outSectionValues;
	}

	vector<string>::iterator it = find(vecNames.begin(), vecNames.end(), valueName);
	if (it != vecNames.end()) {
		int index = it - vecNames.begin();
		ret.x = stof(vecValues[index + 1]);
		ret.y = stof(vecValues[index + 2]);
	}
	else {
		ret.x = -1.0f;
		ret.y = -1.0f;
	}
	return ret;
}

sf::Vector3f Data::GetVector3Float(string valueName)
{
	sf::Vector3f ret;
	replace(valueName.begin(), valueName.end(), ' ', '_');

	vector<string> vecNames;
	vector<string> vecValues;

	if (gettingFromSection) {
		vecNames = inSectionNames;
		vecValues = inSectionValues;
	}
	else {
		vecNames = outSectionNames;
		vecValues = outSectionValues;
	}

	vector<string>::iterator it = find(vecNames.begin(), vecNames.end(), valueName);
	if (it != vecNames.end()) {
		int index = it - vecNames.begin();
		ret.x = stof(vecValues[index + 1]);
		ret.y = stof(vecValues[index + 2]);
		ret.z = stof(vecValues[index + 3]);
	}
	else {
		ret.x = -1.0f;
		ret.y = -1.0f;
		ret.z = -1.0f;
	}
	return ret;
}

sf::Vector2i Data::GetVector2Int(string valueName)
{
	sf::Vector2i ret;
	replace(valueName.begin(), valueName.end(), ' ', '_');

	vector<string> vecNames;
	vector<string> vecValues;

	if (gettingFromSection) {
		vecNames = inSectionNames;
		vecValues = inSectionValues;
	}
	else {
		vecNames = outSectionNames;
		vecValues = outSectionValues;
	}

	vector<string>::iterator it = find(vecNames.begin(), vecNames.end(), valueName);
	if (it != vecNames.end()) {
		int index = it - vecNames.begin();
		ret.x = stoi(vecValues[index + 1]);
		ret.y = stoi(vecValues[index + 2]);
	}
	else {
		ret.x = -1;
		ret.y = -1;
	}
	return ret;
}

sf::Vector3i Data::GetVector3Int(string valueName)
{
	sf::Vector3i ret;
	replace(valueName.begin(), valueName.end(), ' ', '_');

	vector<string> vecNames;
	vector<string> vecValues;

	if (gettingFromSection) {
		vecNames = inSectionNames;
		vecValues = inSectionValues;
	}
	else {
		vecNames = outSectionNames;
		vecValues = outSectionValues;
	}

	vector<string>::iterator it = find(vecNames.begin(), vecNames.end(), valueName);
	if (it != vecNames.end()) {
		int index = it - vecNames.begin();
		ret.x = stoi(vecValues[index + 1]);
		ret.y = stoi(vecValues[index + 2]);
		ret.z = stoi(vecValues[index + 3]);
	}
	else {
		ret.x = -1;
		ret.y = -1;
		ret.z = -1;
	}
	return ret;
}

void Data::AddBool(string valueName, bool value)
{
	replace(valueName.begin(), valueName.end(), ' ', '_');
	dataValues.push_back(to_string(value));
	dataNames.push_back(valueName);
}

void Data::AddInt(string valueName, int value)
{
	replace(valueName.begin(), valueName.end(), ' ', '_');
	dataValues.push_back(to_string(value));
	dataNames.push_back(valueName);
}

void Data::AddUInt(string valueName, uint value)
{
	replace(valueName.begin(), valueName.end(), ' ', '_');
	dataValues.push_back(to_string(value));
	dataNames.push_back(valueName);
}

void Data::AddDouble(string valueName, double value)
{
	replace(valueName.begin(), valueName.end(), ' ', '_');
	dataValues.push_back(to_string(value));
	dataNames.push_back(valueName);
}

void Data::AddFloat(string valueName, float value)
{
	replace(valueName.begin(), valueName.end(), ' ', '_');
	dataValues.push_back(to_string(value));
	dataNames.push_back(valueName);
}

void Data::AddString(string valueName, string value)
{
	replace(valueName.begin(), valueName.end(), ' ', '_');
	dataValues.push_back(value);
	dataNames.push_back(valueName);
}

void Data::AddVector2Float(string valueName, sf::Vector2f value)
{
	replace(valueName.begin(), valueName.end(), ' ', '_');
	CreateSection(valueName);
	dataValues.push_back(to_string(value.x));
	dataNames.push_back("value_X");
	dataValues.push_back(to_string(value.y));
	dataNames.push_back("value_Y");
	CloseSection();
}

void Data::AddVector3Float(string valueName, sf::Vector3f value)
{
	replace(valueName.begin(), valueName.end(), ' ', '_');
	CreateSection(valueName);
	dataValues.push_back(to_string(value.x));
	dataNames.push_back("value_X");
	dataValues.push_back(to_string(value.y));
	dataNames.push_back("value_Y");
	dataValues.push_back(to_string(value.z));
	dataNames.push_back("value_Z");
	CloseSection();
}

void Data::AddVector2Int(string valueName, sf::Vector2i value)
{
	replace(valueName.begin(), valueName.end(), ' ', '_');
	CreateSection(valueName);
	dataValues.push_back(to_string(value.x));
	dataNames.push_back("value_X");
	dataValues.push_back(to_string(value.y));
	dataNames.push_back("value_Y");
	CloseSection();
}

void Data::Addvector3Int(string valueName, sf::Vector3i value)
{
	replace(valueName.begin(), valueName.end(), ' ', '_');
	CreateSection(valueName);
	dataValues.push_back(to_string(value.x));
	dataNames.push_back("value_X");
	dataValues.push_back(to_string(value.y));
	dataNames.push_back("value_Y");
	dataValues.push_back(to_string(value.z));
	dataNames.push_back("value_Z");
	CloseSection();
}
