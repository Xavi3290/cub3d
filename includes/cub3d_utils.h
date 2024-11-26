/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 12:08:52 by cgaratej          #+#    #+#             */
/*   Updated: 2024/11/26 12:09:32 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
	t_vector2	current;
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

typedef struct s_animation
{
	mlx_texture_t *frame0;
	mlx_texture_t *frame1;
	mlx_texture_t *frame2;
	mlx_texture_t *frame3;
	mlx_texture_t *frame4;
	mlx_texture_t *frame5;
	mlx_texture_t *frame6;
	mlx_texture_t *frame7;
	mlx_image_t *curren_img;
	int	index;
	int time;
}	t_animation;

typedef struct s_game {
    mlx_t *mlx;
    mlx_image_t *image;
    t_player player;
	t_animation anim;
    t_map        mapinfo;
    t_vector2    player_pos;
    t_tex        textures;
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