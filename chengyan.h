#pragma once

#include "resource.h"

typedef struct TNode
{
    TCHAR           *pbyName;       /*��ʾ�ڽ����ϵĽڵ�����*/
    TCHAR           *path;          /*�ļ�·��*/
    DWORD           dwLevel;        /*�ڵ��Σ����ڵ�Ϊ1*/
    DWORD           dwAttr;
    struct TNode    *next;
}tNode;

typedef struct TTree
{
    tNode           *first_node;
    tNode           *last_node;
}tTree;
