SOURCES = main.cpp utils.cpp game_interface.cpp
OUTPUT = tic_tac_toe

CXX = g++
CXXFLAGS = -I src/include
LDFLAGS = -L src/lib -lmingw32 -lSDL2main -lSDL2 

$(OUTPUT): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(OUTPUT) $(SOURCES) $(LDFLAGS)

clean:
	rm $(OUTPUT)
