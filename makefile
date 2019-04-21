OBJ=main.o status.o station.o vacuum.o collision.o helpers.o

# Fix the directories to match your file organisation.
CC_FLAGS=-std=gnu99 -I../ZDK -Wall -Werror -g
LD_FLAGS=-L../ZDK -lzdk -lncurses -lm

vac_sim_n10227555: $(OBJ)
	gcc $(OBJ) $(LD_FLAGS) -o $@

%.o : %.c
	gcc -c $< $(CC_FLAGS)
