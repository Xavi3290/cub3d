/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xavi <xavi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 19:36:35 by xroca-pe          #+#    #+#             */
/*   Updated: 2024/11/18 19:49:45 by xavi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// Función auxiliar para obtener el color de la textura basado en coordenadas
int get_texture_color(t_texture *texture, int texX, int texY) {
    if (texX >= 0 && texX < texture->width && texY >= 0 && texY < texture->height) {
        int *pixels = (int *)texture->texture_ptr->pixels; // Obtener los píxeles como un array de int
        return pixels[texY * texture->width + texX];
    }
    return 0; // Retorna negro si está fuera de límites
}


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
    if (ray->sideDistX < 0.0001) 
        ray->sideDistX = 0.0001;
    if (ray->sideDistY < 0.0001) 
        ray->sideDistY = 0.0001;
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

// Dibuja una línea vertical en la imagen utilizando el algoritmo de Bresenham
/*static void draw_line_bresenham(mlx_image_t *image, t_line_params *line)
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
}*/

/*// Función principal para realizar el raycasting y dibujar las paredes en la pantalla
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
}*/

t_texture *select_texture(t_ray *ray, t_game *game) {
    if (ray->side == 0 && ray->stepX < 0)
        return &game->wall_textures[0]; // Norte
    else if (ray->side == 0 && ray->stepX > 0)
        return &game->wall_textures[1]; // Sur
    else if (ray->side == 1 && ray->stepY < 0)
        return &game->wall_textures[2]; // Oeste
    return &game->wall_textures[3]; // Este
}

void draw_textured_line(t_game *game, t_ray *ray, t_line_params *line, t_texture *texture) {
    double wallX = (ray->side == 0) ? (game->player.posY + line->perpWallDist * ray->rayDirY) : (game->player.posX + line->perpWallDist * ray->rayDirX);
    wallX -= floor(wallX);
    int texX = (int)(wallX * texture->width);
    if ((ray->side == 0 && ray->rayDirX > 0) || (ray->side == 1 && ray->rayDirY < 0))
        texX = texture->width - texX - 1;

    int y = line->drawStart;
    while (y < line->drawEnd) {
        int d = y * 256 - HEIGHT * 128 + line->lineHeight * 128;
        int texY = ((d * texture->height) / line->lineHeight) / 256;
        int color = get_texture_color(texture, texX, texY);
        mlx_put_pixel(game->image, line->x, y, color);
        y++;
    }
}


void perform_raycasting(t_game *game) {
    int x = 0;

    draw_sky_and_floor(game);
    while (x < WIDTH) {
        t_ray ray;
        t_line_params line;
        line.x = x;
        process_ray(&ray, game, &line);
        t_texture *texture = select_texture(&ray, game);
        draw_textured_line(game, &ray, &line, texture);
        x++;
    }
}


