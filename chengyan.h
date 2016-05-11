#pragma once

#include "resource.h"

typedef struct TNode
{
    TCHAR           *pbyName;       /*显示在界面上的节点名字*/
    TCHAR           *path;          /*文件路径*/
    DWORD           dwLevel;        /*节点层次，根节点为1*/
    DWORD           dwAttr;
    struct TNode    *next;
}tNode;

typedef struct TTree
{
    tNode           *first_node;
    tNode           *last_node;
}tTree;
