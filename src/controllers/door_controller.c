/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_controller.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 09:45:09 by cgaratej          #+#    #+#             */
/*   Updated: 2024/12/20 16:43:51 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	toggle_door_state(t_game *game, int x, int y)
{
	if (game->mapinfo.map[y][x] == 'D' && game->is_interacting)
		game->mapinfo.map[y][x] = 'O';
	else if (game->mapinfo.map[y][x] == 'O' && !game->is_interacting)
		game->mapinfo.map[y][x] = 'D';
}

static void	check_position(t_game *game, int x, int y)
{
	if (x >= 0 && x < game->mapinfo.width && y >= 0 && y < game->mapinfo.height)
	{
		toggle_door_state(game, x, y);
	}
}

void	check_door_interaction(t_game *game)
{
	int	player_x;
	int	player_y;
	int	y;
	int	x;

	player_x = (int)game->player.pos_x;
	player_y = (int)game->player.pos_y;
	y = player_y - 2;
	while (y <= player_y + 2)
	{
		x = player_x - 2;
		while (x <= player_x + 2)
		{
			check_position(game, x, y);
			x++;
		}
		y++;
	}
}
