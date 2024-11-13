/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 13:23:42 by cgaratej          #+#    #+#             */
/*   Updated: 2024/11/13 11:00:32 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int is_surrounded_by_walls(char **map_cpy, int x, int y, t_map *map)
{
	if (x <= 0 || y <= 0 || x >= map->height - 1 || y >= map->width - 1)
		return 0;
	return (map_cpy[x - 1][y] == '1' && map_cpy[x + 1][y] == '1' &&
			map_cpy[x][y - 1] == '1' && map_cpy[x][y + 1] == '1');
}


int flood_fill_recursive(char **map_cpy, int x, int y, t_map *map)
{
	if (x < 0 || y < 0 || x >= map->height || y >= map->width || \
		map_cpy[x][y] == '1' || map_cpy[x][y] == 'F')
		return (1);
	if (map_cpy[x][y] == ' ')
	{
		if (!is_surrounded_by_walls(map_cpy, x, y, map))
		{
			return (0);
		}
	}
	map_cpy[x][y] = 'F';
	if (!flood_fill_recursive(map_cpy, x - 1, y, map))
		return (0);
	if (!flood_fill_recursive(map_cpy, x + 1, y, map))
		return (0);
	if (!flood_fill_recursive(map_cpy, x, y - 1, map))
		return (0);
	if (!flood_fill_recursive(map_cpy, x, y + 1, map))
		return (0);
	return (1);
}
