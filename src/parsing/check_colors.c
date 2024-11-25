/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_colors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 12:27:43 by cgaratej          #+#    #+#             */
/*   Updated: 2024/11/25 12:36:25 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	check_nums(char *num)
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

static int	process_rgb_prefix(char **rgb, int *flag)
{
	char	*space;
	char	*new_rgb0;

	space = ft_strchr(rgb[0], ' ');
	if (!space)
		return (0);
	*space = '\0';
	if (rgb[0][0] == 'C')
		*flag = 0;
	else
		*flag = 1;
	if (ft_strlen(rgb[0]) != 1)
		return (0);
	new_rgb0 = strdup(space + 1);
	if (!new_rgb0)
		return (0);
	free(rgb[0]);
	rgb[0] = new_rgb0;
	return (1);
}

static void	assign_color_value(t_game *game, int rgb_num, int value, int flag)
{
	if (flag)
	{
		if (rgb_num == 1)
			game->floor_color.r = value;
		else if (rgb_num == 2)
			game->floor_color.g = value;
		else if (rgb_num == 3)
			game->floor_color.b = value;
	}
	else
	{
		if (rgb_num == 1)
			game->sky_color.r = value;
		else if (rgb_num == 2)
			game->sky_color.g = value;
		else if (rgb_num == 3)
			game->sky_color.b = value;
	}
}

static int	check_color_values(char **rgb, t_game *game)
{
	int		i;
	int		rgb_num;
	int		flag;

	rgb_num = 1;
	flag = 1;
	if (!process_rgb_prefix(rgb, &flag))
		return (free_tab(rgb), 0);
	i = 0;
	while (rgb[i])
	{
		if (check_nums(rgb[i]) || (ft_atoi(rgb[i]) > 255 \
			|| ft_atoi(rgb[i]) < 0 || rgb[i][0] == '\n'))
			return (free_tab(rgb), 0);
		assign_color_value(game, rgb_num, ft_atoi(rgb[i]), flag);
		rgb_num++;
		i++;
	}
	if (rgb_num != 4)
		return (free_tab(rgb), 0);
	return (free_tab(rgb), 1);
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
				return (err_msg("Colors", ERR_IN_COLOR, 1), 1);
			colors++;
		}
		i++;
	}
	if (colors != 2)
		return (err_msg("Colors", ERR_IN_COLOR_NONE, 1), 1);
	return (0);
}
