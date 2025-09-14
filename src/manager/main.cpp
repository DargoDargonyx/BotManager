#include <dpp/dpp.h>

//=== Helper libraries ===
#include <iostream>
#include <string>

//=== Classes built in the project ===
#include "ManagerBot.h"

/**
 * @author DargoDargonyx
 * @author
 * @date 08/31/25
 *
 * @brief Main function
 */
int main() {

	//=== Discord channel id's ===
	const dpp::snowflake GENERAL_CHANNEL_ID = 1195202420360364107;
	const dpp::snowflake BOT_COMMAND_CHANNEL_ID = 1409719090968268811;
	const dpp::snowflake GUILD_ID = 1195202419601178664;

	//=== Constructs the ManagerBot and names it Weems ===
	ManagerBot mrWeems(
			"Weems", // name of the bot
			"./../storage/levelData.json", // level data lcoation
			GENERAL_CHANNEL_ID,
			BOT_COMMAND_CHANNEL_ID,
			GUILD_ID
		);

	mrWeems.onReady();

	// Starts the bots
	mrWeems.startInstance();

    return 0;
}
