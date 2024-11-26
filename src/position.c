/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   position.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:16:43 by xavi              #+#    #+#             */
/*   Updated: 2024/11/26 13:02:25 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// Establece la dirección y posición del jugador en el juego
void set_player_position_and_direction(t_game *game, int x, int y)
{
    game->player.posX = x + 0.5;
    game->player.posY = y + 0.5;

    if (game->mapinfo.map[y][x] == 'N')
    {
        game->player.dirX = 0;
        game->player.dirY = -1;
    }
    else if (game->mapinfo.map[y][x] == 'S')
    {
        game->player.dirX = 0;
        game->player.dirY = 1;
    }
    else if (game->mapinfo.map[y][x] == 'E')
    {
        game->player.dirX = 1;
        game->player.dirY = 0;
    }
    else if (game->mapinfo.map[y][x] == 'W')
    {
        game->player.dirX = -1;
        game->player.dirY = 0;
    }
}

void set_player_position(t_game *game)
{
    int	y;
	int	x;
	char **map;

	y = 0;
	map = game->mapinfo.map;
    while (map[y])
    {
        x = 0;
        while (map[y][x])
        {
            if (map[y][x] == 'N' || map[y][x]  == 'S' || map[y][x]  == 'E' \
				|| map[y][x]  == 'W')
            {
                set_player_position_and_direction(game, x, y);
                game->player.planeX = 0.66 * game->player.dirY;
                game->player.planeY = -0.66 * game->player.dirX;
                return ;
            }
            x++;
        }
        y++;
    }
}