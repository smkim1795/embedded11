#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "textlcddrv.h"
#include "textlcd.h"

#define TEXTLCD_DRIVER_NAME		"/dev/peritextlcd"

static int fd;

int textlcdInit(void)
{
	fd = open(TEXTLCD_DRIVER_NAME,O_RDWR);
	if ( fd < 0 )
	{
		perror("driver (//dev//peritextlcd) open error.\n");
		return 1;
	}
	textlcdclear();
}

int textlcdExit(void)
{
	textlcdclear();
	close(fd);
}

int textlcdclear(void)
{
	const char s1[16] = {};
	const char s2[16] = {};
	lcdtextwrite(s1,s2,1);
	lcdtextwrite(s1,s2,2);
}

int lcdtextwrite(const char *str1, const char *str2, int lineFlag)
{
	unsigned int linenum = 0;
	stTextLCD  stlcd; 
	int len; 
	
	memset(&stlcd,0,sizeof(stTextLCD));  //구조체 초기화
	linenum = lineFlag;
	printf("linenum :%d\n", linenum);
	
	if ( linenum == 1) //첫번째 line
	{
		stlcd.cmdData = CMD_DATA_WRITE_LINE_1;
		printf("string:%s\n",str1);
		len = strlen(str1);
		if ( len > COLUMN_NUM)
		{
			memcpy(stlcd.TextData[stlcd.cmdData - 1],str1,COLUMN_NUM);
		}
		else
		{
			memcpy(stlcd.TextData[stlcd.cmdData - 1],str1,len);
		}
	}

	else if ( linenum == 2) //두번째 line
	{
		stlcd.cmdData = CMD_DATA_WRITE_LINE_2;
		printf("string:%s\n",str2);
		len = strlen(str2);
		if ( len > COLUMN_NUM)
		{
			memcpy(stlcd.TextData[stlcd.cmdData - 1],str2,COLUMN_NUM);
		}
		else
		{
			memcpy(stlcd.TextData[stlcd.cmdData - 1],str2,len);
		}
	}

	else 
	{
		printf("linenum : %d  wrong .  range (1 ~ 2)\n", linenum);
		return 1; 
	}

	stlcd.cmd = CMD_WRITE_STRING;
	write(fd,&stlcd,sizeof(stTextLCD));
	return 0;
}
