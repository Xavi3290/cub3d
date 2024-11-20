/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xavi <xavi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:10:40 by xroca-pe          #+#    #+#             */
/*   Updated: 2024/11/20 13:40:07 by xavi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"


void	free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	if (tab)
	{
		free(tab);
		tab = NULL;
	}
}

int	ft_strlen_d(char **str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

char	**copy_map(char **map)
{
	char	**map_tmp;
	int		i;
	int		rows;

	i = 0;
	rows = ft_strlen_d(map);
	map_tmp = ft_calloc(rows + 1, sizeof(char *));
	if (!map_tmp)
		return (NULL);
	while (i < rows)
	{
		map_tmp[i] = ft_strdup(map[i]);
		if (!map_tmp[i])
			return (free_tab(map_tmp), NULL);
		i++;
	}
	return (map_tmp);
}

void free_textures(t_game *game) {
    int i = 0;

    while (i < 4) {
        if (game->wall_textures[i].texture_ptr) {
            mlx_delete_image(game->mlx, game->wall_textures[i].texture_ptr);
            game->wall_textures[i].texture_ptr = NULL;
        }
        i++;
    }
}

void free_texture_arrays(uint32_t ***texture_arrays, int num_textures, int height) {
    int i, j;

    for (i = 0; i < num_textures; i++) {
        for (j = 0; j < height; j++) {
            free(texture_arrays[i][j]);
        }
        free(texture_arrays[i]);
    }
    free(texture_arrays);
}


void load_ptr_textures_in_array(t_game *game, t_texture texture[4]) {
    int i = 0;

    while (i < 4) {
        mlx_texture_t *temp_texture = mlx_load_png(texture[i].path); // Cargar textura
        if (!temp_texture) {
            printf("Error: No se pudo cargar la textura %s\n", texture[i].path);
            exit(1);
        }

        // Convertir textura a imagen y guardar puntero
        texture[i].texture_ptr = mlx_texture_to_image(game->mlx, temp_texture);
        if (!texture[i].texture_ptr) {
            mlx_delete_texture(temp_texture);
            printf("Error: No se pudo convertir la textura %s a imagen\n", texture[i].path);
            exit(1);
        }

        // Configurar dimensiones y datos de textura
        texture[i].width = temp_texture->width;
        texture[i].height = temp_texture->height;
        texture[i].wall_texture = (int *)texture[i].texture_ptr->pixels; // Obtener píxeles

        mlx_delete_texture(temp_texture); // Liberar textura temporal después de convertirla
        i++;
    }
}

/*void convert_texture_to_array(t_texture *texture, uint32_t ***array) {
    int x, y;

    *array = malloc(texture->height * sizeof(uint32_t *));
    if (!*array) {
        printf("Error: No se pudo asignar memoria para la matriz de texturas.\n");
        exit(1);
    }

    for (y = 0; y < texture->height; y++) {
        (*array)[y] = malloc(texture->width * sizeof(uint32_t));
        if (!(*array)[y]) {
            printf("Error: No se pudo asignar memoria para la fila de textura.\n");
            exit(1);
        }

        for (x = 0; x < texture->width; x++) {
            uint8_t *pixels = (uint8_t *)texture->texture_ptr->pixels;
            int index = (y * texture->width + x) * 4;

            uint8_t r = pixels[index];
            uint8_t g = pixels[index + 1];
            uint8_t b = pixels[index + 2];
            uint8_t a = pixels[index + 3];

            (*array)[y][x] = (a << 24) | (r << 16) | (g << 8) | b; // Combinar RGBA
        }
    }
}

void load_textures_to_arrays(t_game *game, uint32_t ****texture_arrays) {
    int i;

    *texture_arrays = malloc(4 * sizeof(uint32_t **));
    if (!*texture_arrays) {
        printf("Error: No se pudo asignar memoria para las texturas.\n");
        exit(1);
    }

    for (i = 0; i < 4; i++) {
        convert_texture_to_array(&game->wall_textures[i], &(*texture_arrays)[i]);
    }
}*/




void setup_textures(t_game *game) {
    t_texture textures[4];
    textures[0] = (t_texture){"img/img_no.png", NULL, NULL, 0, 0};
    textures[1] = (t_texture){"img/img_so.png", NULL, NULL, 0, 0};
    textures[2] = (t_texture){"img/img_ea.png", NULL, NULL, 0, 0};
    textures[3] = (t_texture){"img/img_we.png", NULL, NULL, 0, 0};

    load_ptr_textures_in_array(game, textures); // Cargar las texturas en memoria

    int i = 0;
    while (i < 4) {
        game->wall_textures[i] = textures[i]; // Copiar al juego
        i++;
    }
    // Convertir texturas a matrices de píxeles
    //load_textures_to_arrays(game, &game->texture_arrays);
}


int rgb_to_int(t_rgb color) {
    return (color.r << 24) | (color.g << 16) | (color.b << 8) | 255;
}

// Dibuja una celda en el minimapa con el color especificado
void draw_minimap_cell(t_game *game) {
    int i = 0;
    int color_value = rgb_to_int(game->color);

    while (i < game->tileSize) {
        int j = 0;
        while (j < game->tileSize) {
            mlx_put_pixel(game->image, game->startX + i, game->startY + j, color_value);
            j++;
        }
        i++;
    }
}

// Dibuja el minimapa en la esquina superior izquierda de la pantalla
void draw_minimap(t_game *game) {
    int y = 0;
    while (y < game->map_height/*MAP_HEIGHT*/) {
        int x = 0;
        while (x < game->map_width/*MAP_WIDTH*/) {
            game->startX = x * game->tileSize;
            game->startY = y * game->tileSize;

            if (game->map[y][x] == '1') {
                game->color = game->minimap_wall_color; // Pared
            } else {
                game->color = game->minimap_floor_color; // Suelo
            }
            draw_minimap_cell(game);
            x++;
        }
        y++;
    }
}
/*void draw_minimap(t_game *game) {
    int y = 0;
    while (y < game->map_height) {
        int x = 0;
        while (x < game->map_width) {
            game->startX = x * game->tileSize;
            game->startY = y * game->tileSize;

            if (game->map[y][x] == '1') {
                game->color = game->minimap_wall_color; // Pared
            } else if (game->map[y][x] == ' ') {
                game->color = (t_rgb){0, 0, 0}; // Espacio vacío (negro)
            } else {
                game->color = game->minimap_floor_color; // Suelo
            }
            draw_minimap_cell(game);
            x++;
        }
        y++;
    }
}*/



// Dibuja la posición del jugador en el minimapa
void draw_player_on_minimap(t_game *game) {
    // Ajustar tamaño del jugador en el minimapa para evitar sobrepasar las paredes
    game->startX = (int)(game->player.posX * game->tileSize);
    game->startY = (int)(game->player.posY * game->tileSize);
    game->color = game->minimap_player_color;
    int player_tile_size = game->tileSize / 2; // Ajuste para el tamaño del jugador

    int i = 0;
    while (i < player_tile_size) {
        int j = 0;
        while (j < player_tile_size) {
            mlx_put_pixel(game->image, game->startX + i, game->startY + j, rgb_to_int(game->color));
            j++;
        }
        i++;
    }
}

// Verifica si una posición específica en el mapa contiene una pared
int is_wall(t_game *game, double x, double y) {
    int mapX = (int)(x);
    int mapY = (int)(y);

    // Verifica si está dentro de los límites del mapa
    if (mapX < 0 || mapX >= game->map_width/*MAP_WIDTH*/ || mapY < 0 || mapY >= game->map_height/*MAP_HEIGHT*/)
        return 1;  // Trata posiciones fuera de los límites como paredes

    // Devuelve 1 si la celda contiene una pared ('1') o (' ')
    return (game->map[mapY][mapX] == '1' || game->map[mapY][mapX] == ' ');
}

int is_safe_position(t_game *game, double x, double y) {
    // Añade un margen pequeño para evitar que el jugador entre en la pared
    double margin = 0.2;

    // Verifica las cuatro esquinas alrededor del jugador
    if (is_wall(game, x - margin, y - margin))
        return 0;
    if (is_wall(game, x + margin, y - margin))
        return 0;
    if (is_wall(game, x - margin, y + margin))
        return 0;
    if (is_wall(game, x + margin, y + margin))
        return 0;

    return 1; // Si ninguna de las esquinas tiene pared, es seguro
}

/*int is_wall(t_game *game, double x, double y, double margin) {
    int mapX = (int)x;
    int mapY = (int)y;

    // Verificar si está dentro de los límites del mapa
    if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT)
        return 1;

    // Verificar el margen alrededor del jugador
    if (game->map[mapY][mapX] == '1') {
        double dx = x - mapX;
        double dy = y - mapY;

        if (dx < margin || dx > (1.0 - margin) || dy < margin || dy > (1.0 - margin))
            return 1;
    }

    return 0;
}*/

// Función para liberar recursos del juego
void free_game_resources(t_game *game)
{
    /*if (game->texture_arrays) {
        free_texture_arrays(game->texture_arrays, 4, game->wall_textures[0].height);
        game->texture_arrays = NULL;
    }*/
    free_textures(game);
    if (game->map)
        free_tab(game->map);
    if (game->image)
        mlx_delete_image(game->mlx, game->image);
    if (game->mlx)
        mlx_terminate(game->mlx);
}

// Función para cerrar la ventana correctamente
void close_window(void* param)
{
    (void)param;
    printf("Window closed\n");
    free_game_resources(param);
    
    exit(0);
}

// Verifica si la celda actual es una posición válida del jugador
int is_player_cell(t_game *game, int x, int y)
{
    return (game->map[y][x] == 'N' || game->map[y][x] == 'S' ||
        game->map[y][x] == 'E' || game->map[y][x] == 'W');
}

// Establece la dirección y posición del jugador en el juego
void set_player_position_and_direction(t_game *game, int x, int y)
{
    game->player.posX = x + 0.5;
    game->player.posY = y + 0.5;

    if (game->map[y][x] == 'N')
    {
        game->player.dirX = 0;
        game->player.dirY = -1;
    }
    else if (game->map[y][x] == 'S')
    {
        game->player.dirX = 0;
        game->player.dirY = 1;
    }
    else if (game->map[y][x] == 'E')
    {
        game->player.dirX = 1;
        game->player.dirY = 0;
    }
    else if (game->map[y][x] == 'W')
    {
        game->player.dirX = -1;
        game->player.dirY = 0;
    }
}


// Busca la posición del jugador en el mapa y la inicializa
void set_player_position(t_game *game)
{
    int y = 0;
    while (y < game->map_height/*MAP_HEIGHT*/)
    {
        int x = 0;
        while (x < game->map_width/*MAP_WIDTH*/)
        {
            if (is_player_cell(game, x, y))
            {
                set_player_position_and_direction(game, x, y);
                game->player.planeX = 0.66 * game->player.dirY;
                game->player.planeY = -0.66 * game->player.dirX;
                return; // Termina la función al encontrar al jugador
            }
            x++;
        }
        y++;
    }
}

// Función para inicializar MLX
mlx_t *init_mlx()
{
    mlx_t *mlx;

    mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
    if (!mlx)
    {
        printf("Error: MLX could not initialize.\n");
        exit(1);
    }
    return mlx;
}


// Maneja el movimiento con verificación de colisiones
void handle_movement(t_game *game, int key) {
    double nextX = game->player.posX;
    double nextY = game->player.posY;

    if (key == MLX_KEY_W) {
        nextX += game->player.dirX * MOVE_SPEED;
        nextY += game->player.dirY * MOVE_SPEED;
    } else if (key == MLX_KEY_S) {
        nextX -= game->player.dirX * MOVE_SPEED;
        nextY -= game->player.dirY * MOVE_SPEED;
    }

    // Verifica colisiones con el mapa y limita el movimiento a áreas libres
    /*if (!is_wall(game, nextX, game->player.posY)) {
        game->player.posX = nextX;
    }
    if (!is_wall(game, game->player.posX, nextY)) {
        game->player.posY = nextY;
    }*/
    if (is_safe_position(game, nextX, game->player.posY)) {
        game->player.posX = nextX;
    }
    if (is_safe_position(game, game->player.posX, nextY)) {
        game->player.posY = nextY;
    }
}

// Función para manejar la rotación de la vista (sin mover al jugador)
void handle_movement_sides(t_game *game, int key)
{
    double nextX;
    double nextY;

    if (key == MLX_KEY_A)
    {
        nextX = game->player.posX + game->player.planeX * MOVE_SPEED;
        nextY = game->player.posY + game->player.planeY * MOVE_SPEED;
    }
    else if (key == MLX_KEY_D)
    {
        nextX = game->player.posX - game->player.planeX * MOVE_SPEED;
        nextY = game->player.posY - game->player.planeY * MOVE_SPEED;
    }
    else
        return;

    // Verifica si la siguiente posición tiene una pared antes de mover
    /*if (!is_wall(game, nextX, game->player.posY))
        game->player.posX = nextX;
    if (!is_wall(game, game->player.posX, nextY))
        game->player.posY = nextY;*/
    if (is_safe_position(game, nextX, game->player.posY))
        game->player.posX = nextX;
    if (is_safe_position(game, game->player.posX, nextY))
        game->player.posY = nextY;
}

// Función para manejar la rotación del jugador con las flechas
void handle_rotation_player(t_game *game, int key)
{
    double oldDirX;
    double oldPlaneX;
    double angle;

    oldDirX = game->player.dirX;
    oldPlaneX = game->player.planeX;
    angle = 0.0;
    if (key == MLX_KEY_RIGHT)
        angle = ROTATE_PLAYER_SPEED;
    else if (key == MLX_KEY_LEFT)
        angle = -ROTATE_PLAYER_SPEED;
    game->player.dirX = game->player.dirX * cos(angle) - game->player.dirY * sin(angle);
    game->player.dirY = oldDirX * sin(angle) + game->player.dirY * cos(angle);
    game->player.planeX = game->player.planeX * cos(angle) - game->player.planeY * sin(angle);
    game->player.planeY = oldPlaneX * sin(angle) + game->player.planeY * cos(angle);
}

void handle_jump(t_game *game) {
    if (game->player.is_jumping) {
        game->player.view_offset += game->player.jump_speed;
        game->player.jump_speed -= 0.005; // Gravedad más lenta para un salto más pequeño

        // Limitar la altura máxima del salto
        if (game->player.view_offset > 0.08) { // Menor altura de salto
            game->player.jump_speed = -0.005; // Inicia la caída más lenta
        }

        // Finalizar el salto al volver a la posición inicial
        if (game->player.view_offset <= 0) {
            game->player.view_offset = 0;
            game->player.jump_speed = 0;
            game->player.is_jumping = 0;
        }
    }
}

void handle_diagonal_movement(t_game *game, int key) {
    double nextX = game->player.posX;
    double nextY = game->player.posY;

    if (key == MLX_KEY_E) {
        // Mover en diagonal hacia adelante y la izquierda
        nextX += (game->player.dirX - game->player.planeX) * MOVE_SPEED / sqrt(2);
        nextY += (game->player.dirY - game->player.planeY) * MOVE_SPEED / sqrt(2);
    } else if (key == MLX_KEY_Q) {
        // Mover en diagonal hacia adelante y la derecha
        nextX += (game->player.dirX + game->player.planeX) * MOVE_SPEED / sqrt(2);
        nextY += (game->player.dirY + game->player.planeY) * MOVE_SPEED / sqrt(2);
    }

    // Verificar colisiones
    if (is_safe_position(game, nextX, game->player.posY)) {
        game->player.posX = nextX;
    }
    if (is_safe_position(game, game->player.posX, nextY)) {
        game->player.posY = nextY;
    }
}


// Función de enganche para gestionar la rueda del ratón
void mouse_scroll_hook(double xdelta, double ydelta, void *param)
{
    t_game *game;

    game = (t_game *)param;
    (void)xdelta;
    if (ydelta > 0) // La rueda se desplaza hacia arriba
        handle_rotation_player(game, MLX_KEY_LEFT);
    else if (ydelta < 0) // La rueda se desplaza hacia abajo
        handle_rotation_player(game, MLX_KEY_RIGHT);

    // Redibuja la escena después de la rotación
    perform_raycasting(game);
    draw_minimap(game);
    draw_player_on_minimap(game);
    mlx_image_to_window(game->mlx, game->image, 0, 0);
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
    else if (keydata.key == MLX_KEY_Q || keydata.key == MLX_KEY_E)
        handle_diagonal_movement(game, keydata.key);
    else if (keydata.key == MLX_KEY_LEFT || keydata.key == MLX_KEY_RIGHT)
        handle_rotation_player(game, keydata.key);
    else if (keydata.key == MLX_KEY_SPACE && !game->player.is_jumping) {
        game->player.is_jumping = 1;
        game->player.jump_speed = 0.03; // Velocidad inicial del salto
    }
    printf("Player position1: (%f, %f)\n", game->player.posX, game->player.posY);

    // Redibuja la escena después de la actualización de la posición o dirección
    perform_raycasting(game);
    draw_minimap(game);
    draw_player_on_minimap(game);
    mlx_image_to_window(game->mlx, game->image, 0, 0);
}

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
    //game->tileSize = (WIDTH / MAP_WIDTH) / 5;  
    game->sky_color = (t_rgb){135, 206, 235};        // Azul claro para el cielo
    game->floor_color = (t_rgb){139, 69, 19};        // Marrón para el suelo
    //game->wall_color_light = (t_rgb){255, 255, 255}; // Blanco para pared clara
    //game->wall_color_dark = (t_rgb){170, 170, 170};  // Gris para pared oscura
    game->minimap_wall_color = (t_rgb){85, 85, 85};  // Gris oscuro para paredes del minimapa
    game->minimap_floor_color = (t_rgb){204, 204, 204}; // Gris claro para el suelo del minimapa
    game->minimap_player_color = (t_rgb){0, 255, 0}; // Verde para el jugador en el minimapa
    //load_textures(game);
    game->tileSize = (WIDTH / game->map_width) / 5;
    set_player_position(game);
}

void game_loop(void *param) {
    t_game *game = (t_game *)param;

    if (game->player.is_jumping) {
        handle_jump(game);
    }

    perform_raycasting(game);
    draw_minimap(game);
    draw_player_on_minimap(game);
    mlx_image_to_window(game->mlx, game->image, 0, 0);
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
