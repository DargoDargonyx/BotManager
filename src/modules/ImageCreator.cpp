//=== API library ===
#include <Magick++.h> // Needs to be installed, used for making png's

//=== Helper libraries ===
#include <string>
#include <filesystem> // File managing library

//=== Header file ===
#include "ImageCreator.h"


/**
 * @author DargoDargonyx
 * @date 08/31/25
 *
 * @brief Draws a level bar
 *
 * This method is a helper method used to draw a level
 * for the use of the level command in the ManagerBot file.
 *
 */
Magick::Blob ImageCreator::constructLevelImg(
		int xp, 
		int threshold, 
		const std::string& barColor
	)
{
	using namespace Magick;

	//=== Initialize constant sizes ===
	const int WIDTH = 400;
	const int HEIGHT = 30;
	const int BAR_X = 0;
	const int BAR_Y = 0;

	//=== Sets the canvas ===
	Image image(Geometry(WIDTH, HEIGHT), Color(0, 0, 0, TransparentAlpha));
	image.alphaChannel(ActivateAlphaChannel);
	image.strokeColor("black");
	image.fillColor("gray");
	image.fontPointsize(18);

	//=== Background bar ===
	image.draw(DrawableRoundRectangle(
				BAR_X, 
				BAR_Y, 
				WIDTH, 
				HEIGHT, 
				10, 
				10));

	//=== Fill progress ===
	double percent = static_cast<double>(xp) / threshold;
	int fillWidth = static_cast<int>(WIDTH * percent);
	image.fillColor(barColor);
	image.draw(DrawableRoundRectangle(
				BAR_X, 
				BAR_Y, 
				BAR_X + fillWidth, 
				BAR_Y + HEIGHT,
				10,
				10));

	Magick::Blob blob;
	image.write(&blob, "PNG");
	return blob;
}
