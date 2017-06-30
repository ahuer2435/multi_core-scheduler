#ifndef MAPPING
/******************************************
映射变量定义的声明（外部变量的声明）
******************************************/
extern int mapping[NTASK];		//映射序列，范围0-2
extern unsigned long mapn[RUNTIME];	//映射次数

/******************************************
通用映射算法函数声明
******************************************/
void Copy_map(int *p,int *map);
void Print_mapping(int *p);
void Init_mapping(int *map);
#endif
