<!--
@author DargoDargonyx 
@date 08/26/25
-->

# **<ins>Linux Club Server Bot</ins>**

This project was designed for the purpose of managing a discord server created for the Linux club at Appalachian State University.

<br>

## <ins>Contributors</ins>

- **DargoDargon** | Programmer & Vice President of the Linux Club 2025
- **Ekryito** | Artist

<br>

## <ins>Announcer</ins>

(Include description here)

- **Features**

    - [ ] Basic announcemnet templates
    - [ ] Advanced announcement templates

<br>

## <ins>Filter</ins>

(Include description here)

- **Features**

    - [ ] Basic slur filtering
    - [ ] Advanced slur filtering
    - [ ] Infraction memory system
    - [ ] Punishment system
    - [ ] Spam filtering

<br>

## <ins>Leveler</ins>

The "Leveler" module is used to manage the leveling feature often seen in many discord servers. When users interact in the server the bot is managing some "XP" is attributed to them, this data is stored locally in a JSON file named "levelData.json".

<br>

  **Importing the Class**

  ```cpp
  #include "Modules/Leveler.h"
  ```

  **Loading a JSON file**

  ```cpp
    // Loads a JSON file from the given file address to use in the class methods
    std::string fileDescriptor = "exampleFile.json";
    Leveler::loadFile(fileDescriptor);
  ```
  **Manipulating Data**

  ```cpp
    std::string name = "exampleName";
  
    // Returns the level of the user when given a name
    int level = Leveler::getUserLevel(name);
    std::cout << std::to_string(name); // Prints the level to the terminal

    // Returns the "XP" of the user when given a name
    int xp = Leveler::getUserXP(name);
    std::cout << std::to_string(name); // Prints the XP to the terminal

    // Adds XP to a user when given a name
    int additionalXP = 10;
    Leveler::addXP(name, additionalXP);
  ```

  **Saving Changes to the JSON File**

  ```cpp
    // Saves a JSON file using the given file address
    std::string fileDescriptor = "exampleFile.json";
    Leveler::saveFile(fileDescriptor);
  ```

 **Features TODO**

  - [X] Basic level tracking
  - [X] Basic command capabilities
  - [ ] Advanced level tracking
  - [x] Advanced command capabilities
  - [ ] Spam detection
  - [X] Private messaging on level-up
  - [X] Increased graphical display

<br>

## <ins>RoleManager</ins>

(Include description here)

- **Features**

    - [ ] Basic role assignment management
    - [ ] Advanced role assignment management
