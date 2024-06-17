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

#include "libBitmap.h"
#include "bitmapFileHeader.h"
#include "gyro.h"

#define MODE_STATIC_DIS      0
static int mode;
//static int t;
static char *data;
static int cols = 0, rows = 0;

static int new_x = 310;
static int new_y = 510;
static int accel_x= 0;
static int accel_y = 0;


int main(void){
    int screen_width;
    int screen_height;
    int bits_per_pixel;
    int line_length;

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

        read_bmp("miro.bmp", &data, &cols, &rows);                        // 미로 배경
        fb_write(data, cols,rows);
                
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
                break;                                                           
            }//1번선
            else if((new_y >= 450 && new_y <= 510) && (new_x ==  270)){
                read_bmp("fail.bmp", &data, &cols, &rows);
                fb_write(data, cols,rows); 
                sleep(2);   
                break;
            }//2번선
            else if((new_y >= 450 && new_y <= 510) && (new_x == 400)){
                read_bmp("fail.bmp", &data, &cols, &rows);
                fb_write(data, cols,rows);    
                sleep(2);
                break;
            }//3번선
            else if((new_x >= 400 && new_x <= 670) && (new_y == 450)){
                read_bmp("fail.bmp", &data, &cols, &rows);
                fb_write(data, cols,rows); 
                sleep(2);   
                break;
            }//4번선
            else if((new_x >= 550 && new_x <= 670) && (new_y == 370)){
                read_bmp("fail.bmp", &data, &cols, &rows);
                fb_write(data, cols,rows);    
                sleep(2);
                break;
            }//5번선

            else if((new_y >= 220 && new_y <= 430) && (new_x == 810)){
                read_bmp("fail.bmp", &data, &cols, &rows);
                fb_write(data, cols,rows);    
                sleep(2);
                break;
            }//6번선

            else if((new_y >= 220 && new_y <= 270) && (new_x == 700)){
                read_bmp("fail.bmp", &data, &cols, &rows);
                fb_write(data, cols,rows);  
                sleep(2);
                break;
            }//7번선

            else if((new_x >= 550 && new_x <= 810) && (new_y == 150)){
                read_bmp("fail.bmp", &data, &cols, &rows);
                fb_write(data, cols,rows);   
                sleep(2);
                break;
            }//8번선

            else if((new_x >= 270 && new_x <= 670) && (new_y == 190)){
                read_bmp("fail.bmp", &data, &cols, &rows);
                fb_write(data, cols,rows);    
                sleep(2);
                break;
            }//9번선

            else if((new_y >= 70 && new_y <= 190) && (new_x == 270)){
                read_bmp("fail.bmp", &data, &cols, &rows); 
                fb_write(data, cols,rows);
                sleep(2);  
                break;
            }//10번선

            else if((new_y >= 70 && new_y <= 110) && new_x == 370){
                read_bmp("fail.bmp", &data, &cols, &rows);
                fb_write(data, cols,rows);
                sleep(2);   
                break;
            }//11번선
 
            else if((new_x >= 280 && new_x <= 330) && new_y == 90){
               read_bmp("clear.bmp", &data, &cols, &rows);
               fb_write(data, cols, rows);
               printf("도착점에 도달하였습니다.\n");
               sleep(2); // 2초간 output을 보여준 후 프로그램 종료
            }//성공 지점 도달
            
            else{   
                printf("ok\n");     
                  }
            usleep(125000); 
        }



    fb_close();                                         // 프로그램 종료하면서 각 기능들 또한 종료시켜준다.

    return 0;
}

