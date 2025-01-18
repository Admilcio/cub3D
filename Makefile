NAME = game

CC = cc -g

SRC = main.c src/player.c
OBJ = $(SRC:.c=.o)

LFLAGS = -L./mlx -lmlx -lXext -lX11 -lm -lz
CFLAGS = -Wall -Wextra -Werror -Imlx/l

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LFLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
