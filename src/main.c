/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xavi <xavi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:10:40 by xroca-pe          #+#    #+#             */
/*   Updated: 2024/11/25 19:24:26 by xavi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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

void print_copied_map(char **map) {
    int i = 0;

    printf("Contenido de copy_map:\n");
    while (map[i]) {
        printf("Fila %d: '%s'\n", i, map[i]);
        i++;
    }
    printf("\n");
}*/

// Inicialización del juego y asignación de recursos
void init_game(t_game *game)
{
    
    char *initial_map[] = {
       /* "11111111",
        "10001001",
        "10001001",
        "10000001",
        "10010001",
        "11110111",
        "1E000001",
        "11111111",*/
        "   111111111",
        "   100000001",
        "   100011111",
        "11110001",
        "10000001",
        "10010001",
        "11110111",
        "1E000001",
        "11111111",
        NULL
    };
    game->map = copy_map(initial_map);
    if (!game->map)
    {
        printf("Error: Could not allocate memory for map.\n");
        exit(1);
    }
    //print_copied_map(game->map);
    int rows, cols;
    normalize_map(game->map, &rows, &cols);
    //print_map_to_console(game->map, rows, cols);

    game->map_width = cols;  // Actualizar ancho dinámicamente
    game->map_height = rows; // Actualizar altura dinámicamente
 
    
    game->mlx = init_mlx();
    game->image = mlx_new_image(game->mlx, WIDTH, HEIGHT);
    if (!game->image)
    {
        mlx_terminate(game->mlx);
        printf("Error: Image could not be created.\n");
        free_tab(game->map);
        exit(1);
    }
    game->startX = 0;   
    game->startY = 0;   
    game->sky_color = (t_rgb){135, 206, 235};        // Azul claro para el cielo
    game->floor_color = (t_rgb){139, 69, 19};        // Marrón para el suelo
    game->minimap_wall_color = (t_rgb){85, 85, 85};  // Gris oscuro para paredes del minimapa
    game->minimap_floor_color = (t_rgb){204, 204, 204}; // Gris claro para el suelo del minimapa
    game->minimap_player_color = (t_rgb){0, 255, 0}; // Verde para el jugador en el minimapa
    //load_textures(game);
    game->tileSize = (WIDTH / game->map_width) / 5;
    set_player_position(game);
}




int main(void)
{
    t_game game;

    init_game(&game);
    setup_textures(&game);
    perform_raycasting(&game);
    draw_minimap(&game);
    draw_player_on_minimap(&game);
    mlx_image_to_window(game.mlx, game.image, 0, 0);
    mlx_loop_hook(game.mlx, game_loop, &game);
    mlx_key_hook(game.mlx, key_hook, &game);
    mlx_scroll_hook(game.mlx, mouse_scroll_hook, &game);
    mlx_close_hook(game.mlx, close_window, &game);
    mlx_loop(game.mlx);

    return (0);
}
