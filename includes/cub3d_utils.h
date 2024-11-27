/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 12:08:52 by cgaratej          #+#    #+#             */
/*   Updated: 2024/11/27 14:21:33 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "MLX42/MLX42.h"

typedef struct s_texture
{
	char		*path;
	mlx_image_t	*texture_ptr;
	int			*wall_texture;
	int			width;
	int			height;
}	t_texture;

typedef struct s_tex
{
	char	*no;
	char	*so;
	char	*we;
	char	*ea;
}	t_tex;

typedef struct s_map
{
	char	*path;
	char	**map_textures;
	char	**map;
	int		fd;
	int		line_count;
	int		height;
	int		width;
}	t_map;

typedef struct s_player
{
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	double	view_offset;
	double	jump_speed;
	int		is_jumping;
}	t_player;

typedef struct s_rgb
{
	int	r;
	int	g;
	int	b;
}	t_rgb;

typedef struct s_animation
{
	mlx_texture_t	*frame0;
	mlx_texture_t	*frame1;
	mlx_texture_t	*frame2;
	mlx_texture_t	*frame3;
	mlx_texture_t	*frame4;
	mlx_texture_t	*frame5;
	mlx_texture_t	*frame6;
	mlx_texture_t	*frame7;
	mlx_image_t		*curren_img;
	int				index;
	int				time;
}	t_animation;

typedef struct s_game
{
	mlx_t		*mlx;
	int			start_x;
	int			start_y;
	int			tile_size;
	int			need_redraw;
	t_map		mapinfo;
	t_player	player;
	mlx_image_t	*image;
	t_animation	anim;
	t_texture	wall_textures[4];
	t_tex		textures;
	t_rgb		sky_color;
	t_rgb		floor_color;
	t_rgb		minimap_wall_color;
	t_rgb		minimap_floor_color;
	t_rgb		minimap_player_color;
	t_rgb		color;
}	t_game;

typedef struct s_ray
{
	double	camera_x;
	double	ray_dir_x;
	double	ray_dir_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	int		map_x;
	int		map_y;
	int		step_x;
	int		step_y;
	int		side;
}	t_ray;

typedef struct s_line_params
{
	int		x;
	int		draw_start;
	int		draw_end;
	int		color;
	int		line_height;
	double	perp_wall_dist;
}	t_line_params;