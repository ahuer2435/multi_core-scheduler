/******************************************
模拟退火宏
******************************************/
#define REJECT_MAX ((NTASK)*(NPE))	//每个温度的搜索次数

#ifndef SA
/******************************************
模拟退火映射算法函数声明
******************************************/
int Bin_random(void);
int Int_random(int a,int b);
double F_random();
void Move(int *snew_m,int *map);
double Accepted_new(int disc,double t,double params);
double Temperture(double t,double params,long int i);
int Simulated_annealing(int i);
#endif