/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaratej <cgaratej@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:10:40 by xroca-pe          #+#    #+#             */
/*   Updated: 2024/11/26 13:03:04 by cgaratej         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"


/*// Imprime el contenido de worldMap para verificarlo
void print_world_map(const int worldMap[MAP_HEIGHT][MAP_WIDTH]) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            printf("%d ", worldMap[y][x]);
        }
        printf("\n");
    }
}*/

// Función para inicializar MLX
mlx_t *init_mlx()
{
    mlx_t *mlx;

    mlx = mlx_init(WIDTH, HEIGHT, "cub3D", false);
    if (!mlx)
    {
        printf("Error: MLX could not initialize.\n");
        exit(1);
    }
    return mlx;
}

// Función para copiar el mapa original a un nuevo array
void normalize_map(char **map, int *rows, int *cols) {
    int max_length = 0;
    int i = 0;

    // Determinar la longitud máxima de las filas
    while (map[i]) {
        int length = ft_strlen(map[i]);
        if (length > max_length) {
            max_length = length;
        }
        i++;
    }
    *rows = i;          // Número de filas
    *cols = max_length; // Longitud máxima de las columnas

    // Ajustar cada fila para que tenga la longitud máxima
    i = 0;
    while (map[i]) {
        int length = ft_strlen(map[i]);
        if (length < max_length) {
            char *new_row = ft_calloc(max_length + 1, sizeof(char));
            ft_memset(new_row, ' ', max_length); // Rellenar con espacios
            ft_memcpy(new_row, map[i], length); // Copiar contenido existente
            free(map[i]);
            map[i] = new_row;
        }
        i++;
    }
}

/*void print_map_to_console(char **map, int map_height, int map_width) {
    int y = 0;

    printf("Mapa (%d x %d):\n", map_height, map_width);
    while (y < map_height) {
        int x = 0;
        while (x < map_width) {
            printf("%c", map[y][x]);
            x++;
        }
        printf("\n");
        y++;
    }
    printf("\n");
}

// Función de enganche para gestionar teclas
void key_hook(struct mlx_key_data keydata, void *param)
{
    t_game *game;

    game = (t_game *)param;
    if (keydata.key == MLX_KEY_ESCAPE)
        close_window(param);
    if (keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_S)
        handle_movement(game, keydata.key);
    else if (keydata.key == MLX_KEY_A || keydata.key == MLX_KEY_D)
        handle_movement_sides(game, keydata.key);
    else if (keydata.key == MLX_KEY_LEFT || keydata.key == MLX_KEY_RIGHT)
        handle_rotation_player(game, keydata.key);
    printf("Player position1: (%f, %f)\n", game->player.posX, game->player.posY);
    // Redibuja la escena después de la actualización de la posición o dirección
    perform_raycasting(game);
    draw_minimap(game);
    draw_player_on_minimap(game);
    mlx_image_to_window(game->mlx, game->image, 0, 0);
}*/

// Inicialización del juego y asignación de recursos
void init_game(t_game *game)
{
	game->mapinfo.fd = 0;
	game->mapinfo.height = 0;
	game->mapinfo.width = 0;
	//game->player_pos.x = 0;
	//game->player_pos.y = 0;
	game->startX = 0;
	game->startY = 0;
	game->minimap_wall_color = (t_rgb){85, 85, 85};		// Gris oscuro para paredes del minimapa
	game->minimap_floor_color = (t_rgb){204, 204, 204};	// Gris claro para el suelo del minimapa
	game->minimap_player_color = (t_rgb){0, 255, 0};	// Verde para el jugador en el minimapa
    game->mlx = init_mlx();
    game->image = mlx_new_image(game->mlx, WIDTH, HEIGHT);
    if (!game->image)
    {
        mlx_terminate(game->mlx);
        printf("Error: Image could not be created.\n");
        exit(1);
    }
}

int	parce_map(int argc, char **argv, t_game *game)
{
	if (argc > 2)
		return (err_msg("Args", ERR_MANY_ARGS, 1));
	if (argc < 2)
		return (err_msg("Args", ERR_IN_ARGS, 1));
    if (check_args(argv[1], 1))
        return (1);
    parce_data(argv[1], game);
	if (!game->mapinfo.map[0] && !game->mapinfo.map_textures[0])
		return (free_tab(game->mapinfo.map_textures), \
			free_tab(game->mapinfo.map), err_msg("File", "File Empy", 1));
    if (check_textures(game))
        return (free_tab(game->mapinfo.map_textures), \
				free_tab(game->mapinfo.map), 1);
    if (check_map(game))
		return (free_tab(game->mapinfo.map_textures), \
				free_tab(game->mapinfo.map), 1);
	free_tab(game->mapinfo.map_textures);
	return (0);
}

int main(int argc, char **argv)
{
	t_game game;

	init_game(&game);
	if (parce_map(argc, argv, &game))
		return (1);
	init_anim(&game);
	set_player_position(&game);
	game.tileSize = (WIDTH / game.mapinfo.width) / 5;
	setup_textures(&game);
	perform_raycasting(&game);
	draw_minimap(&game);
	draw_player_on_minimap(&game);
	mlx_image_to_window(game.mlx, game.image, 0, 0);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_loop_hook(game.mlx, animation_loop, &game);
	mlx_key_hook(game.mlx, key_hook, &game);
	mlx_scroll_hook(game.mlx, mouse_scroll_hook, &game);
	mlx_close_hook(game.mlx, close_window, &game);
	mlx_loop(game.mlx);
	return (0);
}
