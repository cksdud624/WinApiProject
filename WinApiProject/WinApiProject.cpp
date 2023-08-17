// WinApiProject.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WinApiProject.h"

using namespace std;
using namespace Gdiplus;
#define MAX_LOADSTRING 100

ULONG_PTR m_gdiplusToken;

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.


#ifdef UNICODE

#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 

#else

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 

#endif



//함수
void Update();
void DrawDoubleBuffering(HDC& hdc);
void StartSetting(HDC& hdc);
void PlayerSystem();
void MonsterSystem(vector<POINT>& route);
void PlayerDraw(Graphics& g);
void MonsterDraw(Graphics& g);
void UISetting();
void UIDraw(Graphics& g);

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPIPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPIPROJECT));

    MSG msg;

    clock_t oldtime = clock();
    clock_t newtime;
    clock_t newframetime;
    clock_t oldframetime = clock();

    int framecheck = 0;

    // 기본 메시지 루프입니다:
    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            newtime = newframetime = clock();
            if (newtime - oldtime >= 34)
            {
                oldtime = newtime;
                framecheck++;
                Update();
            }
            if (newframetime - oldframetime >= 34 * 30)
            {
                oldframetime = newframetime;
                //cout << framecheck << endl;
                framecheck = 0;
            }

        }
    }

    GdiplusShutdown(m_gdiplusToken);

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPIPROJECT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINAPIPROJECT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
static HWND hWnd;


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   GdiplusStartupInput gdiplusStartupInput;
   GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_SYSMENU,
      500, 0, 1016, 1159, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
//
//
//게임 화면
RECT rectView;
//게임 UI 화면
RECT rectViewUI;

Rect rect;

//맵의 격자의 갯수
const POINT Grids = { 25, 25 };

//턴제 격자 사이즈
int GridXSize;
int GridYSize;


//플레이어
Player player;
//몬스터
Monster monster;


vector<Arrow> arrows;//화살
vector<POINT> blocks;//맵에 블록 생성
vector<AnimationEffect> animationeffects;//애니메이션 효과
//더블 버퍼링
HDC mem1dc;
HDC hdc;
HBITMAP hBit, oldBit;

//이미지
Image* playerAction;
Image* swordAction;
Image* spearAction;
Image* UIImage;
Image* UIBackground;
Image* arrowAction;
Image* bossAction;
Image* effect;

UIIcon Background;
UIIcon WeaponIcon;

Astar astar;
//게임 시작 시 설정
int startsetting = 1;
//잔상 반투명화
ColorMatrix illusionMatrix = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 0.2f, 0.0f,
                           0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
ImageAttributes* imageAtt;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        GetClientRect(hWnd, &rectView);
        rectViewUI.top = rectView.bottom - 100;
        rectViewUI.bottom = rectView.bottom;
        rectViewUI.right = rectView.right;
        rectViewUI.left = rectView.left;
        rectView.bottom -= 100;//게임 화면과 UI 화면 분리

        //UI화면은 비동기적으로 움직임



        GridXSize = rectView.right / Grids.x;
        GridYSize = rectView.bottom / Grids.y;

        player.setX(rectView.right / 2);
        player.setY(rectView.bottom / 2);
        playerAction = Image::FromFile(L"images/player.png");
        swordAction = Image::FromFile(L"images/sword.png");
        spearAction = Image::FromFile(L"images/spear.png");
        arrowAction = Image::FromFile(L"images/arrow.png");
        UIImage = Image::FromFile(L"images/UIImage.png");
        UIBackground = Image::FromFile(L"images/UIBackground.png");
        bossAction = Image::FromFile(L"images/Boss.png");
        effect = Image::FromFile(L"images/effect.png");
    }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
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
            hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            if (startsetting == 1)
            {
                StartSetting(hdc);
                startsetting = 0;
            }
            else
                DrawDoubleBuffering(hdc);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);

        SelectObject(mem1dc, oldBit);
        DeleteDC(mem1dc);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
void Update()
{
    static vector<POINT> route;
    static int frame = 0;
    if (frame <= 0)
    {
        route = astar.Route(monster.getX(), monster.getY(), player.getX(), player.getY(), GridXSize, GridYSize, blocks);
        frame = 30;
        cout << route.size() << endl;
    }
    else
        frame--;

    PlayerSystem();
    MonsterSystem(route);


    player.HitCheck(monster);
    player.ProjHitCheck(monster);
    monster.HitCheck(player, arrows);

    for (int i = 0; i < animationeffects.size(); i++)
    {
        if (animationeffects[i].getSpriteX() >= 11)
        {
            animationeffects.erase(animationeffects.begin() + i);
            i = -1;
        }
    }



    InvalidateRect(hWnd, NULL, FALSE);
}

void DrawDoubleBuffering(HDC& hdc)
{
    FillRect(mem1dc, &rectView, (HBRUSH)(COLOR_WINDOW + 2));
    Graphics g(mem1dc);

    Rectangle(mem1dc, player.getX() - player.getWidth(),
        player.getY() - player.getHeight(),
        player.getX() + player.getWidth(),
        player.getY() + player.getHeight());
    PlayerDraw(g);
    MonsterDraw(g);
    UIDraw(g);

    for (int i = 0; i < blocks.size(); i++)
    {
        Rectangle(mem1dc, blocks[i].x * GridXSize - GridXSize / 2, blocks[i].y * GridYSize - GridYSize / 2,
            blocks[i].x * GridXSize + GridXSize / 2, blocks[i].y * GridYSize + GridYSize / 2);
    }

    for (int i = 0; i < animationeffects.size(); i++)
    {
        animationeffects[i].drawAnimationEffect(g, effect);
    }

    BitBlt(hdc, 0, 0, rectView.right, rectViewUI.bottom, mem1dc, 0, 0, SRCCOPY);
}

void StartSetting(HDC& hdc)
{
    mem1dc = CreateCompatibleDC(hdc);
    if (hBit == NULL)
        hBit = CreateCompatibleBitmap(hdc, rectView.right, rectViewUI.bottom);
    oldBit = (HBITMAP)SelectObject(mem1dc, hBit);


    imageAtt = new ImageAttributes;
    imageAtt->SetColorMatrix(&illusionMatrix, ColorMatrixFlagsDefault,
        ColorAdjustTypeBitmap);
    //UI설정
    UISetting();

    astar.Init(Grids.x, Grids.y);
}

void PlayerSystem()
{
    player.changeWeapon(WeaponIcon);//무기 변경
    player.movePos(PlayerMove());//플레이어 이동
    player.correctPosition(rectView, blocks, GridXSize, GridYSize);//맵 밖으로 나가지 않게 함
    player.spriteNFrame();//스프라이트와 프레임 설정
    player.attackCollide(arrows);//공격 충돌 판정

    for (int i = 0; i < arrows.size(); i++)
    {
        arrows[i].movePos();
    }

    CheckArrowOutofArea(arrows, rectView);
}

void MonsterSystem(vector<POINT>& route)
{
    int temp = time(NULL) - monster.getPatterntime();
    if (temp < 50)
        monster.normalMode(route, GridXSize, GridYSize, rectView, player, Grids, blocks, animationeffects);
    else
    {
        for (int i = 0; i < blocks.size(); i++)
        {
            AnimationEffect animationeffect(blocks[i].x * GridXSize, blocks[i].y * GridYSize, GridXSize * 2, GridYSize * 2, 0, 9);
            animationeffects.push_back(animationeffect);
        }
        blocks.clear();
        monster.patternMode(rectView);
    }
}

void PlayerDraw(Graphics& g)
{
    player.action(rect, g, playerAction, imageAtt);//플레이어 애니메이션

    if (player.getWeaponType() == 1)
        player.attack(rect, g, swordAction, arrowAction, arrows);//공격 애니메이션
    else
        player.attack(rect, g, spearAction, arrowAction, arrows);
}

void MonsterDraw(Graphics& g)
{
    Rectangle(mem1dc, monster.getX() - monster.getSizeX() / 2,
        monster.getY() - monster.getSizeY() / 2, monster.getX() + monster.getSizeX() / 2,
        monster.getY() + monster.getSizeY() / 2);

    monster.action(rect, g, bossAction);
    monster.drawProjectiles(g, effect, mem1dc);
}

void UISetting()
{
    Background.setWidth(rectViewUI.right - rectViewUI.left + 140);
    Background.setHeight(rectViewUI.bottom - rectViewUI.top + 60);
    Background.setX(rectViewUI.left - 70);
    Background.setY(rectViewUI.top - 30);

    WeaponIcon.setWidth(rectViewUI.bottom - rectViewUI.top - 20);
    WeaponIcon.setHeight(rectViewUI.bottom - rectViewUI.top - 20);
    WeaponIcon.setX(rectViewUI.left + 30);
    WeaponIcon.setY((rectViewUI.bottom + rectViewUI.top) / 2 - WeaponIcon.getHeight() / 2);
    WeaponIcon.setSizeX(32);
    WeaponIcon.setSizeY(32);
    WeaponIcon.setPosX(0);
    WeaponIcon.setPosY(0);
}

void UIDraw(Graphics& g)
{
    Background.DrawUIBackground(g, UIBackground);
    WeaponIcon.DrawUIIcon(g, UIImage);

    FontFamily fontFamily(L"돋움");
    Font font(&fontFamily, 24, FontStyleRegular, UnitPixel);
    PointF pointF(rectViewUI.right - 300, (rectViewUI.top + rectViewUI.bottom) / 2 - 12);
    SolidBrush solidBrush(Color(255, 255, 0, 0));

    TCHAR text[100];

    _stprintf_s(text, L"몬스터 남은 체력 : %d", monster.getLife());
    g.DrawString(text, -1, &font, pointF, &solidBrush);

    PointF pointF2(rectViewUI.right - 600, (rectViewUI.top + rectViewUI.bottom) / 2 - 12);
    SolidBrush solidBrush2(Color(255, 0, 150, 0));
    _stprintf_s(text, L"플레이어 남은 체력 : %d", player.getLife());
    g.DrawString(text, -1, &font, pointF2, &solidBrush2);
}



// 정보 대화 상자의 메시지 처리기입니다.
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
