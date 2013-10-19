#include"lwt.h"
//#include"Node.h"
#include"Queue.h"
#include<malloc.h>
#include<stdio.h>

/*构造一个空队列*/
Queue *InitQueue()
{
	Queue *pqueue = (Queue *)malloc(sizeof(Queue));
	if(pqueue!=NULL)
	{
		pqueue->front = NULL;
		pqueue->rear = NULL;
		pqueue->size = 0;
	}
	return pqueue;
}

/*销毁一个队列*/
void DestroyQueue(Queue *pqueue)
{
	if(IsEmpty(pqueue)!=1)
		ClearQueue(pqueue);
	free(pqueue);
}

/*清空一个队列*/
void ClearQueue(Queue *pqueue)
{
	while(IsEmpty(pqueue)!=1)
	{
		DeQueue(pqueue,NULL);
	}

}

/*判断队列是否为空*/
int IsEmpty(Queue *pqueue)
{
	if(pqueue->front==NULL&&pqueue->rear==NULL&&pqueue->size==0)
		return 1;
	else
		return 0;
}

/*返回队列大小*/
int GetSize(Queue *pqueue)
{
	return pqueue->size;
}

/*返回队头元素*/
PNode GetFront(Queue *pqueue,Item *pitem)
{
	if(IsEmpty(pqueue)!=1&&pitem!=NULL)
	{
		*pitem = pqueue->front->data;
	}
	return pqueue->front;
}

/*返回队尾元素*/

PNode GetRear(Queue *pqueue,Item *pitem)
{
	if(IsEmpty(pqueue)!=1&&pitem!=NULL)
	{
		*pitem = pqueue->rear->data;
	}
	return pqueue->rear;
}

/*将新元素入队*/
PNode EnQueue(Queue *pqueue,Item item)
{
	PNode pnode = (PNode)malloc(sizeof(Node));
	if(pnode != NULL)
	{
		pnode->data = item;
		pnode->next = NULL;
		
		if(IsEmpty(pqueue))
		{
			pqueue->front = pnode;
		}
		else
		{
			pqueue->rear->next = pnode;
		}
		pqueue->rear = pnode;
		pqueue->size++;
	}
	return pnode;
}

/*队头元素出队*/
PNode DeQueue(Queue *pqueue,Item *pitem)
{
	PNode pnode = pqueue->front;
	if(IsEmpty(pqueue)!=1&&pnode!=NULL)
	{
		if(pitem!=NULL)
			*pitem = pnode->data;
		pqueue->size--;
		pqueue->front = pnode->next;
		free(pnode);
		if(pqueue->size==0)
			pqueue->rear = NULL;
	}
	return pqueue->front;
}

/*遍历队列并对各数据项调用visit函数*/
void QueueTraverse(Queue *pqueue,void (*visit)())
{
	PNode pnode = pqueue->front;
	int i = pqueue->size;
	while(i--)
	{
		visit(pnode->data);
		pnode = pnode->next;
	}
		
}

/******************Circle Queue********************/

/*add new item to CicleQueue*/
/*
PNode EnCircleQueue(Queue *pqueue,Item item)
{
	PNode pnode = (PNode)malloc(sizeof(Node));
	if(pnode != NULL)
	{
		pnode->data = item;
		
		if(IsEmpty(pqueue))
		{
			pqueue->front = pnode;
		}
		else
		{
			pqueue->rear->next = pnode;
		}

		pnode->next = pqueue->front;
		pqueue->rear = pnode;
		pqueue->size++;
	}
	return pnode;
}
*/
/*Get item out of CircleQueue*/

PNode DeCircleQueue(Queue *pqueue,Item *pitem)
{
	PNode pnode = pqueue->front;
	if(IsEmpty(pqueue)!=1&&pnode!=NULL)
	{
		if(pitem!=NULL)
			*pitem = pnode->data;
		pqueue->size--;
		pqueue->front = pnode->next;
		pqueue->rear->next =pqueue->front;
		free(pnode);
		if(pqueue->size==0){
			pqueue->front = NULL;	
			pqueue->rear = NULL;
		}
	}
	return pqueue->front;
}


/*move CircleQueue to next node and return new node*/
PNode MoveCircleQueue(Queue *pqueue,Item *pitem)
{
	if(IsEmpty(pqueue)!=1&&pitem!=NULL)
	{
		pqueue->front=pqueue->front->next;
		pqueue->rear = pqueue->rear->next;
		*pitem = pqueue->front->data;
	}
	return pqueue->front;
}

PNode EnCircleQueue(Queue *pqueue,Item item)
{
	PNode pnode = (PNode)malloc(sizeof(Node));
	if(pnode != NULL)
	{
		pnode->data = item;
		
		if(IsEmpty(pqueue))
		{
			pnode->next=pnode;
			pqueue->rear = pnode;
		}
		else
		{
			pnode->next = pqueue->front;
			pqueue->rear->next = pnode;
		}
		pqueue->front = pnode;
		pqueue->size++;
	}
	return pnode;
}

