/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   position.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xavi <xavi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:16:43 by xavi              #+#    #+#             */
/*   Updated: 2024/11/25 19:21:29 by xavi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// Verifica si la celda actual es una posición válida del jugador
int is_player_cell(t_game *game, int x, int y)
{
    return (game->map[y][x] == 'N' || game->map[y][x] == 'S' ||
        game->map[y][x] == 'E' || game->map[y][x] == 'W');
}

// Establece la dirección y posición del jugador en el juego
void set_player_position_and_direction(t_game *game, int x, int y)
{
    game->player.posX = x + 0.5;
    game->player.posY = y + 0.5;

    if (game->map[y][x] == 'N')
    {
        game->player.dirX = 0;
        game->player.dirY = -1;
    }
    else if (game->map[y][x] == 'S')
    {
        game->player.dirX = 0;
        game->player.dirY = 1;
    }
    else if (game->map[y][x] == 'E')
    {
        game->player.dirX = 1;
        game->player.dirY = 0;
    }
    else if (game->map[y][x] == 'W')
    {
        game->player.dirX = -1;
        game->player.dirY = 0;
    }
}


// Busca la posición del jugador en el mapa y la inicializa
void set_player_position(t_game *game)
{
    int y = 0;
    while (y < game->map_height/*MAP_HEIGHT*/)
    {
        int x = 0;
        while (x < game->map_width/*MAP_WIDTH*/)
        {
            if (is_player_cell(game, x, y))
            {
                set_player_position_and_direction(game, x, y);
                game->player.planeX = 0.66 * game->player.dirY;
                game->player.planeY = -0.66 * game->player.dirX;
                return; // Termina la función al encontrar al jugador
            }
            x++;
        }
        y++;
    }
}