#pragma once
#include "types.h"
#include <vector>


class PipeSolver
{
	//Параметры заданной сетки
	GridParameters grid_parameters_;
	//Количество узлов пространственной сетки
	size_t nx_;
	//Количество узлов временной сетки
	size_t nt_;
	//Пространственная сетка
	std::vector<SolveTypes::MediumPoint> grid_point_;

	//Данные зоны первоначальной среды слева
	SolveTypes::Fragment left_medium;
	//Данные зоны смешивания
	SolveTypes::Fragment mixing_zone;
	//Данные зона разреженного газа
	SolveTypes::Fragment rarefied_gas_region;
	//Данные зоны первоначальной среды справа
	SolveTypes::Fragment right_medium;

	//Передний фронт волны разряжения
	SolveTypes::Border head_front;
	//Задний фронт волны разряжения
	SolveTypes::Border tail_front;
	//Контактный разрыв
	SolveTypes::Border contact_gap;
	//Ударная волна
	SolveTypes::Border shock_wave;

	//Коэффициент вязкости
	double lambda = 2;


public:
	PipeSolver(const GridParameters& grid_parameters, const PipeParameters& pipe_parameters);
	void solve(size_t save_period = 1);
	size_t getQuantityNodesSpatial(void) const;
	size_t getQuantityNodesTime(void) const;


private:

	double halfCutMethod(double epsilon, size_t quantity_iteration) const;
	double func(double p3) const;
	void fillingGrid(double t);
	void writeGrid(size_t index) const;
};