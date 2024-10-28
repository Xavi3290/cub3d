/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xavi <xavi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:10:27 by xroca-pe          #+#    #+#             */
/*   Updated: 2024/10/28 19:30:49 by xavi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define WIDTH 2400
# define HEIGHT 1600
# define MAP_WIDTH 8
# define MAP_HEIGHT 8

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


void perform_raycasting(mlx_image_t *image, t_player *player, const int worldMap[MAP_WIDTH][MAP_HEIGHT]);

#endif