/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xavi <xavi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:27:14 by xavi              #+#    #+#             */
/*   Updated: 2024/11/25 20:04:26 by xavi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// Inicialización del rayo para cada columna de la pantalla
static void init_ray(t_ray *ray, t_player *player, int x)
{
    ray->cameraX = 2 * (WIDTH - x) / (double)WIDTH - 1;
    ray->rayDirX = player->dirX + player->planeX * ray->cameraX;
    ray->rayDirY = player->dirY + player->planeY * ray->cameraX;
    ray->mapX = (int)player->posX;
    ray->mapY = (int)player->posY;
    ray->deltaDistX = fabs(1 / ray->rayDirX);
    ray->deltaDistY = fabs(1 / ray->rayDirY);
}

// Configuración inicial para el DDA
static void init_dda(t_ray *ray, t_player *player)
{
    if (ray->rayDirX < 0)
    {
        ray->stepX = -1;
        ray->sideDistX = (player->posX - ray->mapX) * ray->deltaDistX;
    }
    else
    {
        ray->stepX = 1;
        ray->sideDistX = (ray->mapX + 1.0 - player->posX) * ray->deltaDistX;
    }
    if (ray->rayDirY < 0)
    {
        ray->stepY = -1;
        ray->sideDistY = (player->posY - ray->mapY) * ray->deltaDistY;
    }
    else
    {
        ray->stepY = 1;
        ray->sideDistY = (ray->mapY + 1.0 - player->posY) * ray->deltaDistY;
    }
}

// Función DDA para detectar colisiones
static void perform_dda(t_ray *ray, t_game *game) {
    int hit = 0;
    int max_steps = game->map_width * game->map_height;
    int steps = 0;

    while (hit == 0 && steps < max_steps) {
        steps++;
        if (ray->sideDistX < ray->sideDistY) {
            ray->sideDistX += ray->deltaDistX;
            ray->mapX += ray->stepX;
            ray->side = 0;
        } else {
            ray->sideDistY += ray->deltaDistY;
            ray->mapY += ray->stepY;
            ray->side = 1;
        }

        if (ray->mapX < 0 || ray->mapX >= game->map_width || ray->mapY < 0 || ray->mapY >= game->map_height) {
            hit = 1;
            break;
        }
        if (is_wall(game, ray->mapX, ray->mapY)) {
            hit = 1;
            break;
        }
    }
    if (ray->sideDistX < 0.0001) 
        ray->sideDistX = 0.0001;
    if (ray->sideDistY < 0.0001) 
        ray->sideDistY = 0.0001;
}


// Procesa un rayo y calcula los parámetros necesarios para dibujar la línea de la pared
void process_ray(t_ray *ray, t_game *game, t_line_params *line)
{
    init_ray(ray, &game->player, line->x);
    init_dda(ray, &game->player);
    perform_dda(ray, game);

    // Calcula la distancia perpendicular a la pared
    if (ray->side == 0)
        line->perpWallDist = (ray->mapX - game->player.posX + (1 - ray->stepX) / 2) / ray->rayDirX;
    else
        line->perpWallDist = (ray->mapY - game->player.posY + (1 - ray->stepY) / 2) / ray->rayDirY;

    if (line->perpWallDist < 0.01) 
        line->perpWallDist = 0.01;

    // Calcula la altura de la línea a dibujar
    line->lineHeight = (int)(HEIGHT / line->perpWallDist);

    // Calcula el inicio y fin de la línea a dibujar
    line->drawStart = -line->lineHeight / 2 + HEIGHT / 2;
    if (line->drawStart < 0)
        line->drawStart = 0;

    line->drawEnd = line->lineHeight / 2 + HEIGHT / 2;
    if (line->drawEnd >= HEIGHT)
        line->drawEnd = HEIGHT - 1;
}