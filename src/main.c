#include "../includes/cubo.h"

// our own put_pixel

void put_pixel(int x, int y, int color, t_game *game)
{
    if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
        return;

    int index = y * game->size_line + x * (game->bpp / 8);

    int blue = color % 256;         // Canal azul
    int green = (color / 256) % 256; // Canal verde
    int red = (color / 65536) % 256; // Canal vermelho

    game->data[index] = blue;     // Armazena o azul
    game->data[index + 1] = green; // Armazena o verde
    game->data[index + 2] = red;   // Armazena o vermelho
}

// our own clear_image
void clear_image(t_game *game)
{
    for(int y = 0; y < HEIGHT; y++)
        for(int x = 0; x < WIDTH; x++)
            put_pixel(x, y, 0, game);
}

// utils functions
void draw_square(int x, int y, int size, int color, t_game *game)
{
    for(int i = 0; i < size; i++)
        put_pixel(x + i, y, color, game); // top line
    for(int i = 0; i < size; i++)
        put_pixel(x, y + i, color, game); // left line
    for(int i = 0; i < size; i++)
        put_pixel(x + size, y + i, color, game); // right line
    for(int i = 0; i < size; i++)
        put_pixel(x + i, y + size, color, game); // bottom line
}

void draw_map(t_game *game)
{
    char **map = game->map;
    int color = 0x0000FF;
    for(int y = 0; map[y]; y++)
        for(int x = 0; map[y][x]; x++)
            if(map[y][x] == '1')
                draw_square(x * BLOCK, y * BLOCK, BLOCK, color, game);
}

// distance calculation functions
float distance(float x, float y){
    return sqrt(x * x + y * y);
}

float fixed_dist(float x1, float y1, float x2, float y2, t_game *game)
{
    float delta_x = x2 - x1;
    float delta_y = y2 - y1;
    float angle = atan2(delta_y, delta_x) - game->player.angle;
    float fix_dist = distance(delta_x, delta_y) * cos(angle);
    return fix_dist;
}

// touch function 
bool touch_wall(float px, float py, t_game *game)
{
    int x = px / BLOCK;
    int y = py / BLOCK;

    // Verifique se os índices estão fora dos limites do mapa
    if (x < 0 || y < 0 || y >= game->rows || x >= game->cols) {
        return false; // Fora do mapa, consideramos como não parede
    }

    return game->map[y][x] == '1';
}

t_game *read_map_from_file(t_game *game , const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    game->map = NULL;
    game->rows = 0;
    game->cols = 0;

    char *line;
    while ((line = get_next_line(fd)) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
        }

        if ((int)len > game->cols) {
            game->cols = (int)len;
        }

        char **temp = realloc(game->map, (game->rows + 1) * sizeof(char *));
        if (!temp) {
            perror("Erro de alocação de memória");
            free(line);
            for (int i = 0; i < game->rows; i++) {
                free(game->map[i]);
            }
            free(game->map);
            free(game);
            close(fd);
            return NULL;
        }

        game->map = temp;
        game->map[game->rows++] = line;
    }

    close(fd);

    if (game->rows == 0) {
        printf("Erro: Arquivo vazio ou leitura falhou.\n");
        free(game);
        return NULL;
    }

    // Adicionar um ponteiro NULL ao final do array (opcional)
    char **final_map = realloc(game->map, (game->rows + 1) * sizeof(char *));
    if (!final_map) {
        perror("Erro de alocação ao finalizar o array de mapa");
        for (int i = 0; i < game->rows; i++) {
            free(game->map[i]);
        }
        free(game->map);
        free(game);
        return NULL;
    }

    game->map = final_map;
    game->map[game->rows] = NULL; // Termina o array com NULL
    return game;
}
int	check_extension(const char *filename)
{
	return (strstr(filename, ".cub") != NULL);
}
void get_map(t_game *game, char **av)
{
    if(!check_extension(av[1]))
    {
        printf("Wrong filename : example.cub\n");
        exit(1);
    }
    t_game *loaded_game = read_map_from_file(game, av[1]);
    if (loaded_game)
        *game = *loaded_game; // Copia os dados para o objeto original
}

void init_game(t_game *game, char **av)
{
    get_map(game, av);
    
    init_player(&game->player, game->map);
    if(parse_map(game) == false)
        printf("xablau map errado"), exit(0);//retorna funcao de erro 
    game->mlx = mlx_init();
    game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "Game");
    game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
    game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line, &game->endian);
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
   
}

// raycasting functions
void draw_line(t_player *player, t_game *game, float start_x, int i)
{
    float cos_angle = cos(start_x);
    float sin_angle = sin(start_x);
    float ray_x = player->x;
    float ray_y = player->y;

    while(!touch_wall(ray_x, ray_y, game))
    {
        if(DEBUG)
            put_pixel(ray_x, ray_y, 0xFF00FF, game);
        ray_x += cos_angle;
        ray_y += sin_angle;
    }
    if(!DEBUG)
    {
        float dist = fixed_dist(player->x, player->y, ray_x, ray_y, game);
        float height = (BLOCK / dist) * (WIDTH / 2);
        int start_y = (HEIGHT - height) / 2;
        int end = start_y + height;
        while(start_y < end)
        {
            put_pixel(i, start_y, 255, game);
            start_y++;
        }
    }
}

int draw_loop(t_game *game)
{
    t_player *player = &game->player;
    move_player(player, game);
    clear_image(game);
    if(DEBUG)
    {
        draw_square(player->x, player->y, 10, 0x00FF00, game);
        draw_map(game);
    }
    float fraction = PI / 3 / WIDTH;
    float start_x = player->angle - PI / 6;
    int i = 0;
    while(i < WIDTH)
    {
        draw_line(player, game, start_x, i);
        start_x += fraction;
        i++;
    }
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
    return 0;
}

int main(int ac, char **av)
{
    if (ac != 2) {
        printf("Uso: ./programa <mapa>\n");
        return 1;
    }

    t_game game;
    memset(&game, 0, sizeof(t_game)); // Inicializa com zeros

    // Inicialização
    init_game(&game, av);

    if (!game.mlx) {
        printf("Erro: mlx_init retornou NULL\n");
        return 1;
    }

    if (!game.map) {
        printf("Erro: Mapa não foi carregado corretamente\n");
        return 1;
    }

    // Hooks
    mlx_hook(game.win, 2, 1L << 0, key_press, &game.player);
    mlx_hook(game.win, 3, 1L << 1, key_release, &game.player);
    mlx_loop_hook(game.mlx, draw_loop, &game);
    printf("xablau123\n");
    // Loop principal
    mlx_loop(game.mlx);
    printf("xablau1234\n");

    return 0;
}
