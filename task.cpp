#define TASK					//不包含头文件task.h

#include "public.h"
#include "task.h"

/*************************
全局变量
*************************/
int VNodeWeight[NTASK];	//NTASK个任务的权值
//有向边
int adj_matrix[NTASK][NTASK];		//任务图的邻接表
VNode AdjList[NTASK];	//定义头结点数组，不能定义在create_dag()函数中，否则它会随着函数的结束而被释放掉，其它函数无法引用

#if (NTASK == 50)
/*50节点任务图10代表任务图数。3代表要测试的核心数，即2,4,8.文件rand0000.stg到rand0009.stg的最优值*/
int opt[10][3] = {{131,66,55},{149,94,89},{121,71,71},{145,73,52},{116,71,71},{314,170,170},{216,108,101},{260,130,77},{281,183,183},{263,133,126}};	
#else
/*100节点任务图10代表任务图数。3代表要测试的核心数，即2,4,8.文件rand0000.stg到rand0009.stg的最优值*/
int opt[10][4] = {{291,146,94,94},{267,134,81,81},{288,175,175,175},{263,132,129,129},{267,134,84,84},
					{518,332,332,332},{570,299,299,299},{558,279,205,205},{517,259,166,166},{584,309,309,309}};
#endif

int file_index;			//输入文件的下标
int opt_index_col;		//二维数组最优解的列号

/*************************
函数定义
*************************/
/*******************************************************
1.位置：任务图模型
2.功能：依据任务图的原始数据文件rand000x(0).stg初始化任务图的邻接矩阵adj_matrix[NTASK][NTASK]
		以及任务图的存储节点权值的数组VNodeWeight[NTASK]。并将邻接矩阵输出到text.txt文件，用于验证
3.输入：无
4.返回值：若成功，返回0；否则返回-1
*******************************************************/
int Init_task()
{
	
	int row,col;
	int A[3],*p,temp,index,i;
	FILE *fp,*fpw;

	#if (NTASK == 50)
	/*50节点文件字符串指针处理输入文件名*/
	char *file[10] = {"./50/rand0000.stg","./50/rand0001.stg","./50/rand0002.stg","./50/rand0003.stg","./50/rand0004.stg",
						"./50/rand0005.stg","./50/rand0006.stg","./50/rand0007.stg","./50/rand0008.stg","./50/rand0009.stg"};	
	#else
	/*100节点文件字符串指针处理输入文件名*/
	char *file[10] = {"./100/rand0000.stg","./100/rand0001.stg","./100/rand0002.stg","./100/rand0003.stg","./100/rand0004.stg",
						"./100/rand0005.stg","./100/rand0006.stg","./100/rand0007.stg","./100/rand0008.stg","./100/rand0009.stg"};
	#endif

	file_index = 9;
	fp = fopen(file[file_index],"r");		//打开文件
	fpw = fopen("text.txt","w");
	
	if(fp == NULL)			//测试打开文件是否成功
	{
		printf("read file rand0000.stg failure!\n");
		return -1;
	}
	if(fpw == NULL)
	{
		printf("write file text.txt failure!\n");
		return -2;
	}

	for(index = 0; index < NTASK; index++)	//按行读取stg文件中的任务信息，并用之初始化任务权值数组和邻接矩阵
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

	for(row = 0; row < NTASK; row++)		//将邻接矩阵输出到文件text.txt中
	{
		for(col = 0; col < NTASK; col++)
		{
			fprintf(fpw,"%d,",adj_matrix[row][col]);
		}
		fprintf(fpw,"\n");
	}
	
	fclose(fp);			//关闭文件
	fclose(fpw);
	return 0;
}

/*******************************************************
1.位置：任务图模型
2.功能：依据给定的邻接矩阵、节点权值创建任务图的邻接表的存储结构
3.输入：无
4.返回值：邻接表的第一个数组元素的位置
5.说明：邻接矩阵、结点权值和邻接表的矩阵都是全局性的，因为邻接矩阵
和邻接表在后续函数中会使用到。而节点权值事先设置比较好，这样也容易修改
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
1.位置：任务图模型
2.功能：释放邻接表中的表节点链表
3.输入：第一个表节点指针
4.返回值：无
5.说明：类似删除单链表
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
1.位置：任务图模型
2.功能：销毁dag图的邻接表结构
3.输入：邻接表数组第一个元素地址
4.返回值：无
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
1.功能：计算总的任务量
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

