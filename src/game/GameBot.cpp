#include "GameBot.h"

#include <dpp/dpp.h>
#include "./../../lib/laserpants/dotenv/dotenv.h"


#include <iostream>
#include <string>
#include <map>
#include <chrono>



const std::map<std::string, GameBot::CommandType> GameBot::commandMap = {

	{"destroy_game", CommandType::DESTROYGAME},
	{"test_game", CommandType::TESTGAME}
};


dpp::cluster gameBot;
std::string gameBotName;
dpp::snowflake GAME_GENERAL_CHANNEL_ID;
dpp::snowflake GAME_BOT_COMMMAND_CHANNEL_ID;
dpp::snowflake GUILD_ID;



GameBot::GameBot(
		const std::string& botName, 
		const dpp::snowflake generalChannelId, 
		const dpp::snowflake botCommandChannelId,
		const dpp::snowflake guildId) : 
			gameBot([] {
				dotenv::init();
				const char* token_cstr = getenv("WILLOW_TOKEN");
				if (!token_cstr) {
					std::cerr << "Error: Loading environment file failed" << std::endl;;
					exit(1);
				}
				return std::string(token_cstr);
			}(),
		    dpp::i_default_intents | dpp::i_message_content)
{

	//=== Sets field values ===
	this->gameBotName = botName;
	this->GAME_GENERAL_CHANNEL_ID = generalChannelId;
	this->GAME_BOT_COMMAND_CHANNEL_ID = botCommandChannelId;
	this->GUILD_ID = guildId;
}




void GameBot::onReady() {

	this->gameBot.on_ready([this](const dpp::ready_t& event) {

		//=== Sends a message to a specifc channel on start ===
		std::string startupMsg = this->gameBotName + " is now online!";
		this->gameBot.message_create(dpp::message(
					this->GAME_BOT_COMMAND_CHANNEL_ID, startupMsg));

		//=== Initializes slashcommands ===
        if (dpp::run_once<struct register_bot_commands>()) {
            
			dpp::slashcommand destroyCMD(
					"destroy_game", 
					"Quit Program", 
					this->gameBot.me.id);
            
			dpp::slashcommand testCMD(
					"test_game", 
					"Testing Case", 
					this->gameBot.me.id);

            this->gameBot.guild_command_create(destroyCMD, this->GUILD_ID);
            this->gameBot.guild_command_create(testCMD, this->GUILD_ID);
        }
	});
}


void GameBot::commandListener() {

	this->gameBot.on_slashcommand([this](const dpp::slashcommand_t& event) {
			
		//=== Matches the string mapping for the given command ===
		CommandType commandName = GameBot::CommandType::UNKNOWN;
		auto iter = commandMap.find(event.command.get_command_name());
		if (iter != commandMap.end()) {
			commandName = iter -> second;
		}

		std::string commandTitle;
		switch(commandName) {

			case GameBot::CommandType::DESTROYGAME: {
				commandTitle = "destroy_game";
				this->destroyGameCommand(event);
				break;
			}
			case GameBot::CommandType::TESTGAME: {
				commandTitle = "test_game";
				this->testGameCommand(event);
				break;
			}
			default:
				event.reply("Error: Attempting to use a non-valid command");
				break;
		}
	});
}

void GameBot::messageListener() {

	this->gameBot.on_message_create([this](const dpp::message_create_t& event) {

		std::string userId = std::to_string(event.msg.author.id);
		std::string displayName;
		std::string content = event.msg.content;

	    std::transform(content.begin(), content.end(), content.begin(), ::tolower);    
		std::vector<std::string> greetings = {
			"hey willow",
			"hi willow",
			"hai willow",
			"hello willow",
			"hey hey willow",
			"hey willow!",
			"hi willow!",
			"hai willow!",
			"hello willow",
			"hey hey willow!"
		};

		std::vector<std::string> inputGreetings = {
			"how are you",
			"how are you doing",
			"how goes it"
			"how are you?",
			"how are you doing?",
			"how goes it?"
		};

		if (!event.msg.author.global_name.empty()) {
			displayName = event.msg.author.global_name;
		} else {
			displayName = event.msg.author.username;
		}

		for (const auto& greet : greetings) {

			if (content.find(greet) == 0) { // Starts with greet

				for (const auto& question : inputGreetings) {

					if (content.size() >= question.size() &&
						content.compare(
							content.size() - question.size(), 
							question.size(), 
							question) == 0) 
					{
						std::string response = "Hai " 
							+ displayName 
							+ "! I'm doing great, thanks for asking! <3";

						this->gameBot.message_create(dpp::message(
									event.msg.channel_id, 
									response
								));
						return;
					}
				}

				// Only matched greeting
				std::string response = "Hai " + displayName + "! <3";
				this->gameBot.message_create(dpp::message(
							event.msg.channel_id, 
							response
						));
				return;
			}
		}
	});
}

void GameBot::destroyGameCommand(const dpp::slashcommand_t& event) {

	std::string deathMsg = this->gameBotName + " is now offline!";
	event.reply(deathMsg);
	std::this_thread::sleep_for(std::chrono::milliseconds(250));
	this->gameBot.shutdown();
}

void GameBot::testGameCommand(const dpp::slashcommand_t& event) {

	event.reply("Nothing to test at the moment.");
}

void GameBot::startInstance() {
	this->gameBot.start(dpp::st_wait);
}
