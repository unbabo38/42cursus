#include "../../include/so_long.h"

int	main(int argc, char **argv)
{
	t_game	game;

	game = (t_game){0};
	if (argc != 2)
		error_exit("invalid argument\n");
	if (!has_ber_extension(argv[1]))
		error_exit("invalid filename\n");

	read_line_to_map(&game, argv[1]);
	check_shape(&game);
	check_wall(&game);
	check_elements(&game);
	if (!is_valid_map(&game))
		error_exit("invalid map\n");
	init_mlx(&game);
	free_all(&game);
	return (0);
}
