#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include "mtg.h"
#include "inc\fmod.h"

#define Ver "1.01"

FMOD_SYSTEM *g_System;		// 배경음악 재생을 위한 함수
FMOD_SOUND *g_Sound[6];		// 배경음악(효과음)의 개수
FMOD_CHANNEL *g_Channel[2]; // 동시재생될 개수

#define WIDTH_S 0			// 맵의 시작좌표 x
#define HEIGHT_S 8			// 맵의 시작좌표 y
#define WIDTH 38+2			// 맵의 너비
#define HEIGHT 21+2			// 맵의 높이

#define MY_WALL 0			// 벽 = 0
#define MY_BLANK 1			// 빈 공간 = 1
#define MY_STAR 2			// 별 = 2
#define MY_PLAYER 3			// 플레이어 = 3
#define MY_BLOCK 4			// 장애물 = 4
#define MY_DEAD 5			// 시체;; = 5

#define PLAYER_X 4			// 플레이어 기본 x좌표 = 4
#define PLAYER_Y 21			// 플레이어 기본 y좌표 = 23

#define SPEED 38			// 게임 기본 속도
//#define PLAYTIME 1000		// 게임 플레이 시간 ( 40 초)

typedef struct 
{
	char n_char[7];			// 출력용
	int n_int;				// 계산 및 저장용	
	char name[20];
}PRINT_NUM;

typedef struct
{	
	PRINT_NUM score;
	PRINT_NUM times;
}RECORD;

int map[WIDTH][HEIGHT];		// 전체 맵
int jump = 0;
int jumpcount = 0, jumpcount2 = 0;
int gameover = 0;
RECORD user;
RECORD record;

////////////////////////////////////// 초기화 ///////////////////////////////////////

void GameStart(void);		// 게임 시작 화면
void GameMain(void);		// 게임 메인
void GameIni(void);			// 게임 시작 초기화
void SoundIni(void);		// 게임 소리 초기화
void Release(void);			// 음악재생 후 종료
void Draw(void);			// 게임 전체화면 출력
void DrawMap(void);			// 맵 출력
void DrawHelp(void);		// 게임 위 도움말 표시
int Jump(void);				// 점프
int DrawBlock(void);		// 맵 이동
void GetNewBlock(void);		// 새로운 맵 생성
void MapIni(void);			// 맵 초기화
void GameOver(void);		// 게임 오버
void PrintInt(int);			// 출력을 위한 자료형 변경
void GetRecord(void);		// 최고기록 불러옴
void WriteRecord(void);		// 최고기록 저장
void GetName(void);			// 플레이어 이름 입력받음
void DrawTitle(void);		// 시작화면 출력
void Admin(void);			// 관리자 모드
int CheckPlayer(void);		// 플레이어 현재 위치 찾음 (현재 플레이어 y 좌표)
int CheckGameOver(int);		// 플레이어 게임 오버 확인 (매개변수 : 현재 플레이어 y 좌표)
void CheckBlock(void);		// 불가능한 블럭 패턴 제거
int RandomBGM(void);		// BGM 랜덤으로 재생

//////////////////////////////////////  함수들  ///////////////////////////////////////

void DrawTitle(void)
{
	_DrawTextColorOrg(10,5," I N F I N I T E ",0xf9);
	_DrawTextColorOrg(30,5,"       ",0x22);
	_DrawTextColorOrg(30,6," ",0x22);
	_DrawTextColorOrg(36,6,"  ",0x22);
	_DrawTextColorOrg(30,7,"       ",0x22);
	_DrawTextColorOrg(30,8," ",0x22);
	_DrawTextColorOrg(35,8," ",0x22);
	_DrawTextColorOrg(30,9," ",0x22);
	_DrawTextColorOrg(36,9,"  ",0x22);				// R
	_DrawTextColorOrg(39,7," ",0x22);
	_DrawTextColorOrg(39,8," ",0x22);
	_DrawTextColorOrg(40,9,"   ",0x22);
	_DrawTextColorOrg(43,8," ",0x22);
	_DrawTextColorOrg(43,7," ",0x22);
	_DrawTextColorOrg(44,9," ",0x22);				// u
	_DrawTextColorOrg(46,9," ",0x22);
	_DrawTextColorOrg(46,8," ",0x22);
	_DrawTextColorOrg(46,7,"    ",0x22);
	_DrawTextColorOrg(50,8," ",0x22);
	_DrawTextColorOrg(50,9," ",0x22);				// n
	_DrawTextColorOrg(53,4,"            ",0x00);
	_DrawTextColorOrg(61,5,"   ",0x00);
	_DrawTextColorOrg(59,6,"   ",0x00);
	_DrawTextColorOrg(57,7,"   ",0x00);
	_DrawTextColorOrg(55,8,"   ",0x00);
	_DrawTextColorOrg(53,9,"                ",0x00); // Z
	_DrawTextColorOrg(1,11,"───────────────────────────────────────",0xf9);
	_DrawTextColorOrg(70,8,"★",0xfd);
	_DrawTextColorOrg(40,13,"1 s t   E D I T I O N  ver",0xf1);
	_DrawTextColorOrg(68,13,Ver,0xf0);

	_DrawTextColorOrg(62,1,"CODING By. Z",0xf0);
}

void GetName(void) // 플레이어 이름 입력받기
{
	FMOD_System_PlaySound(g_System,FMOD_CHANNEL_FREE,g_Sound[0],0,&g_Channel[0]); // 타이틀 화면 BGM 재생

	DrawTitle();
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n  \t\t\t\t\t   ");
	_DrawTextColorOrg(21,15,"┌─────────────────┐",0xfc);
	_DrawTextColorOrg(21,16,"│                                  │",0xfc);					
	_DrawTextColorOrg(21,17,"│ Input Your Name :                │",0xfc);
	_DrawTextColorOrg(21,18,"│                                  │",0xfc);	
	_DrawTextColorOrg(21,19,"└─────────────────┘",0xfc);

	gets(user.score.name);
	if(!strcmp(user.score.name,"Admin#"))
		Admin();
	else if(!strcmp(user.score.name,"exit#"))
		exit(1);
	else if(!strcmp(user.score.name,""))  // 이름을 입력하지 않았을 때 제대로 입력하게 만들기
	{
		while(1)
		{
			system("cls");
			_DrawTextColorOrg(25,21,"Input Your Name Correctly!!",0xfc);
			DrawTitle();

			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n  \t\t\t\t\t   ");
			_DrawTextColorOrg(21,15,"┌─────────────────┐",0xfc);
			_DrawTextColorOrg(21,16,"│                                  │",0xfc);					
			_DrawTextColorOrg(21,17,"│ Input Your Name :                │",0xfc);
			_DrawTextColorOrg(21,18,"│                                  │",0xfc);	
			_DrawTextColorOrg(21,19,"└─────────────────┘",0xfc);
			gets(user.score.name);
			fflush(stdin);
			if(strcmp(user.score.name,""))
				break;
		}
	}
	else
	{
		strcpy(user.times.name,user.score.name);
	}
}

void GameStart(void)
{
	system("cls");
	DrawTitle();
	_DrawTextColorOrg(20,15,"┌────────────────────┐",0xfc);	
	_DrawTextColorOrg(20,16,"│                                        │",0xfc);	
	_DrawTextColorOrg(20,17,"│  Press any key to start Infinite RunZ! │",0xfc);
	_DrawTextColorOrg(20,18,"│                                        │",0xfc);
	_DrawTextColorOrg(20,19,"└────────────────────┘",0xfc);
	_DrawTextColorOrg(25,21,"Welcome!",0xfc);
	_DrawTextColorOrg(34,21,user.score.name,0xf0);

	while (!kbhit());

	FMOD_Channel_Stop(g_Channel[0]);
	FMOD_System_PlaySound(g_System,FMOD_CHANNEL_FREE,g_Sound[RandomBGM()],0,&g_Channel[0]); // 메인 화면 BGM 재생
}

void Admin(void)  // 관리자 모드
{
	char command[10]="0";
	int switch1=0, switch2=0;;

	system("cls");
	FMOD_Channel_Stop(g_Channel[0]);	

	while(1)
	{
		DrawTitle();
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n  \t\t\t\t\t   ");
		_DrawTextColorOrg(16,13,"Admin Mode!!",0xfc);		
		_DrawTextColorOrg(21,15,"┌─────────────────┐",0xfc);
		_DrawTextColorOrg(21,16,"│                                  │",0xfc);		
		_DrawTextColorOrg(21,17,"│ Input Command :                  │",0xfc);
		_DrawTextColorOrg(21,18,"│                                  │",0xfc);
		_DrawTextColorOrg(21,19,"└─────────────────┘",0xfc);

		if(!strcmp(command,"reset#"))			
			_DrawTextColorOrg(25,21,"Reset Record Data!!",0xfc);
		else if(switch1)
			_DrawTextColorOrg(25,21,"Set Record Data!!",0xfc);
		else if(switch2)
			_DrawTextColorOrg(25,21,"Set Speed Data!!",0xfc);

		gets(command);

		if(!strcmp(command,"reset#"))
		{
			FILE* fp1=fopen("data\\hi-score.sav","wb");
			FILE* fp2=fopen("data\\best-time.sav","wb");

			record.score.n_int = 0;
			record.times.n_int = 0;

			PrintInt(0);

			strcpy(record.score.name,"NULL");			// 하이스코어에 이름을 복사
			strcpy(record.times.name,"NULL");			// 시간 신기록에  이름을 복사

			fprintf(fp1,"%d %s",record.score.n_int,record.score.name);
			fprintf(fp2,"%d %s",record.times.n_int,record.times.name);
			
			fclose(fp1);
			fclose(fp2);	
		}
		else if(!strcmp(command,"exit#"))
		{
			system("cls");
			GetName();
			break;
		}

		system("cls");
	}
}

void CheckBlock(void)		// 불가능한 블럭 패턴 제거
{
	int i;

	if(map[WIDTH - 2][PLAYER_Y] == MY_BLOCK)
	{
		for(i = 6; i < 18; i++)
		{
			if(map[WIDTH - i][PLAYER_Y] == MY_BLOCK || map[WIDTH - i][PLAYER_Y - 1] == MY_BLOCK)
			{
				map[WIDTH - 2][PLAYER_Y] = MY_BLANK;
				map[WIDTH - 2][PLAYER_Y - 1] = MY_BLANK;
			}
		}
	}
}

void GetNewBlock(void)
{	
	if(rand()%100+1 <= 25)		// 25%
	{				
		map[WIDTH-2][PLAYER_Y-rand()%5] = MY_STAR;
	}

	if(rand()%100+1 <= 10)		// 10%
	{			
		map[WIDTH-2][PLAYER_Y] = MY_BLOCK;
	}
	else if(rand()%100+1 <= 10)		// 10%
	{
		map[WIDTH-2][PLAYER_Y] = MY_BLOCK;
		map[WIDTH-2][PLAYER_Y-1] = MY_BLOCK;
	}

	CheckBlock();
}

int DrawBlock(void)		// 맵 전체를 왼쪽으로 이동
{
	int x, y;

	GetNewBlock();

	if(CheckGameOver(CheckPlayer()) == -1)
	{
		return -1;
	}
	for(x=1; x<WIDTH-2; x++)
	{
		for(y=HEIGHT_S+1; y<HEIGHT-1; y++)
		{
			if(map[x][y] == MY_BLOCK && map[x-1][y] == MY_PLAYER)	// 블럭에 닿으면 게임오버
			{
				return -1;
			}
			else if(map[x][y] == MY_STAR && map[x-1][y] == MY_PLAYER)		// 별 먹었을 때
			{
				user.score.n_int+=10;
				map[x][y] = map[x+1][y];
			}
			else if(map[x][y] == MY_STAR && map[x+1][y] == MY_PLAYER)
				map[x][y] = MY_BLANK;
			else if(map[x][y] == MY_BLOCK && map[x+1][y] == MY_PLAYER)
				map[x][y] = MY_BLANK;
			else if(map[x][y] != MY_PLAYER && map[x+1][y] != MY_PLAYER)
				map[x][y] = map[x+1][y];
		}
	}
	for(y=HEIGHT_S+1; y<HEIGHT-1; y++)
	{
		map[WIDTH-2][y] = MY_BLANK;
	}
	return 0;
}

int Jump(void)
{
	if(CheckGameOver(CheckPlayer()) == -1)
	{
		return -1;
	}
	if(jump && jumpcount<8)
	{
		if(jumpcount%2==0)
		{
			if(map[PLAYER_X][PLAYER_Y-jumpcount2-1] == MY_STAR)
			{
				user.score.n_int+=10;
			}
			else if(map[PLAYER_X][PLAYER_Y-jumpcount2-1] == MY_BLOCK)		// 점프하다가 사망
			{
				//map[PLAYER_X][PLAYER_Y-jumpcount2] = MY_DEAD;
				return -1;
			}
			map[PLAYER_X][PLAYER_Y-jumpcount2-1] = MY_PLAYER;
			map[PLAYER_X][PLAYER_Y-jumpcount2] = MY_BLANK;
			jumpcount++;
			jumpcount2++;
		}
		else
			jumpcount++;
	}
	else if(jump && jumpcount==16)
	{		
		jump=0;
		jumpcount=0;
	}
	else if(jump && jumpcount>=8)
	{
		if(jumpcount%2==0)
		{			
			jumpcount2--;
			if(map[PLAYER_X][PLAYER_Y-jumpcount2] == MY_STAR)
			{
				user.score.n_int+=10;
			}
			else if(map[PLAYER_X][PLAYER_Y-jumpcount2] == MY_BLOCK)		// 내려오다가 사망
			{
				if(map[PLAYER_X][PLAYER_Y-jumpcount2 - 1] == MY_BLOCK)
				{
					map[PLAYER_X][PLAYER_Y-jumpcount2 - 1] = MY_BLANK;
					map[PLAYER_X + 1][PLAYER_Y-jumpcount2 - 1] = MY_BLOCK;
				}
				return -1;
			}
			map[PLAYER_X][PLAYER_Y-jumpcount2] = MY_PLAYER;
			map[PLAYER_X][PLAYER_Y-jumpcount2-1] = MY_BLANK;
			jumpcount++;			
		}
		else
			jumpcount++;
	}
	return 0;
}

void DrawMap(void)
{
	int x, y;

	for(y=HEIGHT_S; y<HEIGHT; y++)
	{
		for(x=WIDTH_S; x<WIDTH; x++)
		{
			if(map[x][y]==MY_WALL)
			{
				if(x==WIDTH_S && y==HEIGHT_S)
					_DrawTextColor(x*2,y,"┌",0xf8);
				else if(x==WIDTH-1 && y==HEIGHT-1)
					_DrawTextColor(x*2,y,"┘",0xf8);
				else if(x==WIDTH_S && y==HEIGHT-1)
					_DrawTextColor(x*2,y,"└",0xf8);
				else if(x==WIDTH-1 && y==HEIGHT_S)
					_DrawTextColor(x*2,y,"┐",0xf8);				
				else if(y==HEIGHT-1 || y==HEIGHT_S)
					_DrawTextColor(x*2,y,"─",0xf8);				
				else
					_DrawTextColor(x*2,y,"│",0xf8);
			}			
			else if(map[x][y]==MY_BLANK)
				_DrawTextColor(x*2,y,"  ",0xf0);

			else if(map[x][y]==MY_BLOCK)
				_DrawTextColor(x*2,y,"■",0xf0);
						
			else if(map[x][y]==MY_STAR)		
				_DrawTextColor(x*2, y,"♥",0xfc);

			else if(map[x][y]==MY_PLAYER)
				_DrawTextColor(x*2,y,"웃",0xf9);

			else if(map[x][y]==MY_DEAD)
			{
				if(map[x][y - 1]==MY_PLAYER)
					_DrawTextColor(x*2,y - 1,"농",0xfc);
				else
					_DrawTextColor(x*2,y,"농",0xfc);
			}
		}
	}
}

void DrawHelp(void)
{
	_DrawTextColor(2,1,"Infinite RunZ  1st EDITION ver",0xf2);				// 게임 설명 출력
	_DrawTextColor(34,1,Ver,0xf0);
	_DrawTextColor(65,1,"CODING By. Z",0xf3);
	_DrawTextColor(2,3,"SCORE : ",0xf0);
	_DrawTextColor(14,3,user.score.n_char,0xf8);
	_DrawTextColor(25,3,"DISTANCE : ",0xf0);
	_DrawTextColor(40,3,user.times.n_char,0xf8);
	_DrawTextColor(2,5,"Hi-SCORE : ",0xf1);
	_DrawTextColor(14,5,record.score.n_char,0xf0);
	_DrawTextColor(25,5,"BEST-RECORD : ",0xf1);
	_DrawTextColor(40,5,record.times.n_char,0xf0);
	_DrawTextColor(46,5,"m",0xf0);
	_DrawTextColor(46,3,"m",0xf0);
	_DrawTextColor(14,7,record.score.name,0xf0);
	_DrawTextColor(38,7,record.times.name,0xf0);	
	_DrawTextColor(48,0,"│",0xf8);
	_DrawTextColor(48,1,"│",0xf8);
	_DrawTextColor(48,2,"│",0xf8);
	_DrawTextColor(48,3,"│",0xf8);
	_DrawTextColor(48,4,"│",0xf8);
	_DrawTextColor(48,5,"│",0xf8);
	_DrawTextColor(48,6,"│",0xf8);
	_DrawTextColor(48,7,"│",0xf8);
	_DrawTextColor(48,8,"┴",0xf8);
	_DrawTextColor(53,3,"Space : Jump",0xf0);
	_DrawTextColor(53,5,"P : pause / Q : quit",0xf0);
	_DrawTextColor(45,23,"copyright_2014_zeikar@naver.com",0xf0);

	if(gameover)
	{
		_DrawTextColor(30,9,"GAME OVER!!!!",0xfc);
		_DrawTextColor(20,11,"Press 'q' key to exit / 'r' key to restart",0xfc);
	}

	if(user.times.n_int / 1000 > 0 && user.times.n_int % 1000 <= 50)
	{
		_DrawTextColor(22,7,"SPEED UP!",0xfc);
	}
}

void Draw(void)
{	
	DrawMap();		// 맵 전체 틀 출력	
	DrawHelp();
}

void PrintInt(int n)  // int형을 char형으로 변환시켜줌
{	
	if(n)		// 플레이어
	{
		user.score.n_char[0] = user.score.n_int/100000+48;
		user.score.n_char[1] = (user.score.n_int%100000)/10000+48;
		user.score.n_char[2] = (user.score.n_int%10000)/1000+48;
		user.score.n_char[3] = (user.score.n_int%1000)/100+48;
		user.score.n_char[4] = (user.score.n_int%100)/10+48;
		user.score.n_char[5] = (user.score.n_int%10)+48;

		user.times.n_char[0] = user.times.n_int/100000+48;
		user.times.n_char[1] = (user.times.n_int%100000)/10000+48;
		user.times.n_char[2] = (user.times.n_int%10000)/1000+48;
		user.times.n_char[3] = (user.times.n_int%1000)/100+48;
		user.times.n_char[4] = (user.times.n_int%100)/10+48;
		user.times.n_char[5] = (user.times.n_int%10)+48;
	}
	else		// 최고기록
	{
		record.score.n_char[0] = record.score.n_int/100000+48;
		record.score.n_char[1] = (record.score.n_int%100000)/10000+48;
		record.score.n_char[2] = (record.score.n_int%10000)/1000+48;
		record.score.n_char[3] = (record.score.n_int%1000)/100+48;
		record.score.n_char[4] = (record.score.n_int%100)/10+48;
		record.score.n_char[5] = (record.score.n_int%10)+48;

		record.times.n_char[0] = record.times.n_int/100000+48;
		record.times.n_char[1] = (record.times.n_int%100000)/10000+48;
		record.times.n_char[2] = (record.times.n_int%10000)/1000+48;
		record.times.n_char[3] = (record.times.n_int%1000)/100+48;
		record.times.n_char[4] = (record.times.n_int%100)/10+48;
		record.times.n_char[5] = (record.times.n_int%10)+48;
	}
}

int RunKey() // 키 입력받는 함수
{

	char k;

	k=_GetKey();
	if(-1==k) return 0;

	if('q'==k) // q 입력받으면 -1 (종료) 반환
	{
		return -1;
	}
	else if ('p'==k)  // 게임 일시정지
	{
		_MessageBoxColor(23,9,34,2," Paused. press any key to resume",0xf4); 
	}
	else if(32 ==k) // 스페이스바 누르면 점프
	{
		if(!jump)
			FMOD_System_PlaySound(g_System,FMOD_CHANNEL_FREE,g_Sound[5],0,&g_Channel[1]);  // 점프할 때 효과음
		jump=1;
	}
	_Invalidate();

	return 0;
}

void RunTimer() // 타이머
{
	static int speed=SPEED;
	static long oldT=0;
	long newT;
	static int killTimer=0;

	if(-1==killTimer)
		return;

	newT=_GetTickCount();
	if(abs(newT-oldT)<speed) // 속도
	{
		return;
	}
	else
	{
		oldT=newT;
	}

	if(-1==Jump())				// 점프하다가 게임 오버
	{
		int y;//= CheckPlayer();
		for(y = 0; y <= CheckPlayer(); y++)
		{
			if(map[PLAYER_X][y] == MY_PLAYER)
				map[PLAYER_X][y] = MY_DEAD;
		}

		WriteRecord();
		gameover=1;
		killTimer=-1;
		_Invalidate();
		GameOver();
		
		GameIni();
		killTimer=0;
		user.times.n_int=0;
	}

	else if(-1==DrawBlock())				// 게임 오버
	{
		WriteRecord();
		gameover=1;
		killTimer=-1;
		map[PLAYER_X][PLAYER_Y] = MY_DEAD;
		_Invalidate();
		
		GameOver();
		
		GameIni();
		killTimer=0;
		//user.times.n_int=0;
	}
	
	user.times.n_int++;
	
	if(user.times.n_int % 1000 == 0)		// 1000점 마다 스피드 업!
	{
		if(speed > 7)
		{
			speed -= 2;
		}
	}
	PrintInt(1);
	_Invalidate();
}

void GameMain(void)
{
	while(1)
	{
		RunTimer();
		if(-1==RunKey()) // 반환값이 -1 이면 게임 종료
		{
			break;
		}
	}
}

void MapIni(void)
{
	int x, y;

	for(y=0; y<HEIGHT; y++)
	{
		for(x=0; x<WIDTH; x++)
		{
			if(x==WIDTH_S || y==HEIGHT_S || x==WIDTH-1 || y==HEIGHT-1)
				map[x][y]=MY_WALL;
			else
				map[x][y]=MY_BLANK;
		}
	}
}

void GameOver(void)
{	
	char key;

	FMOD_System_PlaySound(g_System,FMOD_CHANNEL_FREE,g_Sound[4],0,&g_Channel[1]); // 게임 오버 사운드 재생

	while(1)
	{
		if(kbhit())
		{
			key=getch();
			if(key=='q')		// q키 누르면 게임 종료
				exit(0);

			else if(key=='r')	// r키 누르면 게임 재시작
			{
				jump=0;
				jumpcount=0;
				jumpcount2=0;
				gameover=0;
				break;
			}
		}
	}
}

int CheckPlayer(void)
{
	int y;
	for(y = PLAYER_Y; y < PLAYER_Y+6; y++)
	{
		if(map[PLAYER_X][y] == MY_PLAYER)
			return y;
	}
}

int CheckGameOver(int current)
{
	if(map[PLAYER_X + 1][current] == MY_BLOCK)
	{
		return -1;
	}
	return 0;
}

void GetRecord(void)		// 최고기록 로드
{
	FILE* fp1=fopen("data\\hi-score.sav","rb");
	FILE* fp2=fopen("data\\best-time.sav","rb");
	if(fp1==NULL || fp2==NULL)
	{
		return;
	}	
	fscanf(fp1,"%d %s",&record.score.n_int,record.score.name);
	fscanf(fp2,"%d %s",&record.times.n_int,record.times.name);

	fclose(fp1);
	fclose(fp2);

	PrintInt(0);
}

void WriteRecord(void)		// 최고기록 저장
{
	if (user.score.n_int > record.score.n_int) // 현재 점수가 기록보다 높을 때!! 하이스코어 달성~!!
	{
		FILE* fp=fopen("data\\hi-score.sav","wb");
		record.score.n_int = user.score.n_int;

		PrintInt(0);

		strcpy(record.score.name,user.score.name);  // 하이스코어에 유저 이름을 복사

		fprintf(fp,"%d %s",record.score.n_int,record.score.name);
		fclose(fp);
	}
	if (user.times.n_int > record.times.n_int)		// 시간 신기록
	{
		FILE* fp=fopen("data\\best-time.sav","wb");
		record.times.n_int = user.times.n_int;

		PrintInt(0);

		strcpy(record.times.name,user.times.name);  // 시간 신기록에 유저 이름을 복사

		fprintf(fp,"%d %s",record.times.n_int,record.times.name);
		fclose(fp);
	}
}

void GameIni(void)		// 게임 초기화
{	
	MapIni();

	map[PLAYER_X][PLAYER_Y] = MY_PLAYER;
	user.score.n_int = 0;
	user.times.n_int = 0;	
	PrintInt(1);

	GetNewBlock();
}

int RandomBGM(void)		// BGM 랜덤으로 재생
{
	srand(time(NULL));

	return rand() % 3 + 1;
}

void SoundIni(void)
{
	FMOD_System_Create(&g_System);
	FMOD_System_Init(g_System,10,FMOD_INIT_NORMAL,NULL);

	FMOD_System_CreateSound(g_System,"data\\bgm_lobby.mp3",FMOD_LOOP_NORMAL,0,&g_Sound[0]); // 타이틀 화면 BGM
	FMOD_System_CreateSound(g_System,"data\\bgm_main.ogg",FMOD_LOOP_NORMAL,0,&g_Sound[1]);  // 메인 화면 BGM
	FMOD_System_CreateSound(g_System,"data\\bgm_main2.ogg",FMOD_LOOP_NORMAL,0,&g_Sound[2]);  // 메인 화면2 BGM
	FMOD_System_CreateSound(g_System,"data\\bgm_main3.mp3",FMOD_LOOP_NORMAL,0,&g_Sound[3]);  // 메인 화면3 BGM
	FMOD_System_CreateSound(g_System,"data\\g_end.mp3",FMOD_DEFAULT,0,&g_Sound[4]);         // 게임오버 효과음
	FMOD_System_CreateSound(g_System,"data\\g_jelly.mp3",FMOD_DEFAULT,0,&g_Sound[5]);       // 점프 효과음
}

void Release(void) // 음악재생 후 종료
{
	FMOD_Sound_Release(g_Sound[0]);
	FMOD_Sound_Release(g_Sound[1]);
	FMOD_Sound_Release(g_Sound[2]);
	FMOD_Sound_Release(g_Sound[3]);
	FMOD_Sound_Release(g_Sound[4]);
	FMOD_Sound_Release(g_Sound[5]);
	FMOD_System_Close(g_System);
	FMOD_System_Release(g_System);
}

void main(void)
{
	system("color f0");
	puts("\n * Loading...Please Wait..");

	SoundIni();
	FMOD_System_Update(g_System);

	srand(time(NULL));
	GameIni();

	system("cls");

	GetName();
	_BeginWindow();
	GetRecord();
	GameStart();
	GameMain();
	Release();
}