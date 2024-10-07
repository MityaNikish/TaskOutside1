#include "pipe_solve.hpp"

#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

#include <cmath>

using namespace SolveTypes;


namespace
{
	void write(std::filesystem::path path, std::vector<double> data)
	{
		std::ofstream fout_v(path, std::ios::trunc | std::ios::binary);
		fout_v.write((char*)data.data(), sizeof(double) * data.size());
		fout_v.close();
	}
}


PipeSolver::PipeSolver(const GridParameters& grid_parameters, const PipeParameters& pipe_parameters) : grid_parameters_(grid_parameters)
{
	//Данные первоначальной среды слева и справа
	left_medium.ro = pipe_parameters.left.density;
	left_medium.v = pipe_parameters.left.velocity;

	right_medium.ro = pipe_parameters.right.density;
	right_medium.v = pipe_parameters.right.velocity;

	if (pipe_parameters.left.pressure > pipe_parameters.right.pressure)
	{
		left_medium.p = pipe_parameters.left.pressure;
		right_medium.p = pipe_parameters.right.pressure;
	}
	else
	{
		left_medium.p = pipe_parameters.right.pressure;
		right_medium.p = pipe_parameters.left.pressure;
	}
	//

	const double p1 = left_medium.p;
	const double p2 = right_medium.p;
	const double ro1 = left_medium.ro;
	const double ro2 = right_medium.ro;

	const double p3 = halfCutMethod(0.001, 100000);
	const double ro3 = ro2 * (((lambda + 1) * p3 + (lambda - 1) * p2) / ((lambda - 1) * p3 + (lambda + 1) * p2));

	//Данные среды между Контактным разрывом и Ударной волной
	rarefied_gas_region.p = p3;
	rarefied_gas_region.ro = ro3;
	{
		const double fragment1 = 2 * pow(lambda * pow(p1, 1 / lambda) / ro1, 0.5) / (lambda - 1);
		const double special_power = (lambda - 1) / (2 * lambda);

		rarefied_gas_region.v = fragment1 * (pow(p1, special_power) - pow(p3, special_power));
	}
	//


	//Вычисление скорости контактного разрыва
	contact_gap.u = rarefied_gas_region.v;
	//Вычисление скорости ударной волны
	shock_wave.u = rarefied_gas_region.v * ro3 / (ro3 - ro2);

	//Данные промежуточной среды
	mixing_zone.ro = ro1 * pow(p3 / p1, 1 / lambda);
	mixing_zone.p = rarefied_gas_region.p;
	mixing_zone.v = rarefied_gas_region.v;
	//

	//Вычисление скорости заднего фронта волны разряжения
	tail_front.u = pow(lambda * p3 / mixing_zone.ro, 0.5) - rarefied_gas_region.v;
	//Вычисление скорости переднего фронта волны разряжения
	head_front.u = pow(lambda * p1 / ro1, 0.5);


	nx_ = static_cast<size_t>((grid_parameters_.right_boundary - grid_parameters_.left_boundary) / grid_parameters_.spatial_step) + 1;
	nt_ = static_cast<size_t>(grid_parameters_.total_time / grid_parameters_.time_step) + 1;

	grid_point_.resize(nx_);

	for (size_t i = 0; i < nx_; i++)
	{
		grid_point_[i].pos = grid_parameters_.left_boundary + i * grid_parameters_.spatial_step;
	}
}

void PipeSolver::solve(size_t save_period)
{
	for (size_t n = 0; n < nt_; ++n)
	{
		const double t = static_cast<double>(n) * grid_parameters_.time_step;

		head_front.pos = (-1.0) * t * head_front.u;
		shock_wave.pos = t * shock_wave.u;

		if (head_front.pos < grid_parameters_.left_boundary || grid_parameters_.right_boundary < shock_wave.pos)
		{
			return;
		}

		if (n % save_period == 0)
		{
			fillingGrid(t);
			writeGrid(n / save_period);
		}

	}
}

size_t PipeSolver::getQuantityNodesSpatial(void) const
{
	return nx_;
}

size_t PipeSolver::getQuantityNodesTime(void) const
{
	return nt_;
}

double PipeSolver::halfCutMethod(double epsilon, size_t quantity_iteration) const
{
	double a = left_medium.p;
	double b = right_medium.p;
	double c = (a + b) / 2;

	for (size_t i = 0; i < quantity_iteration; ++i)
	{
		double f_a = func(a);
		double f_b = func(b);

		c = (a + b) / 2;
		double f_c = func(c);

		a = f_a * f_c > 0 ? c : a;
		b = f_b * f_c > 0 ? c : b;

		if (abs(f_c) < epsilon)
		{
			break;
		}
	}
	return c;
}

double PipeSolver::func(double p3) const
{
	const double ro1 = left_medium.ro;
	const double p1 = left_medium.p;

	const double ro2 = right_medium.ro;
	const double p2 = right_medium.p;
	const double u2 = right_medium.v;

	double fragment1 = 2 * pow(lambda * pow(p1, 1 / lambda) / ro1, 0.5) / (lambda - 1);
	double special_power = (lambda - 1) / (2 * lambda);
	double u3 = fragment1 * (pow(p1, special_power) - pow(p3, special_power));

	return (p3 - p2) / ro2 * (1 - ((lambda - 1) * p3 + (lambda + 1) * p2) / ((lambda + 1) * p3 + (lambda - 1) * p2)) - pow(u3 - u2, 2);
}

void PipeSolver::fillingGrid(double t)
{
	head_front.pos = (-1.0) * t * head_front.u;
	tail_front.pos = (-1.0) * t * tail_front.u;
	contact_gap.pos = t * contact_gap.u;
	shock_wave.pos = t * shock_wave.u;

	const double c1 = pow(lambda * left_medium.p / left_medium.ro, 0.5);

	for (auto& point : grid_point_)
	{
		if (point.pos < head_front.pos)
		{
			point.p = left_medium.p;
			point.ro = left_medium.ro;
			point.v = left_medium.v;
		}
		else if (head_front.pos <= point.pos && point.pos < tail_front.pos)
		{
			const double c = 2 * (c1 - point.pos * (lambda - 1) / (2 * t)) / (lambda + 1);

			point.ro = pow(c * c * pow(left_medium.ro, lambda) / (lambda * left_medium.p), 1 / (lambda - 1));
			point.v = 2 * (c1 + point.pos / t) / (lambda + 1);
			point.p = c * c * point.ro / lambda;
		}
		else if (tail_front.pos <= point.pos && point.pos < contact_gap.pos)
		{
			point.p = mixing_zone.p;
			point.ro = mixing_zone.ro;
			point.v = mixing_zone.v;
		}
		else if (contact_gap.pos <= point.pos && point.pos < shock_wave.pos)
		{
			point.p = rarefied_gas_region.p;
			point.ro = rarefied_gas_region.ro;
			point.v = rarefied_gas_region.v;
		}
		else if (shock_wave.pos <= point.pos)
		{
			point.p = right_medium.p;
			point.ro = right_medium.ro;
			point.v = right_medium.v;
		}
	}
}

void PipeSolver::writeGrid(size_t index) const
{
	std::filesystem::path path = std::filesystem::current_path();

	std::vector<double> p(grid_point_.size(), 0);
	std::vector<double> ro(grid_point_.size(), 0);
	std::vector<double> v(grid_point_.size(), 0);

	for (size_t i = 0; i < grid_point_.size(); ++i)
	{
		p[i] = grid_point_[i].p;
		ro[i] = grid_point_[i].p;
		v[i] = grid_point_[i].p;
	}

	write(path / ("treatment\\p\\output_p_" + std::to_string(index) + ".raw"), p);
	write(path / ("treatment\\ro\\output_ro_" + std::to_string(index) + ".raw"), ro);
	write(path / ("treatment\\v\\output_v_" + std::to_string(index) + ".raw"), v);

}