/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   position_controller.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:16:43 by xavi              #+#    #+#             */
/*   Updated: 2024/12/20 17:17:59 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

// Establece la dirección y posición del jugador en el juego
void	set_player_position_and_direction(t_game *game, int x, int y)
{
	game->player.pos_x = x + 0.5;
	game->player.pos_y = y + 0.5;
	if (game->mapinfo.map[y][x] == 'N')
	{
		game->player.dir_x = 0;
		game->player.dir_y = -1;
	}
	else if (game->mapinfo.map[y][x] == 'S')
	{
		game->player.dir_x = 0;
		game->player.dir_y = 1;
	}
	else if (game->mapinfo.map[y][x] == 'E')
	{
		game->player.dir_x = 1;
		game->player.dir_y = 0;
	}
	else if (game->mapinfo.map[y][x] == 'W')
	{
		game->player.dir_x = -1;
		game->player.dir_y = 0;
	}
}

int	is_player(t_game *game, int x, int y)
{
	return (game->mapinfo.map[y][x] == 'N' || game->mapinfo.map[y][x] == 'S' \
		|| game->mapinfo.map[y][x] == 'E' || game->mapinfo.map[y][x] == 'W');
}

void	set_player_position(t_game *game)
{
	int		y;
	int		x;
	char	**map;

	y = 0;
	map = game->mapinfo.map;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (is_player(game, x, y))
			{
				set_player_position_and_direction(game, x, y);
				game->player.plane_x = 0.66 * game->player.dir_y;
				game->player.plane_y = -0.66 * game->player.dir_x;
				return ;
			}
			x++;
		}
		y++;
	}
}
