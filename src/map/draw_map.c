/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:45:47 by cgaratej          #+#    #+#             */
/*   Updated: 2024/12/17 10:04:25 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int32_t	mlx_get_pixel(t_texture *texture, uint32_t x, \
								uint32_t y, t_game *game)
{
	uint8_t	*pixelstart;

	if (x > texture->texture_ptr->width || y > texture->texture_ptr->height)
		return (0);
	pixelstart = texture->texture_ptr->pixels + \
				(y * texture->texture_ptr->width + x) * BPP;
	game->color = (t_rgb){(*pixelstart), *(pixelstart + 1), *(pixelstart + 2)};
	return (rgb_to_int(game->color));
}

// Función para llenar cielo y suelo con colores
void	draw_sky_and_floor(t_game *game)
{
	int	y;
	int	x;
	int	sky_color;
	int	floor_color;
	int	vertical_shift;

	y = 0;
	sky_color = rgb_to_int(game->sky_color);
	floor_color = rgb_to_int(game->floor_color);
	vertical_shift = (int)(game->player.view_offset * HEIGHT);
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			if (y + vertical_shift < HEIGHT / 2)
				mlx_put_pixel(game->image, x, y, sky_color);
			else if (y + vertical_shift >= HEIGHT / 2 \
					&& y + vertical_shift < HEIGHT)
				mlx_put_pixel(game->image, x, y, floor_color);
			x++;
		}
		y++;
	}
}

void	draw_pixels(t_game *game, t_line_params *line, t_texture *texture, \
						int tex_x)
{
	int	y;
	int	draw_distance;
	int	vertical_shift;

	y = line->draw_start;
	vertical_shift = (int)(game->player.view_offset * HEIGHT);
	while (y < line->draw_end)
	{
		draw_distance = (y - vertical_shift) * 256 \
			- HEIGHT * 128 + line->line_height * 128;
		if (y >= 0 && y < HEIGHT) // Verificar que el píxel esté dentro
		{
			mlx_put_pixel(game->image, line->x, y, \
				mlx_get_pixel(texture, tex_x, \
				((draw_distance * texture->height) / line->line_height) / 256, \
				game));
		}
		y++;
	}
}

// Dibuja una línea vertical en la pantalla utilizando texturas
void	draw_textured_line(t_game *game, t_ray *ray, t_line_params *line, \
								t_texture *texture)
{
	double	wall_x;
	int		tex_x;

	if (ray->side == 0)
		wall_x = game->player.pos_y + line->perp_wall_dist * ray->ray_dir_y;
	else
		wall_x = game->player.pos_x + line->perp_wall_dist * ray->ray_dir_x;
	wall_x -= floor(wall_x);
	tex_x = (int)(wall_x * texture->width);
	if ((ray->side == 0 && ray->ray_dir_x > 0) || (ray->side == 1 \
			&& ray->ray_dir_y < 0))
		tex_x = texture->width - tex_x - 1;
	draw_pixels(game, line, texture, tex_x);
}

void	draw_floor(t_game *game, t_line_params *line)
{
	int	y;

	if (line->draw_end < HEIGHT)
	{
		y = line->draw_end;
		while (y < HEIGHT)
		{
			mlx_put_pixel(game->image, line->x, y, \
							rgb_to_int(game->floor_color));
			y++;
		}
	}
}
