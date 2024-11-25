/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xavi <xavi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:22:50 by xavi              #+#    #+#             */
/*   Updated: 2024/11/25 19:23:18 by xavi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"



void	free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	if (tab)
	{
		free(tab);
		tab = NULL;
	}
}

int	ft_strlen_d(char **str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

char	**copy_map(char **map)
{
	char	**map_tmp;
	int		i;
	int		rows;

	i = 0;
	rows = ft_strlen_d(map);
	map_tmp = ft_calloc(rows + 1, sizeof(char *));
	if (!map_tmp)
		return (NULL);
	while (i < rows)
	{
		map_tmp[i] = ft_strdup(map[i]);
		if (!map_tmp[i])
			return (free_tab(map_tmp), NULL);
		i++;
	}
	return (map_tmp);
}

// Función para liberar recursos del juego
void free_game_resources(t_game *game)
{
    /*if (game->texture_arrays) {
        free_texture_arrays(game->texture_arrays, 4, game->wall_textures[0].height);
        game->texture_arrays = NULL;
    }*/
    free_textures(game);
    if (game->map)
        free_tab(game->map);
    if (game->image)
        mlx_delete_image(game->mlx, game->image);
    if (game->mlx)
        mlx_terminate(game->mlx);
}

// Función para cerrar la ventana correctamente
void close_window(void* param)
{
    (void)param;
    printf("Window closed\n");
    free_game_resources(param);
    
    exit(0);
}