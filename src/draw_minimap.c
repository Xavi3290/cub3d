/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xavi <xavi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:06:40 by xavi              #+#    #+#             */
/*   Updated: 2024/11/25 19:07:36 by xavi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// Dibuja una celda en el minimapa con el color especificado
void draw_minimap_cell(t_game *game) {
    int i = 0;
    int color_value = rgb_to_int(game->color);

    while (i < game->tileSize) {
        int j = 0;
        while (j < game->tileSize) {
            mlx_put_pixel(game->image, game->startX + i, game->startY + j, color_value);
            j++;
        }
        i++;
    }
}

// Dibuja el minimapa en la esquina superior izquierda de la pantalla
void draw_minimap(t_game *game) {
    int y = 0;
    while (y < game->map_height/*MAP_HEIGHT*/) {
        int x = 0;
        while (x < game->map_width/*MAP_WIDTH*/) {
            game->startX = x * game->tileSize;
            game->startY = y * game->tileSize;

            if (game->map[y][x] == '1') {
                game->color = game->minimap_wall_color; // Pared
            } else {
                game->color = game->minimap_floor_color; // Suelo
            }
            draw_minimap_cell(game);
            x++;
        }
        y++;
    }
}


// Dibuja la posición del jugador en el minimapa
void draw_player_on_minimap(t_game *game) {
    // Ajustar tamaño del jugador en el minimapa para evitar sobrepasar las paredes
    game->startX = (int)(game->player.posX * game->tileSize);
    game->startY = (int)(game->player.posY * game->tileSize);
    game->color = game->minimap_player_color;
    int player_tile_size = game->tileSize / 2; // Ajuste para el tamaño del jugador

    int i = 0;
    while (i < player_tile_size) {
        int j = 0;
        while (j < player_tile_size) {
            mlx_put_pixel(game->image, game->startX + i, game->startY + j, rgb_to_int(game->color));
            j++;
        }
        i++;
    }
}