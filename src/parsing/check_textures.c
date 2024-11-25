/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:12:06 by cgaratej          #+#    #+#             */
/*   Updated: 2024/11/25 12:29:19 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	get_texture(char *linea, xpm_t *textures)
{
	char	*path;

	path = ft_strchr(linea, ' ');
	if (path)
	{
		path++;
		/*if (check_args(path, 0))
			return (1);*/
		textures = mlx_load_xpm42(path);
		if (!textures)
			return (1);
		(void)textures;
	}
	return (0);
}

int	check_texture(char *map_entry, const char *prefix, xpm_t *texture)
{
	char	*space;

	if (!ft_strncmp(map_entry, prefix, 2))
	{
		space = ft_strchr(map_entry, ' ');
		if (space)
		{
			*space = '\0';
			if (ft_strlen(map_entry) != 2)
				return (0);
		}
		else
			return (0);
		if (get_texture(map_entry, texture))
			return (0);
		return (1);
	}
	return (0);
}

int	check_textures(t_game *game)
{
	int	i;
	int	textures;

	i = 0;
	textures = 0;
	while (game->mapinfo.map_textures[i])
	{
		if (check_texture(game->mapinfo.map_textures[i], "NO", \
			game->textures.no) || check_texture(game->mapinfo.map_textures[i], \
			"SO", game->textures.so) || check_texture(\
			game->mapinfo.map_textures[i], "WE", game->textures.we) || \
			check_texture(game->mapinfo.map_textures[i], "EA", \
			game->textures.ea))
			textures++;
		i++;
	}
	if (textures != NUM_TEXTURES)
		return (err_msg("Textures", "Missing textures", 1));
	if (check_colors(game))
		return (1);
	return (0);
}
