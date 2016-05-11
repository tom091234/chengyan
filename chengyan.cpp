// chengyan.cpp : ����Ӧ�ó������ڵ㡣
//


#include "stdafx.h"
#include "chengyan.h"

#define MAX_LOADSTRING 260

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];            // ����������
TCHAR docRoot[] = TEXT("E:\\github\\chengyan\\doc");
tTree *pTree = NULL;
HWND hwndTreeContent;                   // �������ṹ�ؼ����

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
HTREEITEM insertTree(HWND hTree, HTREEITEM hRoot, LPCTSTR name, bool children, LPARAM lParam);
HTREEITEM AddItemToTree(HWND hwndTV, HTREEITEM hParent, LPWSTR lpszItem, int nLevel);
int BuildTree(HWND hwndTV, tTree *tree);
int AddChildren(HWND hwndTV, HTREEITEM hParent, tNode **NodeLoop);
void FindFiles(TCHAR *folderPath, DWORD level);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CHENGYAN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CHENGYAN));

    MSG msg;

    // ����Ϣѭ��: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CHENGYAN));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CHENGYAN);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        RECT rt;

    case WM_CREATE:
        FindFiles(docRoot, 0);
        GetClientRect(hWnd, &rt);

        hwndTreeContent = CreateWindowEx(0, WC_TREEVIEW, NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT,
            0, 0, rt.right, rt.bottom, hWnd, NULL, hInst, (LPVOID)NULL);
        BuildTree(hwndTreeContent, pTree);
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �����˵�ѡ��: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

HTREEITEM AddItemToTree(HWND hwndTV, HTREEITEM hParent, LPWSTR lpszItem, int nLevel)
{
    TVITEM tvi;
    TVINSERTSTRUCT tvins;

    tvi.mask = TVIF_TEXT | TVIF_IMAGE
        | TVIF_SELECTEDIMAGE | TVIF_PARAM;

    tvi.pszText = lpszItem;
    tvi.cchTextMax = lstrlen(lpszItem);

    tvi.iImage = NULL;
    tvi.iSelectedImage = NULL;

    tvi.lParam = (LPARAM)nLevel;

    tvins.item = tvi;
    tvins.hInsertAfter = TVI_LAST;
    tvins.hParent = hParent;

    return TreeView_InsertItem(hwndTV, &tvins);
}

int BuildTree(HWND hwndTV, tTree *tree)
{
    if (NULL == tree)
    {
        return 0;
    }
    tNode *NodeLoop = NULL;

    NodeLoop = tree->first_node;
    AddChildren(hwndTV, TVI_ROOT, &NodeLoop);
    return 0;
}

int AddChildren(HWND hwndTV, HTREEITEM hParent, tNode **NodeLoop)
{
    HTREEITEM hNewParent;

    for (; NULL != *NodeLoop;)
    {
        TVITEM tvi = { 0 };
        tvi.mask = TVIF_PARAM;
        tvi.hItem = hParent;
        if (TVI_ROOT != hParent)
        {
            TreeView_GetItem(hwndTV, &tvi);
        }
        if (tvi.lParam + 1 == (int)((*NodeLoop)->dwLevel))
        {
            hNewParent = (HTREEITEM)AddItemToTree(hwndTV, hParent, (*NodeLoop)->pbyName, (*NodeLoop)->dwLevel);
            if (NULL != (*NodeLoop)->next)
            {
                if ((*NodeLoop)->dwLevel < (*NodeLoop)->next->dwLevel)
                {
                    *NodeLoop = (*NodeLoop)->next;
                    AddChildren(hwndTV, hNewParent, NodeLoop);
                    if (NULL == *NodeLoop || tvi.lParam + 1 != (int)((*NodeLoop)->dwLevel))
                    {
                        return 0;
                    }
                }
                else if ((*NodeLoop)->dwLevel >(*NodeLoop)->next->dwLevel)
                {
                    *NodeLoop = (*NodeLoop)->next;
                    return 0;
                }
                else
                {
                    *NodeLoop = (*NodeLoop)->next;
                }
            }
            else
            {
                *NodeLoop = (*NodeLoop)->next;
                return 0;
            }
        }
    }
    return 0;
}

tTree * GetNewTree()
{
    tTree *pTree =  (tTree *)(malloc(sizeof(tTree)));
    if (NULL != pTree)
    {
        pTree->first_node = NULL;
        pTree->last_node = NULL;
    }

    return pTree;
}

tNode * GetNewNode()
{
    tNode *pNode = (tNode *)(malloc(sizeof(tNode)));
    if (NULL != pNode)
    {
        pNode->dwLevel = 0;
        pNode->dwAttr = 0;
        pNode->pbyName = (TCHAR *)(malloc(sizeof(TCHAR)*MAX_LOADSTRING));
        pNode->path = (TCHAR *)(malloc(sizeof(TCHAR)*MAX_LOADSTRING));
        pNode->next = NULL;
    }

    return pNode;
}

void AddNode2Tree(tTree *pTree, tNode *pNode)
{
    if (NULL == pTree->first_node)
    {
        pTree->first_node = pNode;
    }
    else
    {
        pTree->last_node->next = pNode;
    }
    pTree->last_node = pNode;
}

void FreeNode(tNode **ppNode)
{
    free((*ppNode)->pbyName);
    free((*ppNode)->path);
    free(*ppNode);
    *ppNode = NULL;
}

void FreeTree(tTree **ppTree)
{
    tNode *nodeTmp;
    tNode *nodeLoop = (*ppTree)->first_node;

    while (NULL != nodeLoop)
    {
        nodeTmp = nodeLoop;
        nodeLoop = nodeLoop->next;
        FreeNode(&nodeTmp);
    }

    free(*ppTree);
    *ppTree = NULL;
}

void FindFiles(TCHAR *folderPath, DWORD level)
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;
    TCHAR fileFilter[MAX_LOADSTRING];

    if (NULL == pTree)
    {
        pTree = GetNewTree();
    }
    level += 1;
    _tcscpy_s(fileFilter, MAX_LOADSTRING, folderPath);
    _tcscat_s(fileFilter, MAX_LOADSTRING, TEXT("\\*.*"));

    hFind = FindFirstFile(fileFilter, &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
    }
    else
    {
        while (FindNextFile(hFind, &FindFileData))
        {
            if (0 == _tcscmp(TEXT("."), FindFileData.cFileName) && (FILE_ATTRIBUTE_DIRECTORY == FindFileData.dwFileAttributes))
            {
            }
            else if (0 == _tcscmp(TEXT(".."), FindFileData.cFileName) && (FILE_ATTRIBUTE_DIRECTORY == FindFileData.dwFileAttributes))
            {
            }
            else if (FILE_ATTRIBUTE_DIRECTORY == FindFileData.dwFileAttributes)
            {
                tNode *pNode = GetNewNode();
                pNode->dwLevel = level;
                pNode->dwAttr = FindFileData.dwFileAttributes;
                _tcscpy_s(pNode->pbyName, MAX_LOADSTRING, FindFileData.cFileName);
                _tcscpy_s(pNode->path, MAX_LOADSTRING, folderPath);
                _tcscat_s(pNode->path, MAX_LOADSTRING, TEXT("\\"));
                _tcscat_s(pNode->path, MAX_LOADSTRING, FindFileData.cFileName);
                AddNode2Tree(pTree, pNode);
                FindFiles(pNode->path, level);
            }
            else
            {
                tNode *pNode = GetNewNode();
                pNode->dwLevel = level;
                pNode->dwAttr = FindFileData.dwFileAttributes;
                _tcscpy_s(pNode->pbyName, MAX_LOADSTRING, FindFileData.cFileName);
                _tcscpy_s(pNode->path, MAX_LOADSTRING, folderPath);
                _tcscat_s(pNode->path, MAX_LOADSTRING, TEXT("\\"));
                _tcscat_s(pNode->path, MAX_LOADSTRING, FindFileData.cFileName);
                AddNode2Tree(pTree, pNode);
            }
        }
    }

    if (NULL == pTree->first_node)
    {
        FreeTree(&pTree);
    }
    return;
}

