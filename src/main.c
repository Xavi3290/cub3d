/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xroca-pe <xroca-pe@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:10:40 by xroca-pe          #+#    #+#             */
/*   Updated: 2024/10/21 20:24:18 by xroca-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"


void close_window(void* param)
{
    (void)param;
    printf("Window closed\n");
    exit(0);
}

int main(void)
{
    mlx_t* mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
    if (!mlx)
    {
        printf("Error initializing MLX\n");
        return (1);
    }
    mlx_close_hook(mlx, &close_window, NULL);
    mlx_loop(mlx);
    mlx_terminate(mlx);
    return (0);
}