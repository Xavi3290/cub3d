/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 10:54:15 by cgaratej          #+#    #+#             */
/*   Updated: 2024/11/26 13:01:36 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	check_map_sides(t_map *map, char **map_tab)
{
	int	i;
	int	j;

	if (check_top_or_bottom(map_tab, 0))
		return (1);
	i = 1;
	while (i < (map->height - 1))
	{
		j = 0;
		while (map_tab[i][j] == ' ' || map_tab[i][j] == '\t')
			j++;
		if (map_tab[i][j] != '1')
			return (1);
		j = ft_strlen(map_tab[i]) - 1;
		while (j > 0 && (map_tab[i][j] == ' ' || map_tab[i][j] == '\t' \
				|| map_tab[i][j] == '\n'))
			j--;
		if (map_tab[i][j] != '1')
			return (1);
		i++;
	}
	if (check_top_or_bottom(map_tab, map->height - 1))
		return (1);
	return (0);
}

int	check_player(char **map, t_game *game)
{
	int	i;
	int	j;
	int	player_count;
	(void)game;

	i = -1;
	player_count = 0;
	while (map[++i])
	{
		j = -1;
		while (map[i][++j])
		{
			if (!is_valid_char(map[i][j]))
				return (1);
			if (map[i][j] == 'N' || map[i][j] == 'S' || map[i][j] == 'E' \
				|| map[i][j] == 'W')
			{
				player_count++;
				if (player_count > 1)
					return (1);
				//game->player_pos.x = i;
				//game->player_pos.y = j;
			}
		}
	}
	return (0);
}

int	check_inside_map(t_game *game)
{
	char	**map;
	int		i;
	int		j;

	map = game->mapinfo.map;
	i = -1;
	while (map[++i])
	{
		j = -1;
		while (++j < game->mapinfo.width)
		{
			if (j >= (int)ft_strlen(map[i]) || map[i][j] == '\0')
				continue ;
			if (map[i][j] == '0')
				if (check_cells(map, i, j))
					return (1);
		}
	}
	return (0);
}

int	check_map(t_game *game)
{
	if (!game->mapinfo.map[0])
		return (err_msg(game->mapinfo.path, ERR_IN_EMPY, 1));
	if (game->mapinfo.width < 3 || game->mapinfo.height < 3)
		return (err_msg(game->mapinfo.path, ERR_IN_SIZE, 1));
	if (check_map_sides(&game->mapinfo, game->mapinfo.map))
		return (err_msg(game->mapinfo.path, ERR_IN_SIDES, 1));
	if (check_player(game->mapinfo.map, game))
		return (err_msg(game->mapinfo.path, ERR_IN_PLAYER, 1));
	if (check_inside_map(game))
		return (err_msg(game->mapinfo.path, ERR_IN_MAP, 1));
	return (0);
}
