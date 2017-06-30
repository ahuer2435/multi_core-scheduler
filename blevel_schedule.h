#ifndef BL_SCHED
/*************************
b_level调度变量声明
*************************/
extern int b_level[NTASK];	//各任务的优先级
extern int b_level_des[NTASK]; //存储TASK的序号
extern int tasks_start[NTASK];			//各任务的开始时间和结束时间
extern LNode *header;
extern LNode *qh;

/*************************
b_level调度函数声明
*************************/
VNode* Vpoint(ArcNode *p);
LNode* DFS();
LNode* Rev_topo_sort(VNode* u);
LNode* List_tail_insert(VNode *p);
void Print_list(LNode *p);
void Free_list(LNode *p);
int B_level_computer();
void Init_b_level_des();
void Des_bubble_sort();
int Scheduling(int *map,int *bld);
#endif