/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:06:40 by xavi              #+#    #+#             */
/*   Updated: 2024/11/27 14:31:01 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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
static void	draw_sky_and_floor(t_game *game)
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

// Función para seleccionar la textura correcta basada en el lado de la pared
t_texture	*select_texture(t_ray *ray, t_game *game)
{
	if (ray->side == 0 && ray->step_x < 0)
		return (&game->wall_textures[0]); // Norte
	else if (ray->side == 0 && ray->step_x > 0)
		return (&game->wall_textures[1]); // Sur
	else if (ray->side == 1 && ray->step_y < 0)
		return (&game->wall_textures[2]); // Oeste
	return (&game->wall_textures[3]); // Este
}

// Dibuja una línea vertical en la pantalla utilizando texturas
static void	draw_textured_line(t_game *game, t_ray *ray, t_line_params *line, \
								t_texture *texture)
{
	double	wall_x;
	int		tex_x;
	int		y;
	int		draw_distance;

	if (ray->side == 0)
		wall_x = game->player.pos_y + line->perp_wall_dist * ray->ray_dir_y;
	else
		wall_x = game->player.pos_x + line->perp_wall_dist * ray->ray_dir_x;
	wall_x -= floor(wall_x); // Mantener la fracción para la coordenada
	tex_x = (int)(wall_x * texture->width);
	if ((ray->side == 0 && ray->ray_dir_x > 0) || (ray->side == 1 \
			&& ray->ray_dir_y < 0))
		tex_x = texture->width - tex_x - 1; // Invertir la textura
	y = line->draw_start;
	while (y < line->draw_end)
	{
		// Calcular la coordenada Y en la textura
		draw_distance = (y - (int)game->player.view_offset * HEIGHT) * 256 \
			- HEIGHT * 128 + line->line_height * 128;
		// Dibujar el píxel en la pantalla si el color es válido
		if (y >= 0 && y < HEIGHT)
		{
			mlx_put_pixel(game->image, line->x, y, \
				mlx_get_pixel(texture, tex_x, \
				((draw_distance * texture->height) / line->line_height) / 256, \
				game));
		}
		y++;
	}
}

// Función principal para realizar el raycasting y dibujar las paredes
void	perform_raycasting(t_game *game)
{
	int				x;
	int				y;
	int				vertical_shift;
	t_ray			ray;
	t_line_params	line;

	x = 0;
	// Dibujar cielo y suelo primero
	draw_sky_and_floor(game);
	// Ajustar el desplazamiento de la vista para el salto
	vertical_shift = (int)(game->player.view_offset * HEIGHT);
	while (x < WIDTH)
	{
		line.x = x;
		process_ray(&ray, game, &line);
		// Ajustar las líneas de las paredes según el desplazamiento
		line.draw_start = -line.line_height / 2 + HEIGHT / 2 + vertical_shift;
		if (line.draw_start < 0)
			line.draw_start = 0;
		line.draw_end = line.line_height / 2 + HEIGHT / 2 + vertical_shift;
		if (line.draw_end >= HEIGHT)
			line.draw_end = HEIGHT - 1;
		// Dibujar cualquier área "desplazada" como suelo o cielo
		if (line.draw_start > 0)
		{
			y = -1;
			while (++y < line.draw_start)
				mlx_put_pixel(game->image, line.x, y, \
								rgb_to_int(game->sky_color));
		}
		if (line.draw_end < HEIGHT)
		{
			y = line.draw_end;
			while (y < HEIGHT)
			{
				mlx_put_pixel(game->image, line.x, y, \
								rgb_to_int(game->floor_color));
				y++;
			}
		}
		draw_textured_line(game, &ray, &line, select_texture(&ray, game));
		x++;
	}
}
