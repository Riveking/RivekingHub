#include "EngineTest.h"
#include <stdio.h>

LPCTSTR EngineTest::fish_files[4] = { L".\\res\\red.png",L".\\res\\green.png" ,L".\\res\\blue.png" ,L".\\res\\yellow.png" };
LPCTSTR EngineTest::fish_name[4] = { L"red",L"green",L"blue",L"yellow" };
float EngineTest::rand_size[5] = { 1.0f,0.9f,0.8f,0.7f,0.6f };
int EngineTest::NUM = 10;
int EngineTest::FISH_WIDTH = 143;
int EngineTest::FISH_HEIGHT = 84;


//WinMain����
int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	LPCTSTR WinTitle = L"������� T_Engine ���Գ���";//��Ϸ���ڱ���
	EngineTest* test = new EngineTest(h_instance, WIN_CLASS, WinTitle,IDI_SHARK, IDI_SHARK, 800, 600);
	T_Engine::pEngine = test;//������ľ�ָ̬��ָ����õ�����
	test->SetFrame(24);//���û���ˢ��Ƶ��

	//myApp->SetFullScreen(TRUE);//ȫ������

	test->StartEngine();
	return TRUE;

}

EngineTest::EngineTest(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title, WORD icon, WORD sm_icon, int winwidth, int winheight)
	:T_Engine(h_instance,sz_winclass,sz_title,icon,sm_icon,winwidth,winheight)
{
	srand((unsigned)time(NULL));
	wnd_width = winwidth;
	wnd_height = winheight;//��ȡ���ڿ��
}

EngineTest::~EngineTest(void){}


void EngineTest::GameInit(){
	seaBed.LoadImageFile(L".\\res\\seabed.jpg");
	seaFloor.LoadImageFile(L".\\res\\seafloor.jpg");
	msgFrame.LoadImageFile(L".\\res\\msg.png");
	for (int i = 0; i < NUM; i++)
	{
		FISH tmpFish;		
		int s = rand() % 5;
		tmpFish.ratio = rand_size[s];
		int w = (int)(rand_size[s] * FISH_WIDTH  );
		int h = (int)(rand_size[s] * FISH_HEIGHT );
		tmpFish.x = rand()%(wnd_width - w);
		tmpFish.y = h  + rand() % (wnd_height - 2 * h);
		tmpFish.oldy = tmpFish.y;//��¼��ԭ��Y��λ��
		tmpFish.updown = rand() % 2;//�����������Σ�0���Σ�1����
		int d = rand() % 2;
		tmpFish.dir = d;//��ķ���
		int f = rand() % 4;
		LPCTSTR str = fish_files[f];
		tmpFish.fish.LoadImageFile(str);
		tmpFish.name = fish_name[f];
		tmpFish.fcount = 0;
		vecFish.push_back(tmpFish);
	}
}				//��Ϸ��ʼ���ӿں���
void EngineTest::GameLogic()
{
	
	vector<FISH>::iterator it;
	for (it = vecFish.begin(); it < vecFish.end(); it++)
	{

		if (it->fcount < 20)
		{
			it->fcount = it->fcount + 1;
			
		}
		if (it->fcount >= 20)
		{
			it -> fcount = 0;
		}

		if (it->dir == 0)
		{
			it->x = it->x - 5 * (it->ratio);//�����ƶ�
			if (it->x < -100)
			{
				it->x = wnd_width;
			}
		}

		if (it->dir == 1)
		{
			it->x = it->x + 5 * (it->ratio);//�����ƶ�
			if (it->x >wnd_width)
			{
				it->x = -100;
			}
		}

		if (it->updown == 1)//����
		{
			it->y = it->y + (rand() % 3) * it->ratio;
			if (it->y > (it->oldy + 100 * it->ratio) || it->y>(wnd_height - FISH_HEIGHT))
			{
				it->updown = 0;
			}
		}
		
		 if (it->updown == 0)//����
		{
			it->y = it->y - (rand()%3)*it->ratio;
			if (it->y < (it->oldy - 100 * it->ratio))
			{
				it->updown = 1;
			}
		}
	}
}//��Ϸ�߼�����  ����GameState


void EngineTest::GamePaint(HDC hdc)	//��Ϸ��ʾ	����ֵΪGameState
{
	seaFloor.PaintImage(hdc, 0, 0, wnd_width, wnd_height, 255);
	seaBed.PaintImage(hdc, 0, 0, wnd_width, wnd_height, 120);

	vector<FISH>::iterator it;
	for (it = vecFish.begin(); it < vecFish.end(); it++)
	{
		int rot = 0;
		int dir = it->dir;

		if (dir == 0)
		{
			rot = 0;//0����
		}
		else if(dir==1)
		{
			rot = TRANS_HFLIP_NOROT;
		}
		RectF msgRect;
		msgRect.Width = (REAL)(msgFrame.GetImageWidth());
		msgRect.Height = (REAL)(msgFrame.GetImageHeight());
		msgRect.X = (REAL)(it->x + (it->ratio*FISH_WIDTH - msgFrame.GetImageWidth()) / 2);
		msgRect.Y = (REAL)(it->y - msgFrame.GetImageHeight());
		msgFrame.PaintImage(hdc,(int)msgRect.X,(int)msgRect.Y,(int)msgRect.Width,(int)msgRect.Height,255);

		T_Graph::PaintText(hdc, msgRect, it->name, 10, L"Arial", Color::White, FontStyleBold, StringAlignmentCenter);
		
		
		it->fish.PaintFrame(it->fish.GetBmpHandle(), hdc, it->x, it->y, it->fcount, 20, FISH_WIDTH, FISH_HEIGHT, it->ratio, rot);
	}

	RectF captionRec;
	captionRec.X = 0.00;
	captionRec.Y = 0.00;
	captionRec.Width = (float)wnd_width;
	captionRec.Height = 60.0f;
	LPCTSTR caption = L"�������λͼģ���ͼʵ��";
	T_Graph::PaintText(hdc, captionRec, caption, 22, L"΢���ź�", Color::White, FontStyleBold, StringAlignmentNear);
	RectF infoRec;
	infoRec.X = 0.00;
	infoRec.Y = (REAL)(wnd_height - 60);
	infoRec.Width = (float)wnd_width;
	infoRec.Height = 60;
	LPCTSTR info = L"�༶����1809�� ѧ��8002118239 ������������ ʱ�䣺2020��11��7��";
	T_Graph::PaintText(hdc, infoRec, info, 14, L"����", Color::White, FontStyleBold, StringAlignmentCenter);

}

void EngineTest::GameEnd(){
	seaFloor.Destroy();
	seaBed.Destroy();
	msgFrame.Destroy();
	vector<FISH>::iterator it;
	for (it = vecFish.begin(); it < vecFish.end(); it++)
	{
		it->fish.Destroy();
	}
}				//��Ϸ��������
void EngineTest::GameKeyAction(int Action)//��Ϸ��������
{

}
//��Ϸ�����Ϊ����	����ֵΪKEY_MOUSE_ACTION
void EngineTest::GameMouseAction(int x,int y,int Action)
{
	
}
