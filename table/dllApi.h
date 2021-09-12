/***********************************************************************
 * Filename:            dllApi.h
 * Version:             V0.1
 * Description:         提供双向链表的通用接口
 *                      参考了linux内核中双向链表的实现方式
 *
 * Authors:             moth96
 **********************************************************************/
#ifndef __DLLAPI_H__
#define __DLLAPI_H__

#include <stdio.h>
#include "log.h"

#define DLLAPI_OK 0
#define DLLAPI_ERROR -1

struct __list_node;
typedef struct __list_node LIST_NODE;

struct __list_node
{
    struct __list_node* prev;
    struct __list_node* next;
};

/********************************************************
 * Function:            OFFSETOF
 * Description:         返回结构体成员地址相对于首地址的偏移
 * input:               @type:  结构体类型
 *                      @member:结构体成员名
 * output:              NULL
 * return:              返回结构体成员地址相对于首地址的偏移
 * calls:               NULL
 *******************************************************/
#define OFFSETOF(type, member) ((size_t) &((type *)0)->member)

/********************************************************
 * Function:            CONTAINER_OF
 * Description:         通过结构体成员的首地址获得整个结构体
 *                      变量的首地址
 * input:               @ptr:   结构体中member的地址
 *                      @type:  结构体类型
 *                      @member:结构体成员名
 * output:              NULL
 * return:              结构体首地址
 * calls:               typeof
 *                      OFFSETOF
 *******************************************************/
#define CONTAINER_OF(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - OFFSETOF(type, member) );})

/********************************************************
 * Function:            list_entry
 * Description:         通过结构体成员的首地址获得整个结构体
 *                      变量的首地址
 * input:               @ptr:   结构体中member的地址
 *                      @type:  结构体类型
 *                      @member:结构体成员名
 * output:              NULL
 * return:              结构体首地址
 * calls:               CONTAINER_OF
 *******************************************************/
#define listEntry(ptr, type, member)                            \
	    CONTAINER_OF(ptr, type, member)

/********************************************************
 * Function:            @pos:   指向结构体的指针
 *                      @head:  链表节点
 *                      @member:结构体成员名listFirstEntry
 * Description:         返回链表指针ptr下一个节点结构体首地址
 * input:               @ptr:   结构体中member的地址
 *                      @type:  结构体类型
 *                      @member:结构体成员名
 * output:              NULL
 * return:              返回ptr下一个节点结构体首地址
 * calls:               listEntry
 *******************************************************/
#define listFirstEntry(ptr, type, member)                       \
	    listEntry((ptr)->next, type, member)

/********************************************************
 * Function:            listNextEntry
 * Description:         返回结构体指针pos下一个节点结构体首地址
 * input:               @pos:   指向结构体的指针
 *                      @member:结构体成员名
 * output:              NULL
 * return:              返回结构体指针pos下一个节点结构体首地址
 * calls:               listEntry
 *******************************************************/
#define listNextEntry(pos, member)                              \
	    listEntry((pos)->member.next, typeof(*(pos)), member)

/********************************************************
 * Function:            listEntryIsHead
 * Description:         判断结构体中链表节点是否头节点
 * input:               @pos:   指向结构体的指针
 *                      @head:  链表节点
 *                      @member:结构体成员名
 * output:              NULL
 * return:              是头结点返回1，不是返回0
 * calls:               NULL
 *******************************************************/
#define listEntryIsHead(pos, head, member)				        \
	    (&pos->member == (head))

/********************************************************
 * Function:            listForEachEntry
 * Description:         通过链表节点遍历含有链表的结构体
 * input:               @pos:   指向结构体的指针
 *                      @head:  链表节点
 *                      @member:结构体成员名
 * output:              NULL
 * return:              NULL
 * calls:               listFirstEntry
 *                      listEntryIsHead
 *                      listNextEntry
 *******************************************************/
#define listForEachEntry(pos, head, member)				        \
	for (pos = listFirstEntry(head, typeof(*(pos)), member);	\
	     !listEntryIsHead(pos, head, member);			        \
	     pos = listNextEntry(pos, member))

int listInit(LIST_NODE* node);
int listAdd(LIST_NODE* node, LIST_NODE* newNode);
int listDel(LIST_NODE* node);
int listIsEmpty(LIST_NODE* node);
int listSwap(LIST_NODE* node1, LIST_NODE* node2);

#endif