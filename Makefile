NAME = libprintf.a

CFLAGS = -Wall -Werror -Wextra -g #-O2
INC = ft_printf.h
SRC = ft_printf.c \
	buffer.c \
	get_flags.c \
	utils.c

OBJ = $(patsubst %.c,%.o,$(SRC))

all: $(NAME)
clean:
	rm -rf $(OBJ)
fclean: clean
	rm -rf $(NAME)
re: fclean all
$(NAME):
	gcc $(CFLAGS) -c $(SRC) $(INC)
	ar -rcs $(NAME) $(OBJ)
test: $(NAME)
	gcc $(NAME) main.c -o test