//=== Header file ===
#include "Tools.h"

//=== Helper library ===
#include <string>
#include <cstdint>

uint32_t Tools::hexToIntColor(const std::string& hex) {
	return std::stoul(hex.substr(1), nullptr, 16);
}

std::string Tools::intColorToHex(const uint32_t& intColor) {
	char buffer[8];
	std::snprintf(buffer, sizeof(buffer), "#%06X", intColor & 0xFFFFFF);
	return std::string(buffer);
}
