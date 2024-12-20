/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_controller.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xroca-pe <xroca-pe@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 09:45:09 by cgaratej          #+#    #+#             */
/*   Updated: 2024/12/20 17:38:47 by xroca-pe         ###   ########.fr       */
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

void	check_open_door(t_game *game)
{
	if (game->is_interacting)
		game->is_interacting = 0;
	else
		game->is_interacting = 1;
}
