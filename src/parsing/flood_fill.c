/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 13:23:42 by cgaratej          #+#    #+#             */
/*   Updated: 2024/11/07 16:41:20 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

char	**copy_map(char **map)
{
	char	**map_tmp;
	int		i;
	int		rows;

	i = 0;
	rows = ft_strlen_d(map);
	map_tmp = malloc(sizeof(char *) * rows + 1);
	if (!map_tmp)
		return (NULL);
	while (i < rows)
	{
		map_tmp[i] = ft_strdup(map[i]);
		if (!map_tmp[i])
			return (free_tab(map_tmp), NULL);
		i++;
	}
	map_tmp[rows] = NULL;
	return (map_tmp);
}
