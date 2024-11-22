/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 10:54:15 by cgaratej          #+#    #+#             */
/*   Updated: 2024/11/21 13:25:33 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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
		if (map_tab[i][j] != '1' && map_tab[i][j] != ' ' && map_tab[i][j] != '\t')
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
		return (err_msg(NULL, ERR_IN_MAP, 1));
	i = 1;
	while (i < (map->height - 1))
	{
		j = 0;
		while (map_tab[i][j] == ' ' || map_tab[i][j] == '\t')
			j++;
		if (map_tab[i][j] != '1')
			return (err_msg(NULL, ERR_IN_MAP, 1));
		j = ft_strlen(map_tab[i]) - 1;
		while (j > 0 && (map_tab[i][j] == ' ' || map_tab[i][j] == '\t' \
				|| map_tab[i][j] == '\n'))
			j--;
		if (map_tab[i][j] != '1')
			return (err_msg(NULL, ERR_IN_MAP, 1));
		i++;
	}
	if (check_top_or_bottom(map_tab, map->height - 1))
		return (err_msg(NULL, ERR_IN_MAP, 1));
	return (0);
}

int check_player(char **map, t_game *game)
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
			if (map[i][j] == 'N' || map[i][j] == 'S' || map[i][j] == 'E' \
				|| map[i][j] == 'W')
			{
				player_count++;
				if (player_count > 1)
					return (err_msg(NULL, ERR_IN_PLAYER, 1));
				game->player_pos.x = i;
				game->player_pos.y = j;
			}
		}
	}
	return (0);
}

int is_blank_line(const char *line)
{
	int i = 0;

	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
			return (0);
		i++;
	}
	return (1);
}

int	check_blank_lines(char **map)
{
	int i = 0;

	while (map[i])
	{
		if (is_blank_line(map[i]))
			return (err_msg(NULL, ERR_IN_MAP, 1), 1);
		i++;
	}
	return (0);
}

int check_map(t_game *game)
{
	char	**map_tmp;

	if (!game->mapinfo.map[0])
		return (err_msg("Map", "Map empy", 1));
	if (check_blank_lines(game->mapinfo.map))
		return (1);
	if (check_map_sides(&game->mapinfo, game->mapinfo.map))
		return (1);
	if (check_player(game->mapinfo.map, game))
		return (1);
	map_tmp = copy_map(game->mapinfo.map);
	if (!flood_fill_iterative(&game->mapinfo, map_tmp, game->player_pos.x, \
		game->player_pos.y))
		return (free_tab(map_tmp), err_msg(NULL, ERR_IN_MAP, 1), 1);
	// Verificar si quedan celdas no visitadas
	/*for (int i = 0; i < game->mapinfo.height; i++)
	{
		int width = ft_strlen(game->mapinfo.map[i]);
		printf("%s", map_tmp[i]);
		for (int j = 0; j < width; j++)
		{
			if (map_tmp[i][j] != 'F' && map_tmp[i][j] != '1' && !ft_isspace(map_tmp[i][j]))
				return (free_tab(map_tmp), printf("Error: Mapa no está completamente conectado\n"), 1);
		}
	}*/
	return (free_tab(map_tmp), 0);
}
