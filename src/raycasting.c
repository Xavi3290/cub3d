/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xavi <xavi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 19:36:35 by xroca-pe          #+#    #+#             */
/*   Updated: 2024/11/04 21:31:07 by xavi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// Función para llenar cielo y suelo con colores
static void draw_sky_and_floor(mlx_image_t *image, int sky_color, int floor_color)
{
    int y;
    int x;

    y = 0;
    while (y < HEIGHT / 2)
    {
        x = 0;
        while (x < WIDTH)
        {
            mlx_put_pixel(image, x, y, sky_color);
            mlx_put_pixel(image, x, y + HEIGHT / 2, floor_color);
            x++;
        }
        y++;
    }
}

// Inicialización del rayo para cada columna de la pantalla
static void init_ray(t_ray *ray, t_player *player, int x)
{
    ray->cameraX = 2 * x / (double)WIDTH - 1;
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
static void perform_dda(t_ray *ray, const int worldMap[MAP_WIDTH][MAP_HEIGHT])
{
    int hit;

    hit = 0;
    while (hit == 0)
    {
        if (ray->sideDistX < ray->sideDistY)
        {
            ray->sideDistX += ray->deltaDistX;
            ray->mapX += ray->stepX;
            ray->side = 0;
        }
        else
        {
            ray->sideDistY += ray->deltaDistY;
            ray->mapY += ray->stepY;
            ray->side = 1;
        }
        if (worldMap[ray->mapX][ray->mapY] == 1)
            hit = 1;
        else if (worldMap[ray->mapX][ray->mapY] == 'N' || worldMap[ray->mapX][ray->mapY] == 'S' ||
                 worldMap[ray->mapX][ray->mapY] == 'E' || worldMap[ray->mapX][ray->mapY] == 'W')
            continue;
    }
}

/*// Dibujo de línea utilizando Bresenham
static void draw_line_bresenham(mlx_image_t *image, int x, int y_start, int y_end, int color)
{
    int y = y_start;
    int delta_y = abs(y_end - y_start);
    int error = 0;
    int y_step;

    if (y_start < y_end)
        y_step = 1;
    else
        y_step = -1;

    while (y != y_end)
    {
        mlx_put_pixel(image, x, y, color);
        y += y_step;
        error += delta_y;
        if (error >= 1)
            error -= 1;
    }
}

// Función principal de raycasting
void perform_raycasting(mlx_image_t *image, t_player *player, const int worldMap[MAP_WIDTH][MAP_HEIGHT])
{
    draw_sky_and_floor(image, 0x87CEEB, 0x8B4513);  // Colores de cielo y suelo
    int x = 0;
    while (x < WIDTH)
    {
        t_ray ray;
        init_ray(&ray, player, x);
        set_step_and_initial_side_dist(&ray, player);
        perform_dda(&ray, worldMap);

        double perpWallDist;
        if (ray.side == 0)
            perpWallDist = (ray.mapX - player->posX + (1 - ray.stepX) / 2) / ray.rayDirX;
        else
            perpWallDist = (ray.mapY - player->posY + (1 - ray.stepY) / 2) / ray.rayDirY;

        int lineHeight = (int)(HEIGHT / perpWallDist);

        int drawStart = -lineHeight / 2 + HEIGHT / 2;
        if (drawStart < 0)
            drawStart = 0;

        int drawEnd = lineHeight / 2 + HEIGHT / 2;
        if (drawEnd >= HEIGHT)
            drawEnd = HEIGHT - 1;

        int color;
        if (ray.side == 1)
            color = 0xAAAAAA;
        else
            color = 0xFFFFFF;

        draw_line_bresenham(image, x, drawStart, drawEnd, color);
        x++;
    }
}*/


// Procesa un rayo y calcula los parámetros necesarios para dibujar la línea de la pared
static void process_ray(t_ray *ray, t_player *player, const int worldMap[MAP_WIDTH][MAP_HEIGHT], t_line_params *line)
{
    init_ray(ray, player, line->x);
    set_step_and_initial_side_dist(ray, player);
    perform_dda(ray, worldMap);

    // Calcula la distancia perpendicular a la pared
    if (ray->side == 0)
        line->perpWallDist = (ray->mapX - player->posX + (1 - ray->stepX) / 2) / ray->rayDirX;
    else
        line->perpWallDist = (ray->mapY - player->posY + (1 - ray->stepY) / 2) / ray->rayDirY;

    // Calcula la altura de la línea a dibujar
    line->lineHeight = (int)(HEIGHT / line->perpWallDist);

    // Calcula el inicio y fin de la línea a dibujar
    line->drawStart = -line->lineHeight / 2 + HEIGHT / 2;
    if (line->drawStart < 0)
        line->drawStart = 0;

    line->drawEnd = line->lineHeight / 2 + HEIGHT / 2;
    if (line->drawEnd >= HEIGHT)
        line->drawEnd = HEIGHT - 1;

    // Establece el color basado en el lado del impacto
    if (ray->side == 1)
        line->color = 0xAAAAAA;
    else
        line->color = 0xFFFFFF;
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
        mlx_put_pixel(image, line->x, y, line->color);
        y += y_step;
        error += delta_y;
        if (error >= 1)
            error -= 1;
    }
}

// Función principal para realizar el raycasting y dibujar las paredes en la pantalla
void perform_raycasting(mlx_image_t *image, t_player *player, const int worldMap[MAP_WIDTH][MAP_HEIGHT])
{
    int x;
    
    draw_sky_and_floor(image, 0x87CEEB, 0x8B4513);  // Colores de cielo y suelo
    x = 0;
    while (x < WIDTH)
    {
        t_ray ray;
        t_line_params line;
        line.x = x;  // Inicializa el valor de X para cada rayo

        // Procesa el rayo y llena los detalles de la línea en la estructura
        process_ray(&ray, player, worldMap, &line);

        // Dibuja la línea utilizando los parámetros calculados
        draw_line_bresenham(image, &line);
        x++;
    }
}
