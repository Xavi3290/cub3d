/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xroca-pe <xroca-pe@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:11:25 by xavi              #+#    #+#             */
/*   Updated: 2024/11/26 20:08:12 by xroca-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// Verifica si una posición específica en el mapa contiene una pared
int is_wall(t_game *game, double x, double y) {
    int mapX = (int)(x);
    int mapY = (int)(y);

    // Verifica si está dentro de los límites del mapa
    if (mapX < 0 || mapX >= game->map_width || mapY < 0 || mapY >= game->map_height)
        return 1;  // Trata posiciones fuera de los límites como paredes

    // Devuelve 1 si la celda contiene una pared ('1') o (' ')
    return (game->map[mapY][mapX] == '1' || game->map[mapY][mapX] == ' ');
}

int is_safe_position(t_game *game, double x, double y) {
    // Añade un margen pequeño para evitar que el jugador entre en la pared
    double margin = 0.2;

    // Verifica las cuatro esquinas alrededor del jugador
    if (is_wall(game, x - margin, y - margin))
        return 0;
    if (is_wall(game, x + margin, y - margin))
        return 0;
    if (is_wall(game, x - margin, y + margin))
        return 0;
    if (is_wall(game, x + margin, y + margin))
        return 0;

    return 1; // Si ninguna de las esquinas tiene pared, es seguro
}

// Maneja el movimiento con verificación de colisiones
void handle_movement(t_game *game, int key) {
    double nextX = game->player.posX;
    double nextY = game->player.posY;

    if (key == MLX_KEY_W) {
        nextX += game->player.dirX * MOVE_SPEED;
        nextY += game->player.dirY * MOVE_SPEED;
    } else if (key == MLX_KEY_S) {
        nextX -= game->player.dirX * MOVE_SPEED;
        nextY -= game->player.dirY * MOVE_SPEED;
    }
    game->need_redraw = 1;

    // Verifica colisiones con el mapa y limita el movimiento a áreas libres
    if (is_safe_position(game, nextX, game->player.posY)) {
        game->player.posX = nextX;
    }
    if (is_safe_position(game, game->player.posX, nextY)) {
        game->player.posY = nextY;
    }
}

// Función para manejar la rotación de la vista (sin mover al jugador)
void handle_movement_sides(t_game *game, int key)
{
    double nextX;
    double nextY;

    if (key == MLX_KEY_A)
    {
        nextX = game->player.posX + game->player.planeX * MOVE_SPEED;
        nextY = game->player.posY + game->player.planeY * MOVE_SPEED;
    }
    else if (key == MLX_KEY_D)
    {
        nextX = game->player.posX - game->player.planeX * MOVE_SPEED;
        nextY = game->player.posY - game->player.planeY * MOVE_SPEED;
    }
    else
        return;
    game->need_redraw = 1;

    // Verifica si la siguiente posición tiene una pared antes de mover
    if (is_safe_position(game, nextX, game->player.posY))
        game->player.posX = nextX;
    if (is_safe_position(game, game->player.posX, nextY))
        game->player.posY = nextY;
}

void handle_diagonal_movement(t_game *game, int key) {
    double nextX = game->player.posX;
    double nextY = game->player.posY;

    if (key == MLX_KEY_E) {
        // Mover en diagonal hacia adelante y la izquierda
        nextX += (game->player.dirX - game->player.planeX) * MOVE_SPEED / sqrt(2);
        nextY += (game->player.dirY - game->player.planeY) * MOVE_SPEED / sqrt(2);
    } else if (key == MLX_KEY_Q) {
        // Mover en diagonal hacia adelante y la derecha
        nextX += (game->player.dirX + game->player.planeX) * MOVE_SPEED / sqrt(2);
        nextY += (game->player.dirY + game->player.planeY) * MOVE_SPEED / sqrt(2);
    }

    game->need_redraw = 1;
    // Verificar colisiones
    if (is_safe_position(game, nextX, game->player.posY)) {
        game->player.posX = nextX;
    }
    if (is_safe_position(game, game->player.posX, nextY)) {
        game->player.posY = nextY;
    }
}