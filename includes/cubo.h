#ifndef GAME_H
#define GAME_H

#include "../mlx/mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

# define BUFFER_SIZE 10
# define WIDTH 1280
# define HEIGHT 720
# define BLOCK 64
# define DEBUG 1

# define W 119
# define A 97
# define S 115
# define D 100
# define LEFT 65361
# define RIGHT 65363

# define PI 3.14159265359

typedef struct s_player
{
    float x;
    float y;
    float angle;

    bool key_up;
    bool key_down;
    bool key_left;
    bool key_right;

    bool left_rotate;
    bool right_rotate;
}   t_player;

typedef struct s_game
{
    void *mlx;
    void *win;
    void *img;

    char *data;
    int bpp;
    int size_line;
    int endian;
    t_player player;

    int rows;        
    int cols;       
    int player_x; 
    int player_y;    
    char player_dir;
    int x;
    int y;
    char **map;
} t_game;

void init_player(t_player *player);
int key_release(int keycode, t_player *player);
int key_press(int keycode, t_player *player);
void move_player(t_player *player);

//get next line
char	*get_next_line(int fd);

int		line_updater(char **line, char buffer[]);
int		ft_strlenm(char const *s1);
char	*ft_strjoinm(char const *s1, char const *s2);
void	buffer_clear(char *buffer);
char	*buffer_total_clear(int fd, char **buffer, char *line);

//parsing map 

bool parse_map(t_game *map);
#endif