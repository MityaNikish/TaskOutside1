#include "reader.hpp"
#include <fstream>

void reader_pipe_parameters(const std::filesystem::path path_file, PipeParameters& data)
{
	std::ifstream fin(path_file);
	fin >> data.left.pressure;
	fin >> data.left.density;
	fin >> data.left.velocity;
	fin >> data.right.pressure;
	fin >> data.right.density;
	fin >> data.right.velocity;
}

void reader_grid_parameters(const std::filesystem::path path_file, GridParameters& data)
{
	std::ifstream fin(path_file);
	fin >> data.spatial_step;
	fin >> data.time_step;
	fin >> data.total_time;
	fin >> data.left_boundary;
	fin >> data.right_boundary;
}