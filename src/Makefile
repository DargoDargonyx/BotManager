# === Compiler Settings ===
CXX = g++
CXXFLAGS = -std=c++20 -Imodules `Magick++-config --cxxflags`
LDFLAGS = `Magick++-config --ldflags --libs` -ldpp

# === Files ===
MANAGER_SRC = $(wildcard manager/*.cpp) $(wildcard modules/*.cpp)
GAME_SRC = $(wildcard game/*.cpp) $(wildcard modules/*.cpp)

MANAGER_TARGET = managerBot
GAME_TARGET = gameBot

# === Rules ===
all:
	$(CXX) $(CXXFLAGS) -o $(GAME_TARGET) $(GAME_SRC) $(LDFLAGS)

game:

manager:
	$(CXX) $(CXXFLAGS) -o $(MANAGER_TARGET) $(MANAGER_SRC) $(LDFLAGS)

# === Clean Target ===
clean:
	rm -f $(MANAGER_TARGET)
	rm -f $(GAME_TARGET)
