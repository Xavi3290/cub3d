/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:12:06 by cgaratej          #+#    #+#             */
/*   Updated: 2024/11/12 13:44:31 by cgaratej         ###   ########.fr       */
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
			{
				return (1);
			}
		}
		if (get_texture(map_entry, texture))
			return (1);
		return (1);
	}
	return (0);
}

int	check_color_values(char **rgb)
{
	int		i;
	char	*new_rgb0;
	char	*space;
	int		rgb_num;

	i = -1;
	rgb_num = 1;
	space = ft_strchr(rgb[0], ' ');
	if (space)
	{
		*space = '\0';
		if (ft_strlen(rgb[0]) != 1)
			return (free_tab(rgb), 0);
		new_rgb0 = strdup(space + 1);
		if (!new_rgb0)
			return (free_tab(rgb), 0);
		free(rgb[0]);
		rgb[0] = new_rgb0;
	}
	else
		return (free_tab(rgb), 0);
	while (rgb[++i])
	{
		if (ft_atoi(rgb[i]) > 255 || ft_atoi(rgb[i]) < 0)
			return (free_tab(rgb), 0);
		rgb_num++;
	}
	if (rgb_num == 3)
		return (free_tab(rgb), 0);
	return (free_tab(rgb), 1);
}

void	ft_process_rgb_color(char *tmp, t_game *game)
{
	if (!ft_strncmp(tmp, "F", 2))
		game->ff = ft_split(tmp, ',');
	else if (!ft_strncmp(tmp, "C", 2))
		game->cc = ft_split(tmp, ',');
	return ;
}

int	check_colors(t_game *game)
{
	int	i;
	int	colors;

	i = 0;
	colors = 0;
	game->cc = NULL;
	game->ff = NULL;
	while (game->mapinfo.map_textures[i])
	{
		if (!ft_strncmp(game->mapinfo.map_textures[i], "F", 1) || \
			!ft_strncmp(game->mapinfo.map_textures[i], "C", 1))
		{
			if (!check_color_values(\
				ft_split(game->mapinfo.map_textures[i], ',')))
				return (err_msg("Colors", "Invalid color values", 1), 1);
			ft_process_rgb_color(game->mapinfo.map_textures[i], game);
			colors++;
		}
		i++;
	}
	if (colors != 2)
		return (err_msg("Colors", "Missing colors", 1), 1);
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
		else
			break ;
		i++;
	}
	if (textures != NUM_TEXTURES)
		return (err_msg("Textures", "Missing textures", 1));
	if (check_colors(game))
		return (1);
	return (0);
}
