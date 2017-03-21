CPP_FILES = $(wildcard *.cpp)
OBJ_FILES = $(notdir $(CPP_FILES:.cpp=.o))

all: $(OBJ_FILES) subsystem

%.o: %.cpp %.hpp
	@printf "Compiling $*.cpp...\n"
	@$(COMPILER) $(FLAGS) -o $(notdir $*).o $*.cpp

.PHONY : subsystem
subsystem:
	@printf "\n=====>Four In A Row<=====\n\n"
	@cd four_in_a_row && $(MAKE)
	@printf "\n=====>Maze<=====\n\n"
	@cd maze && $(MAKE)
	@printf "\n=====>Mine Sweeper<=====\n\n"
	@cd mine_sweeper && $(MAKE)
	@printf "\n=====>Pacman<=====\n\n"
	@cd pacman && $(MAKE)
	@printf "\n=====>Snake<=====\n\n"
	@cd snake && $(MAKE)
	@printf "\n=====>Tetris<=====\n\n"
	@cd tetris && $(MAKE)
	@printf "\n=====>Tic Tac Toe<=====\n\n"
	@cd tic_tac_toe && $(MAKE)
