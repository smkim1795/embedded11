#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h> // for open/close
#include <fcntl.h> // for O_RDWR
#include <sys/ioctl.h> // for ioctl
#include <sys/types.h>
#include <sys/msg.h>
#include <pthread.h>

#include "button.h"
#include "fnd.h"
#include "colorled.h"
#include "led.h"
#include "textlcd.h"
#include "libBitmap.h"
#include "bitmapFileHeader.h"
#include "gyro.h"
#include "buzzer.h"


#define MODE_STATIC_DIS      0
static int msgID;
BUTTON_MSG_T rcv;

static int timer_end=0;
static int mode;
static int t;
static int fail = 0;
static int finish = 0;
static int btn = 0;
static int stage_1 = 0;
static int stage1_end = 0;
static int step = 0;
static int running = 100;
static char *data;
static int cols = 0, rows = 0;
static int stage_2 = 0;
static int stage2_end = 0;
static int count = 0;
static int answer = 0;
static int led_on = 0;

static int new_x = 520;
static int new_y = 380;
static int accel_x= 0;
static int accel_y = 0;


pthread_t thread[4];


void* thread_object_0() {                 // 스레드 0 : FND를 활용한 미로 게임 타이머 생성

    if(stage_1 == 1){                 //미로 게임일 경우
        while(timer_end != 1){
            for(t=20; t>=0; t--){               // 20초 타이머 생성
            mode=MODE_STATIC_DIS;
            fndDisp(t,mode);
            sleep (1);
            }
            timer_end=1;
            break;
        }

        if(timer_end == 1){         // 시간내에 성공하지 못했으면 게임오버
                lcdtextwrite("     Fail...    ", NULL, 1);
                sleep (3);

                read_bmp("choosegame.bmp", &data, &cols, &rows);      // 게임 시작 초기 화면으로 돌아가기 (TFT LCD)
                fb_write(data, cols,rows);
        }
    }
}



void* thread_object_1() {                 // 스레드 1 : FND를 활용한 같은 그림 찾기 타이머 생성

    if(stage_2 == 1){                 //같은 그림 찾기 게임일 경우
        while(timer_end != 1){
            for(t=10; t>=0; t--){               // 10초 타이머 생성
            mode=MODE_STATIC_DIS;
            fndDisp(t,mode);
            sleep (1);
            }
            timer_end=1;
            break;
        }

        if(timer_end == 1){         // 시간내에 고르지 못하면 오답
                pwmSetPercent(0, 0);                                // 오답일 경우 컬러 LED :: Red
                pwmSetPercent(0, 1);
                pwmSetPercent(100, 2)
                //다음 문제로 넘어가야함
        }
    }

}

void* thread_object_2() {                 // 스레드 2 : 미로 게임

    while (loop_break!=1){
        msgrcv(msgID, &rcv.keyInput, sizeof(rcv.keyInput), 0, 0);
        if(rcv.keyInput == 2){             //home 버튼 > 미로게임 선택
            stage_1 = 1;
        //미로 게임 코드 추가 예정
        }
    }
}

void* thread_object_3() {                 // 스레드 3 : 같은 그림 찾기 게임

    while (loop_break!=1){
        msgrcv(msgID, &rcv.keyInput, sizeof(rcv.keyInput), 0, 0);
        if(rcv.keyInput == 4){             //back 버튼 > 같은 그림 찾기 게임 선택
            stage_2 = 1;
        //같은 그림 찾기 게임 코드 추가 예정
        }
    }
}



int main(void){

    int screen_width;
    int screen_height;
    int bits_per_pixel;
    int line_length;

    int msdID = msgget (MESSAGE_ID, IPC_CREAT|0666);        // 버튼 입력 위해 메세지큐 생성
    
    buttonInit();
    pwmLedInit();
    ledLibInit();
    lcdtextInit();
    buzzerInit();
    ledalloff();                                                      // 각 기능들 활용을 위한 초기 설정

    fndDisp(000000,0);                                                // 게임 시작 시 FND 0으로 초기화

    if (fb_init(&screen_width, &screen_height, &bits_per_pixel, &line_length) < 0)
    {
        printf("FrameBuffer Init Failed\r\n");                        // TFT LCD 초기화
    }

    lcdtextWrite("  Which game do ", "    you want?    ", 1);
    lcdtextwrite("  Which game do ", "    you want?    ", 2);

    while(1){
        int returnValue = 0;
        returnValue = msgrcv(msgID, &rcv, sizeof(rcv) - sizeof(long int), 0, IPC_NOWAIT);           // 메세지큐 초기화
        if(returnValue == -1) break;
    }
    
    system("sudo amixer sset 'Speaker' 70%");
    system("sudo aplay ./startbgm.wav");    

    read_bmp("choosegame.bmp", &data, &cols, &rows);                // 게임 시작 초기 화면 :: 게임 고르기
    fb_write(data, cols,rows);
    
    sleep(3);



}



