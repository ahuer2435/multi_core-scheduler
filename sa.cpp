#define SA				//������ͷ�ļ�sa.h
/*************************
ͷ�ļ�
*************************/
#include "public.h"
#include "pe.h"
#include "task.h"
#include "mapping.h"
#include "sa.h"
#include "blevel_schedule.h"

/******************************************
�޲κ���bin_random()���������0��1
******************************************/
int Bin_random(void)
{
	int n;
	//srand((unsigned) time(0));	//����Ҫ�������ô˺����������ɶ���������������ע�͵�������ÿ�����ɵ�����ͬ
	n = rand() % (2-0) + 0; 
	return n;
}


/******************************************
�вκ���Random()���������a��b������[a,b]�ϵ�������ÿ��ֵ�����Ͽ���Ϊ�ȸ��ʵģ������ȷֲ���
ʱ�临�Ӷȣ�o(n)
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
1.���ܣ��������һ��[0,1)�ĸ�����
******************************************/
double F_random()
{
	double result;
	result = (double)Int_random(0,65535) / 65535;
	return result;
}

/***********************************************************
1.���ܣ������������źʹ������ţ�������������ԭ����������ͬ��
�򽫴������ż�1�������������������䵽��������Ĵ�������
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
1.���ܼ����ֵdiscʱ�Ľ��ܸ��ʣ�params��һ��Ԥ��Ĳ���
************************************************/
double Accepted(int disc,double t)
{
	double exponent;
	exponent = -disc / (Task_sum()*t);
	return exp(exponent);
}


/************************************************
1.���ܽ��º�����params��һ��Ԥ�轵�²���
************************************************/
double Temperture(double t,double params,long int i)
{
	double a,b;
	b = i/REJECT_MAX;
	a = pow(params,b);
	return a*t;
}

/************************************************
1.���ܣ�ģ���˻�ӳ���㷨
2.���������ͱ���i����¼һ������ͼһ�����ĵ����д������
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
	//srand((unsigned) time(0));	//һ��ͼһ��������������10���ǣ��˺���Ҫ����10�Σ���ô���Ӿ�Ҫ����һ�����ú����ж��壬
	while(t >= Tf)					//��ʮ�ε��ã�ֻ����һ�����ӡ�
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
				simul_map_r = (double)(cbest - opt[file_index][opt_index_col])/opt[file_index][opt_index_col];			//2����
			}
			else if(NPE == 4)
			{
				opt_index_col = 1;
				simul_map_r = (double)(cbest - opt[file_index][opt_index_col])/opt[file_index][opt_index_col];			//4����
			}
			else if(NPE == 8)
			{
				opt_index_col = 2;
				simul_map_r = (double)(cbest - opt[file_index][opt_index_col])/opt[file_index][opt_index_col];			//8 ����
			}
			else
			{
				printf("NPE ��һ�£�\n");
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