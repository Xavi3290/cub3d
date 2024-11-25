/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 11:26:23 by cgaratej          #+#    #+#             */
/*   Updated: 2024/11/25 12:39:24 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"
#include <fcntl.h>

static int	is_dir(char *arg)
{
	int	fd;

	fd = open(arg, O_DIRECTORY);
	if (fd != -1)
	{
		close(fd);
		return (1);
	}
	return (0);
}

static int	is_cub_file(char *arg)
{
	if (ft_strncmp(arg + ft_strlen(arg) - 4, ".cub", 4))
		return (1);
	return (0);
}

int	check_args(char *argv, int flag)
{
	int	fd;

	if (is_dir(argv))
		err_msg(argv, ERR_IS_DIR, 1);
	fd = open(argv, O_RDONLY);
	if (fd == -1)
		return (err_msg(argv, strerror(errno), 1));
	close(fd);
	if (flag && is_cub_file(argv))
		return (err_msg(argv, ERR_FILE_NOT_CUB, 1));
	return (0);
}
