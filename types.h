#pragma once

//��������� �����
struct MediumParameters
{
	//���������
	double density;
	//��������
	double pressure;
	//��������
	double velocity;
};

//��������� �����
struct PipeParameters
{
	//��������� ����� ����� �� ��������
	MediumParameters left;
	//��������� ����� ������ �� ��������
	MediumParameters right;
};

//��������� �����
struct GridParameters
{
	//��� �� ������������
	double spatial_step;
	//��� �� �������
	double time_step;
	//�������� �����
	double total_time;
	//����� ������� ���������������� �����
	double left_boundary;
	//������ ������� ���������������� �����
	double right_boundary;
};

namespace SolveTypes
{
	//����� �����
	struct MediumPoint
	{

		//���������
		double ro = 0;
		//��������
		double p = 0;
		//��������
		double v = 0;
		//���������������� ����������
		double pos = 0;
	};

	//�������� �����
	struct Fragment
	{
		//���������
		double ro = 0;
		//��������
		double p = 0;
		//��������
		double v = 0;
	};

	//������� ���������� �����
	struct Border
	{
		//��������
		double u = 0;
		//���������������� ����������
		double pos = 0;
	};
}
