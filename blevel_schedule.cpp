#define BL_SCHED				//������ͷ�ļ�blevel_schedule.h
/******************************************
ͷ�ļ�����
******************************************/
#include "public.h"
#include "pe.h"
#include "task.h"
#include "blevel_schedule.h"
#include "cost.h"

/*************************
b_level���ȱ�������
*************************/
int b_level[NTASK];	//����������ȼ�
int b_level_des[NTASK]; //�洢TASK�����
int tasks_start[NTASK];			//������Ŀ�ʼʱ��ͽ���ʱ��
LNode *header = (LNode *)malloc(sizeof(LNode));		//������ͷ���
LNode *qh = header;

/*******************************************************
1.λ�ã����Ⱥ���
2.���ܣ����ܰ�ָ���ڵ��ָ��pת��Ϊָ����Ӧͷ����ָ��q��������ָ��q
3.���룺��ڵ�ָ��
4.����ֵ����Ӧ��ڵ������ͷ���ָ�롣
*******************************************************/
VNode* Vpoint(ArcNode *p)
{
	VNode *q =(VNode *) &AdjList[0];
	return q+(p->index);
}



/*******************************************************
1.λ�ã����Ⱥ���
2.���ܣ�rev_topo_sort(VNode* u)���Ӻ���
3.���룺dag�ڽӱ�ĸ���ͷ���ָ��
4.����ֵ������ͷ����dag����������������ָ��header
5.˵�������ø����Ľڵ㣬����β�巨������������
�����firstarc�����治ͬ��ʵ���Ͼ���ָ����һ���ڵ��ָ�롣
�Դ˽���DAG����������������
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
1.λ�ã����Ⱥ���
2.���ܣ�DFS(VNode* p)���Ӻ���
3.���룺dag�ڽӱ�ĸ���ͷ���ָ��
4.����ֵ������ͷ����dag����������������ָ��header
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
1.λ�ã����Ⱥ���
2.���ܣ������������DAG,�൱�ڽ��������������򣨴�ͷ�ڵ�ĵ�����
3.���룺��
4.����ֵ������ͷ����dag����������������ָ��header
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
1.���ܴ�ӡ�������������˽ṹ��
************************************************/
void Print_list(LNode *p)
{
	printf("������������\n");
	while(p != NULL)
	{
		printf("�ڵ�:%d,Ȩ��:%d \n",p->index,p->weight);
		p = p->next;
	}
}

/************************************************
1.�����ͷŵ�����ռ�
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
1.λ�ã����Ⱥ���
2.���ܣ���������ڵ��b_levelֵ
3.���룺��
4.����ֵ������ͷ����dag����������������ָ��header
5.˵�������ø����Ľڵ㣬����β�巨������������
�����firstarc�����治ͬ��ʵ���Ͼ���ָ����һ���ڵ��ָ�롣
�Դ˽���DAG����������������
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
1.λ�ã����Ⱥ���
2.���ܣ���������ʼ��b_level_des[TASK]���飬���ڴ洢��Ȩֵ�������е���������
3.���룺��
4.����ֵ����
*******************************************************/
void Init_b_level_des()
{
	int i;
	//�洢b_level[NTASK]�����b_level_des[NTASK]
	for(i = 0; i < NTASK; i++)
	{
		b_level_des[i] = i;
	}
}

/*******************************************************
1.λ�ã����Ⱥ���
2.���ܣ�����b_level_des[TASK]��ֵ��b_level_des[TASK]��������b_level[]��������
��������š����õ���ð�������㷨ʵ�֡�
3.���룺��
4.����ֵ����
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
1.λ�ã����Ⱥ���
2.���ܣ����ڸ�����һ��ӳ�䣬ȷ����������Ŀ�ʼִ��ʱ�䣨��
�ɱ���������������������ʱ�����ֵʱ��Ҳ������˸���ӳ�����е�
�����������Ľ���ʱ�䣩
3.���룺ӳ�����к�������ȵ��Ⱥ���������
4.����ֵ��������Ŀ�ʼ����ʱ�䣨ȫ�ֱ����������ڳɱ���������������������ʱ�����ֵʱ��
Ҳ������˸���ӳ�����еĸ����������Ľ���ʱ�䣨����ֵ����
5.˵�����˵��ȵ�ʵ�֣���������DFS������ͼ���ڽӱ���������������γ�һ��
������header��ȫ�֣���ʵ�������ƣ���Ȼ����b_level_computer�������
���������header������������b_levelֵ���õ�һ��b_level[NTASK]���飨ȫ�ֵģ���Ȼ����des_bubble_sort����
����b_level[NTASK]��������ĵ�������˳������b_level_des[NTASK]��ȫ�֣�������ӳ��������Ϊ���ȳ�����������
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
