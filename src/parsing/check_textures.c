/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:12:06 by cgaratej          #+#    #+#             */
/*   Updated: 2024/12/20 16:04:37 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

char	*get_texture(char *linea)
{
	char	*texture;

	if (ft_isspace(linea[0]))
		return (NULL);
	else
	{
		if (check_args(linea, 0))
			return (NULL);
		texture = ft_strdup(linea);
		if (!texture)
			return (NULL);
	}
	return (texture);
}

int	validate_texture_path(char *map_entry, const char *prefix, char **path)
{
	char	*space;
	char	*tmp;

	if (!ft_strncmp(map_entry, prefix, 2))
	{
		space = ft_strchr(map_entry, ' ');
		if (space)
		{
			*path = space + 1;
			tmp = ft_strchr(*path, '\n');
			if (tmp)
				*tmp = '\0';
			*space = '\0';
			if (ft_strlen(map_entry) != 2)
				return (0);
			return (1);
		}
	}
	return (0);
}

int	check_texture(char *map_entry, const char *prefix, char **texture, int num)
{
	char	*path;

	if (validate_texture_path(map_entry, prefix, &path))
	{
		if (num != NUM_TEXTURES)
			*texture = get_texture(path);
		if (!*texture)
			exit(1);
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
			&game->textures.no, textures) || \
			check_texture(game->mapinfo.map_textures[i], "SO", \
			&game->textures.so, textures) || check_texture(\
			game->mapinfo.map_textures[i], "WE", &game->textures.we, textures) \
			|| check_texture(game->mapinfo.map_textures[i], "EA", \
			&game->textures.ea, textures))
			textures++;
		i++;
	}
	if (textures != NUM_TEXTURES)
		return (free_text(game), err_msg("Textures", ERR_IN_TEXTURE_NONE, 1));
	if (check_colors(game))
		return (free_text(game), 1);
	return (0);
}
