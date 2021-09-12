#include <stdio.h>
#include "dllApi.h"
#include <string.h>
#include <stdlib.h>

#define STU_INFO_HEAD(ptr) listEntry(ptr, STU_INFO, list)

typedef int (*cmpFunc)(int, int);

typedef struct __student_info
{
    LIST_NODE list;
    int stuID;
    int stuScore;
    char stuName[16];
} STU_INFO;

typedef struct __global_variable
{
    int classAStuNum;
    int classBStuNum;
    int classCStuNum;
    STU_INFO classA;
    STU_INFO classB;
    STU_INFO classC;
    STU_INFO* classAArr;
    STU_INFO* classBArr;
} GLABAL_VAR;

GLABAL_VAR gVar;

int isBigger(int a, int b)
{
    return a > b ? 1 : 0;
}

int isSmaller(int a, int b)
{
    return a < b ? 1 : 0;
}

int sortList(STU_INFO* head, cmpFunc cmp)
{
    STU_INFO* pos1 = NULL;
    STU_INFO* pos2 = NULL;
    STU_INFO* extreMum = NULL;

    if (NULL == head || NULL == cmp)
    {
        LOG_ERROR("input null pointer");
        return -1;
    }

    for(pos1 = STU_INFO_HEAD(head->list.next); pos1 != STU_INFO_HEAD(head->list.prev); )
    {
        extreMum = pos1;
        for(pos2 = STU_INFO_HEAD(pos1->list.next); pos2 != head; pos2 = STU_INFO_HEAD(pos2->list.next))
        {
            if (cmp(pos2->stuScore, extreMum->stuScore))
            {
                extreMum = pos2;
            }
        }
        if (extreMum != pos1)
        {
            if (extreMum != STU_INFO_HEAD(pos1->list.next))
            {
                listSwap(&pos1->list, &extreMum->list);
                pos1 = STU_INFO_HEAD(extreMum->list.next);
            }
            else
            {
                listSwap(&pos1->list, &extreMum->list);
            }
        }
        else
        {
            pos1 = STU_INFO_HEAD(pos1->list.next);
        }
    }

    return 0;
}

int loadData(STU_INFO* classArr, STU_INFO* head, int size)
{
    int index = 0;

    if (NULL == classArr || NULL == head)
    {
        LOG_ERROR("input null pointer");
        return -1;
    }

    for (index = 0; index < size; index++)
    {
        scanf("%d %s %d", &classArr[index].stuID,
                &classArr[index].stuName[16], &classArr[index].stuScore);
        listAdd(&head->list, &classArr[index].list);
    }

    return 0;
}

void freeData(void)
{
    if (gVar.classAArr)
    {
        free(gVar.classAArr);
        gVar.classAArr = NULL;
    }
    if (gVar.classBArr)
    {
        free(gVar.classBArr);
        gVar.classBArr = NULL;
    }
}

int scanData(void)
{
    memset(&gVar, 0, sizeof(GLABAL_VAR));
    listInit(&gVar.classA.list);
    listInit(&gVar.classB.list);
    listInit(&gVar.classC.list);

    scanf("%d %d %d", &gVar.classAStuNum, &gVar.classBStuNum, &gVar.classCStuNum);

    gVar.classAArr = (STU_INFO*)calloc(gVar.classAStuNum, sizeof(STU_INFO));
    gVar.classBArr = (STU_INFO*)calloc(gVar.classBStuNum, sizeof(STU_INFO));

    if (NULL == gVar.classAArr || NULL == gVar.classBArr)
    {
        LOG_INFO("calloc ERROR");
        goto scanDataErrorExit;
    }

    loadData(gVar.classAArr, &gVar.classA, gVar.classAStuNum);
    loadData(gVar.classBArr, &gVar.classB, gVar.classBStuNum);

    return 0;

scanDataErrorExit:
    freeData();
    return -1;
}

int mergeClass(void)
{
    int classCNum = gVar.classCStuNum;
    STU_INFO* indexA = NULL;
    STU_INFO* indexB = NULL;
    STU_INFO* next = NULL;

    gVar.classA.stuScore = -1;
    gVar.classB.stuScore = -1;

    indexA = STU_INFO_HEAD(gVar.classA.list.next);
    indexB = STU_INFO_HEAD(gVar.classB.list.next);

    while(classCNum--)
    {
        if (indexA->stuScore >= indexB->stuScore)
        {
            if (indexA->stuScore >= 0)
            {
                next = STU_INFO_HEAD(indexA->list.next);
                listAdd(&gVar.classC.list, &indexA->list);
                indexA = next;
            }
        }
        else
        {
            next = STU_INFO_HEAD(indexB->list.next);
            listAdd(&gVar.classC.list, &indexB->list);
            indexB = next;
        }
    }
    return 0;
}

void printData(STU_INFO* head)
{
    STU_INFO* pos = NULL;

    listForEachEntry(pos, &(head->list), list)
    {
        printf("%07d %s %d\n", pos->stuID, pos->stuName, pos->stuScore);
    }
}

int main(int argc, char* argv[])
{
    scanData();
    printf("=====classA=====\n");
    printData(&gVar.classA);
    printf("\n");

    sortList(&gVar.classA, isBigger);
    printData(&gVar.classA);
    printf("\n");

    printf("=====classB=====\n");
    printData(&gVar.classB);
    printf("\n");

    sortList(&gVar.classB, isBigger);
    printData(&gVar.classB);
    printf("\n");

    mergeClass();

    printf("=====classC=====\n");
    printData(&gVar.classC);

    freeData();


    //printf("classA.stuID:%d\n", gVar.classA.stuID);
    return 0;
}