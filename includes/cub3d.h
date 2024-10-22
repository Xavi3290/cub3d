/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xroca-pe <xroca-pe@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:10:27 by xroca-pe          #+#    #+#             */
/*   Updated: 2024/10/22 20:08:20 by xroca-pe         ###   ########.fr       */
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
    const int (*worldMap)[MAP_HEIGHT];
} t_game;

void perform_raycasting(mlx_image_t *image, t_player *player, const int worldMap[MAP_WIDTH][MAP_HEIGHT]);
#endif