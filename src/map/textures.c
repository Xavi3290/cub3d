/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:02:25 by xavi              #+#    #+#             */
/*   Updated: 2024/12/17 10:04:44 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	free_textures(t_game *game)
{
	int	i;

	i = 0;
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
/*void	load_textures_in_array(t_game *game, t_texture texture[4])
{
	int		i;
	xpm_t	*temp_texture;

	i = -1;
	while (++i < 4)
	{
		temp_texture = mlx_load_xpm42(texture[i].path);
		if (!temp_texture)
		{
			err_msg("Texturas", "No se pudo cargar la textura", 1);
			exit(1);
		}
		// Convertir textura a imagen y guardar puntero
		texture[i].texture_ptr = mlx_texture_to_image(game->mlx, \
			&temp_texture->texture);
		if (!texture[i].texture_ptr)
		{
			mlx_delete_xpm42(temp_texture);
			err_msg("Texturas", "No se pudo convertir la textura a imagen", 1);
			exit(1);
		}
		// Configurar dimensiones y datos de textura
		texture[i].width = temp_texture->texture.width;
		texture[i].height = temp_texture->texture.height;
		texture[i].wall_texture = (int *)texture[i].texture_ptr->pixels;
		mlx_delete_xpm42(temp_texture);
	}
}*/

static void	load_single_texture(t_game *game, t_texture *texture)
{
	xpm_t	*temp_texture;

	temp_texture = mlx_load_xpm42(texture->path);
	if (!temp_texture)
	{
		err_msg(texture->path, "Could not loading the texture", 1);
		exit(1);
	}
	texture->texture_ptr = mlx_texture_to_image(game->mlx, \
		&temp_texture->texture);
	if (!texture->texture_ptr)
	{
		mlx_delete_xpm42(temp_texture);
		err_msg("Textures", "Could not convert texture to image", 1);
		exit(1);
	}
	texture->width = temp_texture->texture.width;
	texture->height = temp_texture->texture.height;
	texture->wall_texture = (int *)texture->texture_ptr->pixels;
	mlx_delete_xpm42(temp_texture);
}

void	load_textures_in_array(t_game *game, t_texture texture[4])
{
	int	i;

	i = 0;
	while (i < 4)
	{
		load_single_texture(game, &texture[i]);
		i++;
	}
}

// Inicialización de las texturas del juego
void	setup_textures(t_game *game)
{
	t_texture	textures[4];
	t_texture	door_texture;
	int			i;

	textures[0] = (t_texture){game->textures.no, NULL, NULL, 0, 0};
	textures[1] = (t_texture){game->textures.so, NULL, NULL, 0, 0};
	textures[2] = (t_texture){game->textures.we, NULL, NULL, 0, 0};
	textures[3] = (t_texture){game->textures.ea, NULL, NULL, 0, 0};
	game->path_door_texture = ft_strdup("assets/img/door.xpm42");
	door_texture = (t_texture){game->path_door_texture, NULL, NULL, 0, 0};
	load_textures_in_array(game, textures); // Cargar las texturas en memoria
	load_single_texture(game, &door_texture);
	i = 0;
	while (i < 4)
	{
		game->wall_textures[i] = textures[i]; // Copiar al juego
		i++;
	}
	game->door_texture = door_texture;
	free(game->path_door_texture);
}

int	rgb_to_int(t_rgb color)
{
	return ((color.r << 24) | (color.g << 16) | (color.b << 8) | 255);
}