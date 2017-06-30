#define BL_SCHED				//不包含头文件blevel_schedule.h
/******************************************
头文件声明
******************************************/
#include "public.h"
#include "pe.h"
#include "task.h"
#include "blevel_schedule.h"
#include "cost.h"

/*************************
b_level调度变量定义
*************************/
int b_level[NTASK];	//各任务的优先级
int b_level_des[NTASK]; //存储TASK的序号
int tasks_start[NTASK];			//各任务的开始时间和结束时间
LNode *header = (LNode *)malloc(sizeof(LNode));		//单链表头结点
LNode *qh = header;

/*******************************************************
1.位置：调度函数
2.功能：功能把指向表节点的指针p转化为指向相应头结点的指针q，并返回指针q
3.输入：表节点指针
4.返回值：相应表节点任务的头结点指针。
*******************************************************/
VNode* Vpoint(ArcNode *p)
{
	VNode *q =(VNode *) &AdjList[0];
	return q+(p->index);
}



/*******************************************************
1.位置：调度函数
2.功能：rev_topo_sort(VNode* u)的子函数
3.输入：dag邻接表的各个头结点指针
4.返回值：带有头结点的dag的逆拓扑排序单链表指针header
5.说明：利用给定的节点，采用尾插法，建立单链表
这里的firstarc与上面不同，实际上就是指向下一个节点的指针。
以此建立DAG的逆拓扑排序链表
*******************************************************/
LNode* List_tail_insert(VNode *p)
{
	LNode *l = (LNode *)malloc(sizeof(LNode));
	l->index = p->index;
	l->weight = p->weight;
	l->next = NULL;
	qh->next = l;
	qh = qh->next;
	return header;
}

/*******************************************************
1.位置：调度函数
2.功能：DFS(VNode* p)的子函数
3.输入：dag邻接表的各个头结点指针
4.返回值：带有头结点的dag的逆拓扑排序单链表指针header
*******************************************************/
LNode* Rev_topo_sort(VNode* u)
{
	ArcNode *p;
	u->color = 'G';
	if(u->firstarc != NULL)
	{
		for(p = u->firstarc; p != NULL; p = p->next)
		{
			if(Vpoint(p)->color == 'W')
			{
				Rev_topo_sort(Vpoint(p));
			}
		}	
	}
	return List_tail_insert(u);
}

/*******************************************************
1.位置：调度函数
2.功能：深度优先搜索DAG,相当于建立的逆拓扑排序（带头节点的单链表）
3.输入：无
4.返回值：带有头结点的dag的逆拓扑排序单链表指针header
*******************************************************/
LNode* DFS()
{
	int i;
	LNode *l;
	for(i = 0; i < NTASK; i++)
	{
		AdjList[i].color = 'W';
	}
	for(i = 0; i < NTASK; i++)
	{
		if(AdjList[i].color == 'W')
		{
			 l = Rev_topo_sort(&AdjList[i]);
		}
	}
	return l;
}

/************************************************
1.功能打印单链表（逆序拓扑结构）
************************************************/
void Print_list(LNode *p)
{
	printf("逆序拓扑排序：\n");
	while(p != NULL)
	{
		printf("节点:%d,权重:%d \n",p->index,p->weight);
		p = p->next;
	}
}

/************************************************
1.功能释放单链表空间
************************************************/
void Free_list(LNode *p)
{
	if(p != NULL)
	{
		Free_list(p->next);
		free(p);	
	}
}
	


/*******************************************************
1.位置：调度函数
2.功能：计算各个节点的b_level值
3.输入：无
4.返回值：带有头结点的dag的逆拓扑排序单链表指针header
5.说明：利用给定的节点，采用尾插法，建立单链表
这里的firstarc与上面不同，实际上就是指向下一个节点的指针。
以此建立DAG的逆拓扑排序链表
*******************************************************/
int B_level_computer()
{
	int max;
	LNode *q = header->next;
	ArcNode *ap;
	while(q != NULL)
	{
		max = 0;
		ap = AdjList[q->index].firstarc;
		while(ap != NULL)
		{
			if(b_level[ap->index] > max)
			{
				max = b_level[ap->index];
			}
			ap = ap->next;
		}
		b_level[q->index] = max + q->weight;
		q = q->next;
	}
	return 0;
}


/*******************************************************
1.位置：调度函数
2.功能：创建并初始化b_level_des[TASK]数组，用于存储按权值降序排列的任务序列
3.输入：无
4.返回值：无
*******************************************************/
void Init_b_level_des()
{
	int i;
	//存储b_level[NTASK]的序号b_level_des[NTASK]
	for(i = 0; i < NTASK; i++)
	{
		b_level_des[i] = i;
	}
}

/*******************************************************
1.位置：调度函数
2.功能：设置b_level_des[TASK]的值，b_level_des[TASK]是任务按照b_level[]降序排列
的任务序号。采用的是冒泡排序算法实现。
3.输入：无
4.返回值：无
*******************************************************/
void Des_bubble_sort()
{
	int i,j,tempc,tempd;
	for(i = 0; i < NTASK - 1; i++)
	{
		for(j = 0; j < NTASK - 1 - i; j++)
		{
			if(b_level[j] < b_level[j+1])
			{
				tempc = b_level[j];
				b_level[j] = b_level[j+1];
				b_level[j+1] = tempc;

				tempd = b_level_des[j];
				b_level_des[j] = b_level_des[j+1];
				b_level_des[j+1] = tempd;
			}
		}
	}
 }

/*******************************************************
1.位置：调度函数
2.功能：对于给定的一个映射，确定各个任务的开始执行时间（在
成本函数是三个处理器结束时间最大值时，也计算出了给定映射序列的
各个处理器的结束时间）
3.输入：映射序列和任务调度的先后次序的序列
4.返回值：各任务的开始调度时间（全局变量），（在成本函数是三个处理器结束时间最大值时，
也计算出了给定映射序列的各个处理器的结束时间（返回值））
5.说明：此调度的实现，首先利用DFS函数对图的邻接表进行逆拓扑排序，形成一个
单链表header（全局，由实现所限制），然后由b_level_computer程序根据
这个单链表header计算各个任务的b_level值，得到一个b_level[NTASK]数组（全局的），然后由des_bubble_sort函数
根据b_level[NTASK]创建任务的调度优先顺序数组b_level_des[NTASK]（全局），它和映射序列作为调度程序的输入参数
*******************************************************/
int Scheduling(int *map,int *bld)
{
	int col,row,i,pre_eariest_time,bestc;
	int pe_end[NPE] = {0};
	for(i = 0; i < NTASK; i++)
	{
		col = bld[i];
		pre_eariest_time = 0;
		for(row = 0; row < NTASK; row++)
		{
			if(adj_matrix[row][col] == 1)
			{
				if(pe_end[map[row]] > pre_eariest_time)
				{
					pre_eariest_time = pe_end[map[row]];
				}				
			}
		}

		if(pre_eariest_time > pe_end[map[col]])
		{
			tasks_start[col] = pre_eariest_time;
		}
		else
		{
			tasks_start[col] = pe_end[map[col]];
		}
		pe_end[map[col]] = tasks_start[col] + AdjList[col].weight;
	}

	bestc = Cost(pe_end);
	return bestc;
}
