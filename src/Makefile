OUT_F=../out
EXEC=matrix.out
CXX=clang++
CXXFLAGS=-lraylib -ldl -lrt -lX11 -lGL -pthread -std=gnu++17 #-lpthread 
RELEASE_FLAGS=-Wall -Wextra -Ofast -std=gnu++17 -march=native -DNDEBUG
DEBUG_FLAGS=-Wall -Wextra -Wpedantic -Og -g3 -DDEBUG -std=gnu++17 -Weffc++ -march=native
DEBUGGER=valgrind
DEBUGGER_FLAGS=-s --leak-check=full --show-leak-kinds=all --track-origins=yes
COMPONENTS=$(wildcard components/*.cpp) $(wildcard components/*/*.cpp)
SCENES=$(wildcard scenes/*.cpp)
ARQS=*.o
COMPILATION=DEBUG

ifneq ($(COMPILATION),DEBUG)
	RUNFLAGS:=$(RELEASE_FLAGS)
else
	RUNFLAGS:=$(DEBUG_FLAGS)
endif

all:
	$(CXX) -o $(OUT_F)/$(EXEC) *.cpp $(ARQS) $(CXXFLAGS) $(RUNFLAGS)

.PHONY: components
components:
	$(CXX) -c $(COMPONENTS) $(CXXFLAGS) $(RUNFLAGS)

.PHONY: scenes
scenes:
	$(CXX) -c $(SCENES) $(CXXFLAGS) $(RUNFLAGS)

.PHONY: full
full: components scenes all

run:
	@echo $(HM)
	./$(OUT_F)/$(EXEC)

debug:
	# $(DEBUGGER) $(DEBUGGER_FLAGS) ./$(OUT_F)/$(EXEC)
	$(DEBUGGER) ./$(OUT_F)/$(EXEC)

.PHONY: clean
clean:
	rm *.o $(OUT_F)/$(EXEC)

release:
	make -j components COMPILATION=RELEASE
	make -j scenes COMPILATION=RELEASE
	make -j COMPILATION=RELEASE
