/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:11:25 by xavi              #+#    #+#             */
/*   Updated: 2024/11/27 14:11:55 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// Verifica si una posición específica en el mapa contiene una pared
int	is_wall(t_game *game, double x, double y)
{
	int	map_x;
	int	map_y;

	map_x = (int)(x);
	map_y = (int)(y);
	// Verifica si está dentro de los límites del mapa
	if (map_x < 0 || map_x >= game->mapinfo.width \
		|| map_y < 0 || map_y >= game->mapinfo.height)
		return (1);// Trata posiciones fuera de los límites como paredes
	// Devuelve 1 si la celda contiene una pared ('1') o (' ')
	return (game->mapinfo.map[map_y][map_x] == '1' \
		|| game->mapinfo.map[map_y][map_x] == ' ');
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

// Maneja el movimiento con verificación de colisiones
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

// Función para manejar la rotación de la vista (sin mover al jugador)
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
