#pragma once

//=== API library ===
#include <Magick++.h> // Needs to be installed, used for making png's

//=== Helper library ===
#include <string>

class ImageCreator {
	public:
		virtual ~ImageCreator() {};
		static Magick::Blob constructLevelImg(int, int, const std::string&);
};
