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



#include "fnd.h"

#include "libBitmap.h"

#include "bitmapFileHeader.h"

#include "gyro.h"



#define MODE_COUNT_DIS '1'

#define MODE_STATIC_DIS      0

static int timer_end=0;

static int mode;

static int t;

static int fail = 0;

static int finish = 0;

//static int step = 0;

static int loop_break;

//static int t;

static char *data;

static int cols = 0, rows = 0;

static int miro = 0;



static int new_x = 310;

static int new_y = 510;

static int accel_x= 0;

static int accel_y = 0;



pthread_t thread[1];                        // fnd 활용 위한 스레드



void *thread_object_0(){                 // 스레드 0, FND를 활용한 타이머 생성

        while(timer_end != 1){

            for(t=20; t>=0; t--){               // 20초 타이머 생성

            mode=MODE_STATIC_DIS;

            fndDisp(t,mode);

            sleep (1);

            }

            timer_end=1;

            break;

        }



        if(timer_end == 1 && fail == 0 && finish == 0){         // 실패하지 않았지만 시간내에 성공하지 못했으면

                printf("game over.. !!! \n\n");                                         // 그래도 게임 오버

                read_bmp("fail.bmp", &data, &cols, &rows);      

                fb_write(data, cols,rows);

                sleep(2);

                fb_close();

                  }

    }







int main(void){

    int screen_width;

    int screen_height;

    int bits_per_pixel;

    int line_length;



    fndDisp(000000,0); //fnd 초기화

    if ( fb_init(&screen_width, &screen_height, &bits_per_pixel, &line_length) < 0 )

    {

      printf ("FrameBuffer Init Failed\r\n");                                           // TFT LCD 초기화

    }



    

    read_bmp("start.bmp", &data, &cols, &rows);                // 게임 시작 초기 화면 출력(open이라는 사진)

    fb_write(data, cols,rows);

    

    sleep(3);



    ///////// < 미로 게임 > /////////

        if ( fb_init(&screen_width, &screen_height, &bits_per_pixel, &line_length) < 0 )

        {

            printf ("FrameBuffer Init Failed\r\n");                   // TFT LCD 초기화

        }

        fndDisp(000000,0);



        read_bmp("miro.bmp", &data, &cols, &rows);                        // 미로 배경

        fb_write(data, cols,rows);

        pthread_create(&thread[0],NULL,thread_object_0,NULL); //fnd 20초 카운트 시작

                

        while(1)

        {  

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

                fb_write(data, cols,rows);   

                sleep(2);

				fail = 1;

                break;                                                           

            }//1번선

            else if((new_y >= 450 && new_y <= 510) && (new_x ==  270)){

                read_bmp("fail.bmp", &data, &cols, &rows);

                fb_write(data, cols,rows); 

                sleep(2);

				fail = 1;   

                break;

            }//2번선

            else if((new_y >= 450 && new_y <= 510) && (new_x == 400)){

                read_bmp("fail.bmp", &data, &cols, &rows);

                fb_write(data, cols,rows);    

                sleep(2);

				fail = 1;

                break;

            }//3번선

            else if((new_x >= 400 && new_x <= 670) && (new_y == 450)){

                read_bmp("fail.bmp", &data, &cols, &rows);

                fb_write(data, cols,rows); 

                sleep(2);

				fail = 1;   

                break;

            }//4번선

            else if((new_x >= 550 && new_x <= 670) && (new_y == 370)){

                read_bmp("fail.bmp", &data, &cols, &rows);

                fb_write(data, cols,rows);    

                sleep(2);

				fail = 1;

                break;

            }//5번선



            else if((new_y >= 220 && new_y <= 430) && (new_x == 810)){

                read_bmp("fail.bmp", &data, &cols, &rows);

                fb_write(data, cols,rows);    

                sleep(2);

				fail = 1;

                break;

            }//6번선



            else if((new_y >= 220 && new_y <= 270) && (new_x == 700)){

                read_bmp("fail.bmp", &data, &cols, &rows);

                fb_write(data, cols,rows);  

                sleep(2);

				fail = 1;

                break;

            }//7번선



            else if((new_x >= 550 && new_x <= 810) && (new_y == 150)){

                read_bmp("fail.bmp", &data, &cols, &rows);

                fb_write(data, cols,rows);   

                sleep(2);

				fail = 1;

                break;

            }//8번선



            else if((new_x >= 270 && new_x <= 670) && (new_y == 190)){

                read_bmp("fail.bmp", &data, &cols, &rows);

                fb_write(data, cols,rows);    

                sleep(2);

				fail = 1;

                break;

            }//9번선



            else if((new_y >= 70 && new_y <= 190) && (new_x == 270)){

                read_bmp("fail.bmp", &data, &cols, &rows); 

                fb_write(data, cols,rows);

                sleep(2);

				fail = 1;  

                break;

            }//10번선



            else if((new_y >= 70 && new_y <= 110) && new_x == 370){

                read_bmp("fail.bmp", &data, &cols, &rows);

                fb_write(data, cols,rows);

                sleep(2);

				fail = 1;   

                break;

            }//11번선

 

            else if((new_x >= 260 && new_x <= 360) && new_y == 90){

               read_bmp("clear.bmp", &data, &cols, &rows);

               fb_write(data, cols, rows);

               printf("도착점에 도달하였습니다.\n");

               sleep(2); // 2초간 output을 보여준 후 프로그램 종료

            }//성공 지점 도달

            

            else{   

                printf("ok\n");     

                  }

            timer_end = 1;

            usleep(125000); 

        }



pthread_join(thread[0], NULL);



    fb_close();                                         // 프로그램 종료하면서 각 기능들 또한 종료시켜준다.



    return 0;

}



