/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 15:55:32 by cgaratej          #+#    #+#             */
/*   Updated: 2024/11/18 18:16:35 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int get_rgba(int r, int g, int b, int a) {
  return (r << 24 | g << 16 | b << 8 | a);
}

int32_t mlx_get_pixel(mlx_image_t* image, uint32_t x, uint32_t y) {
  if (x > image->width || y > image->height)
    return 0xFF000000;
  uint8_t* pixelstart = image->pixels + (y * image->width + x) * BPP;
  return get_rgba(*(pixelstart), *(pixelstart + 1),
    * (pixelstart + 2), *(pixelstart + 3));
}

void	update_animation(t_game *game)
{
	static long long	last_time = 0;
	long long			current_time;

	current_time = timestamp(); // Obtiene el tiempo actual en ms
	if ((current_time - last_time) > game->anim.time) // Verifica si ha pasado 1 segundo
	{
		game->anim.index++; // Avanza al siguiente cuadro
		if (game->anim.index > 7)
			game->anim.index = 0; // Reinicia al primer cuadro
		last_time = current_time; // Actualiza el tiempo del último cambio
	}
}

void draw_animation_pixel(t_game *game, mlx_texture_t *texture)
{
    int i;
	int j;

	game->anim.curren_img = mlx_texture_to_image(game->mlx, texture);
	if (!game->anim.curren_img)
	{
		err_msg(NULL, "Failed to create temp image", 1);
		return ;
	}
	i = 0;
    while (i < 37)
	{
        j = 0;
        while (j < 37)
		{
            mlx_put_pixel(game->anim.curren_img, i, j, mlx_get_pixel(game->anim.curren_img, i, j));
            j++;
        }
        i++;
    }
	mlx_image_to_window(game->mlx, game->anim.curren_img, 940, 20);
}

void	draw_animation(t_game *game)
{
	if (game->anim.curren_img)
		mlx_delete_image(game->mlx, game->anim.curren_img);
	if (game->anim.index == 0)
		draw_animation_pixel(game, game->anim.frame0);
	else if (game->anim.index == 1)
		draw_animation_pixel(game, game->anim.frame1);
	else if (game->anim.index == 2)
		draw_animation_pixel(game, game->anim.frame2);
	else if (game->anim.index == 3)
		draw_animation_pixel(game, game->anim.frame3);
	else if (game->anim.index == 4)
		draw_animation_pixel(game, game->anim.frame4);
	else if (game->anim.index == 5)
		draw_animation_pixel(game, game->anim.frame5);
	else if (game->anim.index == 6)
		draw_animation_pixel(game, game->anim.frame6);
	else if (game->anim.index == 7)
		draw_animation_pixel(game, game->anim.frame7);
}

void	init_anim(t_game *game)
{
	game->anim.frame0 = mlx_load_png("./assets/animation/frame1.png");
	game->anim.frame1 = mlx_load_png("./assets/animation/frame2.png");
	game->anim.frame2 = mlx_load_png("./assets/animation/frame3.png");
	game->anim.frame3 = mlx_load_png("./assets/animation/frame4.png");
	game->anim.frame4 = mlx_load_png("./assets/animation/frame5.png");
	game->anim.frame5 = mlx_load_png("./assets/animation/frame6.png");
	game->anim.frame6 = mlx_load_png("./assets/animation/frame7.png");
	game->anim.frame7 = mlx_load_png("./assets/animation/frame8.png");
	game->anim.index = 0;
	game->anim.time = 100;
}

void	animation_loop(void *param)
{
	t_game *game;

	game = (t_game *)param;
	update_animation(game);
	draw_animation(game);
}
