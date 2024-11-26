/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xroca-pe <xroca-pe@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:10:27 by xroca-pe          #+#    #+#             */
/*   Updated: 2024/11/26 19:04:56 by xroca-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define WIDTH 700
# define HEIGHT 600
//# define MAP_WIDTH 8
//# define MAP_HEIGHT 8
# define MOVE_SPEED 0.04
# define ROTATE_VIEW_SPEED 0.03
# define ROTATE_PLAYER_SPEED 0.03
# define BPP sizeof(int32_t)

# include "../libft/libft.h"
# include "MLX42/MLX42.h"
# include <stdio.h>
# include <math.h>

/*typedef struct s_texture {
    mlx_image_t *img;
    int width;
    int height;
    int *data;
} t_texture;*/

typedef struct s_texture {
    char *path;
    mlx_image_t *texture_ptr;
    int *wall_texture;
    int width;
    int height;
} t_texture;


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

typedef struct s_game {
    mlx_t *mlx;
    mlx_image_t *image;
    t_player player;
    //int (*worldMap)[MAP_HEIGHT];
    char **map;
    int startX;
    int startY;
    int tileSize;
    int map_width;  
    int map_height;
    //uint32_t ***texture_arrays;
    t_rgb sky_color;
    t_rgb floor_color;
    //t_rgb wall_color_light;
    //t_rgb wall_color_dark;
    t_rgb minimap_wall_color;
    t_rgb minimap_floor_color;
    t_rgb minimap_player_color;
    t_rgb color;
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



void perform_raycasting(t_game *game);
int is_player_cell(t_game *game, int x, int y);
int is_wall(t_game *game, double x, double y);
//int is_wall(t_game *game, double x, double y, double margin);
int is_safe_position(t_game *game, double x, double y);
int rgb_to_int(t_rgb color);
//int get_texture_color(t_texture *texture, int texX, int texY);
void free_textures(t_game *game);
void setup_textures(t_game *game);
void draw_minimap(t_game *game);
void draw_player_on_minimap(t_game *game);
void handle_movement(t_game *game, int key);
void handle_movement_sides(t_game *game, int key);
void handle_diagonal_movement(t_game *game, int key);
void key_hook(struct mlx_key_data keydata, void *param);
void mouse_scroll_hook(double xdelta, double ydelta, void *param);
void game_loop(void *param);
void set_player_position(t_game *game);
void close_window(void* param);
char	**copy_map(char **map);
void	free_tab(char **tab);
void process_ray(t_ray *ray, t_game *game, t_line_params *line);


#endif