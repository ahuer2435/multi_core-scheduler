/*************************
宏定义
*************************/
#define NTASK 50		//任务节点数
//#define NTASK 100		//任务节点数

/*************************
数据结构
*************************/
//表节点
typedef struct ArcNode
{
	int index;
	struct ArcNode *next;
}ArcNode;

//头节点
typedef struct VNode
{
	int index;		//节点序号
	int weight;		//节点权值
	struct ArcNode *firstarc;		//指向该顶点的第一个孩子
	char color;						//用于逆拓扑排序
	int parent_index;				//用于逆拓扑排序
}VNode;

//在逆拓扑排序中用于存储头结点的节点
typedef struct LNode			
{
	int index;
	int weight;
	struct LNode *next;
}LNode;

#if (NTASK == 50)		//映射序列，范围0-2
extern int opt[10][3];
#else
extern int opt[10][4];
#endif


#ifndef TASK
/*************************
函数声明
*************************/
VNode* Create_dag(void);
void Free_dag(ArcNode *p);
void Destroy_dag(VNode *p);
int Init_task();
int Task_sum();

/**************************************
task.h中定义变量的声明（外部变量的声明）
***************************************/
extern int VNodeWeight[NTASK];
extern int adj_matrix[NTASK][NTASK];
extern VNode AdjList[NTASK];
extern int file_index;
extern int opt_index_col;
#endif
