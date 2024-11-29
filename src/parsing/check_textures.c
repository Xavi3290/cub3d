/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:12:06 by cgaratej          #+#    #+#             */
/*   Updated: 2024/11/29 14:03:45 by cgaratej         ###   ########.fr       */
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

int	check_texture(char *map_entry, const char *prefix, char **texture)
{
	char	*space;
	char	*path;
	char	*tmp;

	if (!ft_strncmp(map_entry, prefix, 2))
	{
		space = ft_strchr(map_entry, ' ');
		if (space)
		{
			path = space + 1;
			tmp = ft_strchr(path, '\n');
			if (tmp)
				*tmp = '\0';
			*space = '\0';
			if (ft_strlen(map_entry) != 2)
				return (0);
		}
		else
			return (0);
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
			&game->textures.no) || check_texture(game->mapinfo.map_textures[i], \
			"SO", &game->textures.so) || check_texture(\
			game->mapinfo.map_textures[i], "WE", &game->textures.we) || \
			check_texture(game->mapinfo.map_textures[i], "EA", \
			&game->textures.ea))
			textures++;
		i++;
	}
	if (textures != NUM_TEXTURES)
		return (err_msg("Textures", ERR_IN_TEXTURE_NONE, 1));
	if (check_colors(game))
		return (1);
	return (0);
}
