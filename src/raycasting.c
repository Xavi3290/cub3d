/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xroca-pe <xroca-pe@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 19:36:35 by xroca-pe          #+#    #+#             */
/*   Updated: 2024/10/22 19:58:03 by xroca-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static void draw_line_bresenham(mlx_image_t *image, int x, int y_start, int y_end, int color)
{
    int y = y_start;
    int delta_y = abs(y_end - y_start);
    int delta_x = 1;  // Línea vertical
    int error = 0;
    int y_step = (y_start < y_end) ? 1 : -1;

    while (y != y_end)
    {
        mlx_put_pixel(image, x, y, color);
        error += delta_y;
        if (error >= delta_x)
        {
            y += y_step;
            error -= delta_x;
        }
    }
}

static void calculate_step(int *stepX, int *stepY, double rayDirX, double rayDirY, double *sideDistX, double *sideDistY, int mapX, int mapY, double posX, double posY, double deltaDistX, double deltaDistY)
{
    if (rayDirX < 0)
    {
        *stepX = -1;
        *sideDistX = (posX - mapX) * deltaDistX;
    }
    else
    {
        *stepX = 1;
        *sideDistX = (mapX + 1.0 - posX) * deltaDistX;
    }
    if (rayDirY < 0)
    {
        *stepY = -1;
        *sideDistY = (posY - mapY) * deltaDistY;
    }
    else
    {
        *stepY = 1;
        *sideDistY = (mapY + 1.0 - posY) * deltaDistY;
    }
}

static int dda(int *mapX, int *mapY, double *sideDistX, double *sideDistY, double deltaDistX, double deltaDistY, int stepX, int stepY, const int worldMap[MAP_WIDTH][MAP_HEIGHT])
{
    int hit = 0, side;
    while (hit == 0)
    {
        if (*sideDistX < *sideDistY)
        {
            *sideDistX += deltaDistX;
            *mapX += stepX;
            side = 0;
        }
        else
        {
            *sideDistY += deltaDistY;
            *mapY += stepY;
            side = 1;
        }
        if (worldMap[*mapX][*mapY] > 0)
            hit = 1;
    }
    return side;
}

void perform_raycasting(mlx_image_t *image, t_player *player, const int worldMap[MAP_WIDTH][MAP_HEIGHT])
{
    int x = 0;
    while (x < WIDTH)
    {
        double cameraX = 2 * x / (double)WIDTH - 1;
        double rayDirX = player->dirX + player->planeX * cameraX;
        double rayDirY = player->dirY + player->planeY * cameraX;
        int mapX = (int)player->posX;
        int mapY = (int)player->posY;
        double deltaDistX = fabs(1 / rayDirX);
        double deltaDistY = fabs(1 / rayDirY);
        double sideDistX, sideDistY;
        int stepX, stepY;
        
        calculate_step(&stepX, &stepY, rayDirX, rayDirY, &sideDistX, &sideDistY, mapX, mapY, player->posX, player->posY, deltaDistX, deltaDistY);
        int side = dda(&mapX, &mapY, &sideDistX, &sideDistY, deltaDistX, deltaDistY, stepX, stepY, worldMap);
        
        double perpWallDist = (side == 0) ? (mapX - player->posX + (1 - stepX) / 2) / rayDirX : (mapY - player->posY + (1 - stepY) / 2) / rayDirY;
        int lineHeight = (int)(HEIGHT / perpWallDist);
        int drawStart = -lineHeight / 2 + HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + HEIGHT / 2;
        if (drawEnd >= HEIGHT) drawEnd = HEIGHT - 1;
        
        int color = (side == 1) ? 0xAAAAAA : 0xFFFFFF;
        draw_line_bresenham(image, x, drawStart, drawEnd, color);
        x++;
    }
}

