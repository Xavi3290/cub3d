/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parce_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:37:26 by cgaratej          #+#    #+#             */
/*   Updated: 2024/11/04 12:24:39 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	get_line_count(char *path)
{
	int fd;
	char *line;
	int line_count = 0;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		err_msg(path, strerror(errno), errno);
	else {
		line = get_next_line(fd);
		while (line != NULL) {
			line_count++;
			free(line);
			line = get_next_line(fd);
		}
		close(fd);
	}
	return line_count;
}

void fill_tabs(t_game *game)
{
	char	*line;
	int		row_mapinfo;
	int		row_map;
	int		is_map_section;

	row_mapinfo = 0;
	row_map = 0;
	is_map_section = 0;
	line = get_next_line(game->mapinfo.fd);
	while (line != NULL) {
		if (!is_map_section && (line[0] == ' ' || line[0] == '1'))
			is_map_section = 1;
		if (is_map_section)
		{
			game->map[row_map] = ft_calloc(ft_strlen(line) + 1, sizeof(char));
			if (!game->map[row_map]) {
				err_msg(NULL, ERR_IN_MALLOC, 0);
				return (free_tab(game->map), free_tab(game->mapinfo.map_textures));
			}
			ft_strcpy(game->map[row_map], line);
			row_map++;
		}
		else
		{
			game->mapinfo.map_textures[row_mapinfo] = ft_calloc(ft_strlen(line) + 1, sizeof(char));
			if (!game->mapinfo.map_textures[row_mapinfo])
			{
				err_msg(NULL, ERR_IN_MALLOC, 0);
				return (free_tab(game->map), free_tab(game->mapinfo.map_textures));
			}
			ft_strcpy(game->mapinfo.map_textures[row_mapinfo], line);
			row_mapinfo++;
		}
		free(line);
		line = get_next_line(game->mapinfo.fd);
	}
	game->mapinfo.map_textures[row_mapinfo] = NULL;
	game->map[row_map] = NULL;
	free(line);
}

void parce_data(char *path, t_game *game)
{
	game->mapinfo.line_count = get_line_count(path);
	game->mapinfo.path = path;

	game->mapinfo.map_textures = ft_calloc(game->mapinfo.line_count + 1, sizeof(char *));
	game->map = ft_calloc(game->mapinfo.line_count + 1, sizeof(char *));
	if (!game->mapinfo.map_textures || !game->map)
	{
		err_msg(NULL, ERR_IN_MALLOC, 0);
		return;
	}
	game->mapinfo.fd = open(path, O_RDONLY);
	if (game->mapinfo.fd < 0)
		err_msg(path, strerror(errno), 1);
	else
	{
		fill_tabs(game);
		close(game->mapinfo.fd);
	}
}
