#define NOMAP			//������ͷ�ļ�nmap.h

#include "public.h"
#include "pe.h"
#include "task.h"
#include "mapping.h"
#include "blevel_schedule.h"

/************************************************
1.��ӳ���㷨�ĵ��ȣ��������е�������ȵ�ͬһ����������
************************************************/
int Non_map(int *map)
{
	int non_map_cost;
	Init_mapping(map);
	non_map_cost = Scheduling(map,b_level_des);
	return non_map_cost;
}
