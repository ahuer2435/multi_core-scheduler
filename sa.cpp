#define SA				//不包含头文件sa.h
/*************************
头文件
*************************/
#include "public.h"
#include "pe.h"
#include "task.h"
#include "mapping.h"
#include "sa.h"
#include "blevel_schedule.h"

/******************************************
无参函数bin_random()，随机产生0和1
******************************************/
int Bin_random(void)
{
	int n;
	//srand((unsigned) time(0));	//但若要连续调用此函数用以生成多个随机数，此行须注释掉，否则每次生成的数相同
	n = rand() % (2-0) + 0; 
	return n;
}


/******************************************
有参函数Random()，随机产生a到b闭区间[a,b]上的整数（每个值基本上可视为等概率的，即均匀分布）
时间复杂度：o(n)
******************************************/
int Int_random(int a,int b)
{
	int n,c,d;
	n = 0;	
	c = b - a;
	while(c)
	{
		n++;
		c = c / 2;
	}
	do
	{
		d = 0;
		for(int i = 0; i < n; i++)
		{
			d = d * 2 + Bin_random();
		}
	}while(d > b-a);	
	return (a + d);	
} 

/******************************************
1.功能：随机产生一个[0,1)的浮点数
******************************************/
double F_random()
{
	double result;
	result = (double)Int_random(0,65535) / 65535;
	return result;
}

/***********************************************************
1.功能：随机产生任务号和处理器号，若处理器号与原处理器号相同，
则将处理器号加1，将随机产生的任务分配到随机产生的处理器上
************************************************************/
void Move(int *snew_m,int *map)
{
	int peid,taskid;
	Copy_map(snew_m,map);
	peid = Int_random(0,NPE-1);
	taskid = Int_random(0,NTASK-1);
	if(snew_m[taskid] == peid)
	{
		peid = (peid + 1) % NPE;
	}
	snew_m[taskid] = peid;
}

/************************************************
1.功能计算差值disc时的接受概率，params是一个预设的参数
************************************************/
double Accepted(int disc,double t)
{
	double exponent;
	exponent = -disc / (Task_sum()*t);
	return exp(exponent);
}


/************************************************
1.功能降温函数，params是一个预设降温参数
************************************************/
double Temperture(double t,double params,long int i)
{
	double a,b;
	b = i/REJECT_MAX;
	a = pow(params,b);
	return a*t;
}

/************************************************
1.功能：模拟退火映射算法
2.参数：整型变量i，记录一个任务图一个核心的运行次数序号
************************************************/
int Simulated_annealing(int i)
{
	double simul_speedup,simul_map_r;
	int rejects = 0;
	double fr,fac;
	double tem_params = 0.95;
	double T0 = exp(-4-1.0/NPE),Tf = exp(-9+1.0/(NTASK*NPE)),t;
	int c,k = 0;
    int sbest_map[NTASK],snew_map[NTASK];
	int cbest,cnew,disc;
	c = Scheduling(mapping,b_level_des);    //cost(s);
	//printf("c = %ld\n",c);
	Copy_map(sbest_map,mapping);              //sbest = mapping;
	cbest = c;
	t = T0;
	//srand((unsigned) time(0));	//一个图一个核心数，运行10次是，此函数要调用10次，那么种子就要在上一个调用函数中定义，
	while(t >= Tf)					//即十次调用，只是用一个种子。
	{
		Move(snew_map,mapping);
		//print_mapping(snew_map);
		cnew = Scheduling(snew_map,b_level_des);
		//printf("cnew = %ld\n",cnew);
		disc = cnew - c;
		fr = F_random();
		fac = Accepted(disc,t);
		//printf("%f  %f\n",fr,fac);
		//Sleep(100);
		if((disc < 0) || (fr < fac))
		{
			if(cnew < cbest)
			{
				Copy_map(sbest_map,snew_map);
				cbest = cnew;
				//rejects = 0;
			}
			Copy_map(mapping,snew_map);	
			c = cnew;
			rejects = 0;
		}
		else 
		{
			if(rejects >= REJECT_MAX * (3*t/T0))
			{
				break;
			}
			rejects++;
		}
		k++;
		if(k == REJECT_MAX)
		{
			/*if(NPE == 2)
			{
				opt_index_col = 0;
				simul_map_r = (double)(cbest - opt[file_index][opt_index_col])/opt[file_index][opt_index_col];			//2核心
			}
			else if(NPE == 4)
			{
				opt_index_col = 1;
				simul_map_r = (double)(cbest - opt[file_index][opt_index_col])/opt[file_index][opt_index_col];			//4核心
			}
			else if(NPE == 8)
			{
				opt_index_col = 2;
				simul_map_r = (double)(cbest - opt[file_index][opt_index_col])/opt[file_index][opt_index_col];			//8 核心
			}
			else
			{
				printf("NPE 不一致！\n");
			}
			simul_speedup = (double)task_sum()/cbest;*/
			//printf("%f\n",simul_speedup);
			//printf("%f\n",simul_map_r);
			t = Temperture(T0,tem_params,mapn[i]);
			k = 0;
		}
		mapn[i]++;
	}	
	//printf("simulated annealing numbers : %ld\n",mapn[i]);
	//printf("cbest = %ld\n",cbest);
	return cbest;
}