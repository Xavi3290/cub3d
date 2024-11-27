/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:10:27 by xroca-pe          #+#    #+#             */
/*   Updated: 2024/11/27 12:24:02 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CUB3D_H
# define CUB3D_H

# ifndef O_DIRECTORY
#  define O_DIRECTORY 00200000
# endif

# define WIDTH 1000
# define HEIGHT 800
//# define MAP_WIDTH 8
//# define MAP_HEIGHT 8
# define MOVE_SPEED 0.04
# define ROTATE_VIEW_SPEED 0.03
# define ROTATE_PLAYER_SPEED 0.03
# define NUM_TEXTURES 4
# define BPP sizeof(int32_t)

# include "../libft/libft.h"
# include "cub3d_utils.h"
# include "MLX42/MLX42.h"
# include <stdio.h>
# include <math.h>
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>
# include <sys/time.h>

# define ERR_FILE_NOT_CUB "Wrong file extension .cub"
# define ERR_FILE_NOT_XMP "Wrong file extension .xmp"
# define ERR_IS_DIR "Wrong is a directory"
# define ERR_IN_MALLOC "Malloc Error"
# define ERR_IN_COLOR "Invalid color values"
# define ERR_IN_COLOR_NONE "Missing colors"
# define ERR_IN_MAP "Map is not closed properly"
# define ERR_IN_SIDES "Map sides are invalid"
# define ERR_IN_SIZE "Map size is invalid"
# define ERR_IN_EMPY "Map is empty"
# define ERR_IN_CHAR "Invalid character in map"
# define ERR_IN_PLAYER "There must be exactly one player character"
# define ERR_IN_ARGS "Arguments are missing"
# define ERR_MANY_ARGS "Many arguments"
# define BPP sizeof(int32_t)

void		perform_raycasting(t_game *game);
int			is_player_cell(t_game *game, int x, int y);
int			is_wall(t_game *game, double x, double y);
int			rgb_to_int(t_rgb color);

int			check_args(char *argv, int flag);
int			check_colors(t_game *game);
int			check_textures(t_game *game);
int			check_map(t_game *game);
int			err_msg(char *detail, char *str, int code);
void		free_tab(char **tab);
void		parce_data(char *path, t_game *game);
int			ft_strlen_d(char **str);
char		**copy_map(char **map);

int			check_cells(char **map, int i, int j);
int			check_top_or_bottom(char **map_tab, int i);
int			is_valid_char(char c);

long long	timestamp(void);
void		animation_loop(void *param);
void		draw_animation(t_game *game);
void		init_anim(t_game *game);
void		draw_animation_pixel(t_game *game, mlx_texture_t *texture);

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