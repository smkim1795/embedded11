#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "buzzer.h"

int main(void)
{
   buzzerInit();

   printf("BGM_START: GAME START!\r\n");
   BGM_START();

   printf("BGM_GAME1: 미로 GAME START!\r\n");
   BGM_GAME1();

   printf("BGM_GAME2: 같은 그림 찾기 GAME START!\r\n");
   BGM_GAME2();

   printf("buzzer off\r\n");
   buzzerExit();
   return 0;
}
