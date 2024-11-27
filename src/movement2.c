/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:14:32 by xavi              #+#    #+#             */
/*   Updated: 2024/11/27 14:11:55 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// Función para manejar la rotación del jugador con las flechas
void	handle_rotation_player(t_game *game, int key)
{
	double	old_dir_x;
	double	old_plane_x;
	double	angle;

	game->need_redraw = 1;
	old_dir_x = game->player.dir_x;
	old_plane_x = game->player.plane_x;
	angle = 0.0;
	if (key == MLX_KEY_RIGHT)
		angle = ROTATE_PLAYER_SPEED;
	else if (key == MLX_KEY_LEFT)
		angle = -ROTATE_PLAYER_SPEED;
	game->player.dir_x = game->player.dir_x * cos(angle) - game->player.dir_y \
		* sin(angle);
	game->player.dir_y = old_dir_x * sin(angle) + game->player.dir_y \
		* cos(angle);
	game->player.plane_x = game->player.plane_x * cos(angle) \
		- game->player.plane_y * sin(angle);
	game->player.plane_y = old_plane_x * sin(angle) + game->player.plane_y \
		* cos(angle);
}

void	handle_jump(t_game *game)
{
	if (game->player.is_jumping)
	{
		game->player.view_offset += game->player.jump_speed;
		game->player.jump_speed -= 0.005;
		// Limitar la altura máxima del salto
		if (game->player.view_offset > 0.08) // Menor altura de salto
			game->player.jump_speed = -0.005; // Inicia la caída más lenta
		// Finalizar el salto al volver a la posición inicial
		if (game->player.view_offset <= 0)
		{
			game->player.view_offset = 0;
			game->player.jump_speed = 0;
			game->player.is_jumping = 0;
		}
	}
}

// Función de enganche para gestionar la rueda del ratón
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

	game = (t_game *)param;
	if (keydata.key == MLX_KEY_ESCAPE)
		close_window(param);
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
	printf("Player position1: (%f, %f)\n", game->player.pos_x, \
		game->player.pos_y);
	// Redibuja la escena después de la actualización de la posición o dirección
	if (game->need_redraw)
	{
		perform_raycasting(game);
		draw_minimap(game);
		draw_player_on_minimap(game);
		mlx_image_to_window(game->mlx, game->image, 0, 0);
		game->need_redraw = 0;
	}
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
