/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:10:40 by xroca-pe          #+#    #+#             */
/*   Updated: 2024/12/10 12:57:55 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

mlx_t	*init_mlx(void)
{
	mlx_t	*mlx;

	mlx = mlx_init(WIDTH, HEIGHT, "cub3D", false);
	if (!mlx)
	{
		err_msg("Error", "MLX could not initialize", 1);
		exit(1);
	}
	return (mlx);
}

void	normalize_map(t_game *game)
{
	int		max_length;
	int		i;
	int		length;
	char	*new_row;

	max_length = game->mapinfo.width;
	i = 0;
	while (game->mapinfo.map[i])
	{
		length = ft_strlen(game->mapinfo.map[i]);
		if (length < max_length)
		{
			new_row = ft_calloc(max_length + 1, sizeof(char));
			ft_memset(new_row, ' ', max_length);
			ft_memcpy(new_row, game->mapinfo.map[i], length);
			free(game->mapinfo.map[i]);
			game->mapinfo.map[i] = new_row;
		}
		i++;
	}
}

void	init_game(t_game *game)
{
	game->mapinfo.fd = 0;
	game->mapinfo.height = 0;
	game->mapinfo.width = 0;
	game->start_x = 0;
	game->start_y = 0;
	game->minimap_wall_color = (t_rgb){85, 85, 85};
	game->minimap_floor_color = (t_rgb){204, 204, 204};
	game->minimap_player_color = (t_rgb){0, 255, 0};
	game->mlx = init_mlx();
	game->image = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	if (!game->image)
	{
		mlx_terminate(game->mlx);
		err_msg("Error", "Failed to create image", 1);
		exit(1);
	}
}

int	parce_map(int argc, char **argv, t_game *game)
{
	if (argc > 2)
		return (err_msg("Args", ERR_MANY_ARGS, 1));
	if (argc < 2)
		return (err_msg("Args", ERR_IN_ARGS, 1));
	if (check_args(argv[1], 1))
		return (1);
	parce_data(argv[1], game);
	if (!game->mapinfo.map[0] && !game->mapinfo.map_textures[0])
		return (free_tab(game->mapinfo.map_textures), \
			free_tab(game->mapinfo.map), err_msg("File", "File Empy", 1));
	if (check_textures(game))
		return (free_tab(game->mapinfo.map_textures), \
				free_tab(game->mapinfo.map), 1);
	if (check_map(game))
		return (free_tab(game->mapinfo.map_textures), \
				free_tab(game->mapinfo.map), free_text(game), 1);
	free_tab(game->mapinfo.map_textures);
	return (0);
}

int	main(int argc, char **argv)
{
	t_game	game;

	init_game(&game);
	if (parce_map(argc, argv, &game))
		return (1);
	init_anim(&game);
	set_player_position(&game);
	game.tile_size = (WIDTH / game.mapinfo.width) / 5;
	setup_textures(&game);
	perform_raycasting(&game);
	normalize_map(&game);
	draw_minimap(&game);
	draw_player_on_minimap(&game);
	mlx_image_to_window(game.mlx, game.image, 0, 0);
	mlx_loop_hook(game.mlx, animation_loop, &game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_key_hook(game.mlx, key_hook, &game);
	mlx_scroll_hook(game.mlx, mouse_scroll_hook, &game);
	mlx_close_hook(game.mlx, close_window, &game);
	mlx_loop(game.mlx);
	return (0);
}
