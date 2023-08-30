// WinApiProject.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#define _CRT_SECURE_NO_WARNINGS
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
                Update();
                oldtime = newtime;
                framecheck++;
            }
            if (newframetime - oldframetime >= 34 * 30)
            {
                oldframetime = newframetime;
                cout << framecheck << endl;
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
      500, 0, 816, 959, nullptr, nullptr, hInstance, nullptr);

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


enum GamePage
{
    LOGO,
    STAGESELECT,
    STAGE,
    STAGEEND,
    RESET
};

#define STAGECOUNT 2
#define PATTERNTIME 60

int CurrentStage = 0;
int ClearedStage = 0;

GamePage gamepage = LOGO;
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
//아이템
Item item;
//마우스 위치
int mouseposX = -100;
int mouseposY = -100;

//플레이어
Player* player;
//몬스터
Monster* monster;
vector<Monster> minimonsters;
vector<vector<POINT>> routes;

Button logoButton;
Button stageselectButton[STAGECOUNT];


vector<Arrow> arrows;//화살
vector<Block> blocks;//맵에 블록 생성
vector<AnimationEffect> animationeffects;//애니메이션 효과
vector<DamageText> damagetexts;
//더블 버퍼링
HDC mem1dc;
HDC hdc;
HBITMAP hBit, oldBit;

//이미지
Image* logo;
Image* logobackground;
Image* buttonimage;
Image* stageselect;
Image* stageselectbutton[2];
Image* stageinfo;

Image* playerAction;
Image* swordAction;
Image* spearAction;
Image* UIImage;
Image* UIBackground;
Image* arrowAction;
Image* bossAction[2];
Image* monsterAction;
Image* effects[3];
Image* terrain;
Image* map;
Image* UIBar[3];
Image* stone;
Image* flame;
Image* selecticon;
Image* items;

Image* signal[2];

Image* imagefont[10];

Image* ready;
Image* fight;
Image* gameover;
Image* stageclear;
TextureBrush* tbrush;

int flip = 0;
int patternhit = 0;
int patternmode = 0;
int patterntimeadd = 0;

int timer = 0;

int damage = 0;

clock_t gamemanage;
int pause = 0;

UIIcon Background;
UIIcon WeaponIcon[3];
UIIcon Weapontype;

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

        logo = Image::FromFile(L"images/logo.png");
        logobackground = Image::FromFile(L"images/logobackground.png");
        buttonimage = Image::FromFile(L"images/button.png");
        stageselect = Image::FromFile(L"images/stageselect.png");
        playerAction = Image::FromFile(L"images/player.png");
        swordAction = Image::FromFile(L"images/sword.png");
        spearAction = Image::FromFile(L"images/spear.png");
        arrowAction = Image::FromFile(L"images/arrow.png");
        UIImage = Image::FromFile(L"images/UIImage.png");
        UIBackground = Image::FromFile(L"images/UIBackground.png");
        terrain = Image::FromFile(L"images/terrain.png");
        map = Image::FromFile(L"images/map.png");
        stone = Image::FromFile(L"images/Projectiles/stone.png");
        flame = Image::FromFile(L"images/Projectiles/flame.png");
        selecticon = Image::FromFile(L"images/selecticon.png");
        ready = Image::FromFile(L"images/ready.png");
        fight = Image::FromFile(L"images/fight.png");
        gameover = Image::FromFile(L"images/gameover.png");
        stageclear = Image::FromFile(L"images/stageclear.png");
        items = Image::FromFile(L"images/items.png");
        stageinfo = Image::FromFile(L"images/stageinfo.png");
        monsterAction = Image::FromFile(L"images/monster1.png");

        TCHAR temp[50];

        for (int i = 0; i < 2; i++)
        {
            _stprintf_s(temp, L"images/circlebutton%d.png", i + 1);
            stageselectbutton[i] = Image::FromFile(temp);
        }

        for (int i = 0; i < 3; i++)
        {
            _stprintf_s(temp, L"images/Effects/effect%d.png", i + 1);
            effects[i] = Image::FromFile(temp);
        }

        for (int i = 0; i < 2; i++)
        {
            _stprintf_s(temp, L"images/boss%d.png", i + 1);
            bossAction[i] = Image::FromFile(temp);
        }

        for (int i = 0; i < 10; i++)
        {
            _stprintf_s(temp, L"images/Fonts/%d.png", i);
            imagefont[i] = Image::FromFile(temp);
        }


        for (int i = 1; i < 4; i++)
        {
            _stprintf_s(temp, L"images/Bar/%d.png", i);
            UIBar[i - 1] = Image::FromFile(temp);
        }

        for (int i = 1; i < 3; i++)
        {
            _stprintf_s(temp, L"images/signal/%d.png", i);
            signal[i - 1] = Image::FromFile(temp);
        }

        tbrush = new TextureBrush(map);


        CSound::Init();
        SoundManager* sm = SoundManager::getInstance();

        sm->addSound("sounds/sword.wav");
        sm->addSound("sounds/stone.wav");
        sm->addSound("sounds/spear.wav");
        sm->addSound("sounds/hit.wav");
        sm->addSound("sounds/heal.wav");
        sm->addSound("sounds/fire.wav");
        sm->addSound("sounds/arrow.wav");
        sm->addSound("sounds/dangerzone.wav");
        sm->addSound("sounds/block.wav");
        sm->addSound("sounds/background.wav");
        sm->addSound("sounds/dash.wav");
        sm->addSound("sounds/click.wav");

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
    case WM_LBUTTONDOWN:
        mouseposX = LOWORD(lParam);
        mouseposY = HIWORD(lParam);
        break;
    case WM_LBUTTONUP:
        mouseposX = -100;
        mouseposY = -100;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        SelectObject(mem1dc, oldBit);
        DeleteDC(mem1dc);
        delete tbrush;
        CSound::Release();
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
void Update()
{

    if (gamepage == STAGE)
    {
        if (clock() - gamemanage < 2500)
            pause = 1;
        else if (clock() - gamemanage < 3000)
        {
            pause = 1;
            monster->setPatternTime(time(NULL));
        }
        else
            pause = 0;

        if (pause == 0)
        {
            static vector<POINT> route;
            static int frame = 0;
            if (frame <= 0)
            {
                route = astar.Route(monster->getX(), monster->getY(), player->getX(), player->getY(), GridXSize, GridYSize, blocks, 1);
                for (int i = 0; i < minimonsters.size(); i++)
                {
                    routes[i] = astar.Route(minimonsters[i].getX(), minimonsters[i].getY(), player->getX(), player->getY(), GridXSize, GridYSize, blocks, 0);
                }
                frame = 30;
            }
            else
                frame--;

            PlayerSystem();
            MonsterSystem(route);

            for (int i = 0; i < damagetexts.size(); i++)
            {
                damagetexts[i].update(*player, *monster);
            }

            for (int i = 0; i < damagetexts.size(); i++)
            {
                if (damagetexts[i].getLeftFrame() <= 0)
                {
                    damagetexts.erase(damagetexts.begin() + i);
                    i = -1;
                }
            }

            for (int i = 0; i < blocks.size(); i++)
            {
                blocks[i].update();
            }

            int hit = player->HitCheck(*monster, animationeffects);
            int projhit = player->ProjHitCheck(*monster, animationeffects);
            for (int i = 0; i < minimonsters.size(); i++)
            {
                player->HitCheck(minimonsters[i], animationeffects);
                int special = 0;
                minimonsters[i].HitCheck(*player, arrows, animationeffects, special);
            }


            for (int i = 0; i < minimonsters.size(); i++)
            {
                if (minimonsters[i].getLife() <= 0)
                {
                    minimonsters.erase(minimonsters.begin() + i);
                    routes.erase(routes.begin() + i);
                    i = -1;
                }
            }



            if (patternmode == 1 && (hit > 0 || projhit > 0))
                patternhit++;


            int special = 0;
            int monhit = monster->HitCheck(*player, arrows, animationeffects, special);


            if (special == 1)
                patterntimeadd++;

            for (int i = 0; i < animationeffects.size(); i++)
            {
                if (animationeffects[i].getSpriteX() >= 11)
                {
                    animationeffects.erase(animationeffects.begin() + i);
                    i = -1;
                }
            }


            if (hit > 0 || projhit > 0 || monhit > 0)
            {
                DamageText damageText;
                damageText.setOverLen(50);
                if (hit > 0)
                {
                    damageText.setType(0);
                    damageText.setDamage(hit);
                    damageText.setX(player->getX());
                    damageText.setY(player->getY());
                }
                else if (projhit > 0)
                {
                    damageText.setType(0);
                    damageText.setDamage(projhit);
                    damageText.setX(player->getX());
                    damageText.setY(player->getY());
                }
                else
                {
                    damageText.setType(1);
                    damageText.setDamage(monhit);
                    damageText.setX(monster->getX());
                    damageText.setY(monster->getY());
                }
                damagetexts.push_back(damageText);
            }
        }

        if (monster->getLife() <= 0 || player->getLife() <= 0)
        {
            gamepage = STAGEEND;
            gamemanage = clock();
            pause = 1;
        }
    }
    else if (gamepage == LOGO)
    {
        if (logoButton.OnClickRectangle(mouseposX, mouseposY) == 1)
        {
            gamepage = STAGESELECT;
            mouseposX = -100;
            mouseposY = -100;
            SoundManager* sm = SoundManager::getInstance();
            sm->playSound("sounds/click.wav");
        }
    }
    else if (gamepage == STAGESELECT)
    {
        for (int i = 0; i < STAGECOUNT; i++)
        {
            if (stageselectButton[i].OnClickCircle(mouseposX, mouseposY) == 1)
            {
                if (ClearedStage < i)
                    return;
                if (i == 0)
                    monster = new Monster;
                else if (i == 1)
                    monster = new MonsterType2;

                player = new Player;

                CurrentStage = i + 1;
                gamepage = STAGE;
                gamemanage = clock();

                SoundManager* sm = SoundManager::getInstance();
                sm->playSound("sounds/click.wav");
                sm->playSound("sounds/background.wav");
            }
        }
    }
    else if (gamepage == STAGEEND)
    {
        SoundManager* sm = SoundManager::getInstance();
        sm->stopSound("sounds/background.wav");
        if (clock() - gamemanage >= 3000)
            gamepage = RESET;
    }
    else if (gamepage == RESET)
    {
        if(monster->getLife() <= 0)
            ClearedStage = CurrentStage;
        arrows.clear();
        blocks.clear();
        animationeffects.clear();
        damagetexts.clear();
        minimonsters.clear();
        routes.clear();
        delete monster;
        patternhit = 0;
        timer = 0;
        gamepage = STAGESELECT;
        patterntimeadd = 0;
        item.setCount(1);
    }
    InvalidateRect(hWnd, NULL, FALSE);
}

void DrawDoubleBuffering(HDC& hdc)
{
    Graphics g(mem1dc);
    if (gamepage == STAGE)
    {
        rect.X = 0;
        rect.Y = 0;
        rect.Width = rectView.right;
        rect.Height = rectView.bottom;

        g.FillRectangle(tbrush, rect);

        for (int i = 0; i < blocks.size(); i++)
        {
            blocks[i].drawBlock(g, terrain, GridXSize, GridYSize);
        }

        PlayerDraw(g);
        MonsterDraw(g);
        UIDraw(g);

        for (int i = 0; i < animationeffects.size(); i++)
        {
            animationeffects[i].drawAnimationEffect(g, effects);
        }

        for (int i = 0; i < damagetexts.size(); i++)
        {
            int damage = damagetexts[i].getDamage();
            rect.Width = 34;
            rect.Height = 72;
            rect.Y = damagetexts[i].getY() - damagetexts[i].getOverLen() - rect.Height / 4;
            
            vector<int> digit;

            while (damage != 0)
            {
                digit.push_back(damage % 10);
                damage /= 10;
            }

            int correctpos = (digit.size() - 1) * rect.Width / 4;

            for (int j = digit.size() - 1; j >= 0; j--)
            {
                rect.X = damagetexts[i].getX() - rect.Width / 2 + (digit.size() - 1 - j) * rect.Width / 2 - correctpos;
                g.DrawImage(imagefont[digit[j]], rect, 0, 0, 46, 92, UnitPixel);
            }
        }


        if (pause == 1)
        {
            SolidBrush sbrush(Color(128, 0, 0, 0));
            rect.X = rectView.left;
            rect.Y = rectView.top;
            rect.Width = rectView.right;
            rect.Height = rectView.bottom;
            g.FillRectangle(&sbrush, rect);

            rect.Width = 200;
            rect.Height = 100;

            rect.X = rectView.right / 2 - 100;
            rect.Y = rectView.bottom / 2 - 50;

            if (clock() - gamemanage <= 2500)
                g.DrawImage(ready, rect);
            else
                g.DrawImage(fight, rect);
        }
    }
    else if (gamepage == LOGO)
    {
        rect.X = 0;
        rect.Y = 0;
        rect.Width = rectView.right;
        rect.Height = rectViewUI.bottom;
        g.DrawImage(stageselect, rect);

        rect.X = rectView.right / 4 - 120;
        rect.Y = rectView.bottom / 4 - 70;
        rect.Width = rectView.right - 150;
        rect.Height = rectView.bottom / 5;

        g.DrawImage(logobackground, rect);

        rect.X = rectView.right / 4 - 50;
        rect.Y = rectView.bottom / 4 - 50;
        rect.Width = rectView.right / 1.5;
        rect.Height = rectView.bottom / 6;
        g.DrawImage(logo, rect);
       

        logoButton.drawRectangleButton(g, buttonimage, L"Game Start");
    }
    else if (gamepage == STAGESELECT)
    {
        rect.X = 0;
        rect.Y = 0;
        rect.Width = rectView.right;
        rect.Height = rectViewUI.bottom;
        g.DrawImage(stageselect, rect);

        for (int i = 0; i < STAGECOUNT; i++)
        {
            if(i > ClearedStage)
                stageselectButton[i].drawCircleButton(g, stageselectbutton[1]);
            else
                stageselectButton[i].drawCircleButton(g, stageselectbutton[0]);
            StringFormat stringFormat;
            stringFormat.SetAlignment(StringAlignmentCenter);
            FontFamily fontFamily(L"돋움");
            Font font(&fontFamily, 30, FontStyleBold, UnitPixel);
            PointF pointF(stageselectButton[i].getX(), stageselectButton[i].getY() + 50);
            SolidBrush solidBrush(Color(255, 255, 255, 255));
            TCHAR temp[10];
            _stprintf_s(temp, L"Stage %d", i + 1);
            g.DrawString(temp, -1, &font, pointF, &stringFormat, &solidBrush);
        }
    }
    else if (gamepage == STAGEEND)
    {
        if (pause == 1)
        {
            SolidBrush sbrush(Color(128, 0, 0, 0));
            rect.X = rectView.left;
            rect.Y = rectView.top;
            rect.Width = rectView.right;
            rect.Height = rectView.bottom;
            g.FillRectangle(&sbrush, rect);

            rect.Width = 400;
            rect.Height = 100;

            rect.X = rectView.right / 2 - 200;
            rect.Y = rectView.bottom / 2 - 50;

            if (player->getLife() <= 0)
                g.DrawImage(gameover, rect);
            else
                g.DrawImage(stageclear, rect);

            UIDraw(g);
        }
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

    item.setType(0);
    item.setName("potion");
    item.setNum(20);
    item.setCount(1);
}

void PlayerSystem()
{
    player->changeWeapon(Weapontype);//무기 변경
    player->movePos(PlayerMove());//플레이어 이동
    player->correctPosition(rectView, blocks, GridXSize, GridYSize);//맵 밖으로 나가지 않게 함
    player->spriteNFrame();//스프라이트와 프레임 설정
    player->attackCollide(arrows);//공격 충돌 판정
    player->useItem(item, animationeffects);

    for (int i = 0; i < arrows.size(); i++)
    {
        arrows[i].movePos();
    }

    CheckArrowOutofArea(arrows, rectView);
}

void MonsterSystem(vector<POINT>& route)
{
    timer = time(NULL) - monster->getPatterntime();

    if (patternmode == 0)
    {
        monster->normalMode(route, GridXSize, GridYSize, rectView, *player, Grids, blocks, animationeffects);
        if (timer + patterntimeadd >= PATTERNTIME)
        {
            patternmode = 1;
            monster->clearDangerZone();
            patterntimeadd = 0;
        }
    }
    else if (patternmode == 1)
    {
        int patterncheck = monster->patternMode(rectView, GridXSize, GridYSize, Grids, *player, animationeffects, minimonsters.size());
        if (monster->getPatternStart() == 0)
        {
            for (int i = 0; i < blocks.size(); i++)
            {
                AnimationEffect animationeffect((blocks[i].getX() - 1) * GridXSize, (blocks[i].getY() - 1) * GridYSize, GridXSize * 2, GridYSize * 2, 0, 9, 1);
                animationeffects.push_back(animationeffect);

            }
            blocks.clear();
            monster->setPatternStart(1);
            patternmode = 1;
        }

        if (monster->getLeftPatternProgress() <= 0)
        {
            monster->setPatternTime(time(NULL));

            if (patternhit <= 5)
            {
                monster->setPatternStart(2);
                monster->groggyMode();
                patternmode = 2;
            }
            else
            {
                monster->setPatternStart(0);
                patternmode = 0;
            }
            patternhit = 0;
        }

        if (patterncheck == 1)
        {
            Monster minimonster;
            minimonster.setX(minimonster.Randomize(2, Grids.x) * GridXSize);
            minimonster.setY(minimonster.Randomize(2, Grids.y) * GridYSize);
            minimonster.setMaxLife(20);
            minimonster.setLife(20);
            minimonster.setSizeX(GridXSize * 2);
            minimonster.setSizeY(GridYSize * 2);
            minimonsters.push_back(minimonster);
            vector<POINT> temp;
            routes.push_back(temp);
        }

        for (int i = 0; i < minimonsters.size(); i++)
        {
            minimonsters[i].ChasePlayer(routes[i], GridXSize, GridYSize);
        }
    }
    else if (patternmode == 2)
    {
        if (timer >= 7)
        {
            monster->setPatternTime(time(NULL));
            monster->setPatternStart(0);
            patternmode = 0;
            monster->setGroggy(0);
        }
    }
}

void PlayerDraw(Graphics& g)
{
    player->action(rect, g, playerAction, imageAtt);//플레이어 애니메이션

    if (player->getWeaponType() == 1)
        player->attack(rect, g, swordAction, arrowAction, arrows);//공격 애니메이션
    else
        player->attack(rect, g, spearAction, arrowAction, arrows);
}

void MonsterDraw(Graphics& g)
{
    flip++;

    if (flip == 3)
    {
        stone->RotateFlip(Rotate90FlipNone);
        flip = 0;
    }

    monster->action(rect, g, bossAction[CurrentStage - 1]);
    monster->drawProjectiles(g, effects, stone, flame);
    monster->drawDangerZones(g, mem1dc);

    for (int i = 0; i < minimonsters.size(); i++)
    {
        minimonsters[i].action(rect, g, monsterAction);
    }
}

void UISetting()
{
    Background.setWidth(rectViewUI.right - rectViewUI.left + 140);
    Background.setHeight(rectViewUI.bottom - rectViewUI.top + 60);
    Background.setX(rectViewUI.left - 70);
    Background.setY(rectViewUI.top - 30);

    for (int i = 0; i < 3; i++)
    {
        WeaponIcon[i].setWidth(rectViewUI.bottom - rectViewUI.top - 45);
        WeaponIcon[i].setHeight(rectViewUI.bottom - rectViewUI.top - 45);
        WeaponIcon[i].setX(rectViewUI.left + 30 + i * WeaponIcon[i].getWidth() - 20);
        WeaponIcon[i].setY((rectViewUI.bottom + rectViewUI.top) / 2 - WeaponIcon[i].getHeight() / 2 + 10);
        WeaponIcon[i].setSizeX(32);
        WeaponIcon[i].setSizeY(32);
        WeaponIcon[i].setPosX(0);
        if (i == 1)
            WeaponIcon[i].setPosX(128);
        if (i == 2)
            WeaponIcon[i].setPosX(160);
        WeaponIcon[i].setPosY(0);
    }

    {
        Weapontype.setWidth(rectViewUI.bottom - rectViewUI.top - 45);
        Weapontype.setHeight(rectViewUI.bottom - rectViewUI.top - 45);
        Weapontype.setX(10);
        Weapontype.setY((rectViewUI.bottom + rectViewUI.top) / 2 - Weapontype.getHeight() / 2 + 10);
        Weapontype.setSizeX(32);
        Weapontype.setSizeY(32);
        Weapontype.setPosX(0);
        Weapontype.setPosY(0);
    }

    {
        logoButton.setX(rectView.right / 2);
        logoButton.setY(rectView.bottom / 2 + 300);
        logoButton.setWidth(200);
        logoButton.setHeight(50);
    }
    {
        stageselectButton[0].setX(400);
        stageselectButton[0].setY(300);
        stageselectButton[0].setRadius(30);
    }

    {
        stageselectButton[1].setX(500);
        stageselectButton[1].setY(500);
        stageselectButton[1].setRadius(30);
    }
}

void UIDraw(Graphics& g)
{
    Background.DrawUIBackground(g, UIBackground);
    for (int i = 0; i < 3; i++)
    {
        WeaponIcon[i].DrawUIIcon(g, UIImage);
    }

    Weapontype.DrawUIIcon(g, selecticon);

    {
        rect.X = 10;
        rect.Y = rectViewUI.top + 5;
        rect.Width = 48 * 4;
        rect.Height = 16 * 2;

        g.DrawImage(UIBar[0], rect);
        double leftlife = (double)player->getLife() / (double)player->getMaxLife();
        rect.Width *= leftlife;
        g.DrawImage(UIBar[1], rect, 0, 0, UIBar[1]->GetWidth() * leftlife, UIBar[1]->GetHeight(), UnitPixel);

        rect.X = 300;
        rect.Y = rectViewUI.top + 5;
        rect.Width = 48 * 6;
        rect.Height = 16 * 2;
        g.DrawImage(UIBar[0], rect);

        leftlife = (double)monster->getLife() / (double)monster->getMaxLife();

        rect.Width *= leftlife;

        g.DrawImage(UIBar[2], rect, 0, 0, UIBar[2]->GetWidth() * leftlife, UIBar[2]->GetHeight(), UnitPixel);
    }

    {
        StringFormat stringFormat;
        stringFormat.SetAlignment(StringAlignmentCenter);
        FontFamily fontFamily(L"돋움");
        Font font(&fontFamily, 17, FontStyleBold, UnitPixel);
        PointF pointF(445, rectViewUI.top + 12);
        PointF pointF2(110, rectViewUI.top + 12);
        SolidBrush solidBrush(Color(255, 255, 255, 255));

        TCHAR text[100];

        _stprintf_s(text, L"%d", monster->getLife());
        g.DrawString(text, -1, &font, pointF, &stringFormat, &solidBrush);
        _stprintf_s(text, L"%d", player->getLife());
        g.DrawString(text, -1, &font, pointF2, &stringFormat, &solidBrush);
        rect.X = WeaponIcon[2].getX() + WeaponIcon[2].getWidth();
        rect.Y = WeaponIcon[2].getY();
        rect.Width = WeaponIcon[2].getWidth();
        rect.Height = WeaponIcon[2].getHeight();
        g.DrawImage(items, rect, 32 * 4, 32 * 9, 32, 32, UnitPixel);
        if (item.getCount() <= 0)
        {
            SolidBrush sbrush(Color(100, 0, 0, 0));
            g.FillRectangle(&sbrush, rect);
        }
    }

    {
        rect.X = 300;
        rect.Y = rectViewUI.top + 35;
        rect.Width = 50;
        rect.Height = 50;

        if (patternhit <= 5)
            g.DrawImage(signal[0], rect);
        else
            g.DrawImage(signal[1], rect);
    }

    {
        rect.X = 600;
        rect.Y = rectViewUI.top;
        rect.Width = 100;
        rect.Height = 70;
        g.DrawImage(stageinfo, rect);


        rect.X += 80;
        rect.Width = 50;
        g.DrawImage(imagefont[CurrentStage], rect);
    }

    {
        TCHAR temp[10];
        StringFormat stringFormat;
        stringFormat.SetAlignment(StringAlignmentCenter);
        FontFamily fontFamily(L"돋움");
        Font font(&fontFamily, 27, FontStyleBold, UnitPixel);
        PointF pointF(380, rectViewUI.top + 45);
        SolidBrush solidBrush(Color(255, 0, 0, 0));

        _stprintf_s(temp, L"%d", timer + patterntimeadd);
        g.DrawString(temp, -1, &font, pointF, &stringFormat, &solidBrush);
    }
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
