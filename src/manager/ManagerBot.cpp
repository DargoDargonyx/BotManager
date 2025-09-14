//=== API libraries ===
#include "./../../lib/laserpants/dotenv/dotenv.h"
#include "dpp/dpp.h"
#include <Magick++.h> // Needs to be installed, used for making png's

//=== Helper libraries ===
#include <iostream>
#include <string>
#include <map> // helps create mappings to string values
#include <thread>
#include <chrono> // time management library

//=== Classes built in the project ===
#include "./../modules/Announcer.h"
#include "./../modules/Filter.h"
#include "./../modules/Leveler.h"
#include "./../modules/RoleManager.h"
#include "./../modules/ImageCreator.h"
#include "./../modules/Tools.h"

//=== Header file ===
#include "ManagerBot.h"

/**
 * @author DargoDargonyx
 * @date 08/31/25
 *
 * @brief Slash command name mapping.
 */
const std::map<std::string, ManagerBot::CommandType> ManagerBot::commandMap = {

	{"ping", CommandType::PING},
	{"level", CommandType::LEVEL},
	{"destroy", CommandType::DESTROY},
	{"test", CommandType::TEST}
};

//=== Class fields ===
std::string levelFileDescriptor;
dpp::cluster managerBot;
std::string managerBotName;
dpp::snowflake GENERAL_CHANNEL_ID;
dpp::snowflake BOT_COMMMAND_CHANNEL_ID;

/**
 * @author DargoDargonyx
 * @date 08/31/25
 *
 * @brief Four-argument constructor for the class.
 *
 * This constructor has a lengthy parameter list, and is the constructor
 * inteded to use for the purposes of the orginal creation. This
 * constructor also makes use of a lambda function in its header
 * in order to instantiate the dpp::cluster bot from the discord API.
 *
 * @param botName The name of the bot.
 * @param levelFileDescriptor This is the file location to store the level data.
 * @param generalChannelId The general channel ID for the discord server.
 * @param botCommandChannelId The bot commands channel ID for the discord server.
 */
ManagerBot::ManagerBot(
		const std::string& botName, 
		const std::string& levelFileDescriptor, 
		const dpp::snowflake generalChannelId, 
		const dpp::snowflake botCommandChannelId) : 
			managerBot([] {
				dotenv::init();
				const char* token_cstr = getenv("WEEMS_TOKEN");
				if (!token_cstr) {
					std::cerr << "Error: Loading environment file failed" << std::endl;;
					exit(1);
				}
				return std::string(token_cstr);
			}()) {

	//=== Sets field values ===
	this->managerBotName = botName;
	this->levelFileDescriptor = levelFileDescriptor;
	this->GENERAL_CHANNEL_ID = generalChannelId;
	this->BOT_COMMAND_CHANNEL_ID = botCommandChannelId;

	//=== Loads the XP JSON file ====
	Leveler::loadFile(levelFileDescriptor);

	//=== Starts helper functions ===
	levelTracker();
	commandListener();
}

/**
 * @author DargoDargonyx
 * @date 08/31/25
 *
 * @brief Handles commands for when the bot first starts.
 *
 * This method will send a message to a channel when the bot is
 * to display that the bot is online, then the bot will ensure
 * that all slahscommands are registered.
 */
void ManagerBot::onReady() {

	Magick::InitializeMagick(nullptr);
	this->managerBot.on_ready([this](const dpp::ready_t& event) {

		//=== Sends a message to a specifc channel on start ===
		std::string startupMsg = this->managerBotName + " is now online!";
		this->managerBot.message_create(dpp::message(
					this->BOT_COMMAND_CHANNEL_ID, startupMsg));


		//=== Initializes slashcommands ===
		if (dpp::run_once<struct register_bot_commands>()) {
			
			//=== Ping command ===
			this->managerBot.global_command_create(
					dpp::slashcommand("ping", "Ping Pong!", this->managerBot.me.id));

			//=== Level command ===
			this->managerBot.global_command_create(
					dpp::slashcommand(
							"level", 
							"Level Status Check", 
							this->managerBot.me.id
					).add_option(
						dpp::command_option(
								dpp::co_user, 
								"other_user", 
								"Option to check other users level", 
								false)));

			//=== Destroy command ===
			this->managerBot.global_command_create(
					dpp::slashcommand(
						"destroy", 
						"Quit Program", 
						this->managerBot.me.id));

			//=== Test command ===
			this->managerBot.global_command_create(
					dpp::slashcommand(
						"test", 
						"Testing Case", 
						this->managerBot.me.id
					).add_option(
							dpp::command_option(
								dpp::co_string, 
								"input_num", 
								"Testing option", 
								false)));
		}
	});
}

/**
 * @author DargoDargonyx
 * @date 08/31/25
 *
 * @brief Level system manager.
 *
 * This method is the main level system manager and tracks activities
 * in the discord server that could earn a user XP.
 */
void ManagerBot::levelTracker() {

	this->managerBot.on_message_create([this](const dpp::message_create_t& event) {

		std::string userId = std::to_string(event.msg.author.id);
		int messageXP = 10;
		std::string displayName;

		if (!event.msg.author.global_name.empty()) {
			displayName = event.msg.author.global_name;
		} else {
			displayName = event.msg.author.username;
		}

		if (event.msg.author.id != this->managerBot.me.id) {

			//=== Adds XP and checks if the user leveled up ===
			if (Leveler::addXP(userId, displayName, 10)) {
				std::string mention = "<@" + userId + ">";
				std::string content = mention + " You have leveled up! You are now level " 
					+ std::to_string(Leveler::getUserLevel(userId)) + "!";

				dpp::message reply(event.msg.channel_id, content);
				reply.set_allowed_mentions(true);
				this->managerBot.message_create(reply);
			}

			//=== Saves changes ===
			Leveler::saveFile(this->levelFileDescriptor);
		}
	});
}

/**
 * @author DargoDargonyx
 * @date 08/31/25
 *
 * @brief Listens out for commands in the discord server.
 *
 * This method is called within the constructor and listens out
 * for command calls in the discord server and uses a switch
 * statement to call the appropriate helper method.
 */
void ManagerBot::commandListener() {

	this->managerBot.on_slashcommand([this](const dpp::slashcommand_t& event) {
			
		//=== Matches the string mapping for the given command ===
		CommandType commandName = ManagerBot::CommandType::UNKNOWN;
		auto iter = commandMap.find(event.command.get_command_name());
		if (iter != commandMap.end()) {
			commandName = iter -> second;
		}

		std::string commandTitle;
		switch(commandName) {

			case ManagerBot::CommandType::PING:{
				commandTitle = "PING";
				this->pingCommand(event);
				break;
			}
			case ManagerBot::CommandType::LEVEL: {
				commandTitle = "LEVEL";
				this->levelCommand(event);
				break;
			}
			case ManagerBot::CommandType::DESTROY: {
				commandTitle = "DESTROY";
				this->destroyCommand(event);
				break;
			}
			case ManagerBot::CommandType::TEST: {
				commandTitle = "TEST";
				auto param = event.get_parameter("input_num");

				if (std::holds_alternative<std::string>(param)) {
					std::string value = std::get<std::string>(param);
					this->testCommand(event, value);
				} else {
					this->testCommand(event);
				}
				break;
			}
			default:
				event.reply("Error: Attempting to use a non-valid command");
				break;
		}
	});
}

/**
 * @author DargoDargonyx
 * @date 08/31/25
 *
 * @brief Helper method for the ping command.
 *
 * This method/command is used to ensure that the bot
 * is running and will respond to slashcommands.
 *
 * @param event The slashcommand event.
 */
void ManagerBot::pingCommand(const dpp::slashcommand_t& event) {

	event.reply("Pong!");
}

/**
 * @author DargoDargonyx
 * @date 08/31/25
 *
 * @brief Helper method for the level command.
 *
 * This method/command will construct an embed to send to the
 * user who activates the command, detailing their level data
 * and sending a level bar visualization png with the help
 * of the "ImageCreator" module.
 *
 * @param event The slashcommand event.
 */
void ManagerBot::levelCommand(const dpp::slashcommand_t& event) {

	dpp::snowflake userId;
	std::string barColor; // a color value stored as a hex

	auto name = event.get_parameter("other_user");
	if (std::holds_alternative<dpp::snowflake>(name)) {
		userId = std::get<dpp::snowflake>(name);
		barColor = "#00f7d6"; // teal in hex
	} else {
		userId = event.command.get_issuing_user().id;
		barColor = "#0073ff"; // light blue in hex
	}

	std::string user = std::to_string(userId);
	std::string username = Leveler::getUserName(user);
	int xp = Leveler::getUserXP(user);
	int level = Leveler::getUserLevel(user);
	int threshold = Leveler::thresholdCalculator(user);

	//=== Generates a new image ===
	Magick::Blob levelBar = ImageCreator::constructLevelImg(
			xp, 
			threshold,
			barColor	
		);

	//=== Creates new embed ===
	dpp::embed levelEmbed = dpp::embed()
		.set_color(Tools::hexToIntColor(barColor)) // light blue in HEX converted to int
		.set_title(username)
		.add_field(
			"Level",
			std::to_string(level),
			true
		)
		.add_field(
			"XP",
			std::to_string(xp) + "/" 
				+ std::to_string(threshold),
			true
		)
		.set_image("attachment://levelbar.png");

	//=== Turns the embed into a message ===
	dpp::message msg(event.command.channel_id, levelEmbed);
	msg.add_file("levelbar.png", std::string(
				static_cast<const char*>(levelBar.data()), levelBar.length()));

	//=== Replys with the embed ===
	event.reply(msg);
}

/**
 * @author DargoDargonyx
 * @date 08/31/25
 *
 * @brief Helper method for the destroy command.
 *
 * This method/command is used to shutdown the
 * bot (TEMPORARY/ONLY BEING USED FOR TESTING).
 *
 * @param event The slashcommand event.
 */
void ManagerBot::destroyCommand(const dpp::slashcommand_t& event) {

	std::string deathMsg = this->managerBotName + " is now offline!";
	event.reply(deathMsg);
	std::this_thread::sleep_for(std::chrono::milliseconds(250));
	this->managerBot.shutdown();
}

/**
 * @author DargoDargonyx
 * @date 08/31/25
 *
 * @brief Helper method for the test command.
 *
 * This method/command is used to test new features
 * during development.
 *
 * @param event The slashcommand event.
 */
void ManagerBot::testCommand(const dpp::slashcommand_t& event) {

	event.reply("Nothing to test at the moment.");
}

void ManagerBot::testCommand(const dpp::slashcommand_t& event, std::string num) {

	event.reply("Test Num: " + num);
}

uint32_t getMemberDisplayColor(const dpp::guild_member& member, const dpp::guild& guild) {

	uint32_t memberColor = 0;
	int highestPosition = -1;

	for (const dpp::snowflake& roleId : member.get_roles()) {

		const dpp::role* rolePtr = dpp::find_role(roleId);

		if (rolePtr && rolePtr->colour > 0 && rolePtr->position > highestPosition) {

			highestPosition = rolePtr->position;
			memberColor = rolePtr->colour;
		}
	}

	return memberColor;
}

/**
 * @author DargoDargonyx
 * @date 08/31/25
 *
 * @brief Starts the instance of the bot.
 */
void ManagerBot::startInstance() {
	this->managerBot.start(dpp::st_wait);
}
