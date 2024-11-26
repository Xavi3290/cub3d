/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 12:53:00 by cgaratej          #+#    #+#             */
/*   Updated: 2024/11/26 12:41:15 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	err_msg(char *detail, char *str, int code)
{
	ft_putstr_fd("cub3D: Error", 2);
	if (detail)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(detail, 2);
	}
	if (str)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(str, 2);
	}
	ft_putstr_fd("\n", 2);
	return (code);
}

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

long long	timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void free_game_resources(t_game *game)
{
    /*if (game->texture_arrays) {
        free_texture_arrays(game->texture_arrays, 4, game->wall_textures[0].height);
        game->texture_arrays = NULL;
    }*/
	if (game->mapinfo.map)
		free_tab(game->mapinfo.map);
	if (game->anim.curren_img)
		mlx_delete_image(game->mlx, game->anim.curren_img);
    free_textures(game);
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