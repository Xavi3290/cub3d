/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 15:55:32 by cgaratej          #+#    #+#             */
/*   Updated: 2024/11/25 13:17:15 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	update_animation(t_game *game)
{
	static long long	last_time = 0;
	long long			current_time;

	current_time = timestamp();
	if ((current_time - last_time) > game->anim.time)
	{
		game->anim.index++;
		if (game->anim.index > 7)
			game->anim.index = 0;
		last_time = current_time;
	}
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
	t_game	*game;

	game = (t_game *)param;
	update_animation(game);
	draw_animation(game);
}
