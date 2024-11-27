/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:27:14 by xavi              #+#    #+#             */
/*   Updated: 2024/11/27 14:24:33 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// Inicialización del rayo para cada columna de la pantalla
static void	init_ray(t_ray *ray, t_player *player, int x)
{
	ray->camera_x = 2 * (WIDTH - x) / (double)WIDTH - 1;
	ray->ray_dir_x = player->dir_x + player->plane_x * ray->camera_x;
	ray->ray_dir_y = player->dir_y + player->plane_y * ray->camera_x;
	ray->map_x = (int)player->pos_x;
	ray->map_y = (int)player->pos_y;
	ray->delta_dist_x = fabs(1 / ray->ray_dir_x);
	ray->delta_dist_y = fabs(1 / ray->ray_dir_y);
}

// Configuración inicial para el DDA
static void	init_dda(t_ray *ray, t_player *player)
{
	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (player->pos_x - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - player->pos_x) * ray->delta_dist_x;
	}
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (player->pos_y - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - player->pos_y) * ray->delta_dist_y;
	}
}

// Función DDA para detectar colisiones
static void	perform_dda(t_ray *ray, t_game *game)
{
	int	hit;
	int	max_steps;
	int	steps;

	hit = 0;
	max_steps = game->mapinfo.width * game->mapinfo.height;
	steps = 0;
	while (hit == 0 && steps < max_steps)
	{
		steps++;
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (ray->map_x < 0 || ray->map_x >= game->mapinfo.width \
			|| ray->map_y < 0 || ray->map_y >= game->mapinfo.height)
		{
			hit = 1;
			break ;
		}
		if (is_wall(game, ray->map_x, ray->map_y))
		{
			hit = 1;
			break ;
		}
	}
	if (ray->side_dist_x < 0.0001)
		ray->side_dist_x = 0.0001;
	if (ray->side_dist_y < 0.0001)
		ray->side_dist_y = 0.0001;
}

// Procesa un rayo y calcula los parámetros necesarios para dibujar la pared
void	process_ray(t_ray *ray, t_game *game, t_line_params *line)
{
	init_ray(ray, &game->player, line->x);
	init_dda(ray, &game->player);
	perform_dda(ray, game);
	// Calcula la distancia perpendicular a la pared
	if (ray->side == 0)
		line->perp_wall_dist = (ray->map_x - game->player.pos_x \
			+ (1 - ray->step_x) / 2) / ray->ray_dir_x;
	else
		line->perp_wall_dist = (ray->map_y - game->player.pos_y \
			+ (1 - ray->step_y) / 2) / ray->ray_dir_y;
	if (line->perp_wall_dist < 0.01)
		line->perp_wall_dist = 0.01;
	// Calcula la altura de la línea a dibujar
	line->line_height = (int)(HEIGHT / line->perp_wall_dist);
	// Calcula el inicio y fin de la línea a dibujar
	line->draw_start = -line->line_height / 2 + HEIGHT / 2;
	if (line->draw_start < 0)
		line->draw_start = 0;
	line->draw_end = line->line_height / 2 + HEIGHT / 2;
	if (line->draw_end >= HEIGHT)
		line->draw_end = HEIGHT - 1;
}
