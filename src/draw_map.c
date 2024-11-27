/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/11/27 12:22:11 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/cub3d.h"

// Función auxiliar para obtener el color de la textura basado en coordenadas
/*static int get_texture_color(t_texture *texture, int texX, int texY) {
    if (texX >= 0 && texX < texture->width && texY >= 0 && texY < texture->height) {
        int *pixels = (int *)texture->texture_ptr->pixels;
        return pixels[texY * texture->width + texX];
    }
    return 0; // Retorna negro si está fuera de límites
}*/


static int32_t	mlx_get_pixel(t_texture *texture, uint32_t x, uint32_t y, t_game *game)
{
	uint8_t	*pixelstart;

	if (x > texture->texture_ptr->width || y > texture->texture_ptr->height)
		return (0);
	pixelstart = texture->texture_ptr->pixels + (y * texture->texture_ptr->width + x) * BPP;
    game->color = (t_rgb){*(pixelstart), *(pixelstart + 1), *(pixelstart + 2)};

    return (rgb_to_int(game->color));
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
static void draw_sky_and_floor(t_game *game) {
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
static void draw_textured_line(t_game *game, t_ray *ray, t_line_params *line, t_texture *texture) {
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
        int color = mlx_get_pixel(texture, texX, texY, game); //get_texture_color(texture, texX, texY);

        // Dibujar el píxel en la pantalla si el color es válido
        if (y >= 0 && y < HEIGHT) {
            mlx_put_pixel(game->image, line->x, y, color);
        }
        y++;
    }
}

// Función principal para realizar el raycasting y dibujar las paredes en la pantalla
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


