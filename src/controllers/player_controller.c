/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_controller.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:22:01 by cgaratej          #+#    #+#             */
/*   Updated: 2024/12/17 10:14:51 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

// Maneja la rotación del jugador
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

// Maneja el movimiento hacia adelante y atrás
void	handle_movement(t_game *game, int key)
{
	double	next_x;
	double	next_y;

	next_x = game->player.pos_x;
	next_y = game->player.pos_y;
	if (key == MLX_KEY_W)
	{
		next_x += game->player.dir_x * MOVE_SPEED;
		next_y += game->player.dir_y * MOVE_SPEED;
	}
	else if (key == MLX_KEY_S)
	{
		next_x -= game->player.dir_x * MOVE_SPEED;
		next_y -= game->player.dir_y * MOVE_SPEED;
	}
	game->need_redraw = 1;
	// Verifica colisiones con el mapa y limita el movimiento a áreas libres
	if (is_safe_position(game, next_x, game->player.pos_y))
		game->player.pos_x = next_x;
	if (is_safe_position(game, game->player.pos_x, next_y))
		game->player.pos_y = next_y;
}

// Maneja el movimiento lateral
void	handle_movement_sides(t_game *game, int key)
{
	double	next_x;
	double	next_y;

	if (key == MLX_KEY_A)
	{
		next_x = game->player.pos_x + game->player.plane_x * MOVE_SPEED;
		next_y = game->player.pos_y + game->player.plane_y * MOVE_SPEED;
	}
	else if (key == MLX_KEY_D)
	{
		next_x = game->player.pos_x - game->player.plane_x * MOVE_SPEED;
		next_y = game->player.pos_y - game->player.plane_y * MOVE_SPEED;
	}
	else
		return ;
	game->need_redraw = 1;
	// Verifica si la siguiente posición tiene una pared antes de mover
	if (is_safe_position(game, next_x, game->player.pos_y))
		game->player.pos_x = next_x;
	if (is_safe_position(game, game->player.pos_x, next_y))
		game->player.pos_y = next_y;
}

// Maneja el movimiento diagonal
void	handle_diagonal_movement(t_game *game, int key)
{
	double	next_x;
	double	next_y;

	next_x = game->player.pos_x;
	next_y = game->player.pos_y;
	if (key == MLX_KEY_E)
	{
		// Mover en diagonal hacia adelante y la izquierda
		next_x += (game->player.dir_x - game->player.plane_x) \
			* MOVE_SPEED / sqrt(2);
		next_y += (game->player.dir_y - game->player.plane_y) \
			* MOVE_SPEED / sqrt(2);
	}
	else if (key == MLX_KEY_Q)
	{
		// Mover en diagonal hacia adelante y la derecha
		next_x += (game->player.dir_x + game->player.plane_x) \
			* MOVE_SPEED / sqrt(2);
		next_y += (game->player.dir_y + game->player.plane_y) \
			* MOVE_SPEED / sqrt(2);
	}
	game->need_redraw = 1;
	// Verificar colisiones
	if (is_safe_position(game, next_x, game->player.pos_y))
		game->player.pos_x = next_x;
	if (is_safe_position(game, game->player.pos_x, next_y))
		game->player.pos_y = next_y;
}

// Maneja el salto del jugador
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
