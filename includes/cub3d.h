/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xavi <xavi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:10:27 by xroca-pe          #+#    #+#             */
/*   Updated: 2024/11/04 21:19:45 by xavi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define WIDTH 1000
# define HEIGHT 800
# define MAP_WIDTH 8
# define MAP_HEIGHT 8
# define MOVE_SPEED 0.05
# define ROTATE_VIEW_SPEED 0.03
# define ROTATE_PLAYER_SPEED 0.03

# include "../libft/libft.h"
# include "MLX42/MLX42.h"
# include <stdio.h>
# include <math.h>

typedef struct s_player {
    double posX;
    double posY;
    double dirX;
    double dirY;
    double planeX;
    double planeY;
} t_player;

typedef struct s_game {
    mlx_t *mlx;
    mlx_image_t *image;
    t_player player;
    int (*worldMap)[MAP_HEIGHT];
} t_game;

typedef struct s_ray {
    double cameraX;
    double rayDirX;
    double rayDirY;
    int mapX;
    int mapY;
    double sideDistX;
    double sideDistY;
    double deltaDistX;
    double deltaDistY;
    int stepX;
    int stepY;
    int side;
} t_ray;

typedef struct s_line_params {
    int x;
    int drawStart;
    int drawEnd;
    int color;
    int lineHeight;
    double perpWallDist;
} t_line_params;



void perform_raycasting(mlx_image_t *image, t_player *player, const int worldMap[MAP_WIDTH][MAP_HEIGHT]);
int is_player_cell(t_game *game, int x, int y);

#endif