/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xroca-pe <xroca-pe@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:10:40 by xroca-pe          #+#    #+#             */
/*   Updated: 2024/10/22 20:20:38 by xroca-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"


void close_window(void* param)
{
    (void)param;
    printf("Window closed\n");
    exit(0);
}

void key_hook(struct mlx_key_data keydata, void *param)
{
    t_game *game = (t_game *)param;
    
    if (keydata.key == MLX_KEY_ESCAPE)
        close_window(param);
    
    if (keydata.key == MLX_KEY_W)  // Mover hacia adelante
    {
        game->player.posX += game->player.dirX * 0.1;
        game->player.posY += game->player.dirY * 0.1;
    }
    if (keydata.key == MLX_KEY_S)  // Mover hacia atrás
    {
        game->player.posX -= game->player.dirX * 0.1;
        game->player.posY -= game->player.dirY * 0.1;
    }
    if (keydata.key == MLX_KEY_A)  // Girar a la izquierda
    {
        double oldDirX = game->player.dirX;
        game->player.dirX = game->player.dirX * cos(0.1) - game->player.dirY * sin(0.1);
        game->player.dirY = oldDirX * sin(0.1) + game->player.dirY * cos(0.1);
    }
    if (keydata.key == MLX_KEY_D)  // Girar a la derecha
    {
        double oldDirX = game->player.dirX;
        game->player.dirX = game->player.dirX * cos(-0.1) - game->player.dirY * sin(-0.1);
        game->player.dirY = oldDirX * sin(-0.1) + game->player.dirY * cos(-0.1);
    }

    perform_raycasting(game->image, &game->player, game->worldMap);
    mlx_image_to_window(game->mlx, game->image, 0, 0);
}



int main(void)
{
     const int worldMap[MAP_WIDTH][MAP_HEIGHT] = {
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 1, 0, 0, 1},
        {1, 0, 1, 0, 'N', 0, 0, 1},  // Jugador en 'N'
        {1, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 0, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1}
    };

    mlx_t* mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
    if (!mlx)
        return (1);
    
    mlx_image_t* image = mlx_new_image(mlx, WIDTH, HEIGHT);
    if (!image)
        return (1);

    t_player player = {3.5, 3.5, -1, 0, 0, 0.66};  // Jugador en la posición inicial

    t_game game;
    game.mlx = mlx;
    game.image = image;
    game.player = player;
    game.worldMap = worldMap;

    perform_raycasting(image, &game.player, game.worldMap);

    mlx_image_to_window(mlx, image, 0, 0);
    mlx_key_hook(mlx, key_hook, &game);  // Pasa el puntero de 'game'
    mlx_loop(mlx);
    mlx_terminate(mlx);
    return (0);
}

