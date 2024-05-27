#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "buzzer.h"

#define MAX_SCALE_STEP 8
#define BUZZER_BASE_SYS_PATH "/sys/bus/platform/devices/"
#define BUZZER_FILENAME "peribuzzer"
#define BUZZER_ENABLE_NAME "enable"
#define BUZZER_FREQUENCY_NAME "frequency"

static char gBuzzerBaseSysDir[128];
static const int musicScale[MAX_SCALE_STEP] =
{
   262, /*do*/ 294,330,349,392,440,494, /* si */ 523
//  0           1   2   3   4   5   6            7
};

static int fd_en, fd_fre;
static char path_en[200] = {0,}, path_freq[200] = {0,};

int findBuzzerSysPath(char *gBuzzerBaseSysDir) //버저 경로 찾기
{
   DIR * dir_info = opendir(BUZZER_BASE_SYS_PATH);
   int ifNotFound = 1;
   if (dir_info != NULL){
      while (1){
         struct dirent *dir_entry;
         dir_entry = readdir (dir_info);
         if (dir_entry == NULL) break;
         if (strncasecmp(BUZZER_FILENAME, dir_entry->d_name, strlen(BUZZER_FILENAME)) == 0)
         {
            ifNotFound = 0;
            sprintf(gBuzzerBaseSysDir,"%s%s/",BUZZER_BASE_SYS_PATH,dir_entry->d_name);
         }
      }
   }

   printf("find %s\n",gBuzzerBaseSysDir);
   return ifNotFound;
}

int buzzerInit(void) //+Buzzerenable, setFrequency (sprintf)
{
   if(findBuzzerSysPath(gBuzzerBaseSysDir) == 1)
   {
      printf("Failed. File not exist\r\n");
      return 0;
   }
   
   sprintf(path_en,"%s%s",gBuzzerBaseSysDir,BUZZER_ENABLE_NAME);
   sprintf(path_fre,"%s%s",gBuzzerBaseSysDir,BUZZER_FREQUENCY_NAME);

   fd_en=open(path_en,O_WRONLY);
   fd_fre=open(path_fre,O_WRONLY);

   return 1;
}

int buzzerPlaySong(int scale)
{
   dprintf(fd_fre, "%d", scale);
   write(fd_en, &"1", 1);

   return 1;
}

int buzzerStopSong(void)
{
   write(fd_en, &"0", 1);

   return 1;
}

int buzzerExit(void)
{
   buzzerStopSong();
   close(fd_fre);
   close(fd_en);
   return 0;
}

int BGM_START(void)
{   
   buzzerPlaySong(musicScale[0]);
   usleep(150000);
   buzzerPlaySong(musicScale[2]);
   usleep(150000);
   buzzerPlaySong(musicScale[4]);
   usleep(150000);
   buzzerPlaySong(musicScale[7]); 
   usleep(450000); //도미솔도
   
   buzzerStopSong(); // 부저 끄기
}

int BGM_GAME1(void) //도미솔 도미솔 라라라 솔 파파파 미미미 레레레 도 (미로 전용 BGM)
{
    buzzerPlaySong(musicScale[0]); 
    usleep(150000);
    buzzerPlaySong(musicScale[2]);
    usleep(150000);
    buzzerPlaySong(musicScale[4]);
    usleep(150000);
    buzzerPlaySong(musicScale[0]);
    usleep(150000);
    buzzerPlaySong(musicScale[2]); 
    usleep(150000);  
    buzzerPlaySong(musicScale[4]); 
    usleep(150000); //도미솔도미솔
    buzzerPlaySong(musicScale[5]);
    usleep(150000);
    buzzerPlaySong(musicScale[5]);
    usleep(150000);
    buzzerPlaySong(musicScale[5]);
    usleep(150000);
    buzzerPlaySong(musicScale[4]); 
    usleep(450000); //라라라솔

    buzzerPlaySong(musicScale[3]);
    usleep(150000);
    buzzerPlaySong(musicScale[3]);
    usleep(150000);
    buzzerPlaySong(musicScale[3]);
    usleep(150000);
    buzzerPlaySong(musicScale[2]); 
    usleep(150000);
    buzzerPlaySong(musicScale[2]); 
    usleep(150000);
    buzzerPlaySong(musicScale[2]);
    usleep(150000); //파파파미미미
    buzzerPlaySong(musicScale[1]);
    usleep(150000);
    buzzerPlaySong(musicScale[1]);
    usleep(150000);
    buzzerPlaySong(musicScale[1]);
    usleep(150000);
    buzzerPlaySong(musicScale[0]);
    usleep(450000); //레레레도

    buzzerStopSong(); // 부저 끄기
}

int BGM_GAME2(void) //솔미미 파레레 도레미파 솔솔솔 솔미미미 파레레 도미솔솔 미미미(같은그림찾기 전용 BGM))
{
    buzzerPlaySong(musicScale[4]);
    usleep(150000);
    buzzerPlaySong(musicScale[2]);
    usleep(150000);
    buzzerPlaySong(musicScale[2]);
    usleep(300000);//솔미미
    buzzerPlaySong(musicScale[3]);
    usleep(150000);
    buzzerPlaySong(musicScale[1]);
    usleep(150000);
    buzzerPlaySong(musicScale[1]);
    usleep(300000);//파레레

    buzzerPlaySong(musicScale[0]);
    usleep(150000);
    buzzerPlaySong(musicScale[1]);
    usleep(150000);
    buzzerPlaySong(musicScale[2]);
    usleep(150000);
    buzzerPlaySong(musicScale[3]);
    usleep(150000);
    buzzerPlaySong(musicScale[4]);
    usleep(150000);
    buzzerPlaySong(musicScale[4]);
    usleep(150000);
    buzzerPlaySong(musicScale[4]);
    usleep(300000);//도레미파솔솔솔

    buzzerPlaySong(musicScale[4]);
    usleep(150000);
    buzzerPlaySong(musicScale[2]);
    usleep(150000);
    buzzerPlaySong(musicScale[2]);
    usleep(150000);
    buzzerPlaySong(musicScale[2]);
    usleep(150000);
    buzzerPlaySong(musicScale[3]);
    usleep(150000);
    buzzerPlaySong(musicScale[1]);
    usleep(150000);
    buzzerPlaySong(musicScale[1]);
    usleep(300000);//솔미미미 파레레

    buzzerPlaySong(musicScale[0]);
    usleep(150000);
    buzzerPlaySong(musicScale[2]);
    usleep(150000);
    buzzerPlaySong(musicScale[4]);
    usleep(150000);
    buzzerPlaySong(musicScale[4]);
    usleep(150000);
    buzzerPlaySong(musicScale[2]);
    usleep(150000);
    buzzerPlaySong(musicScale[2]);
    usleep(150000);
    buzzerPlaySong(musicScale[2]);
    usleep(300000);//도미솔솔 미미미

    buzzerStopSong(); // 부저 끄기
}
