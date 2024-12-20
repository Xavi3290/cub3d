/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xroca-pe <xroca-pe@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:27:14 by xavi              #+#    #+#             */
/*   Updated: 2024/12/20 17:53:58 by xroca-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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
		ray->side_dist_x = (ray->map_x + 1.0 - player->pos_x)
			* ray->delta_dist_x;
	}
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (player->pos_y - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - player->pos_y)
			* ray->delta_dist_y;
	}
}

static int	execute_dda_loop(t_ray *ray, t_game *game, int max_steps, int steps)
{
	int	hit;

	hit = 0;
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
		hit = check_hit(ray, game);
		if (game->mapinfo.map[ray->map_y][ray->map_x] == 'D')
			return (2);
	}
	return (hit);
}

static void	perform_dda(t_ray *ray, t_game *game)
{
	int	hit;
	int	max_steps;

	max_steps = game->mapinfo.width * game->mapinfo.height;
	hit = execute_dda_loop(ray, game, max_steps, 0);
	if (ray->side_dist_x < 0.0001)
		ray->side_dist_x = 0.0001;
	if (ray->side_dist_y < 0.0001)
		ray->side_dist_y = 0.0001;
	if (hit == 2)
		ray->special = 1;
	else
		ray->special = 0;
}

void	process_ray(t_ray *ray, t_game *game, t_line_params *line)
{
	init_ray(ray, &game->player, line->x);
	init_dda(ray, &game->player);
	perform_dda(ray, game);
	if (ray->side == 0)
		line->perp_wall_dist = (ray->map_x - game->player.pos_x + (1
					- ray->step_x) / 2) / ray->ray_dir_x;
	else
		line->perp_wall_dist = (ray->map_y - game->player.pos_y + (1
					- ray->step_y) / 2) / ray->ray_dir_y;
	if (line->perp_wall_dist < 0.01)
		line->perp_wall_dist = 0.01;
	line->line_height = (int)(HEIGHT / line->perp_wall_dist);
	line->draw_start = -line->line_height / 2 + HEIGHT / 2;
	if (line->draw_start < 0)
		line->draw_start = 0;
	line->draw_end = line->line_height / 2 + HEIGHT / 2;
	if (line->draw_end >= HEIGHT)
		line->draw_end = HEIGHT - 1;
}
