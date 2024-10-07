#pragma once
#include "types.h"
#include <vector>


class PipeSolver
{
	//��������� �������� �����
	GridParameters grid_parameters_;
	//���������� ����� ���������������� �����
	size_t nx_;
	//���������� ����� ��������� �����
	size_t nt_;
	//���������������� �����
	std::vector<SolveTypes::MediumPoint> grid_point_;

	//������ ���� �������������� ����� �����
	SolveTypes::Fragment left_medium;
	//������ ���� ����������
	SolveTypes::Fragment mixing_zone;
	//������ ���� ������������ ����
	SolveTypes::Fragment rarefied_gas_region;
	//������ ���� �������������� ����� ������
	SolveTypes::Fragment right_medium;

	//�������� ����� ����� ����������
	SolveTypes::Border head_front;
	//������ ����� ����� ����������
	SolveTypes::Border tail_front;
	//���������� ������
	SolveTypes::Border contact_gap;
	//������� �����
	SolveTypes::Border shock_wave;

	//����������� ��������
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