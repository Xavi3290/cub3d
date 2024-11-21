/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xavi <xavi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 19:36:35 by xroca-pe          #+#    #+#             */
/*   Updated: 2024/11/21 19:35:57 by xavi             ###   ########.fr       */
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
/*int get_texture_color(t_texture *texture, int texX, int texY) {
    if (texX >= 0 && texX < texture->width && texY >= 0 && texY < texture->height) {
        // Obtener el puntero de píxeles
        uint8_t *pixels = (uint8_t *)texture->texture_ptr->pixels;
        int index = (texY * texture->width + texX) * 4; // Cada píxel tiene 4 bytes (RGBA)

        // Extraer componentes RGBA
        uint8_t r = pixels[index];       // Rojo
        uint8_t g = pixels[index + 1];   // Verde
        uint8_t b = pixels[index + 2];   // Azul
        uint8_t a = pixels[index + 3];   // Alfa

        // Si el alfa es 0 (transparente), retorna 0 (negro)
        if (a == 0) return 0;

        // Combinar los colores en un solo entero (suponiendo formato ABGR para MLX42)
        return (a << 24) | (b << 16) | (g << 8) | r;
    }
    return 0; // Retorna negro si está fuera de límites
}*/



// Función para llenar cielo y suelo con colores
/*static void draw_sky_and_floor(t_game *game) {
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
}*/

void draw_sky_and_floor(t_game *game) {
    int y = 0;
    int x;
    int sky_color = rgb_to_int(game->sky_color);
    int floor_color = rgb_to_int(game->floor_color);

    // Calcula el desplazamiento vertical
    int vertical_shift = (int)(game->player.view_offset * HEIGHT);

    while (y < HEIGHT) {
        x = 0;
        while (x < WIDTH) {
            if (y + vertical_shift < HEIGHT / 2) {
                // Dibuja el cielo desplazado hacia abajo
                mlx_put_pixel(game->image, x, y, sky_color);
            } else if (y + vertical_shift >= HEIGHT / 2 && y + vertical_shift < HEIGHT) {
                // Dibuja el suelo desplazado hacia arriba
                mlx_put_pixel(game->image, x, y, floor_color);
            }
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
    int max_steps = game->map_width * game->map_height/*MAP_WIDTH * MAP_HEIGHT*/;
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

        if (ray->mapX < 0 || ray->mapX >= game->map_width/*MAP_WIDTH*/ || ray->mapY < 0 || ray->mapY >= game->map_height/*MAP_HEIGHT*/) {
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

// Función para seleccionar la textura correcta basada en el lado de la pared
t_texture *select_texture(t_ray *ray, t_game *game) {
    if (ray->side == 0 && ray->stepX < 0)
        return &game->wall_textures[0]; // Norte
    else if (ray->side == 0 && ray->stepX > 0)
        return &game->wall_textures[1]; // Sur
    else if (ray->side == 1 && ray->stepY < 0)
        return &game->wall_textures[2]; // Oeste
    return &game->wall_textures[3]; // Este
}

// Dibuja una línea vertical en la pantalla utilizando texturas
void draw_textured_line(t_game *game, t_ray *ray, t_line_params *line, t_texture *texture) {
    double wallX;
    if (ray->side == 0) {
        wallX = game->player.posY + line->perpWallDist * ray->rayDirY;
    } else {
        wallX = game->player.posX + line->perpWallDist * ray->rayDirX;
    }
    wallX -= floor(wallX); // Mantener la fracción para la coordenada de textura

    int texX = (int)(wallX * texture->width);
    if ((ray->side == 0 && ray->rayDirX > 0) || (ray->side == 1 && ray->rayDirY < 0)) {
        texX = texture->width - texX - 1; // Invertir la textura si es necesario
    }

    // Agregar el desplazamiento del salto
    int vertical_shift = (int)(game->player.view_offset * HEIGHT);

    int y = line->drawStart;
    while (y < line->drawEnd) {
        // Calcular la coordenada Y en la textura
        int d = (y - vertical_shift) * 256 - HEIGHT * 128 + line->lineHeight * 128;
        int texY = ((d * texture->height) / line->lineHeight) / 256;

        // Obtener el color del píxel desde la textura
        int color = get_texture_color(texture, texX, texY);

        // Dibujar el píxel en la pantalla si el color es válido
        if (y >= 0 && y < HEIGHT) {
            mlx_put_pixel(game->image, line->x, y, color);
        }
        y++;
    }
}





/*void perform_raycasting(t_game *game) {
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
}*/

void perform_raycasting(t_game *game) {
    int x = 0;

    // Dibujar cielo y suelo primero
    draw_sky_and_floor(game);

    // Ajustar el desplazamiento de la vista para el salto
    int vertical_shift = (int)(game->player.view_offset * HEIGHT);

    while (x < WIDTH) {
        t_ray ray;
        t_line_params line;
        line.x = x;

        process_ray(&ray, game, &line);

        // Ajustar las líneas de las paredes según el desplazamiento
        line.drawStart = -line.lineHeight / 2 + HEIGHT / 2 + vertical_shift;
        if (line.drawStart < 0)
            line.drawStart = 0;

        line.drawEnd = line.lineHeight / 2 + HEIGHT / 2 + vertical_shift;
        if (line.drawEnd >= HEIGHT)
            line.drawEnd = HEIGHT - 1;

        // Dibujar cualquier área "desplazada" como suelo o cielo
        if (line.drawStart > 0) {
            for (int y = 0; y < line.drawStart; y++) {
                mlx_put_pixel(game->image, line.x, y, rgb_to_int(game->sky_color));
            }
        }
        if (line.drawEnd < HEIGHT) {
            for (int y = line.drawEnd; y < HEIGHT; y++) {
                mlx_put_pixel(game->image, line.x, y, rgb_to_int(game->floor_color));
            }
        }

        // Selección de textura y dibujo
        t_texture *texture = select_texture(&ray, game);
        draw_textured_line(game, &ray, &line, texture);

        x++;
    }
}



