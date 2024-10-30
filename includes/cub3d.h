/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:10:27 by xroca-pe          #+#    #+#             */
/*   Updated: 2024/10/30 11:07:29 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# ifndef O_DIRECTORY
#  define O_DIRECTORY 00200000
# endif

# define WIDTH 800
# define HEIGHT 600

# include "../libft/libft.h"
# include "MLX42/MLX42.h"
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>

# define ERR_FILE_NOT_CUB "Error\nWrong file extension .cub\n"
# define ERR_FILE_NOT_XMP "Error\nWrong file extension .xmp\n"
# define ERR_IS_DIR "Error\nWrong is a directory\n"
# define ERR_IN_MALLOC "Error\nMalloc Error\n"

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


typedef struct s_map
{
	int			fd;
    char        **map;
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
}   t_game;

int		check_args(char *argv, int flag);
int		err_msg(char *detail, char *str, int code);
void	free_tab(void **tab);
void	parce_data(char *path, t_game *game);

#endif