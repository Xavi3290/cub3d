/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 13:01:41 by cgaratej          #+#    #+#             */
/*   Updated: 2024/12/13 11:21:00 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	get_rgba(int r, int g, int b, int a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

static int32_t	mlx_get_pixel_png(mlx_image_t *image, uint32_t x, uint32_t y)
{
	uint8_t	*pixelstart;

	if (x > image->width || y > image->height)
		return (0xFF000000);
	pixelstart = image->pixels + (y * image->width + x) * BPP;
	return (get_rgba(*(pixelstart), *(pixelstart + 1), \
					*(pixelstart + 2), *(pixelstart + 3)));
}

void	draw_animation_pixel(t_game *game, mlx_texture_t *texture)
{
	int	i;
	int	j;

	game->anim.curren_img = mlx_texture_to_image(game->mlx, texture);
	if (!game->anim.curren_img)
	{
		err_msg("Animation", "Failed to create temp image", 1);
		return ;
	}
	i = 0;
	while (i < 37)
	{
		j = 0;
		while (j < 37)
		{
			mlx_put_pixel(game->anim.curren_img, i, j, \
								mlx_get_pixel_png(game->anim.curren_img, i, j));
			j++;
		}
		i++;
	}
	mlx_image_to_window(game->mlx, game->anim.curren_img, WIDTH - 50, 20);
}
