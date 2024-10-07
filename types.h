#pragma once

//Параметры среды
struct MediumParameters
{
	//Плотность
	double density;
	//Давление
	double pressure;
	//Скорость
	double velocity;
};

//Параметры трубы
struct PipeParameters
{
	//Параметры среды слева от заслонки
	MediumParameters left;
	//Параметры среды справа от заслонки
	MediumParameters right;
};

//Параметры сетки
struct GridParameters
{
	//Шаг по пространству
	double spatial_step;
	//Шаг по времени
	double time_step;
	//Конечное время
	double total_time;
	//Левая граница пространственной сетки
	double left_boundary;
	//Правая граница пространственной сетки
	double right_boundary;
};

namespace SolveTypes
{
	//Точка среды
	struct MediumPoint
	{

		//Плотность
		double ro = 0;
		//Давление
		double p = 0;
		//Скорость
		double v = 0;
		//Пространственная координата
		double pos = 0;
	};

	//Фрагмент трубы
	struct Fragment
	{
		//Плотность
		double ro = 0;
		//Давление
		double p = 0;
		//Скорость
		double v = 0;
	};

	//Граница фрагментов трубы
	struct Border
	{
		//Скорость
		double u = 0;
		//Пространственная координата
		double pos = 0;
	};
}
