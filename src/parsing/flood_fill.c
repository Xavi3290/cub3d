/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 13:23:42 by cgaratej          #+#    #+#             */
/*   Updated: 2024/11/21 13:23:36 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int is_surrounded(char **map_cpy, int x, int y, t_map *map)
{
	if (x <= 0 || y <= 0 || x >= map->height - 1 || y >= map->width - 1)
		return 0;
	return (map_cpy[x - 1][y] == '1' && map_cpy[x + 1][y] == '1' &&
			map_cpy[x][y - 1] == '1' && map_cpy[x][y + 1] == '1');
}

int	is_valid_char(char c)
{
	return (c == '0' || c == 'N' || c == 'S' || c == 'W' || c == 'E');
}

int	flood_fill_iterative(t_map *maps, char **map_tmp, int start_x, int start_y)
{
	t_vector2	stack[maps->width * maps->height];
	t_vector2	current;
	int			top;
	int			x;
	int			y;

	top = 0;
	stack[top++] = (t_vector2){start_x, start_y};
	while (top > 0)
	{
		current = stack[--top];
		x = current.x;
		y = current.y;
		if (map_tmp[x][y] == 'F' || map_tmp[x][y] == '1')
			continue;
		if (!is_valid_char(map_tmp[x][y]) && map_tmp[x][y] != ' ')
			return (printf("Error: Caracter inválido '%c'\n", map_tmp[x][y]), 0);
		if (map_tmp[x][y] == ' ' && !is_surrounded(map_tmp, x, y, maps))
			return (err_msg(maps->path, "no está rodeada de paredes", 0));
		map_tmp[x][y] = 'F';
		if (x > 0) stack[top++] = (t_vector2){x - 1, y};
		if (x < maps->height - 1) stack[top++] = (t_vector2){x + 1, y};
		if (y > 0) stack[top++] = (t_vector2){x, y - 1};
		if (y < maps->width - 1) stack[top++] = (t_vector2){x, y + 1};
	}
	return (1);
}
