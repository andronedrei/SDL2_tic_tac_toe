SOURCES = main.cpp utils.cpp game_interface.cpp game_logic.cpp
OUTPUT = tic_tac_toe

CXX = g++
CXXFLAGS = -I src/include
LDFLAGS = -L src/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image 

$(OUTPUT): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(OUTPUT) $(SOURCES) $(LDFLAGS)

clean:
	rm $(OUTPUT)
