/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:10:40 by xroca-pe          #+#    #+#             */
/*   Updated: 2024/11/04 12:26:29 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void close_window(void* param)
{
	(void)param;
	printf("Window closed\n");
	exit(0);
}

int main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
		return (1);
	if (check_args(argv[1], 1))
		return (1);
	parce_data(argv[1], &game);
	if (check_textures(&game))
	{
		free_tab(game.mapinfo.map_textures);
		free_tab(game.map);
		return (1);
	}
	if (check_map(&game))
	{
		free_tab(game.mapinfo.map_textures);
		free_tab(game.map);
		return (1);
	}
	free_tab(game.mapinfo.map_textures);
	free_tab(game.map);
	/*mlx_t* mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
	if (!mlx)
	{
		printf("Error initializing MLX\n");
		return (1);
	}
	mlx_close_hook(mlx, &close_window, NULL);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (0);*/
}
