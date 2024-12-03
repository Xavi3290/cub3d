/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_minimap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:06:40 by xavi              #+#    #+#             */
/*   Updated: 2024/12/03 12:39:14 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// Dibuja una celda en el minimapa con el color especificado
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

// Dibuja el minimapa en la esquina superior izquierda de la pantalla
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
				game->color = game->minimap_wall_color; //Pared
			else
				game->color = game->minimap_floor_color; //Suelo
			draw_minimap_cell(game);
			x++;
		}
		y++;
	}
}

// Dibuja la posición del jugador en el minimapa
void	draw_player_on_minimap(t_game *game)
{
	int	player_tile_size;
	int	i;
	int	j;

	// Ajustar tamaño del jugador en el minimapa para evitar sobrepasar
	game->start_x = (int)(game->player.pos_x * game->tile_size);
	game->start_y = (int)(game->player.pos_y * game->tile_size);
	game->color = game->minimap_player_color;
	player_tile_size = game->tile_size / 2; // Ajuste para el tamaño del jugador
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
