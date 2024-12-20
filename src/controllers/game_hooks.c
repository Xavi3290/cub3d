/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_hooks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:23:44 by cgaratej          #+#    #+#             */
/*   Updated: 2024/12/20 16:41:28 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	is_wall(t_game *game, double x, double y)
{
	int	map_x;
	int	map_y;

	map_x = (int)(x);
	map_y = (int)(y);
	// Verifica si está dentro de los límites del mapa
	if (map_x < 0 || map_x >= game->mapinfo.width \
		|| map_y < 0 || map_y >= game->mapinfo.height)
		return (1);
	// Devuelve 1 si la celda contiene una pared ('1') o (' ')
	return (game->mapinfo.map[map_y][map_x] == '1' \
		|| game->mapinfo.map[map_y][map_x] == ' ' \
		|| game->mapinfo.map[map_y][map_x] == 'D');
}

int	is_safe_position(t_game *game, double x, double y)
{
	double	margin;

	margin = 0.2;
	// Verifica las cuatro esquinas alrededor del jugador
	if (is_wall(game, x - margin, y - margin))
		return (0);
	if (is_wall(game, x + margin, y - margin))
		return (0);
	if (is_wall(game, x - margin, y + margin))
		return (0);
	if (is_wall(game, x + margin, y + margin))
		return (0);
	return (1); // Si ninguna de las esquinas tiene pared, es seguro
}

void	mouse_scroll_hook(double xdelta, double ydelta, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	(void)xdelta;
	if (ydelta > 0) // La rueda se desplaza hacia arriba
		handle_rotation_player(game, MLX_KEY_LEFT);
	else if (ydelta < 0) // La rueda se desplaza hacia abajo
		handle_rotation_player(game, MLX_KEY_RIGHT);
	// Redibuja la escena después de la rotación
	if (game->need_redraw)
	{
		perform_raycasting(game);
		draw_minimap(game);
		draw_player_on_minimap(game);
		mlx_image_to_window(game->mlx, game->image, 0, 0);
		game->need_redraw = 0;
	}
}

// Función de enganche para gestionar teclas
void	key_hook(struct mlx_key_data keydata, void *param)
{
	t_game	*game;

	if (!keydata.action)
		return;
	game = (t_game *)param;
	if (keydata.key == MLX_KEY_ESCAPE)
		close_window(param);
	if (keydata.key == MLX_KEY_T)
	{
		if (game->is_interacting)
			game->is_interacting = 0;
		else
			game->is_interacting = 1;
		printf("asd %d\n", game->is_interacting);
	}
	if (keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_S)
		handle_movement(game, keydata.key);
	else if (keydata.key == MLX_KEY_A || keydata.key == MLX_KEY_D)
		handle_movement_sides(game, keydata.key);
	else if (keydata.key == MLX_KEY_Q || keydata.key == MLX_KEY_E)
		handle_diagonal_movement(game, keydata.key);
	else if (keydata.key == MLX_KEY_LEFT || keydata.key == MLX_KEY_RIGHT)
		handle_rotation_player(game, keydata.key);
	else if (keydata.key == MLX_KEY_SPACE && !game->player.is_jumping)
	{
		game->player.is_jumping = 1;
		game->player.jump_speed = 0.03; // Velocidad inicial del salto
	}
	printf("%d\n", game->is_interacting);
	/*printf("Player position1: (%f, %f)\n", game->player.pos_x, \
		game->player.pos_y);*/
	/*if (game->need_redraw)
	{
		perform_raycasting(game);
		draw_minimap(game);
		draw_player_on_minimap(game);
		mlx_image_to_window(game->mlx, game->image, 0, 0);
		game->need_redraw = 0;
	}*/
}

// Función de bucle principal del juegp para el salto y el raycasting
void	game_loop(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (game->player.is_jumping)
	{
		handle_jump(game);
		game->need_redraw = 1;
	}
	if (game->need_redraw)
	{
		perform_raycasting(game);
		draw_minimap(game);
		draw_player_on_minimap(game);
		mlx_image_to_window(game->mlx, game->image, 0, 0);
		game->need_redraw = 0;
	}
}
