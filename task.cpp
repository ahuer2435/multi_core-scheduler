#define TASK					//������ͷ�ļ�task.h

#include "public.h"
#include "task.h"

/*************************
ȫ�ֱ���
*************************/
int VNodeWeight[NTASK];	//NTASK�������Ȩֵ
//�����
int adj_matrix[NTASK][NTASK];		//����ͼ���ڽӱ�
VNode AdjList[NTASK];	//����ͷ������飬���ܶ�����create_dag()�����У������������ź����Ľ��������ͷŵ������������޷�����

#if (NTASK == 50)
/*50�ڵ�����ͼ10��������ͼ����3����Ҫ���Եĺ���������2,4,8.�ļ�rand0000.stg��rand0009.stg������ֵ*/
int opt[10][3] = {{131,66,55},{149,94,89},{121,71,71},{145,73,52},{116,71,71},{314,170,170},{216,108,101},{260,130,77},{281,183,183},{263,133,126}};	
#else
/*100�ڵ�����ͼ10��������ͼ����3����Ҫ���Եĺ���������2,4,8.�ļ�rand0000.stg��rand0009.stg������ֵ*/
int opt[10][4] = {{291,146,94,94},{267,134,81,81},{288,175,175,175},{263,132,129,129},{267,134,84,84},
					{518,332,332,332},{570,299,299,299},{558,279,205,205},{517,259,166,166},{584,309,309,309}};
#endif

int file_index;			//�����ļ����±�
int opt_index_col;		//��ά�������Ž���к�

/*************************
��������
*************************/
/*******************************************************
1.λ�ã�����ͼģ��
2.���ܣ���������ͼ��ԭʼ�����ļ�rand000x(0).stg��ʼ������ͼ���ڽӾ���adj_matrix[NTASK][NTASK]
		�Լ�����ͼ�Ĵ洢�ڵ�Ȩֵ������VNodeWeight[NTASK]�������ڽӾ��������text.txt�ļ���������֤
3.���룺��
4.����ֵ�����ɹ�������0�����򷵻�-1
*******************************************************/
int Init_task()
{
	
	int row,col;
	int A[3],*p,temp,index,i;
	FILE *fp,*fpw;

	#if (NTASK == 50)
	/*50�ڵ��ļ��ַ���ָ�봦�������ļ���*/
	char *file[10] = {"./50/rand0000.stg","./50/rand0001.stg","./50/rand0002.stg","./50/rand0003.stg","./50/rand0004.stg",
						"./50/rand0005.stg","./50/rand0006.stg","./50/rand0007.stg","./50/rand0008.stg","./50/rand0009.stg"};	
	#else
	/*100�ڵ��ļ��ַ���ָ�봦�������ļ���*/
	char *file[10] = {"./100/rand0000.stg","./100/rand0001.stg","./100/rand0002.stg","./100/rand0003.stg","./100/rand0004.stg",
						"./100/rand0005.stg","./100/rand0006.stg","./100/rand0007.stg","./100/rand0008.stg","./100/rand0009.stg"};
	#endif

	file_index = 9;
	fp = fopen(file[file_index],"r");		//���ļ�
	fpw = fopen("text.txt","w");
	
	if(fp == NULL)			//���Դ��ļ��Ƿ�ɹ�
	{
		printf("read file rand0000.stg failure!\n");
		return -1;
	}
	if(fpw == NULL)
	{
		printf("write file text.txt failure!\n");
		return -2;
	}

	for(index = 0; index < NTASK; index++)	//���ж�ȡstg�ļ��е�������Ϣ������֮��ʼ������Ȩֵ������ڽӾ���
	{
		fscanf(fp,"%d",&A[0]);
		fscanf(fp,"%d",&A[1]);
		fscanf(fp,"%d",&A[2]);
		VNodeWeight[index] = A[1];
		p = (int *)malloc(A[2] * sizeof(int));
		for(i = 0; i < A[2]; i++)
		{
			fscanf(fp,"%d",&temp);
			if(temp > 0)
			{
				adj_matrix[temp-1][index] = 1;
			}
		}
		free(p);
	}

	for(row = 0; row < NTASK; row++)		//���ڽӾ���������ļ�text.txt��
	{
		for(col = 0; col < NTASK; col++)
		{
			fprintf(fpw,"%d,",adj_matrix[row][col]);
		}
		fprintf(fpw,"\n");
	}
	
	fclose(fp);			//�ر��ļ�
	fclose(fpw);
	return 0;
}

/*******************************************************
1.λ�ã�����ͼģ��
2.���ܣ����ݸ������ڽӾ��󡢽ڵ�Ȩֵ��������ͼ���ڽӱ�Ĵ洢�ṹ
3.���룺��
4.����ֵ���ڽӱ�ĵ�һ������Ԫ�ص�λ��
5.˵�����ڽӾ��󡢽��Ȩֵ���ڽӱ�ľ�����ȫ���Եģ���Ϊ�ڽӾ���
���ڽӱ��ں��������л�ʹ�õ������ڵ�Ȩֵ�������ñȽϺã�����Ҳ�����޸�
*******************************************************/
VNode* Create_dag(void)
{
	int i,j;
	ArcNode *p,*q;
	for(i = 0; i < NTASK; i++)
	{
		AdjList[i].index = i;
		AdjList[i].weight = VNodeWeight[i];
		AdjList[i].firstarc = NULL;
		for(j = 0; j < NTASK; j++)
		{
			if(adj_matrix[i][j] == 1)
			{
				p = (ArcNode *)malloc(sizeof(ArcNode));
				p->index = j;
				if(AdjList[i].firstarc == NULL)
				{
					AdjList[i].firstarc = p;
					q = p;
				}
				else
				{
					q->next = p;
					q = q->next;
				}
			}
		}
		q->next = NULL;
	}
	return &AdjList[0];
}

/*******************************************************
1.λ�ã�����ͼģ��
2.���ܣ��ͷ��ڽӱ��еı�ڵ�����
3.���룺��һ����ڵ�ָ��
4.����ֵ����
5.˵��������ɾ��������
*******************************************************/
void Free_dag(ArcNode *p)
{
	if(p != NULL)
	{
		Free_dag(p->next);
		free(p);	
	}
}

/*******************************************************
1.λ�ã�����ͼģ��
2.���ܣ�����dagͼ���ڽӱ�ṹ
3.���룺�ڽӱ������һ��Ԫ�ص�ַ
4.����ֵ����
*******************************************************/
void Destroy_dag(VNode *p)
{
	int i;
	ArcNode *q;
	for(i = 0; i < NTASK; i++)
	{
		q = p[i].firstarc;
		Free_dag(q);
	}
}

/*******************************************************
1.���ܣ������ܵ�������
*******************************************************/
int Task_sum()
{
	int i,sum = 0;
	for(i = 0; i < NTASK; i++)
	{
		sum += VNodeWeight[i];
	}
	//printf("%d ",sum);
	return sum;	
}

