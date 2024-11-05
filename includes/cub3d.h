/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:10:27 by xroca-pe          #+#    #+#             */
/*   Updated: 2024/11/04 14:18:58 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# ifndef O_DIRECTORY
#  define O_DIRECTORY 00200000
# endif

# define WIDTH 800
# define HEIGHT 600
# define NUM_TEXTURES 4

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

enum e_texture_index
{
	NORTH = 0,
	SOUTH = 1,
	EAST = 2,
	WEST = 3
};

typedef struct s_tex
{
	mlx_texture_t		*no;
	mlx_texture_t		*so;
	mlx_texture_t		*we;
	mlx_texture_t		*ea;
}	t_tex;

typedef struct s_vectro2
{
	int	x;
	int y;
}	t_vector2;

typedef struct s_map
{
	int			fd;
    char        **map_textures;
	int			line_count;
	char		*path;
	int			height;
	int			width;
	int			index_end_of_map;
}	t_map;

typedef struct s_game
{
    mlx_t *mlx;
    mlx_image_t *image;
    /*t_player player;
    int (*worldMap)[MAP_HEIGHT];*/
    t_map mapinfo;
	char	**map;
	t_vector2	player_pos;
	t_tex textures;
	char		**cc;
	char		**ff;
}   t_game;

int		check_args(char *argv, int flag);
int		err_msg(char *detail, char *str, int code);
void	free_tab(char **tab);
void	parce_data(char *path, t_game *game);
int		check_textures(t_game *game);
int		check_map(t_game *game);
int		ft_strlen_d(char **str);

#endif