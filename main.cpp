#include "types.h"
#include "reader.hpp"
#include "pipe_solve.hpp"


int main()
{
	std::filesystem::path pipe_data_path = std::filesystem::current_path() / "medium.txt";
	std::filesystem::path grid_data_path = std::filesystem::current_path() / "grid.txt";

	PipeParameters pipe_data;
	GridParameters grid_data;

	reader_pipe_parameters(pipe_data_path, pipe_data);
	reader_grid_parameters(grid_data_path, grid_data);

	PipeSolver pipe_solver(grid_data, pipe_data);

	pipe_solver.solve(10);

	return 0;
}