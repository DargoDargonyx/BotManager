#pragma once

//=== Helper libraries ===
#include <string>
#include <cstdint>

class Tools {
	public:
		virtual ~Tools() {};
		static uint32_t hexToIntColor(const std::string& hex);
		static std::string intColorToHex(const uint32_t& intColor);
};
