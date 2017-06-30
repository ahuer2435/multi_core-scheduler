/*************************
ģ���˻�ͷ�ļ�
*************************/
#include "public.h"
#include "task.h"
#include "pe.h"
#include "blevel_schedule.h"
#include "sa.h"
#include "nomap_map.h"
#include "mapping.h"

/************************************************
	���ܺ���������
************************************************/
int Print_map_spd_r(void);

/************************************************
1.����������
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
1.����:��ӡ10�����е�ӳ�䣬���ٶȺ����ƫ��ľ�ֵ
************************************************/
int Print_map_spd_r(void)
{
	unsigned long map_num_sum = 0;			//mapn[RUNTIME]����ֵ
	int i;
	double speedup[RUNTIME],map_r[RUNTIME],map_bcost[RUNTIME];
	double simul_map_bcost = 0,non_map_cost;
	double simul_map_speedup = 0,simul_map_r = 0;
	
	non_map_cost = Non_map(mapping);					//�������mapping[NTASK]����Ӧ����ȫ�ֱ���
	srand((unsigned) time(0));							//������������ͬһ�����ӣ�����ÿ�����ɵ��������һ���ģ����������simulated_annealing()�����У�ע�͵ĵط���
	for(i = 0; i < RUNTIME; i++)
	{
		Init_mapping(mapping);
		mapn[i] = 0;
		map_bcost[i] = Simulated_annealing(i);			//����ӳ���㷨��Ҫ���µĵط�
		//printf("map_bcost[i] = %ld\n",map_bcost[i]);
		speedup[i] = (double)non_map_cost/map_bcost[i];
		if(NPE == 2)
		{
			opt_index_col = 0;
			map_r[i] = (double)(map_bcost[i] - opt[file_index][opt_index_col])/opt[file_index][opt_index_col];			//2����
		}
		else if(NPE == 4)
		{
			opt_index_col = 1;
			map_r[i] = (double)(map_bcost[i] - opt[file_index][opt_index_col])/opt[file_index][opt_index_col];			//4����
		}
		else if(NPE == 8)
		{
			opt_index_col = 2;
			map_r[i] = (double)(map_bcost[i] - opt[file_index][opt_index_col])/opt[file_index][opt_index_col];			//8 ����
		}
		else if(NPE == 16)
		{
			opt_index_col = 3;
			map_r[i] = (double)(map_bcost[i] - opt[file_index][opt_index_col])/opt[file_index][opt_index_col];			//8 ����
		}
		else
		{
			printf("NPE ��һ�£�\n");
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
	
	printf("��������non_map_cost:            %f\n",non_map_cost);
	printf("�����ܴ�����10�����о�ֵ:        %lf\n",(double)map_num_sum/RUNTIME);		//������(double)���������0.000000�����������������%f��ӡ����������0.000000��ԭ��δ֪
	printf("simul_map_bcost���Ž�10�ξ�ֵ:   %f\n",simul_map_bcost/RUNTIME);
	printf("simul_map_speedup���ٱ�10�ξ�ֵ: %f\n",simul_map_speedup/RUNTIME);
	printf("simul_map_r���ƫ��10�ξ�ֵ:     %f\n",simul_map_r/RUNTIME);
	printf("%d\n",Task_sum());
	return 0;
}
/*��������ͨ�Ŵ���ʱ��������ȵ����ȴ����ǲ���ģ�
����ͨ�Ŵ���ʱ����ͬ��ӳ�����У���������ȴ���Ҳ
�ǲ�ͬ�ģ���Ϊӳ�䵽ͬһ�������ϵ�ͨ�Ŵ��۽�����
Ϊ0*/


