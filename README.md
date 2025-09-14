<!--
@author DargoDargonyx 
@date 09/14/25
-->

# **<ins>Linux Club Server Bot</ins>**

This project was designed for the purpose of managing a discord server created for the Linux club at Appalachian State University.

<br>

## <ins>Contributors</ins>

- **DargoDargon** | Programmer & Vice President of the Linux Club 2025
- **Ekryito** | Artist

<br>

## <ins>Bots</ins>

#### <ins>**ManagerBot**</ins>

This bot is used to manage basic features of the given discord server.

- Features List
    - [x] Manages users levels
    - [x] Tells users their level on command
    - [x] Allows users to check another persons level
    - [x] Tells users when they level up
    - [ ] Makes a users level card match their main role color
    - [ ] Doesn't allow a user to spam in a chat in order to gain xp
    - [ ] Doesn't allow users to spam the channel chats
    - [ ] Give warnings to users who are violating rules
    - [ ] Doesn't allow slurs in the server channels
    - [ ] Allows admin to send out predetermined announcements

#### <ins>**GameBot**</ins>
    
This bot is used to manage game features of the given discord server.

- Features List
    - [ ] Manages card inventory system
    - [ ] Creates random card events
    - [ ] Shows users their cards on command


## <ins>Modules</ins>

#### <ins>**Announcer**</ins>

The Announcer module is used to make prefab announcements in the discord server with the use of the ManagerBot.

#### <ins>**Filter**</ins>

The Filter module is used to help filter unruly messages in the discord server with the use of the ManagerBot.

#### <ins>**ImageCreator**</ins>

The ImageCreator module is used by the ManagerBot to generate images on command.

#### <ins>**Leveler**</ins>

The Leveler module is used by the ManagerBot to manage the levels of users in the discord server.

<ins>Importing the Class</ins>

  ```cpp
  #include "Modules/Leveler.h"
  ```

<ins>Loading a JSON file</ins>

  ```cpp
    // Loads a JSON file from the given file address to use in the class methods
    std::string fileDescriptor = "exampleFile.json";
    Leveler::loadFile(fileDescriptor);
  ```
<ins>Manipulating Data</ins>

  ```cpp
    std::string userId = "exampleName";
  
    // Returns the level of the user when given a user id
    int level = Leveler::getUserLevel(userId);
    std::cout << std::to_string(level); // Prints the level to the terminal

    // Returns the "XP" of the user when given a user id
    int xp = Leveler::getUserXP(userId);
    std::cout << std::to_string(xp); // Prints the XP to the terminal

    // Adds XP to a user when given a user id
    int additionalXP = 10;
    Leveler::addXP(userId, additionalXP);
  ```

<ins>Saving Changes to the JSON File</ins>

  ```cpp
    // Saves a JSON file using the given file address
    std::string fileDescriptor = "exampleFile.json";
    Leveler::saveFile(fileDescriptor);
  ```

#### <ins>**RoleManager**</ins>

The RoleManager module is used to help manage low level roles in the discord server with the use of the ManagerBot.

#### <ins>**Tools**</ins>

The Tools module is used for general purpose helpful methods that can be used by any of the classes in the project.
