/***********************************************************************
 * Filename:            dllApi.c
 * Version:             V0.1
 * Description:         实现了双向链表的初始化、增删节点、判空计数等借口
 *
 * Authors:             moth96
 **********************************************************************/
#include "dllApi.h"

/********************************************************
 * Function:            listInit
 * Description:         初始化节点
 * input:               @node:  要初始化的双向链表节点
 * output:              NULL
 * return:              成功返回:       DLLAPI_OK
 *                      其他情况返回:   DLLAPI_ERROR
 * calls:               NULL
 *******************************************************/
int listInit(LIST_NODE* node)
{
    if (NULL == node)
    {
        LOG_ERROR("null pointer error\n");
        return DLLAPI_ERROR;
    }

    node->next = node;
    node->prev = node;

    return DLLAPI_OK;
}

/********************************************************
 * Function:            listAdd
 * Description:         在指定节点后面添加一个节点
 * input:               @node:      链表中的节点
 *                      @newNode:   要添加的新节点
 * output:              NULL
 * return:              成功返回:       DLLAPI_OK
 *                      其他情况返回:   DLLAPI_ERROR
 * calls:
 *******************************************************/
int listAdd(LIST_NODE* node, LIST_NODE* newNode)
{
    if (NULL == node || NULL == newNode)
    {
        LOG_ERROR("null pointer error\n");
        return DLLAPI_ERROR;
    }

    if (NULL == node->next || NULL == node->prev)
    {
        LOG_INFO("add failed, node not in list\n");
        return DLLAPI_ERROR;
    }

    node->next->prev = newNode;
    newNode->next = node->next;
    newNode->prev = node;
    node->next = newNode;

    return DLLAPI_OK;
}

/********************************************************
 * Function:            listDel
 * Description:         删除链表中的节点
 * input:               @node:  要删除的节点
 * output:              NULL
 * return:              成功返回:       DLLAPI_OK
 *                      其他情况返回:   DLLAPI_ERROR
 * calls:               NULL
 *******************************************************/
int listDel(LIST_NODE* node)
{
    if (NULL == node)
    {
        LOG_ERROR("null pointer error\n");
        return DLLAPI_ERROR;
    }

    if (NULL == node->next || NULL == node->prev)
    {
        LOG_INFO("del failed, node not in list\n");
        return DLLAPI_ERROR;
    }

    if (node == node->next || node == node->prev)
    {
        LOG_INFO("del failed, can not delete head node\n");
        return DLLAPI_ERROR;
    }

    node->prev->next = node->next;
    node->next->prev = node->prev;

    /* 删除后node前后指针指向NULL */
    node->next = NULL;
    node->prev = NULL;

    return DLLAPI_OK;
}

/********************************************************
 * Function:            listIsEmpty
 * Description:         判断链表是否为空
 * input:               @node:  链表中的节点
 * output:              NULL
 * return:              为空返回1，不为空返回0
 *                      其他情况返回:   DLLAPI_ERROR
 * calls:               NULL
 *******************************************************/
int listIsEmpty(LIST_NODE* node)
{
    if (NULL == node)
    {
        LOG_ERROR("null poiner error\n");
        return DLLAPI_ERROR;
    }

    if (NULL == node->next || NULL == node->prev)
    {
        LOG_INFO("node not in list\n");
        return DLLAPI_ERROR;
    }

    return node == node->next;
}

int listSwap(LIST_NODE* node1, LIST_NODE* node2)
{
    LIST_NODE tmp;

    if (NULL == node1 || NULL == node2)
    {
        LOG_ERROR("null pointer error\n");
        return DLLAPI_ERROR;
    }

    if (!node1->next || !node1->prev || !node2->next || !node2->prev)
    {
        LOG_INFO("node not in list\n");
        return DLLAPI_ERROR;
    }

    node1->next->prev = &tmp;
    node1->prev->next = &tmp;
    tmp.next = node1->next;
    tmp.prev = node1->prev;

    node2->next->prev = node1;
    node2->prev->next = node1;
    node1->next = node2->next;
    node1->prev = node2->prev;

    tmp.next->prev = node2;
    tmp.prev->next = node2;
    node2->next = tmp.next;
    node2->prev = tmp.prev;

    return DLLAPI_OK;
}

/********************************************************
 * Function:            listNodeNum
 * Description:         计算链表中节点数目
 * input:               @node:  链表中的节点
 * output:              NULL
 * return:              返回链表节点数目
 *                      异常时返回0
 * calls:               NULL
 *******************************************************/
int listNodeNum(LIST_NODE* node)
{
    int num = 0;
    LIST_NODE* index = NULL;

    if (NULL == node)
    {
        LOG_ERROR("null pointer error\n");
        return 0;
    }

    if (NULL == node->next || NULL == node->prev)
    {
        LOG_INFO("node not in list\n");
        return 0;
    }

    for (index = node; index->next != node; index = index->next)
    {
        num++;
    }

    return num;
}