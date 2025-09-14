//=== API library ===
#include "./../../lib/nlohmann/json.hpp" // JSON library

//=== Helper libraries ===
#include <iostream>
#include <fstream> // helps manage inout and output streams
#include <filesystem> // helps manage files

//=== Header file ===
#include "Leveler.h"

//=== Namespace ===
using json = nlohmann::json;

//=== Class fields ===
json Leveler::levelData;
std::string Leveler::fileLocation;

/**
 * @author DargoDargonyx
 * @date 08/31/25
 *
 * @brief Method to load JSON files.
 *
 * This method will take in a string representation of the file
 * location in reference to the existing directory that the
 * main.cpp file is located and create the JSON file if it
 * does not exist.
 *
 * @param fileString The given file location represented as a string.
 * @return Whether or not the method was able to successfully load the JSON file.
 */
void Leveler::loadFile(const std::string& fileString) {

	Leveler::fileLocation = fileString;

	//=== Ensures that a file in the given location does in fact exist ===
	if (!Leveler::fileExists(fileString)) {
		std::cout << "JSON file {" << Leveler::fileLocation << "} has been created" << std::endl;
		Leveler::saveFile(Leveler::fileLocation);
	}

	//=== Loads the file into the local memory ===
	std::ifstream inFile(Leveler::fileLocation);
	if (inFile.is_open()) {
		inFile >> levelData;
		inFile.close();
		std::cout << "JSON file {" << Leveler::fileLocation << "} has been loaded into bot" << std::endl;
	}
}

/**
 * @author DargoDargonyx
 * @date 08/31/25
 *
 * @brief Saves JSON to the given location.
 *
 * This method will save the contents of the current
 * JSON object "levelData" onto a JSON file at the
 * location given.
 *
 * @param fileString The given file location represented as a string.
 */
void Leveler::saveFile(const std::string& fileString) {

	std::ofstream outFile(fileString);
	if (outFile.is_open()) {
		outFile << levelData.dump(4);
		outFile.close();
		std::cout << "JSON file {" << fileString << "} has been saved" << std::endl;
	} else {
		std::cout << "File saving error: Outstream was not open" << std::endl;
	}
}

/**
 * @author DargoDargonyx
 * @date 08/31/25
 *
 * @brief Checks if a file exists.
 *
 * This method will return true if a file exists
 * at the given file location and false if not.
 *
 * @param fileString The given file location represented as a string.
 * @return Whether or not the file exists.
 */
bool Leveler::fileExists(const std::string& fileString) {

	std::filesystem::path filePath = fileString;
	if (!std::filesystem::exists(filePath)) {
		std::cout << "File: {" << fileString << "} does not exist" << std::endl;
		return false;
	} else {
		return true;
	}
}

/**
 * @author DargoDargonyx
 * @date 08/31/25
 *
 * @brief User information update method.
 *
 * This method will take in the ID of the user who's information
 * needs to be updated as a string and how much XP needs to be added
 * to their "account" and will ensure that the XP has been distributed
 * into the user's levels.
 *
 * @param userId The ID of the user who's data needs to change.
 * @param username The username of the user who's data is to be changed.
 * @param xp The amount of xp to be added to the user's "account".
 * @return Whether or not the user leveled up.
 */
bool Leveler::addXP(const std::string& userId, const std::string& username, int xp) {

	//=== Checks if there is an existing entry ===
	if (!Leveler::levelData.contains(userId) || !levelData[userId].is_array()) {
		Leveler::levelData[userId] = json::array({username, 0, xp});
	}

	int64_t level = Leveler::levelData[userId][1].get<int64_t>();
	int64_t currentXP = Leveler::levelData[userId][2].get<int64_t>();
	currentXP += xp;

	bool levelUpStatus = false;

	//=== Ensures that XP is distributed ===
	while (true) {
		int threshold = Leveler::thresholdCalculator(userId);
		if (currentXP >= threshold) {
			currentXP -= threshold;
			level++;
			levelUpStatus = true;
		} else {
			break;
		}
	}

	//=== Saves changes to local memory JSON ===
	Leveler::levelData[userId][1] = level;
	Leveler::levelData[userId][2] = currentXP;

	//=== Checks to see the user changed their name ===
	if (Leveler::levelData[userId][0] != username) {
		Leveler::levelData[userId][0] = username;
	}

	return levelUpStatus;
}

/**
 * @author DargoDargonyx
 * @date 08/31/25
 *
 * @brief Returns the given user's name.
 *
 * This method will check to see if the given user has an
 * entry in the JSON file and returns the name if there is
 * an entry and an empty string if not.
 *
 * @param userId The ID of the user who's name is to be returned.
 * @return The name of the given user.
 */
std::string Leveler::getUserName(const std::string& userId) {

	if (Leveler::levelData.contains(userId)) {
		return Leveler::levelData[userId][0];
	} else {
		return "";
	}
}

/*
 * @author DargoDargonyx
 * @date 08/31/25
 *
 * @brief Returns the given user's level.
 *
 * This method will check to see if the given user has an
 * entry in the JSON file and returns the level of the user
 * if there is an entry and zero if not.
 *
 * @param userId The ID of the user who's level is to be returned.
 * @return The level of the given user.
 */
int Leveler::getUserLevel(const std::string& userId) {

	if (Leveler::levelData.contains(userId)) {
		return Leveler::levelData[userId][1];
	} else {
		return 0;
	}
}

/**
 * @author DargoDargonyx
 * @date 08/31/25
 *
 * @brief Returns the given user's amount of XP.
 *
 * This method will check to see if the given user has XP
 * and return the user's amount of XP as an integer if there
 * is XP and returns zero if not.
 * 
 * @param userId The ID of the user who's XP value is to be returned.
 * @return The amount of XP the given user has.
 */
int Leveler::getUserXP(const std::string& userId) {

	if (Leveler::levelData.contains(userId)) {
		return Leveler::levelData[userId][2];
	} else {
		return 0;
	}
}

/**
 * @author DargoDargonyx
 * @date 08/31/25
 *
 * @brief Returns an estimate of the level percentage of the given user.
 *
 * @param userId The ID of the user who's level percentage is to be returned.
 * @return The level percentage estimate as a number out of 100.
 */
int Leveler::getPercentEstimate(const std::string& userId) {

	return static_cast<int>((static_cast<float>(Leveler::levelData[userId][2]) 
			/ static_cast<float>(Leveler::thresholdCalculator(userId))) * 100);
}

/**
 * @author DargoDargonyx
 * @date 08/31/25
 *
 * @brief Helper fucntion used to calculate the level threshold.
 *
 * @param userId The ID of the user who's threshold needs to be calculated.
 * @return The level threshold.
 */
int Leveler::thresholdCalculator(const std::string& userId) {

	if (!Leveler::levelData.contains(userId)) {
		return 50;
	}
	const int levelBias = 50;
	const int levelWeight = 10;

	int level = Leveler::levelData[userId][1].get<int>();
	return (levelWeight * level * level) + levelBias;
}
