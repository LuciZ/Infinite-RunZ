
#include <stdio.h>
#include <stdlib.h>
#include "mtg.h"
// 시간 나면 FOMD로 노래도 넣어보세요.
/*

	For XP,Vista,Windows 7
	2010.5.29 by clccclcc
*/
/* --------------------------------------------------------------------*/

/*--------------------------------------------------------*/
#ifdef _MSWINDOWS_ // 윈도우일 경우에만 컴파일
/*--------------------------------------------------------*/
#include <windows.h>
#include <conio.h>


	//double buffring
    void __BitBltBuffer();
	void __ClearBuffer();	 
	void __DrawTextBuffer(int x,int y,char* msg);
	void __DrawTextBufferColor(int x,int y,char* msg,int color);
	void __InvalidateBuffer();
	void __InvalidateBufferColor();



///////////////////////////////////////////////////
void _Delay(int time)
{
    Sleep(time);
}

long _GetTickCount()
{
        return GetTickCount();
}
///////////////////////////////////////////////////

/* Standard error macro for reporting API errors */
 #define PERR(bSuccess, api){if(!(bSuccess)) printf("%s:Error %d from %s on line %d\n", __FILE__, GetLastError(), api, __LINE__);}

 void _cls() // 화면 초기화
 {
        HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
        COORD coordScreen = { 0, 0 };    /* here's where we'll home the
                                                                                cursor */
        BOOL bSuccess;
        DWORD cCharsWritten;
        CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
        DWORD dwConSize;                 /* number of character cells in
                                                                                the current buffer */

        /* get the number of character cells in the current buffer */

        bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );
        PERR( bSuccess, "GetConsoleScreenBufferInfo" );
        dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

        /* fill the entire screen with blanks */

        bSuccess = FillConsoleOutputCharacter( hConsole, (TCHAR) ' ',
           dwConSize, coordScreen, &cCharsWritten );
        PERR( bSuccess, "FillConsoleOutputCharacter" );

        /* get the current text attribute */

        bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );
        PERR( bSuccess, "ConsoleScreenBufferInfo" );

        /* now set the buffer's attributes accordingly */

        bSuccess = FillConsoleOutputAttribute( hConsole, csbi.wAttributes,
           dwConSize, coordScreen, &cCharsWritten );
        PERR( bSuccess, "FillConsoleOutputAttribute" );

        /* put the cursor at (0, 0) */

        bSuccess = SetConsoleCursorPosition( hConsole, coordScreen );
        PERR( bSuccess, "SetConsoleCursorPosition" );
        return;
 }

 void _cls1( void )
{
  DWORD n;                         /* Number of characters written */
  DWORD size;                      /* number of visible characters */
  COORD coord = {0};               /* Top left screen position */
  CONSOLE_SCREEN_BUFFER_INFO csbi;

  /* Get a handle to the console */
  HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );

  GetConsoleScreenBufferInfo ( h, &csbi );

  /* Find the number of characters to overwrite */
  size = csbi.dwSize.X * csbi.dwSize.Y;

  /* Overwrite the screen buffer with whitespace */
  FillConsoleOutputCharacter ( h, TEXT ( ' ' ), size, coord, &n );
  GetConsoleScreenBufferInfo ( h, &csbi );
  FillConsoleOutputAttribute ( h, csbi.wAttributes, size, coord, &n );

  /* Reset the cursor to the top left position */
  SetConsoleCursorPosition ( h, coord );
}


///////////////////////
char _GetKey() // 키 입력받는 함수
{
        char key=-1;
        if(kbhit())
        {
                key=getch();
        }

        return key;
}
void _BeginWindow() // 처음 화면
{
        CONSOLE_CURSOR_INFO CurInfo;

        CurInfo.dwSize=1;
        CurInfo.bVisible=FALSE;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CurInfo);

        _cls(); // 화면 초기화
}
void _EndWindow() // 화면 초기화
{
        _cls();
}
void _Invalidate() // 콘솔창에 색깔, 테트리스 창 출력
{
	__InvalidateBuffer();
	
    //    _cls();
    //    Draw();
}

void _InvalidateColor() // 콘솔창에 색깔 출력
{
	__InvalidateBufferColor();
	
    //    _cls();
    //    Draw();
}

void _InvalidateBack() // 색깔 넣지 않고 테트리스 창 출력 
{
	Draw();
	_BitBlt();
}
void _DrawText(int x,int y,char* msg) // 글자 출력
{

	__DrawTextBuffer(x,y,msg);
//		DWORD id;
//        COORD co;
//		HANDLE hW;
//        co.X=x;
//        co.Y=y;
        
//		hW=GetStdHandle(STD_OUTPUT_HANDLE);
//        WriteConsoleOutputCharacter(hW,msg,strlen(msg),co,&id);	
}
void _DrawTextColor(int x,int y,char* msg,int color) // 색깔있는 글자 출력
{
	__DrawTextBufferColor(x,y,msg,color); // 색깔있는 글자 출력
}

void _BitBlt()
{
	__BitBltBuffer();
}

//----------------------------------------Begin Double Buffering ---------
#define SCREEN_WIDTH 80  // 화면 전체 너비
#define SCREEN_HEIGHT 25  // 화면 전체 높이
CHAR_INFO  g_Buffer[SCREEN_HEIGHT][SCREEN_WIDTH];
int g_TextColor=0xf0; // 16진수 앞자리 : 배경색상, 뒷자리 : 글자 색상 (검은색 0 , 흰색 f)

void _SetColor(int color) // 콘솔 화면 색깔을 정할 수 있는 함수
{
	g_TextColor=color;
}
void __BitBltBuffer()
{
	HANDLE hOutput = (HANDLE)GetStdHandle( STD_OUTPUT_HANDLE ); 

	COORD dwBufferSize = { SCREEN_WIDTH,SCREEN_HEIGHT }; 
	COORD dwBufferCoord = { 0, 0 }; 
	SMALL_RECT rcRegion = { 0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1 }; 

	CHAR_INFO buffer[SCREEN_HEIGHT][SCREEN_WIDTH]; 

	ReadConsoleOutput( hOutput, (CHAR_INFO *)buffer, dwBufferSize, 
		dwBufferCoord, &rcRegion ); 

 

	WriteConsoleOutput( hOutput, (CHAR_INFO *)g_Buffer, dwBufferSize, 
		dwBufferCoord, &rcRegion ); 

}
void __ClearBuffer() // 콘솔화면 전체에 색깔을 삽입
{
	int x,y;

	for(y=0;y<25;y++)
	{
		for(x=0;x<80;x++)
		{
			g_Buffer[y][x].Char.AsciiChar=' ';
			g_Buffer[y][x].Attributes=g_TextColor;
		}
	}
	g_Buffer[24][79].Char.AsciiChar='\0';
}
void __DrawTextBuffer(int x,int y,char* msg) // 문자 출력
{
	int i;

	for(i=0;i<strlen(msg);i++)
	{
		g_Buffer[y][x+i].Char.AsciiChar=msg[i];
		g_Buffer[y][x+i].Attributes=g_TextColor;
	}
}
void __DrawTextBufferColor(int x,int y,char* msg,int color) // 색깔있는 문자 출력
{
	int i;

	for(i=0;i<strlen(msg);i++)
	{
		g_Buffer[y][x+i].Char.AsciiChar=msg[i];
		g_Buffer[y][x+i].Attributes=color;
	}
}
void __InvalidateBuffer() // 콘솔창에 색깔과 테트리스 창 출력
{	
	__ClearBuffer();	
	Draw();
	_BitBlt();
}

void __InvalidateBufferColor() // 콘솔창에 배경색깔만 출력
{	
	__ClearBuffer();
	_BitBlt();
}
//--------------------------------------End Duble Buffering...---------------------------
 
void _DrawTextOrg(int x,int y,char* msg) // 문자 출력
{

		DWORD id;
        COORD co;
		HANDLE hW;
        co.X=x;
        co.Y=y;
        
		hW=GetStdHandle(STD_OUTPUT_HANDLE);
        WriteConsoleOutputCharacter(hW,msg,strlen(msg),co,&id);	
}
void _DrawTextColorOrg(int x,int y,char* msg,int color) // 색깔있는 문자 출력
{
		CONSOLE_SCREEN_BUFFER_INFO   BufInfo;
		COORD Coor;

		DWORD id;
        COORD co;
		HANDLE hW;
        co.X=x;
        co.Y=y;
        
		hW=GetStdHandle(STD_OUTPUT_HANDLE);

		GetConsoleScreenBufferInfo(hW,&BufInfo);
	    Coor.X=BufInfo.dwCursorPosition.X;
	    Coor.Y=BufInfo.dwCursorPosition.Y;


        WriteConsoleOutputCharacter(hW,msg,strlen(msg),co,&id);	

		FillConsoleOutputAttribute(hW,color,strlen(msg),co,&id);


}
 
void _MessageBox(int x,int y,int w,int h,char* msg) // 메시지 박스 출력
{
        char key;
        int tx,ty;
        int len=(w-strlen(msg))/2;

        for(tx=0;tx<w;tx++)
        {
                _DrawTextOrg(x+tx,y,"-");
        }

        for(tx=0;tx<w;tx++)
        {
                _DrawTextOrg(x+tx,y+h,"-");
        }

        for(ty=0;ty<h;ty++)
        {
                _DrawTextOrg(x,y+ty,"|");
        }

        for(ty=0;ty<h;ty++)
        {
                _DrawTextOrg(x+w,y+ty,"|");
        }
        _DrawTextOrg(x+len,y+h/2,msg);
        key=getch();
}
void _MessageBoxEx(int x,int y,int w,int h,char* msg) // 메시지 박스 출력
{	
	int loop_len;
	int loop;
	int index;
	int d_index;

	char line[2048];
	char key;
	int tx,ty;
	int len=(w-strlen(msg))/2;

	int x_start; // x + (len - strlen(line)/2)

	for(tx=0;tx<w;tx++)
	{
		_DrawTextOrg(x+tx,y,"-");
	}

	for(tx=0;tx<w;tx++)
	{
		_DrawTextOrg(x+tx,y+h,"-");
	}

	for(ty=0;ty<h;ty++)
	{
		_DrawTextOrg(x,y+ty,"|");
	}

	for(ty=0;ty<h;ty++)
	{
		_DrawTextOrg(x+w,y+ty,"|");
	}
	///////////////////////////
	loop_len=1;
	index=0;
	while(1)
	{
		if('\n'==msg[index])
		{
			loop_len++;
		}
		if('\0'==msg[index])
		{
			break;
		}
		index++;
	}
	loop=0;
	index=0;
	d_index=0;
	while(1)
	{
		if('\n'==msg[index])
		{
			line[d_index]='\0';
			d_index=0;
			_DrawTextOrg(x + (w/2 - strlen(line)/2),y+(h/2-loop_len/2)+ loop,line);
			loop++;
			index +=1;		
		}
		else if('\0'==msg[index])
		{
			line[d_index]='\0';
			_DrawTextOrg(x + (w/2 - strlen(line)/2),y+(h/2-loop_len/2)+ loop,line);
			break;
		}
		else 
		{
			line[d_index]=msg[index];			
			d_index++;
			index++;		
		}
			
	}

	key=getch();
}
void _MessageBoxColor(int x,int y,int w,int h,char* msg,int color) // 색깔있는 메시지박스 출력
{
        char key;
        int tx,ty;
        int len=(w-strlen(msg))/2;

        for(tx=0;tx<w;tx++)
        {
                _DrawTextColorOrg(x+tx,y,"-",color);
        }

        for(tx=0;tx<w;tx++)
        {
                _DrawTextColorOrg(x+tx,y+h,"-",color);
        }

        for(ty=0;ty<h;ty++)
        {
                _DrawTextColorOrg(x,y+ty,"|",color);
        }

        for(ty=0;ty<h;ty++)
        {
                _DrawTextColorOrg(x+w,y+ty,"|",color);
        }
        _DrawTextColorOrg(x+len,y+h/2,msg,color);
        key=getch();
}
void _MessageBoxExColor(int x,int y,int w,int h,char* msg,int color) // 색깔있는 메시지박스 출력
{	
	int loop_len;
	int loop;
	int index;
	int d_index;

	char line[2048];
	char key;
	int tx,ty;
	int len=(w-strlen(msg))/2;

	int x_start; // x + (len - strlen(line)/2)

	for(tx=0;tx<w;tx++)
	{
		_DrawTextColorOrg(x+tx,y,"-",color);
	}

	for(tx=0;tx<w;tx++)
	{
		_DrawTextColorOrg(x+tx,y+h,"-",color);
	}

	for(ty=0;ty<h;ty++)
	{
		_DrawTextColorOrg(x,y+ty,"|",color);
	}

	for(ty=0;ty<h;ty++)
	{
		_DrawTextColorOrg(x+w,y+ty,"|",color);
	}
	///////////////////////////
	loop_len=1;
	index=0;
	while(1)
	{
		if('\n'==msg[index])
		{
			loop_len++;
		}
		if('\0'==msg[index])
		{
			break;
		}
		index++;
	}
	loop=0;
	index=0;
	d_index=0;
	while(1)
	{
		if('\n'==msg[index])
		{
			line[d_index]='\0';
			d_index=0;
			_DrawTextColorOrg(x + (w/2 - strlen(line)/2),y+(h/2-loop_len/2)+ loop,line,color);
			loop++;
			index +=1;		
		}
		else if('\0'==msg[index])
		{
			line[d_index]='\0';
			_DrawTextColorOrg(x + (w/2 - strlen(line)/2),y+(h/2-loop_len/2)+ loop,line,color);
			break;
		}
		else 
		{
			line[d_index]=msg[index];			
			d_index++;
			index++;		
		}
			
	}

	key=getch();
}


#endif
