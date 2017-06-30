#define NOMAP			//不包含头文件nmap.h

#include "public.h"
#include "pe.h"
#include "task.h"
#include "mapping.h"
#include "blevel_schedule.h"

/************************************************
1.无映射算法的调度，即将所有的任务调度到同一个处理器上
************************************************/
int Non_map(int *map)
{
	int non_map_cost;
	Init_mapping(map);
	non_map_cost = Scheduling(map,b_level_des);
	return non_map_cost;
}
