/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:02:25 by xavi              #+#    #+#             */
/*   Updated: 2024/11/27 12:21:17 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// Función para liberar las texturas de la memoria
void free_textures(t_game *game)
{
    int i = 0;

    while (i < 4)
	{
        if (game->wall_textures[i].texture_ptr)
		{
            mlx_delete_image(game->mlx, game->wall_textures[i].texture_ptr);
            game->wall_textures[i].texture_ptr = NULL;
        }
        i++;
    }
}

// Cargar texturas en un array de texturas
void load_textures_in_array(t_game *game, t_texture texture[4])
{
    int i = 0;

    while (i < 4)
	{
        xpm_t *temp_texture = mlx_load_xpm42(texture[i].path); // Cargar textura XPM
        if (!temp_texture) {
            printf("Error: No se pudo cargar la textura %s\n", texture[i].path);
            exit(1);
        }
        // Convertir textura a imagen y guardar puntero
        texture[i].texture_ptr = mlx_texture_to_image(game->mlx, &temp_texture->texture);
        if (!texture[i].texture_ptr)
		{
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

// Inicialización de las texturas del juego
void setup_textures(t_game *game)
{
	t_texture textures[4];

    textures[0] = (t_texture){game->textures.no, NULL, NULL, 0, 0};
    textures[1] = (t_texture){game->textures.so, NULL, NULL, 0, 0};
    textures[2] = (t_texture){game->textures.ea, NULL, NULL, 0, 0};
    textures[3] = (t_texture){game->textures.we, NULL, NULL, 0, 0};
    load_textures_in_array(game, textures); // Cargar las texturas en memoria
    int i = 0;
    while (i < 4)
	{
        game->wall_textures[i] = textures[i]; // Copiar al juego
        i++;
    }
}

// Función para convertir un color RGB en un entero
int rgb_to_int(t_rgb color)
{
    return (color.r << 24) | (color.g << 16) | (color.b << 8) | 255;
}
