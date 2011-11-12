include common.mk

SRC = cterm.c

obj/cterm: $(addprefix obj/,$(SRC:.c=.o)) obj/libterm.so
	gcc $(CFLAGS) -o $@ $< -Lobj -lterm -lcurses