/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xavi <xavi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:10:40 by xroca-pe          #+#    #+#             */
/*   Updated: 2024/10/28 19:33:18 by xavi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// Función para cerrar la ventana correctamente
void close_window(void* param)
{
    (void)param;
    printf("Window closed\n");
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

// Función de enganche para movimiento y rotación del jugador
void key_hook(struct mlx_key_data keydata, void *param)
{
    t_game *game = (t_game *)param;

    if (keydata.key == MLX_KEY_ESCAPE)
        close_window(param);

    if (keydata.key == MLX_KEY_W)
    {
        game->player.posX += game->player.dirX * 0.1;
        game->player.posY += game->player.dirY * 0.1;
    }
    else if (keydata.key == MLX_KEY_S)
    {
        game->player.posX -= game->player.dirX * 0.1;
        game->player.posY -= game->player.dirY * 0.1;
    }
    else if (keydata.key == MLX_KEY_A)
    {
        double oldDirX = game->player.dirX;
        game->player.dirX = game->player.dirX * cos(0.1) - game->player.dirY * sin(0.1);
        game->player.dirY = oldDirX * sin(0.1) + game->player.dirY * cos(0.1);
    }
    else if (keydata.key == MLX_KEY_D)
    {
        double oldDirX = game->player.dirX;
        game->player.dirX = game->player.dirX * cos(-0.1) - game->player.dirY * sin(-0.1);
        game->player.dirY = oldDirX * sin(-0.1) + game->player.dirY * cos(-0.1);
    }

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
    mlx_loop(game.mlx);
    mlx_terminate(game.mlx);

    return (0);
}
