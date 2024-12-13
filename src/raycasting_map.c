/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_map.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:43:34 by cgaratej          #+#    #+#             */
/*   Updated: 2024/12/13 11:10:20 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static void	calculate_wall_limits(t_line_params *line, int vertical_shift)
{
	line->draw_start = -line->line_height / 2 + HEIGHT / 2 + vertical_shift;
	if (line->draw_start < 0)
		line->draw_start = 0;
	line->draw_end = line->line_height / 2 + HEIGHT / 2 + vertical_shift;
	if (line->draw_end >= HEIGHT)
		line->draw_end = HEIGHT - 1;
}

// Función para seleccionar la textura correcta basada en el lado de la pared
t_texture	*select_texture(t_ray *ray, t_game *game)
{
	if (ray->special)
		return (&game->door_texture);
	if (ray->side == 0 && ray->step_x < 0)
		return (&game->wall_textures[0]); // Norte
	else if (ray->side == 0 && ray->step_x > 0)
		return (&game->wall_textures[1]); // Sur
	else if (ray->side == 1 && ray->step_y < 0)
		return (&game->wall_textures[2]); // Oeste
	return (&game->wall_textures[3]); // Este
}

static void	draw_ceiling(t_game *game, t_line_params *line)
{
	int	y;

	if (line->draw_start > 0)
	{
		y = -1;
		while (++y < line->draw_start)
		{
			mlx_put_pixel(game->image, line->x, y, \
							rgb_to_int(game->sky_color));
		}
	}
}

void check_door_interaction(t_game *game)
{
	int player_x = (int)game->player.pos_x;
	int player_y = (int)game->player.pos_y;

	for (int y = player_y - 1; y <= player_y + 1; y++)
	{
		for (int x = player_x - 1; x <= player_x + 1; x++)
		{
			if (x >= 0 && x < game->mapinfo.width && y >= 0 && y < game->mapinfo.height)
			{
				if (game->mapinfo.map[y][x] == 'D' && game->is_interacting)
				{
					game->mapinfo.map[y][x] = 'O';
				}
				else if (game->mapinfo.map[y][x] == 'O' && game->is_interacting)
				{
					game->mapinfo.map[y][x] = 'D';
				}
			}
		}
	}
}

void	perform_raycasting(t_game *game)
{
	int				x;
	int				vertical_shift;
	t_ray			ray;
	t_line_params	line;

	x = 0;
	draw_sky_and_floor(game); // Dibuja el cielo y el suelo
	vertical_shift = (int)(game->player.view_offset * HEIGHT);
	check_door_interaction(game);
	while (x < WIDTH)
	{
		line.x = x;
		process_ray(&ray, game, &line);
		calculate_wall_limits(&line, vertical_shift);
		draw_ceiling(game, &line);
		draw_floor(game, &line);
		draw_textured_line(game, &ray, &line, select_texture(&ray, game));
		x++;
	}
}
