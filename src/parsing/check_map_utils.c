/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 12:41:55 by cgaratej          #+#    #+#             */
/*   Updated: 2024/12/10 13:01:22 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	is_valid_char(char c)
{
	return (c == '0' || c == 'N' || c == 'S' \
			|| c == 'W' || c == 'E' || c == '1' || c == 'D' || ft_isspace(c));
}

int	check_top_or_bottom(char **map_tab, int i)
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

int	check_cells(char **map, int i, int j)
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
