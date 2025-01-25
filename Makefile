NAME = game

CC = cc -g

SRC = src/main.c src/player.c src/check_map.c src/get_next_line/get_next_line_utils.c src/get_next_line/get_next_line.c
OBJ = $(SRC:.c=.o)

LFLAGS = -L./mlx -lmlx -lXext -lX11 -lm
CFLAGS = -Wall -Wextra -Werror -Imlx/l

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LFLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
