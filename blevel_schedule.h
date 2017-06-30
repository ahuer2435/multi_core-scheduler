#ifndef BL_SCHED
/*************************
b_level���ȱ�������
*************************/
extern int b_level[NTASK];	//����������ȼ�
extern int b_level_des[NTASK]; //�洢TASK�����
extern int tasks_start[NTASK];			//������Ŀ�ʼʱ��ͽ���ʱ��
extern LNode *header;
extern LNode *qh;

/*************************
b_level���Ⱥ�������
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