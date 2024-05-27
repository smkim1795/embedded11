#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "buzzer.h"

#define MAX_SCALE_STEP 17
#define BUZZER_BASE_SYS_PATH "/sys/bus/platform/devices/"
#define BUZZER_FILENAME "peribuzzer"
#define BUZZER_ENABLE_NAME "enable"
#define BUZZER_FREQUENCY_NAME "frequency"

char gBuzzerBaseSysDir[128];
const int musicScale[MAX_SCALE_STEP] =
{
   262, /*do*/ 294,330,349,392,440,494, /* si */ 523, 329, 350, 247, 213, 262, 165, 174, 196, 220
//  0           1   2   3   4   5   6            7    8    9   10(시) 11   12  13(미)14(파)15(솔)16(라)
};

int freIndex;

int findBuzzerSysPath() //버저 경로 찾기
{
   DIR * dir_info = opendir(BUZZER_BASE_SYS_PATH);
   int ifNotFound = 1;
   if (dir_info != NULL){
      while (1){
         struct dirent *dir_entry;
         dir_entry = readdir (dir_info);
         if (dir_entry == NULL) break;
         if (strncasecmp(BUZZER_FILENAME, dir_entry->d_name, strlen(BUZZER_FILENAME)) == 0){
            ifNotFound = 0;
            sprintf(gBuzzerBaseSysDir,"%s%s/",BUZZER_BASE_SYS_PATH,dir_entry->d_name);
            }
      }
   }

   printf("find %s\n",gBuzzerBaseSysDir);

   return ifNotFound;
}

/*void doHelp(void)
{
   printf("Usage:\n");
   printf("buzzertest <buzzerNo> \n");
   printf("buzzerNo: \n");
   printf("do(1),re(2),mi(3),fa(4),sol(5),ra(6),si(7),do(8) \n");
   printf("off(0)\n");
}*/

void buzzerEnable(int bEnable)
{
   char path[200];
   sprintf(path,"%s%s",gBuzzerBaseSysDir,BUZZER_ENABLE_NAME);
   int fd=open(path,O_WRONLY);
   if ( bEnable) write(fd, &"1", 1);
   else write(fd, &"0", 1);
   close(fd);
}

void setFrequency(int frequency)
{
   char path[200];
   sprintf(path,"%s%s",gBuzzerBaseSysDir,BUZZER_FREQUENCY_NAME);
   int fd=open(path,O_WRONLY);
   dprintf(fd, "%d", frequency);
   close(fd);
}

int buzzerInit(void)
{
   if(freIndex > MAX_SCALE_STEP)
   {
      printf(" <buzzerNo> over range \n");
      doHelp();
      return 1;
   }
   return freIndex;
}

int buzzerPlaySong(int freIndex)
{
   if(freIndex > 0 || freIndex < MAX_SCALE_STEP)
   {
      setFrequency(musicScale[freIndex-1]);
      buzzerEnable(1);
   }
}

int buzzerStopSong(void)
{
 
   if ( freIndex == 0)
   {
      buzzerEnable(0);
   }
}

int buzzerExit(void)
{
   buzzerStopSong();
   close(fd);
   return 0;
}

int BGM1(void)
{
    buzzerPlaySong(musicScale[4]);
    usleep(300000);
    buzzerPlaySong(musicScale[8]);
    usleep(150000);
    buzzerPlaySong(musicScale[9]);
    usleep(150000);
    buzzerPlaySong(musicScale[4]);
    usleep(300000);
    buzzerPlaySong(musicScale[8]); 
    usleep(150000);  //5 392, 329, 350, 392, 329, //5

    buzzerPlaySong(musicScale[9]); 
    usleep(150000);
    buzzerPlaySong(musicScale[4]);
    usleep(150000);
    buzzerPlaySong(musicScale[10]);
    usleep(150000);
    buzzerPlaySong(musicScale[11]);
    usleep(150000);
    buzzerPlaySong(musicScale[10]); 
    usleep(150000); //10  350, 392, 245, 213, 245, //10

    buzzerPlaySong(musicScale[12]);
    usleep(150000);
    buzzerPlaySong(musicScale[1]);
    usleep(150000);
    buzzerPlaySong(musicScale[2]);
    usleep(150000);
    buzzerPlaySong(musicScale[3]); 
    usleep(150000);
    buzzerPlaySong(musicScale[2]); 
    usleep(300000); //15 262, 294, 330, 349, 330, //15

    buzzerPlaySong(musicScale[12]);
    usleep(150000);
    buzzerPlaySong(musicScale[1]);
    usleep(150000);
    buzzerPlaySong(musicScale[2]);
    usleep(300000);


    buzzerPlaySong(musicScale[13]);
    usleep(150000);
    buzzerPlaySong(musicScale[14]);
    usleep(150000); //20 262, 294, 330, 110, 130, //20

    buzzerPlaySong(musicScale[15]);
    usleep(150000);
    buzzerPlaySong(musicScale[16]);
    usleep(150000);
    buzzerPlaySong(musicScale[15]);
    usleep(150000);
    buzzerPlaySong(musicScale[14]);
    usleep(150000);
    buzzerPlaySong(musicScale[15]);
    usleep(150000); //25 173, 221, 173, 130, 173, //25



    buzzerPlaySong(musicScale[0]);
    usleep(150000);
    buzzerPlaySong(musicScale[10]);
    usleep(150000);
    buzzerPlaySong(musicScale[0]);
    usleep(150000);
    buzzerPlaySong(musicScale[16]);
    usleep(300000);


    buzzerPlaySong(musicScale[0]);
    usleep(150000); //30 262, 245, 262, 221, 262, //30

    buzzerPlaySong(musicScale[10]);
    usleep(150000);
    buzzerPlaySong(musicScale[16]);
    usleep(300000); 

    buzzerPlaySong(musicScale[15]);
    usleep(150000);
    buzzerPlaySong(musicScale[14]);
    usleep(150000);
    buzzerPlaySong(musicScale[15]);
    usleep(150000); //35 245, 221, 173, 130, 173, //35

    buzzerPlaySong(musicScale[14]);
    usleep(150000);
    buzzerPlaySong(musicScale[13]);
    usleep(150000);
    buzzerPlaySong(musicScale[14]);
    usleep(150000);
    buzzerPlaySong(musicScale[15]);
    usleep(150000);
    buzzerPlaySong(musicScale[16]);
    usleep(150000); //40 130, 110, 130, 173, 221, //40

    buzzerPlaySong(musicScale[10]);
    usleep(150000);
    buzzerPlaySong(musicScale[0]);
    usleep(150000);
    buzzerPlaySong(musicScale[16]);
    usleep(300000);
    buzzerPlaySong(musicScale[0]);
    usleep(150000);
    buzzerPlaySong(musicScale[10]);
    usleep(150000); //45 245, 262, 221, 262, 245, //45

    buzzerPlaySong(musicScale[0]);
    usleep(300000);
    buzzerPlaySong(musicScale[10]);
    usleep(150000);
    buzzerPlaySong(musicScale[0]);
    usleep(150000);
    buzzerPlaySong(musicScale[10]);
    usleep(150000);
    buzzerPlaySong(musicScale[16]);
    usleep(150000); //50 262, 245, 262, 245, 221, //50

    buzzerPlaySong(musicScale[10]);
    usleep(150000);
    buzzerPlaySong(musicScale[0]);
    usleep(150000);
    buzzerPlaySong(musicScale[1]);
    usleep(150000);
    buzzerPlaySong(musicScale[2]);
    usleep(150000);
    buzzerPlaySong(musicScale[3]);
    usleep(150000); //55 245, 262, 294, 330, 349, //55

    buzzerPlaySong(musicScale[4]);
    usleep(150000); 
    buzzerStopSong(); // 부저 끄기
}
