/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 10:54:15 by cgaratej          #+#    #+#             */
/*   Updated: 2024/11/25 12:20:11 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	is_valid_char(char c)
{
	return (c == '0' || c == 'N' || c == 'S' \
			|| c == 'W' || c == 'E' || c == '1' || ft_isspace(c));
}

static int	check_top_or_bottom(char **map_tab, int i)
{
	int	j;

	if (!map_tab || !map_tab[i])
		return (1);
	j = 0;
	while (map_tab[i][j] && (map_tab[i][j] == ' ' || map_tab[i][j] == '\t'))
		j++;
	if (map_tab[i][j] != '1')
		return (1);
	while (map_tab[i][j] && map_tab[i][j] != '\n')
	{
		if (map_tab[i][j] != '1' && map_tab[i][j] != ' ' \
			&& map_tab[i][j] != '\t')
			return (1);
		j++;
	}
	while (j > 0 && (map_tab[i][j - 1] == ' ' || map_tab[i][j - 1] == '\t'))
		j--;
	return (map_tab[i][j - 1] != '1');
}

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
				game->player_pos.x = i;
				game->player_pos.y = j;
			}
		}
	}
	return (0);
}

int	check(char **map, int i, int j)
{
	if (j == 0 || j - 1 >= (int)ft_strlen(map[i]) \
		|| map[i][j - 1] == ' ')
		return (1);
	if (j + 1 >= (int)ft_strlen(map[i]) || map[i][j + 1] == '\0' \
		|| map[i][j + 1] == ' ')
		return (1);
	if (i == 0 || j >= (int)ft_strlen(map[i - 1]) \
		|| map[i - 1][j] == '\0' || map[i - 1][j] == ' ')
		return (1);
	if (!map[i + 1] || j >= (int)ft_strlen(map[i + 1]) \
		|| map[i + 1][j] == '\0' || map[i + 1][j] == ' ')
		return (1);
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
				if (check(map, i, j))
					return (1);
		}
	}
	return (0);
}

int	check_map(t_game *game)
{
	//char	**map_tmp;
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
	/*map_tmp = copy_map(game->mapinfo.map);
	if (!flood_fill_iterative(&game->mapinfo, map_tmp, game->player_pos.x, \
		game->player_pos.y))
		return (free_tab(map_tmp), err_msg(NULL, ERR_IN_MAP, 1), 1);*/
	return (/*free_tab(map_tmp),*/ 0);
}
