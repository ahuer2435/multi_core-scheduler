#ifndef MAPPING
/******************************************
ӳ�����������������ⲿ������������
******************************************/
extern int mapping[NTASK];		//ӳ�����У���Χ0-2
extern unsigned long mapn[RUNTIME];	//ӳ�����

/******************************************
ͨ��ӳ���㷨��������
******************************************/
void Copy_map(int *p,int *map);
void Print_mapping(int *p);
void Init_mapping(int *map);
#endif
