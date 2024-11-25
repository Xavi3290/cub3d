/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xavi <xavi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:02:25 by xavi              #+#    #+#             */
/*   Updated: 2024/11/25 20:44:03 by xavi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// Función para liberar las texturas de la memoria
void free_textures(t_game *game) {
    int i = 0;

    while (i < 4) {
        if (game->wall_textures[i].texture_ptr) {
            mlx_delete_image(game->mlx, game->wall_textures[i].texture_ptr);
            game->wall_textures[i].texture_ptr = NULL;
        }
        i++;
    }
}

// Cargar texturas en un array de texturas
void load_textures_in_array(t_game *game, t_texture texture[4]) {
    int i = 0;

    while (i < 4) {
        xpm_t *temp_texture = mlx_load_xpm42(texture[i].path); // Cargar textura XPM
        if (!temp_texture) {
            printf("Error: No se pudo cargar la textura %s\n", texture[i].path);
            exit(1);
        }

        // Convertir textura a imagen y guardar puntero
        texture[i].texture_ptr = mlx_texture_to_image(game->mlx, &temp_texture->texture);
        if (!texture[i].texture_ptr) {
            mlx_delete_xpm42(temp_texture);
            printf("Error: No se pudo convertir la textura %s a imagen\n", texture[i].path);
            exit(1);
        }

        // Configurar dimensiones y datos de textura
        texture[i].width = temp_texture->texture.width;
        texture[i].height = temp_texture->texture.height;
        texture[i].wall_texture = (int *)texture[i].texture_ptr->pixels; // Obtener píxeles

        mlx_delete_xpm42(temp_texture); // Liberar textura temporal después de convertirla
        i++;
    }
}

/*void test_texture_render(t_game *game, t_texture *texture) {
    for (int y = 0; y < texture->height; y++) {
        for (int x = 0; x < texture->width; x++) {
            int color = get_texture_color(texture, x, y);
            mlx_put_pixel(game->image, x, y, color);
        }
    }
    mlx_image_to_window(game->mlx, game->image, 0, 0);
}*/

// Inicialización de las texturas del juego
void setup_textures(t_game *game) {
    t_texture textures[4];
    textures[0] = (t_texture){"img/bluestone.xpm42", NULL, NULL, 0, 0};
    textures[1] = (t_texture){"img/bluestone.xpm42", NULL, NULL, 0, 0};
    textures[2] = (t_texture){"img/eagle.xpm42", NULL, NULL, 0, 0};
    textures[3] = (t_texture){"img/eagle.xpm42", NULL, NULL, 0, 0};

    load_textures_in_array(game, textures); // Cargar las texturas en memoria

    int i = 0;
    while (i < 4) {
        game->wall_textures[i] = textures[i]; // Copiar al juego
        i++;
    }
    // Convertir texturas a matrices de píxeles
    //load_textures_to_arrays(game, &game->texture_arrays);
    //test_texture_render(game, &game->wall_textures[0]);
}

// Función para convertir un color RGB en un entero
int rgb_to_int(t_rgb color) {
    return (color.r << 24) | (color.g << 16) | (color.b << 8) | 255;
}