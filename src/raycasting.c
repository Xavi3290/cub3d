/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xavi <xavi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 19:36:35 by xroca-pe          #+#    #+#             */
/*   Updated: 2024/11/12 21:35:41 by xavi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// Función para llenar cielo y suelo con colores
static void draw_sky_and_floor(t_game *game) {
    int y = 0;
    int x;
    int sky_color = rgb_to_int(game->sky_color);
    int floor_color = rgb_to_int(game->floor_color);

    while (y < HEIGHT / 2) {
        x = 0;
        while (x < WIDTH) {
            mlx_put_pixel(game->image, x, y, sky_color);
            mlx_put_pixel(game->image, x, y + HEIGHT / 2, floor_color);
            x++;
        }
        y++;
    }
}


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
static void set_step_and_initial_side_dist(t_ray *ray, t_player *player)
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
    int max_steps = MAP_WIDTH * MAP_HEIGHT;
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

        if (ray->mapX < 0 || ray->mapX >= MAP_WIDTH || ray->mapY < 0 || ray->mapY >= MAP_HEIGHT) {
            hit = 1;
            break;
        }
        if (is_wall(game, ray->mapX, ray->mapY)) {
            hit = 1;
            break;
        }
    }
}


// Procesa un rayo y calcula los parámetros necesarios para dibujar la línea de la pared
static void process_ray(t_ray *ray, t_game *game, t_line_params *line)
{
    init_ray(ray, &game->player, line->x);
    set_step_and_initial_side_dist(ray, &game->player);
    perform_dda(ray, game);

    // Calcula la distancia perpendicular a la pared
    if (ray->side == 0)
        line->perpWallDist = (ray->mapX - game->player.posX + (1 - ray->stepX) / 2) / ray->rayDirX;
    else
        line->perpWallDist = (ray->mapY - game->player.posY + (1 - ray->stepY) / 2) / ray->rayDirY;

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

// Dibuja una línea vertical en la imagen utilizando el algoritmo de Bresenham
static void draw_line_bresenham(mlx_image_t *image, t_line_params *line)
{
    int y;
    int delta_y;
    int error;
    int y_step;

    y = line->drawStart;
    delta_y = abs(line->drawEnd - line->drawStart);
    error = 0;
    // Determina la dirección del paso en Y
    if (line->drawStart < line->drawEnd)
        y_step = 1;
    else
        y_step = -1;

    // Dibuja la línea píxel por píxel
    while (y != line->drawEnd)
    {
        if (y >= 0 && y < HEIGHT)
            mlx_put_pixel(image, line->x, y, line->color);
        y += y_step;
        error += delta_y;
        if (error >= 1)
            error -= 1;
    }
}

// Función principal para realizar el raycasting y dibujar las paredes en la pantalla
void perform_raycasting(t_game *game) {
    int x = 0;
    draw_sky_and_floor(game);

    while (x < WIDTH) {
        t_ray ray;
        t_line_params line;
        line.x = x;

        process_ray(&ray, game, &line);

        // Selección del color de pared en función del lado
        if (ray.side == 1) {
            line.color = rgb_to_int(game->wall_color_dark);
        } else {
            line.color = rgb_to_int(game->wall_color_light);
        }

        draw_line_bresenham(game->image, &line);
        x++;
    }
}

