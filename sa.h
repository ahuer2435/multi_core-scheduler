/******************************************
ģ���˻��
******************************************/
#define REJECT_MAX ((NTASK)*(NPE))	//ÿ���¶ȵ���������

#ifndef SA
/******************************************
ģ���˻�ӳ���㷨��������
******************************************/
int Bin_random(void);
int Int_random(int a,int b);
double F_random();
void Move(int *snew_m,int *map);
double Accepted_new(int disc,double t,double params);
double Temperture(double t,double params,long int i);
int Simulated_annealing(int i);
#endif