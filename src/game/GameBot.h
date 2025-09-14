#pragma once

//=== API library ===
#include <dpp/dpp.h>

//=== Helper libraries
#include <string>
#include <map> // Helps create mappings to string values

class GameBot {
	public:
		GameBot(
			const std::string&,
			const dpp::snowflake,
			const dpp::snowflake,
			const dpp::snowflake
		);
		void startInstance();
		void onReady();
		void commandListener();
		void messageListener();
	private:
		enum class CommandType {
			DESTROYGAME,
			TESTGAME,
			UNKNOWN
		};
		static const std::map<std::string, CommandType> commandMap;
		std::string gameBotName;
		dpp::cluster gameBot;
		dpp::snowflake GAME_GENERAL_CHANNEL_ID;
		dpp::snowflake GAME_BOT_COMMAND_CHANNEL_ID;
		dpp::snowflake GUILD_ID;
		void destroyGameCommand(const dpp::slashcommand_t&);
		void testGameCommand(const dpp::slashcommand_t&);
};
