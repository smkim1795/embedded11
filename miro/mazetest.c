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

//#include "button.h"
#include "fnd.h"
//#include "colorled.h"
//#include "led.h"
#include "textlcd.h"
#include "libBitmap.h"
#include "bitmapFileHeader.h"
#include "gyro.h"
#include "buzzer.h"

#define MODE_COUNT_DIS '1'
#define MODE_STATIC_DIS      0
static int msgID;
//BUTTON_MSG_T rcv;

static int timer_end=0;  //제한시간 flag
static int mode;
static int t;
static int fail = 0;
static int finish = 0;
static int btn = 0;
static int stage_1 = 0;  //미로 게임 flag
static int stage1_end = 0;
static int step = 0;
static int running = 100;
static char *data;
static int cols = 0, rows = 0;
static int stage_2 = 0;  //같은그림찾기게임 flag
static int stage2_end = 0;
static int count = 0;
static int answer = 0;
static int led_on = 0;

static int new_x = 310;
static int new_y = 510;
static int accel_x= 0;
static int accel_y = 0;
static int screen_width;
static int screen_height;
static int bits_per_pixel;
static int line_length;

pthread_t thread[3];


void* thread_object_0() {                 // 스레드 0 : FND를 활용한 미로 게임 타이머 생성

    while(1){

        if(stage_1 == 1){                 //미로 게임일 경우
            timer_end  = 0; //타이머 초기화
            fail = 0; //실패 조건 초기화

            while(timer_end != 1 && fail != 1 ){
                for(t=20; t>=0; t--){               // 20초 타이머 생성
                mode=MODE_STATIC_DIS;
                fndDisp(t,mode);
                sleep (1);

            // 타이머가 끝나거나 실패 flag가 뜨면 루프 종료
                if(timer_end == 1 || fail == 1){
                    break;
                }
            }
            timer_end = 1;
        }
        if(timer_end == 1 || fail == 1){         // 시간내에 성공하지 못했거나 벽에 닿으면 게임오버
                lcdtextwrite("     Fail...    ", NULL, 1);
                sleep (3);
                // 미로게임+타이머 처음부터 다시 시작
                stage_1 = 1;
                fail = 0; //실패 flag 초기화
            }
        }
    }
    return NULL;
}

void* thread_object_1() {                 // 스레드 1 : 미로 게임 bgm 재생

    while(stage_1 == 1 && timer_end != 0 && fail == 0) {  //미로게임을 플레이중일때만

        system("sudo aplay ./mazebgm.wav");  //bgm 재생
    
    }
}


void* thread_object_2() {                 // 스레드 2 : 미로 게임

    while (1){
        if(stage_1 == 1){
                if ( fb_init(&screen_width, &screen_height, &bits_per_pixel, &line_length) < 0 )
                    {
                        printf ("FrameBuffer Init Failed\r\n");                   // TFT LCD 초기화
                    }
                fndDisp(000000,0);

                read_bmp("miro.bmp", &data, &cols, &rows);                        // 미로 배경
                fb_write(data, cols,rows);
                stage1_end = 0;
                finish = 0;
                new_x = 310;
                new_y = 510; // 시작 지점 초기화

                 while(stage1_end != 1 && finish != 1){
                    read_bmp("player.bmp",&data,&cols,&rows);         // 미로 게임에서 사용되는 선 긋는 용 bmp 출력
                    fb_write_c(data,cols,rows, new_x , new_y);
                
                    accel_x = get_accel_x();                                    // 가속도 센서 값 불러옴
                    accel_y = get_accel_y();
                    printf("\n");
                
                    if(accel_x >= 4000 ){                       // !! 사용자가 키트를 쥐고 TFT LCD를 바라보는 시점에서의
                        printf("a : %d\n" ,accel_x);      // x축 = new_y ::: y축 = new_x
                        new_y  = new_y + 10;                
                    }
                    else if(accel_x <= -4000){
                        printf("a : %d\n" ,accel_x);
                        new_y = new_y - 10;
                    }
                    else if(accel_y >= 4000){               // 사용자가 키트를 기울이는 방향대로 aim이 이동
                        printf("b : %d\n" ,accel_y);
                        new_x = new_x + 10;
                    }
                    else if(accel_y <= -4000){
                        printf("b : %d\n" ,accel_y);
                        new_x = new_x - 10;
                    }

                    read_bmp("player.bmp",&data,&cols,&rows);                  // 이동하는 aim의 새로운 좌표를 업데이트하여 다시 출력
                    fb_write_c(data,cols,rows,new_x, new_y);
                    printf("x: %d       y: %d\n\n",new_y, new_x);          // aim의 실시간 좌표 확인
                
            
                    if((new_x >= 130 && new_x <= 390) && (new_y == 370)){
                        read_bmp("fail.bmp", &data, &cols, &rows);
                        sleep(2);
                        fail = 1;    
                        break;                                                           
                    }//1번선
                    else if((new_y >= 450 && new_y <= 510) && (new_x ==  270)){
                        read_bmp("fail.bmp", &data, &cols, &rows);
                        sleep(2);
                        fail = 1;    
                        break;
                    }//2번선
                    else if((new_y >= 450 && new_y <= 510) && (new_x == 400)){
                        read_bmp("fail.bmp", &data, &cols, &rows);    
                        sleep(2);
                        fail = 1;
                        break;
                    }//3번선
                    else if((new_x >= 400 && new_x <= 670) && (new_y == 450)){
                        read_bmp("fail.bmp", &data, &cols, &rows);    
                        sleep(2);
                        fail = 1;
                        break;
                    }//4번선
                    else if((new_x >= 550 && new_x <= 670) && (new_y == 370)){
                        read_bmp("fail.bmp", &data, &cols, &rows);    
                        sleep(2);
                        fail = 1;
                        break;
                    }//5번선

                    else if((new_y >= 220 && new_y <= 430) && (new_x == 810)){
                        read_bmp("fail.bmp", &data, &cols, &rows);    
                        sleep(2);
                        fail = 1;
                        break;
                    }//6번선

                    else if((new_y >= 220 && new_y <= 270) && (new_x == 700)){
                        read_bmp("fail.bmp", &data, &cols, &rows);    
                        sleep(2);
                        fail = 1;
                        break;
                    }//7번선

                    else if((new_x >= 550 && new_x <= 810) && (new_y == 150)){
                        read_bmp("fail.bmp", &data, &cols, &rows); 
                        sleep(2);
                        fail = 1;   
                        break;
                    }//8번선

                    else if((new_x >= 270 && new_x <= 670) && (new_y == 190)){
                        read_bmp("fail.bmp", &data, &cols, &rows);  
                        sleep(2);  
                        fail = 1;
                        break;
                    }//9번선

                    else if((new_y >= 70 && new_y <= 190) && (new_x == 270)){
                        read_bmp("fail.bmp", &data, &cols, &rows);  
                        sleep(2);
                        fail = 1;  
                        break;
                    }//10번선

                    else if((new_y >= 70 && new_y <= 110) && new_x == 370){
                        read_bmp("fail.bmp", &data, &cols, &rows);
                        sleep(2);
                        fail = 1;
                        break;
                    }//11번선
 
                    else if((new_x >= 280 && new_x <= 330) && new_y == 90){
                        read_bmp("clear.bmp", &data, &cols, &rows);
                        fb_write(data, cols, rows);
                        printf("도착점에 도달하였습니다.\n");
                        sleep(2); // 2초간 output을 보여준 후 프로그램 종료
                        finish = 1;
                        break;
                    }//성공 지점 도달

                    else{   
                        printf("ok\n");     //이동중인 상태
                    }
                    usleep(125000); 
                }

                if (finish == 1){
                    exit(0);  //성공 시 게임 종료
                }
        }
    }    
    return NULL;
}

int main(void){

 //   int msdID = msgget (MESSAGE_ID, IPC_CREAT|0666);        // 버튼 입력 위해 메세지큐 생성
    

    textlcdInit();
    buzzerInit();
                                                   // 각 기능들 활용을 위한 초기 설정

    fndDisp(000000,0);                                                // 게임 시작 시 FND 0으로 초기화

    if (fb_init(&screen_width, &screen_height, &bits_per_pixel, &line_length) < 0)
    {
        printf("FrameBuffer Init Failed\r\n");                        // TFT LCD 초기화
    }

 

    read_bmp("start.bmp", &data, &cols, &rows);                // 게임 시작 초기 화면 :: STRAT PAGE
    fb_write(data, cols,rows);
    system("sudo amixer sset 'Speaker' 70%");
    system("sudo aplay ./startbgm.wav");       
    sleep(8);

    ///////// < 미로 게임 > /////////
    if ( fb_init(&screen_width, &screen_height, &bits_per_pixel, &line_length) < 0 )
    {
        printf ("FrameBuffer Init Failed\r\n");                   // TFT LCD 초기화
    }
    fndDisp(000000,0);

    read_bmp("miro.bmp", &data, &cols, &rows);                        // 미로 배경
    fb_write(data, cols,rows);
    

    pthread_create(&thread[0],NULL,thread_object_0,NULL);                   // 미로게임 타이머 스레드 생성
    pthread_create(&thread[1],NULL,thread_object_1,NULL);                   // 미로게임 bgm 스레드 생성
    pthread_create(&thread[2],NULL,thread_object_2,NULL);                   // 미로게임 스레드 생성

    stage_1 = 1; //미로게임 시작
    
    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);
    pthread_join(thread[2], NULL);

    textlcdExit();
    close_bmp();
    fb_close();                                         // 프로그램 & 각 기능들 종료

    return 0;
}



