/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parce_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:37:26 by cgaratej          #+#    #+#             */
/*   Updated: 2024/11/11 14:23:30 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	get_line_count(char *path)
{
	int		fd;
	int		line_count;
	char	*line;

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

void	process_map_section(t_game *game, char *line)
{
	game->mapinfo.map[game->mapinfo.height] = ft_calloc(\
		ft_strlen(line) + 1, sizeof(char));
	if (!game->mapinfo.map[game->mapinfo.height])
	{
		err_msg(NULL, ERR_IN_MALLOC, 0);
		return (free_tab(game->mapinfo.map), \
			free_tab(game->mapinfo.map_textures));
	}
	ft_strcpy(game->mapinfo.map[game->mapinfo.height], line);
	if (ft_strlen(line) > (size_t)game->mapinfo.width)
		game->mapinfo.width = ft_strlen(line);
	game->mapinfo.height++;
}

void	process_texture_section(t_game *game, char *line, int *row_mapinfo)
{
	game->mapinfo.map_textures[*row_mapinfo] = ft_calloc(\
		ft_strlen(line) + 1, sizeof(char));
	if (!game->mapinfo.map_textures[*row_mapinfo])
	{
		err_msg(NULL, ERR_IN_MALLOC, 0);
		return (free_tab(game->mapinfo.map), \
			free_tab(game->mapinfo.map_textures));
	}
	ft_strcpy(game->mapinfo.map_textures[*row_mapinfo], line);
	(*row_mapinfo)++;
}

void	fill_tabs(t_game *game)
{
	char	*line;
	int		row_mapinfo;
	int		is_map_section;

	row_mapinfo = 0;
	game->mapinfo.height = 0;
	game->mapinfo.width = 0;
	is_map_section = 0;
	line = get_next_line(game->mapinfo.fd);
	while (line != NULL)
	{
		if (!is_map_section && (line[0] == ' ' || line[0] == '1'))
			is_map_section = 1;
		if (is_map_section)
			process_map_section(game, line);
		else
			process_texture_section(game, line, &row_mapinfo);
		free(line);
		line = get_next_line(game->mapinfo.fd);
	}
	game->mapinfo.map_textures[row_mapinfo] = NULL;
	game->mapinfo.map[game->mapinfo.height] = NULL;
	free(line);
}

void	parce_data(char *path, t_game *game)
{
	game->mapinfo.line_count = get_line_count(path);
	game->mapinfo.path = path;
	game->mapinfo.map_textures = ft_calloc(game->mapinfo.line_count \
		+ 1, sizeof(char *));
	game->mapinfo.map = ft_calloc(game->mapinfo.line_count + 1, sizeof(char *));
	if (!game->mapinfo.map_textures || !game->mapinfo.map)
	{
		err_msg(NULL, ERR_IN_MALLOC, 0);
		return ;
	}
	game->mapinfo.fd = open(path, O_RDONLY);
	if (game->mapinfo.fd < 0)
	{
		err_msg(path, strerror(errno), 1);
		return ;
	}
	else
	{
		fill_tabs(game);
		close(game->mapinfo.fd);
	}
}
