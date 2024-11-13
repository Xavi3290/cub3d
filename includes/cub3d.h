/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:10:27 by xroca-pe          #+#    #+#             */
/*   Updated: 2024/11/13 09:07:21 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# ifndef O_DIRECTORY
#  define O_DIRECTORY 00200000
# endif

# define WIDTH 1000
# define HEIGHT 800
# define MAP_WIDTH 8
# define MAP_HEIGHT 8
# define MOVE_SPEED 0.04
# define ROTATE_VIEW_SPEED 0.03
# define ROTATE_PLAYER_SPEED 0.03
# define NUM_TEXTURES 4

# include "../libft/libft.h"
# include "MLX42/MLX42.h"
# include <stdio.h>
# include <math.h>
# include "../libft/libft.h"
# include "MLX42/MLX42.h"
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>

# define ERR_FILE_NOT_CUB "Wrong file extension .cub"
# define ERR_FILE_NOT_XMP "Wrong file extension .xmp"
# define ERR_IS_DIR "Wrong is a directory"
# define ERR_IN_MALLOC "Malloc Error"
# define ERR_IN_MAP "Invadio map not closed"
# define ERR_IN_CHAR "Invalid character in map"
# define ERR_IN_PLAYER "There must be exactly one player character"

typedef struct s_tex
{
    xpm_t        *no;
    xpm_t        *so;
    xpm_t        *we;
    xpm_t        *ea;
}    t_tex;

typedef struct s_vectro2
{
    int	x;
    int	y;
}    t_vector2;

typedef struct s_map
{
    int            fd;
    char        *path;
    char        **map_textures;
    char        **map;
    int            line_count;
    int            height;
    int            width;
}    t_map;

typedef struct s_player {
    double posX;
    double posY;
    double dirX;
    double dirY;
    double planeX;
    double planeY;
} t_player;

typedef struct s_rgb {
    int r;
    int g;
    int b;
} t_rgb;

typedef struct s_game {
    mlx_t *mlx;
    mlx_image_t *image;
    t_player player;
    //int (*worldMap)[MAP_HEIGHT];
    char **map;
    int startX;
    int startY;
    int tileSize;
    t_rgb sky_color;
    t_rgb floor_color;
    t_rgb wall_color_light;
    t_rgb wall_color_dark;
    t_rgb minimap_wall_color;
    t_rgb minimap_floor_color;
    t_rgb minimap_player_color;
    t_rgb color;
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



void perform_raycasting(t_game *game);
int is_player_cell(t_game *game, int x, int y);
int is_wall(t_game *game, double x, double y);
int rgb_to_int(t_rgb color);

#endif