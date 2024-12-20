/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_minimap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xroca-pe <xroca-pe@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:06:40 by xavi              #+#    #+#             */
/*   Updated: 2024/12/20 17:35:17 by xroca-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	draw_minimap_cell(t_game *game)
{
	int	i;
	int	j;
	int	color_value;

	i = 0;
	color_value = rgb_to_int(game->color);
	while (i < game->tile_size)
	{
		j = 0;
		while (j < game->tile_size)
		{
			mlx_put_pixel(game->image, game->start_x + i, \
				game->start_y + j, color_value);
			j++;
		}
		i++;
	}
}

void	draw_minimap(t_game *game)
{
	int	y;
	int	x;

	y = 0;
	while (y < game->mapinfo.height && game->mapinfo.map[y])
	{
		x = 0;
		while (x < game->mapinfo.width && game->mapinfo.map[y][x])
		{
			game->start_x = x * game->tile_size;
			game->start_y = y * game->tile_size;
			if (game->mapinfo.map[y][x] == '1')
				game->color = game->minimap_wall_color;
			else if (game->mapinfo.map[y][x] == 'D')
				game->color = game->door_color;
			else if (game->mapinfo.map[y][x] == '0' || is_player(game, x, y) \
				|| game->mapinfo.map[y][x] == 'O')
				game->color = game->minimap_floor_color;
			else
				game->color = game->none_color;
			draw_minimap_cell(game);
			x++;
		}
		y++;
	}
}

void	draw_player_on_minimap(t_game *game)
{
	int	player_tile_size;
	int	i;
	int	j;

	game->start_x = (int)(game->player.pos_x * game->tile_size);
	game->start_y = (int)(game->player.pos_y * game->tile_size);
	game->color = game->minimap_player_color;
	player_tile_size = game->tile_size;
	i = 0;
	while (i < player_tile_size)
	{
		j = 0;
		while (j < player_tile_size)
		{
			mlx_put_pixel(game->image, game->start_x + i, \
				game->start_y + j, rgb_to_int(game->color));
			j++;
		}
		i++;
	}
}
