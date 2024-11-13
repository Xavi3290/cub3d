/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:12:06 by cgaratej          #+#    #+#             */
/*   Updated: 2024/11/13 13:36:12 by cgaratej         ###   ########.fr       */
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
int	check_nums(char *num)
{
	int	i;

	i = 0;
	while (num[i])
	{
		if (!ft_isspace(num[i]))
			if (!ft_isdigit(num[i]))
				return (1);
		i++;
	}
	return (0);
}
int	check_color_values(char **rgb, t_game *game)
{
	int		i;
	char	*new_rgb0;
	char	*space;
	int		rgb_num;
	int		flag;

	i = -1;
	rgb_num = 1;
	space = ft_strchr(rgb[0], ' ');
	flag = 1;
	if (space)
	{
		*space = '\0';
		if (rgb[0][0] == 'C')
			flag = 0;
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
		if (check_nums(rgb[i]) || (ft_atoi(rgb[i]) > 255 || ft_atoi(rgb[i]) < 0 || rgb[i][0] == '\n'))
			return (free_tab(rgb), 0);
		if (flag)
		{
			if (rgb_num == 1)
				game->floor_color.r = ft_atoi(rgb[i]);
			else if (rgb_num == 2)
				game->floor_color.g = ft_atoi(rgb[i]);
			else if (rgb_num == 3)
				game->floor_color.b = ft_atoi(rgb[i]);
		}
		else if (!flag)
		{
			if (rgb_num == 1)
				game->sky_color.r = ft_atoi(rgb[i]);
			else if (rgb_num == 2)
				game->sky_color.g = ft_atoi(rgb[i]);
			else if (rgb_num == 3)
				game->sky_color.b = ft_atoi(rgb[i]);
		}
		rgb_num++;
	}
	if (rgb_num == 3)
		return (free_tab(rgb), 0);
	return (free_tab(rgb),1);
}

int	check_colors(t_game *game)
{
	int	i;
	int	colors;

	i = 0;
	colors = 0;
	while (game->mapinfo.map_textures[i])
	{
		if (!ft_strncmp(game->mapinfo.map_textures[i], "F", 1) || \
			!ft_strncmp(game->mapinfo.map_textures[i], "C", 1))
		{
			if (!check_color_values(\
				ft_split(game->mapinfo.map_textures[i], ','), game))
				return (err_msg("Colors", "Invalid color values", 1), 1);
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
		i++;
	}
	if (textures != NUM_TEXTURES)
		return (err_msg("Textures", "Missing textures", 1));
	if (check_colors(game))
		return (1);
	return (0);
}
