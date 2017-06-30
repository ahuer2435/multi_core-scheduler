/*************************
模拟退火头文件
*************************/
#include "public.h"
#include "task.h"
#include "pe.h"
#include "blevel_schedule.h"
#include "sa.h"
#include "nomap_map.h"
#include "mapping.h"

/************************************************
	功能函数的声明
************************************************/
int Print_map_spd_r(void);

/************************************************
1.功能主函数
************************************************/
int main(void)
{
	Init_task();
	Create_dag();
	DFS();
	B_level_computer();
	//Print_list(header);
	Destroy_dag(&AdjList[0]);
	Free_list(header);
	//Print_list(header);
	Init_b_level_des();
	Des_bubble_sort();
	Print_map_spd_r();
	return 0;
}

/************************************************
1.功能:打印10次运行的映射，加速度和相对偏差的均值
************************************************/
int Print_map_spd_r(void)
{
	unsigned long map_num_sum = 0;			//mapn[RUNTIME]的总值
	int i;
	double speedup[RUNTIME],map_r[RUNTIME],map_bcost[RUNTIME];
	double simul_map_bcost = 0,non_map_cost;
	double simul_map_speedup = 0,simul_map_r = 0;
	
	non_map_cost = Non_map(mapping);					//这里决定mapping[NTASK]还是应该是全局变量
	srand((unsigned) time(0));							//整个程序不许用同一个种子，否者每次生成的随机数是一样的（即此语句在simulated_annealing()函数中，注释的地方）
	for(i = 0; i < RUNTIME; i++)
	{
		Init_mapping(mapping);
		mapn[i] = 0;
		map_bcost[i] = Simulated_annealing(i);			//新型映射算法需要更新的地方
		//printf("map_bcost[i] = %ld\n",map_bcost[i]);
		speedup[i] = (double)non_map_cost/map_bcost[i];
		if(NPE == 2)
		{
			opt_index_col = 0;
			map_r[i] = (double)(map_bcost[i] - opt[file_index][opt_index_col])/opt[file_index][opt_index_col];			//2核心
		}
		else if(NPE == 4)
		{
			opt_index_col = 1;
			map_r[i] = (double)(map_bcost[i] - opt[file_index][opt_index_col])/opt[file_index][opt_index_col];			//4核心
		}
		else if(NPE == 8)
		{
			opt_index_col = 2;
			map_r[i] = (double)(map_bcost[i] - opt[file_index][opt_index_col])/opt[file_index][opt_index_col];			//8 核心
		}
		else if(NPE == 16)
		{
			opt_index_col = 3;
			map_r[i] = (double)(map_bcost[i] - opt[file_index][opt_index_col])/opt[file_index][opt_index_col];			//8 核心
		}
		else
		{
			printf("NPE 不一致！\n");
		}
	}

	for(i = 0; i < RUNTIME; i++)
	{
		simul_map_bcost += map_bcost[i];
		simul_map_speedup += speedup[i];
		simul_map_r += map_r[i];
		map_num_sum += mapn[i];
		//printf("map_bcost[i] = %ld\n",map_bcost[i]);
	}
	
	printf("总任务量non_map_cost:            %f\n",non_map_cost);
	printf("迭代总次数，10次运行均值:        %lf\n",(double)map_num_sum/RUNTIME);		//若不加(double)，结果就是0.000000，两个整数相除，以%f打印结果，会输出0.000000，原因未知
	printf("simul_map_bcost最优解10次均值:   %f\n",simul_map_bcost/RUNTIME);
	printf("simul_map_speedup加速比10次均值: %f\n",simul_map_speedup/RUNTIME);
	printf("simul_map_r相对偏差10次均值:     %f\n",simul_map_r/RUNTIME);
	printf("%d\n",Task_sum());
	return 0;
}
/*当不考虑通信代价时，任务调度的优先次序是不变的；
考虑通信代价时，不同的映射序列，其任务调度次序也
是不同的，因为映射到同一处理器上的通信代价将会置
为0*/


