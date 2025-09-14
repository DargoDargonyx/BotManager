#pragma once

//=== API Library ===
#include "./../../lib/nlohmann/json.hpp"

//=== Helper library ===
#include <string>

//=== Namespace ===
using json = nlohmann::json; 

class Leveler {
	public:
		virtual ~Leveler() {};
		static void loadFile(const std::string&);
		static void saveFile(const std::string&);
		static bool fileExists(const std::string&);
		static bool addXP(const std::string&, const std::string&, int);
		static std::string getUserName(const std::string&);
		static int getUserXP(const std::string&);
		static int getUserLevel(const std::string&);
		static int getPercentEstimate(const std::string&);
		static int thresholdCalculator(const std::string&);
	private:
		static json levelData;
		static std::string fileLocation;
};
