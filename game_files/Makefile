CPP_FILES = $(wildcard *.cpp)
OBJ_FILES = $(notdir $(CPP_FILES:.cpp=.o))

all: $(OBJ_FILES) subsystem

%.o: %.cpp %.hpp
	@printf "Compiling $*.cpp...\n"
	@$(COMPILER) $(FLAGS) -o $(notdir $*).o $*.cpp

.PHONY : subsystem
subsystem:
	@cd four_in_a_row && $(MAKE)
	@cd maze && $(MAKE)
	@cd mine_sweeper && $(MAKE)
	@cd snake && $(MAKE)
	@cd tetris && $(MAKE)
	@cd tic_tac_toe && $(MAKE)
