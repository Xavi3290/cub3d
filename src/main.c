/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xroca-pe <xroca-pe@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:10:40 by xroca-pe          #+#    #+#             */
/*   Updated: 2024/10/29 20:24:56 by xroca-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// Verifica si una posición específica en el mapa contiene una pared
/*int is_wall(t_game *game, double x, double y)
{
    int mapX = (int)x;
    int mapY = (int)y;

    if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT)
        return 1;  // Trata posiciones fuera de límites como paredes

    return game->worldMap[mapY][mapX] == 1;
}*/

// Verifica si una posición específica en el mapa contiene una pared
int is_wall(t_game *game, double x, double y)
{
    int mapX = (int)x;
    int mapY = (int)y;

    // Verifica si está dentro de los límites del mapa
    if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT)
        return 1;  // Trata posiciones fuera de los límites como paredes

    // Solo considera como pared las celdas que contienen el valor 1
    if (game->worldMap[mapY][mapX] == 1)
        return 1;

    return 0; // Permite cualquier otra celda
}



// Función para liberar recursos del juego
void free_game_resources(t_game *game)
{
    if (game->worldMap)
        free(game->worldMap);
    if (game->image)
        mlx_delete_image(game->mlx, game->image);
    if (game->mlx)
        mlx_terminate(game->mlx);
}


// Función para cerrar la ventana correctamente
void close_window(void* param)
{
    (void)param;
    printf("Window closed\n");
    free_game_resources(param);
    exit(0);
}

// Configura la posición y dirección del jugador según el mapa
void set_player_position(t_game *game)
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            if (game->worldMap[y][x] == 'N' || game->worldMap[y][x] == 'S' ||
                game->worldMap[y][x] == 'E' || game->worldMap[y][x] == 'W')
            {
                game->player.posX = x + 0.5;
                game->player.posY = y + 0.5;

                if (game->worldMap[y][x] == 'N')
                    game->player.dirX = -1, game->player.dirY = 0;
                else if (game->worldMap[y][x] == 'S')
                    game->player.dirX = 1, game->player.dirY = 0;
                else if (game->worldMap[y][x] == 'E')
                    game->player.dirX = 0, game->player.dirY = 1;
                else if (game->worldMap[y][x] == 'W')
                    game->player.dirX = 0, game->player.dirY = -1;

                game->player.planeX = 0.66 * game->player.dirY;
                game->player.planeY = -0.66 * game->player.dirX;
                return;
            }
        }
    }
}

// Función para inicializar MLX
mlx_t *init_mlx()
{
    mlx_t *mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
    if (!mlx)
    {
        printf("Error: MLX could not initialize.\n");
        exit(1);
    }
    return mlx;
}

// Función para manejar el movimiento hacia adelante y hacia atrás
/*void handle_movement(t_game *game, int key)
{
    if (key == MLX_KEY_W)
    {
        game->player.posX += game->player.dirX * MOVE_SPEED;
        game->player.posY += game->player.dirY * MOVE_SPEED;
    }
    else if (key == MLX_KEY_S)
    {
        game->player.posX -= game->player.dirX * MOVE_SPEED;
        game->player.posY -= game->player.dirY * MOVE_SPEED;
    }
}*/

// Maneja el movimiento con verificación de colisiones
void handle_movement(t_game *game, int key)
{
    double nextX, nextY;

    if (key == MLX_KEY_W)
    {
        nextX = game->player.posX + game->player.dirX * MOVE_SPEED;
        nextY = game->player.posY + game->player.dirY * MOVE_SPEED;
    }
    else if (key == MLX_KEY_S)
    {
        nextX = game->player.posX - game->player.dirX * MOVE_SPEED;
        nextY = game->player.posY - game->player.dirY * MOVE_SPEED;
    }
    else
        return;

    // Verifica si la siguiente posición tiene una pared antes de mover
    if (!is_wall(game, nextX, game->player.posY))
        game->player.posX = nextX;
    if (!is_wall(game, game->player.posX, nextY))
        game->player.posY = nextY;
}

// Función para manejar la rotación de la vista (sin mover al jugador)
void handle_rotation_view(t_game *game, int key)
{
    double oldDirX = game->player.dirX;
    double oldPlaneX = game->player.planeX;
    double angle = 0.0;

    if (key == MLX_KEY_A)
        angle = ROTATE_VIEW_SPEED;
    else if (key == MLX_KEY_D)
        angle = -ROTATE_VIEW_SPEED;

    game->player.dirX = game->player.dirX * cos(angle) - game->player.dirY * sin(angle);
    game->player.dirY = oldDirX * sin(angle) + game->player.dirY * cos(angle);
    game->player.planeX = game->player.planeX * cos(angle) - game->player.planeY * sin(angle);
    game->player.planeY = oldPlaneX * sin(angle) + game->player.planeY * cos(angle);
}

// Función para manejar la rotación del jugador con las flechas
void handle_rotation_player(t_game *game, int key)
{
    double oldDirX = game->player.dirX;
    double oldPlaneX = game->player.planeX;
    double angle = 0.0;

    if (key == MLX_KEY_LEFT)
        angle = ROTATE_PLAYER_SPEED;
    else if (key == MLX_KEY_RIGHT)
        angle = -ROTATE_PLAYER_SPEED;

    game->player.dirX = game->player.dirX * cos(angle) - game->player.dirY * sin(angle);
    game->player.dirY = oldDirX * sin(angle) + game->player.dirY * cos(angle);
    game->player.planeX = game->player.planeX * cos(angle) - game->player.planeY * sin(angle);
    game->player.planeY = oldPlaneX * sin(angle) + game->player.planeY * cos(angle);
}

// Función de enganche para gestionar teclas
void key_hook(struct mlx_key_data keydata, void *param)
{
    t_game *game = (t_game *)param;

    if (keydata.key == MLX_KEY_ESCAPE)
        close_window(param);

    if (keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_S)
        handle_movement(game, keydata.key);
    else if (keydata.key == MLX_KEY_A || keydata.key == MLX_KEY_D)
        handle_rotation_view(game, keydata.key);
    else if (keydata.key == MLX_KEY_LEFT || keydata.key == MLX_KEY_RIGHT)
        handle_rotation_player(game, keydata.key);

    // Redibuja la escena después de la actualización de la posición o dirección
    perform_raycasting(game->image, &game->player, game->worldMap);
    mlx_image_to_window(game->mlx, game->image, 0, 0);
}

// Inicialización del juego y asignación de recursos
void init_game(t_game *game)
{
    const int worldMap[MAP_HEIGHT][MAP_WIDTH] = {
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 1, 0, 0, 1},
        {1, 0, 1, 0, 'N', 0, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 0, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1}
    };

    // Reserva memoria para worldMap
    game->worldMap = malloc(sizeof(int) * MAP_HEIGHT * MAP_WIDTH);
    if (!game->worldMap)
    {
        printf("Error: Could not allocate memory for worldMap.\n");
        exit(1);
    }

    // Copia los datos del mapa
    ft_memcpy(game->worldMap, worldMap, sizeof(worldMap));

    // Inicializa otros recursos del juego
    game->mlx = init_mlx();
    game->image = mlx_new_image(game->mlx, WIDTH, HEIGHT);
    if (!game->image)
    {
        mlx_terminate(game->mlx);
        printf("Error: Image could not be created.\n");
        exit(1);
    }

    set_player_position(game);
}


int main(void)
{
    t_game game;

    init_game(&game);

    perform_raycasting(game.image, &game.player, game.worldMap);
    mlx_image_to_window(game.mlx, game.image, 0, 0);
    mlx_key_hook(game.mlx, key_hook, &game);
    mlx_close_hook(game.mlx, close_window, &game);
    mlx_loop(game.mlx);

    return (0);
}
