#include <dpp/dpp.h>

//=== Helper libraries ===
#include <iostream>
#include <string>

//=== Classes built in the project ===
#include "GameBot.h"

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

	//=== Constructs the GameBot and names it Willow ===
	GameBot msWillow(
			"Willow", // name of the bot
			GENERAL_CHANNEL_ID,
			BOT_COMMAND_CHANNEL_ID,
			GUILD_ID
		);	

	msWillow.onReady();
	msWillow.commandListener();
	msWillow.messageListener();

	// Starts the bot
	msWillow.startInstance();

    return 0;
}
