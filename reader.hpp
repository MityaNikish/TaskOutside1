#pragma once
#include <filesystem>
#include "types.h"

void reader_pipe_parameters(const std::filesystem::path path_file, PipeParameters& data);

void reader_grid_parameters(const std::filesystem::path path_file, GridParameters& data);