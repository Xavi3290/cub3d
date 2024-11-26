/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 12:08:52 by cgaratej          #+#    #+#             */
/*   Updated: 2024/11/26 13:12:00 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
# include "MLX42/MLX42.h"

typedef struct s_texture {
    char *path;
    mlx_image_t *texture_ptr;
    int *wall_texture;
    int width;
    int height;
} t_texture;

typedef struct s_tex
{
    char	*no;
    char	*so;
    char	*we;
    char	*ea;
}    t_tex;

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
    double view_offset;
    double jump_speed;
    int     is_jumping;
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
    t_tex        textures;
    int startX;
    int startY;
    int tileSize;
    t_rgb sky_color;
    t_rgb floor_color;
    t_rgb minimap_wall_color;
    t_rgb minimap_floor_color;
    t_rgb minimap_player_color;
    t_rgb color;
    uint32_t ***texture_arrays;
    t_texture wall_textures[4];
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