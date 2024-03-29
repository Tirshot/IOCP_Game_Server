﻿// GameCoding.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "pch.h"
#include "GameCoding.h"
#include "Game.h"
#include "Sprite.h"
#include "Texture.h"
#include "ChatInput.h"
#include "TimeManager.h"
#include "ThreadManager.h"
#include "ChatManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "MyPlayer.h"
#include "DevScene.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND g_hWnd;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;

    // 시스템의 로케일 사용
    setlocale(LC_ALL, "");

    // 게임 초기화
    Game game;
    game.Init(g_hWnd);
    MSG msg = {};


    // 기본 메시지 루프입니다:
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
           game.Update();
           game.Render();
        }
    }
    return (int)msg.wParam;
}


//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex{};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(107));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"GameCoding";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(108));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    RECT windowRect = { 0, 0, 800, 600 };
    ::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

    HWND hWnd = CreateWindowW(L"GameCoding", L"Client", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, hInstance, nullptr);

    g_hWnd = hWnd;


    if (!hWnd)
        return FALSE;

    ::ShowWindow(hWnd, nCmdShow);
    ::UpdateWindow(hWnd);

    return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
bool isChatting = false;
static WCHAR str1[256];
int len = 0;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            isChatting = false;
            memset(str1, 0, sizeof(str1));
            GET_SINGLE(ChatManager)->SetInvisibleChat();
            GET_SINGLE(ChatManager)->SetVisibleChatInput(false);

            MyPlayer* myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
            myPlayer->SetState(IDLE);
        }

        // 엔터 키를 누르면 채팅 입력 시작 또는 종료
        if (wParam == VK_RETURN)
        {
            isChatting = !isChatting;

            // 조합 중인 문자를 출력
            if (isChatting)
            {
                HIMC hImc = ImmGetContext(hWnd);
                int compLen = ImmGetCompositionString(hImc, GCS_COMPSTR, NULL, 0);

                // 조합 중인 문자열이 있는 경우
                if (compLen > 0)
                {
                    // 조합 중인 문자열을 가져오기
                    WCHAR* buffer = new WCHAR[compLen + 1];
                    int length = ImmGetCompositionString(hImc, GCS_COMPSTR, buffer, compLen);
                    buffer[length] = L'\0';

                    // 현재까지 입력된 문자열과 조합 중인 문자열을 합쳐서 출력
                    wcscpy_s(str1 + len, 256 - len, buffer);
                    len += length;

                    delete[] buffer;

                    // 조합 상태 초기화
                    ImmNotifyIME(hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
                }

                InvalidateRect(hWnd, NULL, FALSE);
                GET_SINGLE(ChatManager)->SetVisibleChat();
                GET_SINGLE(ChatManager)->SetVisibleChatInput(true);

                ImmReleaseContext(hWnd, hImc);
            }
        }
        break;

    case WM_CHAR:
        if (isChatting)
        {
            if (wParam == VK_BACK)
            {
                if (len > 0)
                {
                    len--;
                    str1[len + 1] = L'\0';
                    InvalidateRect(hWnd, NULL, FALSE);
                    GET_SINGLE(ChatManager)->GetChatInput()->RemoveTextChar(str1, len + 1);
                    return 0;
                }
            }
            len = (int)wcslen(str1);
            str1[len] = static_cast<WCHAR>(wParam);
            str1[len + 1] = L'\0';

            // 현재 조합 중인 문자열을 그대로 출력
            GET_SINGLE(ChatManager)->SetVisibleChat();
            GET_SINGLE(ChatManager)->GetChatInput()->AddTextChar(str1, len + 1);
        }
        else if (isChatting == false && wParam == VK_RETURN)
        {
            MyPlayer* myPlayer = GET_SINGLE(SceneManager)->GetMyPlayer();
            if (myPlayer && myPlayer->info.state() == CHAT)
            {
                str1[0] = L' ';
                GET_SINGLE(ChatManager)->SendMessageToServer(str1);
                memset(str1, 0, sizeof(str1));
            }
            GET_SINGLE(ChatManager)->SetVisibleChatInput(false);
        }
        break;
    
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case 105:
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