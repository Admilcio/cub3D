# include "../includes/cubo.h"

void init_player(t_player *player, char **map)
{
    for (int y = 0; map[y]; y++)
    {
        for (int x = 0; map[y][x]; x++)
        {
            if (map[y][x] == 'N')
            {
                player->x = x * BLOCK + BLOCK / 2;
                player->y = y * BLOCK + BLOCK / 2;
                player->angle = PI / 2;
                return;
            }
        }
    }

    // Caso o 'N' não seja encontrado, define uma posição padrão
    player->x = WIDTH / 2;
    player->y = HEIGHT / 2;
    player->angle = PI / 2;

    printf("Aviso: 'N' não encontrado no mapa. Posição inicial padrão aplicada.\n");
}

int key_press(int keycode, t_player *player)
{
    if(keycode == W)
        player->key_up = true;
    if(keycode == S)
        player->key_down = true;
    if(keycode == A)
        player->key_left = true;
    if(keycode == D)
        player->key_right = true;
    if(keycode == LEFT)
        player->left_rotate = true;
    if(keycode == RIGHT)
        player->right_rotate = true;
    return 0;
}

int key_release(int keycode, t_player *player)
{
    if(keycode == W)
        player->key_up = false;
    if(keycode == S)
        player->key_down = false;
    if(keycode == A)
        player->key_left = false;
    if(keycode == D)
        player->key_right = false;
    if(keycode == LEFT)
        player->left_rotate = false;
    if(keycode == RIGHT)
        player->right_rotate = false;
    return 0;
}

void move_player(t_player *player, t_game *game)
{
    int speed = 3;
    float angle_speed = 0.03;
    float cos_angle = cos(player->angle);
    float sin_angle = sin(player->angle);

    // Rotação do jogador
    if (player->left_rotate)
        player->angle -= angle_speed;
    if (player->right_rotate)
        player->angle += angle_speed;
    if (player->angle > 2 * PI)
        player->angle = 0;
    if (player->angle < 0)
        player->angle = 2 * PI;

    // Cálculo da próxima posição antes de mover
    float next_x = player->x;
    float next_y = player->y;

    if (player->key_up)
    {
        next_x += cos_angle * speed;
        next_y += sin_angle * speed;
    }
    if (player->key_down)
    {
        next_x -= cos_angle * speed;
        next_y -= sin_angle * speed;
    }
    if (player->key_left)
    {
        next_x += sin_angle * speed;
        next_y -= cos_angle * speed;
    }
    if (player->key_right)
    {
        next_x -= sin_angle * speed;
        next_y += cos_angle * speed;
    }

    // Verifica colisão antes de atualizar a posição do jogador
    if (!touch_wall(next_x, player->y, game)) 
        player->x = next_x; // Move apenas se não colidir no eixo X
    if (!touch_wall(player->x, next_y, game)) 
        player->y = next_y; // Move apenas se não colidir no eixo Y
}
