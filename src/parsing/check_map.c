/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 10:54:15 by cgaratej          #+#    #+#             */
/*   Updated: 2024/11/06 12:48:00 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int check_sides(t_game *game, int rows)
{
	int	i;
	int	long_rows;

	i = 0;
	while (i < rows)
	{
		long_rows = ft_strlen(game->mapinfo.map[i]);
		if (long_rows < 1)
			return (1);
		if (!ft_isspace(game->mapinfo.map[i][0]) && \
			game->mapinfo.map[i][0] != '1')
			return (1);
		if (!ft_isspace(game->mapinfo.map[i][long_rows - 2]) && \
			game->mapinfo.map[i][long_rows - 2] != '1')
			return (1);
		i++;
	}
	return (0);
}

int	check_edges(t_game *game)
{
	size_t	i;
	int		rows;

	rows = ft_strlen_d(game->mapinfo.map);
	i = 0;
	while (i < ft_strlen(game->mapinfo.map[0]))
	{
		if (!ft_isspace(game->mapinfo.map[0][i]) && \
			game->mapinfo.map[0][i] != '1')
			return (err_msg(NULL, ERR_IN_MAP, 1), 1);
		i++;
	}
	i = 0;
	while (i < ft_strlen(game->mapinfo.map[rows - 1]))
	{
		if (!ft_isspace(game->mapinfo.map[rows - 1][i]) && \
			game->mapinfo.map[rows - 1][i] != '1')
			return (err_msg(NULL, ERR_IN_MAP, 1), 1);
		i++;
	}
	if (check_sides(game, rows))
		return (err_msg(NULL, ERR_IN_MAP, 1), 1);
	return (0);
}

int check_charactes(char **map, t_game *game)
{
	int	i;
	int	j;
	int	player_count;

	i = -1;
	player_count = 0;
	while (map[++i])
	{
		j = -1;
		while (map[i][++j])
		{
			if (map[i][j] != '0' && map[i][j] != '1' && map[i][j] != 'N' \
				&& map[i][j] != 'S' && map[i][j] != 'E' && map[i][j] != 'W' \
				&& !ft_isspace(map[i][j]))
				return (err_msg(NULL, ERR_IN_CHAR, 1), 1);
			if (map[i][j] == 'N' || map[i][j] == 'S' || map[i][j] == 'E' \
				|| map[i][j] == 'W')
			{
				player_count++;
				if (player_count > 1)
					return (err_msg(NULL, ERR_IN_PLAYER, 1));
				game->player_pos.x = j;
				game->player_pos.y = i;
			}
		}
	}
	return (0);
}

int check_map(t_game *game)
{
	if (check_edges(game))
		return (1);
	if (check_charactes(game->mapinfo.map, game))
		return (1);
	return (0);
}
