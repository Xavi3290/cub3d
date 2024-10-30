/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parce_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:37:26 by cgaratej          #+#    #+#             */
/*   Updated: 2024/10/30 11:10:52 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int get_line_count(char *path)
{
	int		fd;
	char	*line;
	int		line_count;

	line_count = 0;
	fd = open(path, O_RDONLY);
	if (fd < 0)
		err_msg(path, strerror(errno), errno);
	else
	{
		line = get_next_line(fd);
		while (line != NULL)
		{
			line_count++;
			free(line);
			line = get_next_line(fd);
		}
		close(fd);
	}
	return (line_count);
}
void fill_tab(int row, int column, int i, t_game *game)
{
	char	*line;

	line = get_next_line(game->mapinfo.fd);
	while (line != NULL)
	{
		game->mapinfo.map[row] = ft_calloc(ft_strlen(line), sizeof(char));
		if (!game->mapinfo.map[row])
		{
			err_msg(NULL, ERR_IN_MALLOC, 0);
			return (free_tab(game->mapinfo.map));
		}
		while (line[i] != '\0')
			game->mapinfo.map[row][column++] = line[i++];
		game->mapinfo.map[row++][column] = '\0';
		column = 0;
		i = 0;
		line = get_next_line(game->mapinfo.fd);
	}
	game->mapinfo.map[row] = NULL;
}

void parce_data(char *path, t_game *game)
{
	int		row;
	int		i;
	size_t	column;

	i = 0;
	row = 0;
	column = 0;
	game->mapinfo.line_count = get_line_count(path);
	game->mapinfo.path = path;
	game->mapinfo.map = ft_calloc(game->mapinfo.line_count + 1, sizeof(char *));
	if (!game->mapinfo.map)
	{
		err_msg(NULL, ERR_IN_MALLOC, 0);
		return ;
	}
	game->mapinfo.fd = open(path, O_RDONLY);
	if (game->mapinfo.fd < 0)
		err_msg(path, strerror(errno), 1);
	else
	{
		fill_tab(row, column, i, game);
		close(game->mapinfo.fd);
	}
	/*while (map.file[map.line_count])
	{
		if (map.file[map.line_count][0] == 'NO')
			get_resolution(&map, game);
		else if (map.file[map.line_count][0] == 'SO')
			get_texture(&map, game, 0);
		else if (map.file[map.line_count][0] == 'WE')
			get_texture(&map, game, 1);
		else if (map.file[map.line_count][0] == 'EA')
			get_texture(&map, game, 2);
	}*/
}
