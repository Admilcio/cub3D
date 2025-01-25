#include "../includes/cubo.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void fill_map_lines_with_spaces(char **map, int rows, int max_cols) {
    int line_length;
    for (int i = 0; i < rows; i++) {
            line_length = strlen(map[i]);
        if (line_length < max_cols) {
            map[i] = realloc(map[i], max_cols + 1);
            if (!map[i]) {
                perror("Erro ao realocar linha do mapa");
                exit(EXIT_FAILURE);
            }
            memset(map[i] + line_length, ' ', max_cols - line_length);
            map[i][max_cols] = '\0';
        }
    }
}

bool flood_fill(char **map, int rows, int cols, int x, int y, bool **visited) {
    if (x < 0 || y < 0 || x >= rows || y >= cols || visited[x][y]) {
        return false; 
    }

    if (map[x][y] == '1') {
        return false;
    }

    if (map[x][y] == '0' || map[x][y] == 'N' || map[x][y] == 'S' || map[x][y] == 'E' || map[x][y] == 'W') {
        visited[x][y] = true;
    } else {
        return false;  
    }

    bool escape = flood_fill(map, rows, cols, x + 1, y, visited) ||
                  flood_fill(map, rows, cols, x - 1, y, visited) ||
                  flood_fill(map, rows, cols, x, y + 1, visited) ||
                  flood_fill(map, rows, cols, x, y - 1, visited);

    return escape;
}




bool check_borders(t_game *game) {
    for (int i = 0; (size_t)i < strlen(game->map[0]); i++) {
        
            if (game->map[0][i] != '1'){
                if(game->map[0][i] != ' ')
                {
                    printf("entrou666\n");
                    return false;
                }
            }
    }

    for (int i = 0; (size_t)i < strlen(game->map[game->rows - 1]); i++) {
        if (game->map[game->rows - 1][i] != '1') 
        {
            if(game->map[game->rows - 1][i] != ' ')
            {
                    printf("entrou666 1\n");
                    return false;
            }
        }
    }

    for (int i = 1; i < game->rows - 1; i++) {
        if (game->map[i][0] != '1') {
            {
                    printf("entrou666\n");
                    return false;
            }
        }

        int line_length = strlen(game->map[i]);
        if (game->map[i][line_length - 1] != '1') 
        {
            if (game->map[i][line_length - 1] != ' ')
            {
                    printf("entrou666\n");
                    return false;
            }
        }
    }
    printf("xibliu\n");
    return true;
}


bool parse_map(t_game *game) {
    int player_count = 0;
    int i = 0;
    while (game->map[i])
    {
        printf("%s\n", game->map[i]);
        i++;
    }
    fill_map_lines_with_spaces(game->map, game->rows, game->cols);

    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            char c = game->map[i][j];
            if (c == 'N' || c == 'S' || c == 'E' || c == 'W') {
                game->player_x = i;
                game->player_y = j;
                game->player_dir = c;
                player_count++;
            } else if (c != '0' && c != '1' && c != ' ') {
                printf("Mapa contém um caractere inválido: '%c' na posição (%d, %d)\n", c, i, j);
                return false;
            }
        }
    }

    if (player_count != 1) {
        printf("O mapa deve conter exatamente 1 posição inicial do jogador (encontrado: %d).\n", player_count);
        return false;
    }

    bool **visited = malloc(game->rows * sizeof(bool *));
    for (int i = 0; i < game->rows; i++) {
        visited[i] = calloc(game->cols, sizeof(bool));
    }

    if (!check_borders(game)) {
        printf("As bordas do mapa não são paredes ('1').\n");
        return false;
    }

    if (flood_fill(game->map, game->rows, game->cols, game->player_x, game->player_y, visited)) {
        printf("O mapa possui espaços abertos acessíveis a partir do jogador.\n");
        for (int i = 0; i < game->rows; i++) free(visited[i]);
        free(visited);
        return false;
    }

    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            if ((game->map[i][j] == '0' || game->map[i][j] == 'N' || game->map[i][j] == 'S' || game->map[i][j] == 'E' || game->map[i][j] == 'W') && !visited[i][j]) {
                printf("O mapa está aberto em uma área não cercada por paredes. Erro na posição (%d, %d).\n", i, j);
                for (int i = 0; i < game->rows; i++) free(visited[i]);
                free(visited);
                return false;
            }
        }
    }

    for (int i = 0; i < game->rows; i++) free(visited[i]);
    free(visited);

    return true;
}

