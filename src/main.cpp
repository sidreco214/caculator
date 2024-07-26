#ifndef UNICODE
#define UNICODE
#endif

#pragma comment(linker, "/SUBSYSTEM:WINDOWS")

#include <stdlib.h>
#include "windows.h"

#include <string>
#include <vector>

#include "core/calc_tree.hpp"

bool is_caculated = false;
int num_op = 0;
int num_cp = 0;
std::string input_str;
std::string result_str = "";

extern "C" LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInstance;

extern "C" int APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow) {
    //hPrevInstance 사용안함, legacy 호환성
    //lpCmdLine은 프로그램을 실행할 때 전달되는 문자열, argv
    //nCmdShow는 창을 보통상태로 열 것인지, 최대화, 최소화된 상태로 열 것인지
    hInstance = hInst;

    HWND hWnd;
    MSG msg;
    WNDCLASS wndclass = {};

    static LPCWSTR wndclass_name = L"Caculator";

    wndclass.style = CS_HREDRAW | CS_VREDRAW; //창 높이, 너비 변경시 redraw
    wndclass.lpfnWndProc = WndProc;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIconW(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursorW(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(COLOR_WINDOW + 1);
    wndclass.lpszClassName = wndclass_name;
    if(!RegisterClassW(&wndclass)) return EXIT_FAILURE;

    hWnd = CreateWindowW(
        wndclass_name,
        L"Caculator",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        410,
        490,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    //Message 처리
    try{
        while(GetMessageW(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        
    }
    catch(std::exception& e) {
        MessageBoxA(hWnd, e.what(), "error", NULL);
    }
    return (int)msg.wParam;
}

typedef enum _Bottons {
    B0 = 0,
    B1, B2, B3, B4, B5, B6, B7, B8, B9, BDOT, BADD, BSUB, BMUL, BDIV, BPOW,
    BSQR, BSQRT, BCALC, BAC, BBACKSPACE, BOPENPARENTHESIS, BCLOSEPARENTHSIS,
    BEXP
} Bottons;

void btn_fn(HWND hWnd, Bottons btn, std::string& input_str, std::string& result_str) {
    if(btn != BAC && btn != BBACKSPACE && is_caculated) return;

    switch(btn) {
        case B0:
            input_str.push_back('0');
            break;

        case B1:
            input_str.push_back('1');
            break;

        case B2:
            input_str.push_back('2');
            break;

        case B3:
            input_str.push_back('3');
            break;

        case B4:
            input_str.push_back('4');
            break;

        case B5:
            input_str.push_back('5');
            break;

        case B6:
            input_str.push_back('6');
            break;

        case B7:
            input_str.push_back('7');
            break;

        case B8:
            input_str.push_back('8');
            break;

        case B9:
            input_str.push_back('9');
            break;

        case BDOT:
            input_str.push_back('.');
            break;

        case BADD:
            input_str.push_back('+');
            break;

        case BSUB:
            input_str.push_back('-');
            break;

        case BMUL:
            input_str.push_back('*');
            break;

        case BDIV:
            input_str.push_back('/');
            break;

        case BPOW:
            input_str.push_back('^');
            break;
        
        case BSQR:
            input_str.push_back('^');
            input_str.push_back('2');
            break;

        case BSQRT:
            input_str = input_str + "sqrt(";
            ++num_op;
            break;

        case BCALC:
            if(input_str.empty()) return;
            if(*input_str.rbegin() == '(') return;
            if(num_op - num_cp > 0) 
                for(int i=0; i<num_op-num_cp; ++i) input_str.push_back(')');
            [&]() {
                auto func = Calc::calc_tree(input_str);
                result_str = std::to_string(func.get());
                for(auto iter = result_str.rbegin(); iter != result_str.rend(); ++iter) {
                    if(*iter == '0' || *iter == '.') result_str.pop_back();
                    else if(*iter == '.') {
                        result_str.pop_back();
                        break;
                    }
                    else break;
                }
            }();
            is_caculated = true;
            input_str.push_back('=');
            break;

        case BAC:
            is_caculated = false;
            input_str = "";
            result_str = "";
            break;

        case BBACKSPACE:
            if(input_str.empty()) return;
            if(is_caculated) {
                is_caculated = false;
                result_str = "";
            }
            input_str.pop_back();
            break;

        case BOPENPARENTHESIS:
            input_str.push_back('(');
            ++num_op;
            break;

        case BCLOSEPARENTHSIS:
            if(num_op <= num_cp) return;
            if(*input_str.rbegin() == '(') return;
            input_str.push_back(')');
            ++num_cp;
            break;

        case BEXP:
            input_str = input_str + "exp(";
            ++num_op;
            break;
    }

    //update screen
    RECT rect;
    GetClientRect(hWnd, &rect);
    rect.bottom /= 3;
    InvalidateRect(hWnd, &rect, TRUE); 
};

void PutGridLayout(HWND hWnd, RECT& rect, int nrow, int ncol, std::vector<HWND>& v) {
    int width = static_cast<int>((rect.right - rect.left)/static_cast<float>(ncol) + 0.5);
    int height = static_cast<int>((rect.bottom - rect.top)/static_cast<float>(nrow) + 0.5);

    for(int i=0; i < nrow; ++i) {
        for(int j=0; j < ncol; ++j) {
            int index = i*ncol+j;
            if(index >= v.size()) return;
            SetWindowPos(
                v[index],
                HWND_TOP,
                rect.left + width*j,
                rect.top + height*i,
                width,
                height,
                SWP_ASYNCWINDOWPOS | SWP_SHOWWINDOW
            );
        }
    }
}

void CreateBtns(HWND hWnd, RECT& rect, int nrow, int ncol, std::vector<HWND>& buttons) {
    std::vector<std::pair<LPCWSTR, HMENU>> btns = {
        std::pair(L"(", (HMENU)BOPENPARENTHESIS),
        std::pair(L")", (HMENU)BCLOSEPARENTHSIS),
        std::pair(L"AC", (HMENU)BAC),
        std::pair(L"<-", (HMENU)BBACKSPACE),

        std::pair(L"x^y", (HMENU)BPOW),
        std::pair(L"x^2", (HMENU)BSQR),
        std::pair(L"\u221A", (HMENU)BSQRT),
        std::pair(L"exp", (HMENU)BEXP),

        std::pair(L"7", (HMENU)B7),
        std::pair(L"8", (HMENU)B8),
        std::pair(L"9", (HMENU)B9),
        std::pair(L"+", (HMENU)BADD),

        std::pair(L"4", (HMENU)B4),
        std::pair(L"5", (HMENU)B5),
        std::pair(L"6", (HMENU)B6),
        std::pair(L"-", (HMENU)BSUB),

        std::pair(L"1", (HMENU)B1),
        std::pair(L"2", (HMENU)B2),
        std::pair(L"3", (HMENU)B3),
        std::pair(L"*", (HMENU)BMUL),

        std::pair(L".", (HMENU)BDOT),
        std::pair(L"0", (HMENU)B0),
        std::pair(L"=", (HMENU)BCALC),
        std::pair(L"/", (HMENU)BADD)
    };

    HWND hWnd_child;
    int btn_width = static_cast<int>((rect.right - rect.left)/static_cast<float>(ncol) + 0.5);
    int btn_height = static_cast<int>((rect.bottom - rect.top)/static_cast<float>(nrow) + 0.5);

    for(int i=0; i < nrow; ++i) {
        for(int j=0; j < ncol; ++j) {
            int index = i*ncol+j;
            if(index >= btns.size()) return;
            hWnd_child = CreateWindowW(
                L"BUTTON",
                btns[index].first,
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                rect.left + btn_width*j,
                rect.top + btn_height*i,
                btn_width,
                btn_height,
                hWnd,
                btns[index].second,
                hInstance,
                NULL
            );
            buttons.push_back(hWnd_child);
        }
    }

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC hDC;
    PAINTSTRUCT ps;
    RECT rect1;
    RECT rect2;
    RECT btn_grid_rect;

    HFONT hFont;
    HFONT hFont_old;

    static std::vector<HWND> buttons;

    switch(message) {
        case WM_CREATE:
            GetClientRect(hWnd, &rect1);
            btn_grid_rect.left = rect1.left;
            btn_grid_rect.right = rect1.right;
            btn_grid_rect.top = rect1.bottom/3;
            btn_grid_rect.bottom = rect1.bottom;
            CreateBtns(hWnd, btn_grid_rect, 6, 4, buttons);
            return 0;
        
        case WM_DESTROY:
            PostQuitMessage(1);
            return 0;
        
        case WM_SIZE:
            GetClientRect(hWnd, &rect1);
            btn_grid_rect.left = rect1.left;
            btn_grid_rect.right = rect1.right;
            btn_grid_rect.top = rect1.bottom/3;
            btn_grid_rect.bottom = rect1.bottom;
            PutGridLayout(hWnd, btn_grid_rect, 6, 4, buttons);
        
        case WM_PAINT:
            GetClientRect(hWnd, &rect1);
            rect2.left = rect1.left + 20;
            rect2.right = rect1.right - 20;
            rect2.top = rect1.top + 40;
            rect2.bottom = rect1.bottom / 6;

            hDC = BeginPaint(hWnd, &ps);
            hFont = CreateFontW(
                40,
                0,
                0,
                0,
                FW_DONTCARE,
                FALSE,
                FALSE,
                FALSE,
                DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_SWISS,
                L"Arial"
            );
            hFont_old = (HFONT)SelectObject(hDC, hFont);

            //draw input str
            //InvalidateRect(hWnd, &rect2, TRUE);
            DrawTextA(
                hDC,
                input_str.c_str(),
                -1, //null 문자까지 읽기
                &rect2,
                DT_SINGLELINE | DT_RIGHT | DT_VCENTER
            );

            //draw result
            rect2.top = rect1.bottom / 6;
            rect2.bottom = rect1.bottom / 3;
            //InvalidateRect(hWnd, &rect2, TRUE);
            DrawTextA(
                hDC,
                result_str.c_str(),
                -1, //null 문자까지 읽기
                &rect2,
                DT_SINGLELINE | DT_RIGHT | DT_VCENTER
            );
            SelectObject(hDC, hFont_old);
            DeleteObject(hFont);
            EndPaint(hWnd, &ps);
            return 0;
        
        case WM_COMMAND:
            btn_fn(hWnd, (Bottons)LOWORD(wParam), input_str, result_str);
            return 0;
    }

    return DefWindowProcW(hWnd, message, wParam, lParam);
}