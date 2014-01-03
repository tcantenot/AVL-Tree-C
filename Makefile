EXE=avl

all:
	gcc -o $(EXE) main.c avl_tree.c

run:
	@./$(EXE)
