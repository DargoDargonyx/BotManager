#pragma once

//=== API libraries ===
#include "dpp/dpp.h"

//=== Helper libraries ===
#include <string>
#include <map> // helps create mappings to string values

class ManagerBot {
	public:
		dpp::cluster botInitializer();
		ManagerBot(
			const std::string&,
			const std::string&,
			const dpp::snowflake,
			const dpp::snowflake,
			const dpp::snowflake
		);
		void startInstance();
		void onReady();
	private:
		enum class CommandType {	
			PING,
			LEVEL,
			DESTROY,
			TEST,
			UNKNOWN
		};
		static const std::map<std::string, CommandType> commandMap;
		std::string levelFileDescriptor;
		std::string managerBotName;
		dpp::cluster managerBot;
		dpp::snowflake GENERAL_CHANNEL_ID;
		dpp::snowflake BOT_COMMAND_CHANNEL_ID;
		dpp::snowflake GUILD_ID;
		void levelTracker();
		void commandListener();
		void pingCommand(const dpp::slashcommand_t&);
		void levelCommand(const dpp::slashcommand_t&);
		void destroyCommand(const dpp::slashcommand_t&);
		void testCommand(const dpp::slashcommand_t&);
		void testCommand(const dpp::slashcommand_t&, std::string num);
};
