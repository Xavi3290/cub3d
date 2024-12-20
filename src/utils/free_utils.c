/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:35:51 by cgaratej          #+#    #+#             */
/*   Updated: 2024/12/20 16:09:50 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	free_text(t_game *game)
{
	if (game->textures.no)
		free(game->textures.no);
	if (game->textures.so)
		free(game->textures.so);
	if (game->textures.we)
		free(game->textures.we);
	if (game->textures.ea)
		free(game->textures.ea);
}

void	free_game_resources(t_game *game)
{
	if (game->mapinfo.map)
		free_tab(game->mapinfo.map);
	if (game->anim.curren_img)
		mlx_delete_image(game->mlx, game->anim.curren_img);
	free_textures(game);
	free_text(game);
	if (game->image)
		mlx_delete_image(game->mlx, game->image);
	if (game->mlx)
		mlx_terminate(game->mlx);
}

void	close_window(void *param)
{
	(void)param;
	printf("Window closed\n");
	free_game_resources(param);
	exit(0);
}
