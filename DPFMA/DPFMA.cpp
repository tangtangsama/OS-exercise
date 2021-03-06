#include<stdio.h>
#include<stdlib.h>

#define MAX_LENGTH_OF_MEMORY 1024	// 默认内存最大空间为1024KB
#define MAX_LENGTH_OF_DEBRIS 2		// 默认最大碎片大小为2KB

// 内存地址结构体
struct mem_block
{
	int addr;           // 起始地址
	int length;         // 内存分区大小
	int num;			// 作业号
	int state;			// 分区状态，0表示空闲，1表示已分配
	mem_block *prev;    // 前向指针
	mem_block *next;    // 后向指针
}headBlock;


void initMemory();		// 初始化内存分区链
int firstFit(int, int);	// 首次适应算法
int bestFit(int, int);	// 最佳适应算法
int freeMemoey(int);	// 释放内存分区
void showMemoey();		// 显示内存分区情况

						// 主函数
int main()
{
	int chooseAlgorithm, chooseFunction, num, length;
	initMemory();

	// 选择模拟算法
	while (1)
	{
		printf("****************模拟内存动态分区分配******************\n\n");
		printf("请选择模拟的算法：\n\n");
		printf("1 首次适应算法\t2 最佳适应算法\t0 退出\n");
		printf("请选择： ");
		scanf("%d", &chooseAlgorithm);
		if (chooseAlgorithm == 0) return -1;
		else if (chooseAlgorithm == 1)
		{
			printf("\n******************************************************\n");
			printf("首次适应算法，开始算法模拟!\n");
			break;
		}
		else if (chooseAlgorithm == 2)
		{
			printf("********************************************************\n");
			printf("最佳适应算法，开始算法模拟!\n");
			break;
		}
		else
		{
			printf("选项错误！请输入 0/1/2！\n\n");
		}
	}

	// 模拟动态分区分配算法
	while (1)
	{
		printf("\n");
		printf("1 分配内存    2 回收内存    3 显示内存情况    0 退出\n");
		printf("请选择： ");
		scanf("%d", &chooseFunction);

		if (chooseFunction == 0) break;
		else if (chooseFunction == 1)
		{
			printf("请输入作业号： ");
			scanf("%d", &num);
			printf("请输入分配给该作业的内存大小(KB)： ");
			scanf("%d", &length);
			if (length <= 0)
			{
				printf("输入错误！分配内存大小须为正值！\n");
				continue;
			}

			// 调用对应算法
			if (chooseAlgorithm == 1)
			{
				firstFit(num, length);
			}
			else if (chooseAlgorithm == 2)
			{
				bestFit(num, length);
			}
		}
		else if (chooseFunction == 2)
		{
			printf("请输入要回收的作业号： ");
			scanf("%d", &num);
			freeMemoey(num);
		}
		else if (chooseFunction == 3) {
			showMemoey();
		}
		else
		{
			printf("选项错误！请输入 0/1/2/3！\n\n");
		}
	}
	printf("模拟结束！\n");
	system("pause");
	return 0;
}

// 初始化内存分区链
void initMemory()
{
	// 初始化第一个分区
	mem_block *firstBlock = (mem_block *)malloc(sizeof(mem_block));
	firstBlock->addr = 0;
	firstBlock->length = MAX_LENGTH_OF_MEMORY;     // 内存初始大小
	firstBlock->state = 0;
	firstBlock->num = -1;
	firstBlock->prev = &headBlock;
	firstBlock->next = NULL;

	headBlock.prev = NULL;
	headBlock.next = firstBlock;
}

// 首次适应算法
int firstFit(int num, int length)
{
	mem_block *nptr = headBlock.next;
	while (nptr != NULL)	// 寻找适合的空闲分区，首次适应算法要求按照地址空间顺序寻找到第一个适合的空闲分区分配给当前作业
	{
		if (nptr->state == 0 && nptr->length >= length)		// 找到合适分区
		{
			if (nptr->length - length <= MAX_LENGTH_OF_DEBRIS)	// 剩余空间不超过最大碎片大小，整块分配
			{
				nptr->state = 1;
				nptr->num = num;
			}
			else {												// 剩余空间过大，只分配大小为length的空间
				mem_block *block = (mem_block *)malloc(sizeof(mem_block));	// block表示从当前分区分配后剩余的新分区
				block->addr = nptr->addr + length;				// 地址后移length长度
				block->length = nptr->length - length;			// 大小减小length
				block->state = 0;
				block->num = -1;

				block->prev = nptr;								// 修改分区节点指针
				block->next = nptr->next;						// 插入新分区
				if (nptr->next != NULL)
				{
					nptr->next->prev = block;
				}
				nptr->next = block;

				nptr->length = length;							// 分配该空闲分区
				nptr->state = 1;
				nptr->num = num;
			}
			printf("内存分配成功！\n");
			return 1;
		}
		nptr = nptr->next;										// 未找到合适分区
	}
	printf("不存在合适的内存分区，分配失败!\n");
	return 0;
}

// 最佳适应算法
int bestFit(int num, int length)
{
	mem_block *targetBlock = NULL;			// 目标分区
	int targetSize = MAX_LENGTH_OF_MEMORY;	// 目标分区大小
	mem_block *nptr = headBlock.next;

	while (nptr != NULL)					// 寻找最佳空闲区间，最佳适应算法要求将满足要求又是最小的空闲分区分配给当前作业
	{
		if (nptr->state == 0 && nptr->length >= length && nptr->length <= targetSize)
		{
			targetBlock = nptr;				// 遍历一遍链表，找到满足要求的最小空闲分区
			targetSize = nptr->length;
		}
		nptr = nptr->next;
	}
	if (targetBlock != NULL)				// 查找到适合的分区
	{
		if (targetBlock->length - length <= MAX_LENGTH_OF_DEBRIS)	// 剩余空间不超过最大碎片大小，整块分配
		{
			targetBlock->state = 1;
			targetBlock->num = num;
		}
		else {									// 剩余空间过大，只分配大小为length的空间						
			mem_block *block = (mem_block *)malloc(sizeof(mem_block));
			block->addr = targetBlock->addr + length;
			block->length = targetBlock->length - length;
			block->state = 0;
			block->num = -1;

			block->prev = targetBlock;			// 修改分区节点指针
			block->next = targetBlock->next;
			if (targetBlock->next != NULL)
			{
				targetBlock->next->prev = block;
			}
			targetBlock->next = block;

			targetBlock->length = length;		// 分配空闲区间
			targetBlock->state = 1;
			targetBlock->num = num;
		}
		printf("内存分配成功！\n");
		return 1;
	}
	else
	{
		printf("不存在合适的内存分区，分配失败！\n");
		return 0;
	}
}

// 回收内存分区
int freeMemoey(int num)
{
	int flag = 0;
	mem_block *nptr = headBlock.next, *tempNptr;
	while (nptr != NULL)
	{
		if (nptr->state == 1 && nptr->num == num)	// 根据作业号查找对应的内存块
		{
			flag = 1;

			// 相邻上下两分区为空闲，同时合并
			if ((nptr->prev != &headBlock && nptr->prev->state == 0) && (nptr->next != NULL && nptr->next->state == 0))
			{
				// 合并上区间
				tempNptr = nptr;
				nptr = nptr->prev;
				nptr->length += tempNptr->length;
				nptr->next = tempNptr->next;
				tempNptr->next->prev = nptr;
				free(tempNptr);
				// 合并下区间
				tempNptr = nptr->next;
				nptr->length += tempNptr->length;
				nptr->next = tempNptr->next;
				if (tempNptr->next != NULL)
				{
					tempNptr->next->prev = nptr;
				}
				free(tempNptr);		// 释放当前指针
			}
			// 只有相邻上分区为空闲
			else if ((nptr->prev != &headBlock && nptr->prev->state == 0) && (nptr->next == NULL || nptr->next->state == 1))
			{
				
				tempNptr = nptr;
				nptr = nptr->prev;
				nptr->length += tempNptr->length;
				nptr->next = tempNptr->next;
				if (tempNptr->next != NULL)
				{
					tempNptr->next->prev = nptr;
				}
				free(tempNptr);
			}
			// 只有相邻下分区为空闲
			else if ((nptr->prev == &headBlock || nptr->prev->state == 1) && (nptr->next != NULL && nptr->next->state == 0))
			{
				tempNptr = nptr->next;
				nptr->length += tempNptr->length;
				nptr->state = 0;
				nptr->num = -1;
				nptr->next = tempNptr->next;
				if (tempNptr->next != NULL)
				{
					tempNptr->next->prev = nptr;
				}
				free(tempNptr);
			}
			// 相邻上下分区都已分配，无需合并
			else
			{
				nptr->state = 0;
				nptr->num = -1;
			}
		}
		nptr = nptr->next;
	}
	if (flag == 1)
	{
		// 回收成功
		printf("内存分区回收成功...\n");
		return 1;
	}
	else
	{
		// 找不到目标作业，回收失败
		printf("找不到目标作业，内存分区回收失败...\n");
		return 0;
	}
}

// 显示内存分区情况
void showMemoey()
{
	printf("**********************************************\n");
	printf("**         当前的内存分配情况如下：         **\n");
	printf("**********************************************\n");
	printf("** 起始地址 | 空间大小 | 工作状态 | 作业号  **\n");
	mem_block *nptr = headBlock.next;
	while (nptr != NULL)
	{
		printf("**------------------------------------------**\n");
		printf("**");
		printf("  %4dk   |", nptr->addr);
		printf("  %3dk    |", nptr->length);
		printf("   %s   |", nptr->state == 0 ? "Free" : "Busy");
		if (nptr->num > 0)
		{
			printf("   %2d    ", nptr->num);
		}
		else
		{
			printf("         ");
		}
		printf("**\n");
		nptr = nptr->next;
	}
	printf("**********************************************\n");
}