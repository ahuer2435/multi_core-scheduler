/*************************
�궨��
*************************/
#define NTASK 50		//����ڵ���
//#define NTASK 100		//����ڵ���

/*************************
���ݽṹ
*************************/
//��ڵ�
typedef struct ArcNode
{
	int index;
	struct ArcNode *next;
}ArcNode;

//ͷ�ڵ�
typedef struct VNode
{
	int index;		//�ڵ����
	int weight;		//�ڵ�Ȩֵ
	struct ArcNode *firstarc;		//ָ��ö���ĵ�һ������
	char color;						//��������������
	int parent_index;				//��������������
}VNode;

//�����������������ڴ洢ͷ���Ľڵ�
typedef struct LNode			
{
	int index;
	int weight;
	struct LNode *next;
}LNode;

#if (NTASK == 50)		//ӳ�����У���Χ0-2
extern int opt[10][3];
#else
extern int opt[10][4];
#endif


#ifndef TASK
/*************************
��������
*************************/
VNode* Create_dag(void);
void Free_dag(ArcNode *p);
void Destroy_dag(VNode *p);
int Init_task();
int Task_sum();

/**************************************
task.h�ж���������������ⲿ������������
***************************************/
extern int VNodeWeight[NTASK];
extern int adj_matrix[NTASK][NTASK];
extern VNode AdjList[NTASK];
extern int file_index;
extern int opt_index_col;
#endif
