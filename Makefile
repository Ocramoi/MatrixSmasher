OUT_F=out
EXEC=matrix
CXX=clang++
CXXFLAGS=-lraylib -ldl -lrt -lX11 -lGL -lpthread -std=gnu++17
RELEASE_FLAGS=-Wall -Wextra -Ofast -std=gnu++17 -march=native
DEBUG_FLAGS=-Wall -Wextra -Wpedantic -Og -g3 -DDEBUG -std=gnu++17 -Weffc++ -march=native
VALGRIND_FLAGS=-s --leak-check=full --show-leak-kinds=all --track-origins=yes
COMPONENTS=$(wildcard components/*.cpp) $(wildcard components/*/*.cpp)
SCENES=$(wildcard scenes/*.cpp)
ARQS=*.o

all:
	$(CXX) -o $(OUT_F)/$(EXEC) *.cpp $(ARQS) $(CXXFLAGS) $(RELEASE_FLAGS)

debug:
	$(CXX) -o $(OUT_F)/$(EXEC) *.cpp $(ARQS) $(CXXFLAGS) $(DEBUG_FLAGS)

components_o:
	$(CXX) -c $(COMPONENTS) $(CXXFLAGS) $(RELEASE_FLAGS)

scenes_o:
	$(CXX) -c $(SCENES) $(CXXFLAGS) $(RELEASE_FLAGS)

run:
	./$(OUT_F)/$(EXEC)

clean:
	rm *.o