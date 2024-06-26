#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h>    // for open/close
#include <fcntl.h>     // for O_RDWR
#include <sys/ioctl.h> // for ioctl
#include <sys/types.h>
#include <sys/msg.h>
#include <pthread.h>

#include <sys/kd.h>

#include "button.h"
#include "fnd.h"
#include "colorled.h"
#include "led.h"
#include "textlcd.h"
#include "libBitmap.h"
#include "bitmapFileHeader.h"
#include "gyro.h"
#include "buzzer.h"

#define MODE_STATIC_DIS 0
static int msgID;
static int timer_end = 0; //제한시간 flag
static int mode;
static int t;
static int fail = 0;
static int stage = 0; 
static int stage1_end = 0;
static char *data;
static int cols = 0, rows = 0;
static int stage2_end = 0;

static int led_on = 0;

static int new_x = 310;
static int new_y = 510;
static int accel_x = 0;
static int accel_y = 0;
static int screen_width;
static int screen_height;
static int bits_per_pixel;
static int line_length;

pthread_t thread[2];

void *thread_object_0()
{ // 스레드 0 : FND를 활용한 미로 게임 타이머 생성
    while (1)
    {
        if (stage == 1 && stage1_end == 0)
        {                  //미로 게임일 경우
            timer_end = 0; //타이머 초기화
            fail = 0;      //실패 조건 초기화
            
            stage1_end = 0;
            cols = 0;
            rows = 0;
            new_x = 310;
            new_y = 510; // 시작 지점 초기화

            if (fb_init(&screen_width, &screen_height, &bits_per_pixel, &line_length) < 0)
            {
                printf("FrameBuffer Init Failed\r\n"); // TFT LCD 초기화
            }
            fndDisp(000000, 0);

            read_bmp("miro.bmp", &data, &cols, &rows); // 미로 배경
            fb_write(data, cols, rows);

            while (timer_end == 0 && fail == 0)
            {
                for (t = 20; t >= 0; t--)
                { // 20초 타이머 생성
                    mode = MODE_STATIC_DIS;
                    fndDisp(t, mode);
                    sleep(1);
                    if (fail == 1)
                        break;
                }
                timer_end = 1;
                break;
            }
            if (stage1_end == 1)
            {
                lcdtextwrite("    Ya Hohoho    ", NULL, 1);
                for (int i = 0; i < 8; i++) //성공 시 led 8개 ON
                {
                    ledOnOff(i, 1);
                }
                sleep(1);
                for (int i = 0; i < 8; i++) //led 8개 OFF
                {
                    ledOnOff(i, 0);
                }
                sleep(1);                   // 2초간 output을 보여준 후 프로그램 종료
                for (int i = 0; i < 8; i++) //성공 시 led 8개 ON
                {
                    ledOnOff(i, 1);
                }
                sleep(1);
                for (int i = 0; i < 8; i++) //led 8개 OFF
                {
                    ledOnOff(i, 0);
                }
                textlcdclear();
                return NULL;
            }
            if ((timer_end == 1 || fail == 1) && stage1_end == 0)
            { // 시간내에 성공하지 못했거나 벽에 닿으면 게임오버
                lcdtextwrite("     Fail...    ", NULL, 1);
                sleep(3);
                textlcdclear();
                // 미로게임+타이머 처음부터 다시 시작
                stage = 1;
                fail = 0; //실패 flag 초기화
            }
        }
    }
}

void *thread_object_1()
{ // 스레드 1 : 미로 게임
    while (1)
    {
        if (stage == 1)
        {
            stage1_end = 0;
            cols = 0;
            rows = 0;
            new_x = 310;
            new_y = 510; // 시작 지점 초기화

            if (fb_init(&screen_width, &screen_height, &bits_per_pixel, &line_length) < 0)
            {
                printf("FrameBuffer Init Failed\r\n"); // TFT LCD 초기화
            }
            fndDisp(000000, 0);

            read_bmp("miro.bmp", &data, &cols, &rows); // 미로 배경
            fb_write(data, cols, rows);
            
            while (stage1_end == 0)
            {
                read_bmp("player.bmp", &data, &cols, &rows); // 미로 게임에서 사용되는  player.bmp 출력
                fb_write_c(data, cols, rows, new_x, new_y);

                accel_x = get_accel_x(); // 가속도 센서 값 불러옴
                accel_y = get_accel_y();
                printf("\n");

                if (accel_x >= 4000)
                {                                // 사용자가 키트를 쥐고 TFT LCD를 바라보는 시점에서
                    printf("a : %d\n", accel_x); // x축 = new_y ::: y축 = new_x
                    new_y = new_y + 10;
                }
                else if (accel_x <= -4000)
                {
                    printf("a : %d\n", accel_x);
                    new_y = new_y - 10;
                }
                else if (accel_y >= 4000)// 사용자가 키트를 기울이는 방향대로 player 이동
                { 
                    printf("b : %d\n", accel_y);
                    new_x = new_x + 10;
                }
                else if (accel_y <= -4000)
                {
                    printf("b : %d\n", accel_y);
                    new_x = new_x - 10;
                }

                read_bmp("player.bmp", &data, &cols, &rows); // 이동하는 player의 새로운 좌표를 업데이트하여 다시 출력
                fb_write_c(data, cols, rows, new_x, new_y);
                printf("x: %d       y: %d\n\n", new_y, new_x); // player의 실시간 좌표 확인

                if ((new_x >= 130 && new_x <= 390) && (new_y == 370))
                {
                    read_bmp("fail.bmp", &data, &cols, &rows);
                    sleep(2);
                    fail = 1;
                    break;
                } //1번선
                else if ((new_y >= 450 && new_y <= 510) && (new_x == 270))
                {
                    read_bmp("fail.bmp", &data, &cols, &rows);
                    sleep(2);
                    fail = 1;
                    break;
                } //2번선
                else if ((new_y >= 450 && new_y <= 510) && (new_x == 400))
                {
                    read_bmp("fail.bmp", &data, &cols, &rows);
                    sleep(2);
                    fail = 1;
                    break;
                } //3번선
                else if ((new_x >= 400 && new_x <= 670) && (new_y == 450))
                {
                    read_bmp("fail.bmp", &data, &cols, &rows);
                    sleep(2);
                    fail = 1;
                    break;
                } //4번선
                else if ((new_x >= 550 && new_x <= 670) && (new_y == 370))
                {
                    read_bmp("fail.bmp", &data, &cols, &rows);
                    sleep(2);
                    fail = 1;
                    break;
                } //5번선

                else if ((new_y >= 220 && new_y <= 430) && (new_x == 810))
                {
                    read_bmp("fail.bmp", &data, &cols, &rows);
                    sleep(2);
                    fail = 1;
                    break;
                } //6번선

                else if ((new_y >= 220 && new_y <= 270) && (new_x == 700))
                {
                    read_bmp("fail.bmp", &data, &cols, &rows);
                    sleep(2);
                    fail = 1;
                    break;
                } //7번선

                else if ((new_x >= 550 && new_x <= 810) && (new_y == 150))
                {
                    read_bmp("fail.bmp", &data, &cols, &rows);
                    sleep(2);
                    fail = 1;
                    break;
                } //8번선

                else if ((new_x >= 270 && new_x <= 670) && (new_y == 190))
                {
                    read_bmp("fail.bmp", &data, &cols, &rows);
                    sleep(2);
                    fail = 1;
                    break;
                } //9번선

                else if ((new_y >= 70 && new_y <= 190) && (new_x == 270))
                {
                    read_bmp("fail.bmp", &data, &cols, &rows);
                    sleep(2);
                    fail = 1;
                    break;
                } //10번선

                else if ((new_y >= 70 && new_y <= 110) && new_x == 370)
                {
                    read_bmp("fail.bmp", &data, &cols, &rows);
                    sleep(2);
                    fail = 1;
                    break;
                } //11번선

                else if ((new_x >= 400 && new_x <= 970) && new_y == 510)
                {
                    read_bmp("fail.bmp", &data, &cols, &rows);
                    sleep(2);
                    fail = 1;
                    break;
                } //12번선

                 else if ((new_y >= 70 && new_y <= 510) && new_x == 970)
                {
                    read_bmp("fail.bmp", &data, &cols, &rows);
                    sleep(2);
                    fail = 1;
                    break;
                } //13번선

                 else if ((new_x >= 570 && new_x <= 940) && new_y == 80)
                {
                    read_bmp("fail.bmp", &data, &cols, &rows);
                    sleep(2);
                    fail = 1;
                    break;
                } //14번선

                 else if ((new_y >= 70 && new_y <= 510) && new_x == 130)
                {
                    read_bmp("fail.bmp", &data, &cols, &rows);
                    sleep(2);
                    fail = 1;
                    break;
                } //15번선

                 else if ((new_x >= 130 && new_x <= 270) && new_y == 70)
                {
                    read_bmp("fail.bmp", &data, &cols, &rows);
                    sleep(2);
                    fail = 1;
                    break;
                } //16번선

                 else if ((new_x >= 130 && new_x <= 270) && new_y == 510)
                {
                    read_bmp("fail.bmp", &data, &cols, &rows);
                    sleep(2);
                    fail = 1;
                    break;
                } //형 1번선

                 else if ((new_x >= 130 && new_x <= 530) && new_y == 260)
                {
                    read_bmp("fail.bmp", &data, &cols, &rows);
                    sleep(2);
                    fail = 1;
                    break;
                } //형 2번선

                else if ((new_y >= 290 && new_y <= 340) && new_x == 570)
                {
                    read_bmp("fail.bmp", &data, &cols, &rows);
                    sleep(2);
                    fail = 1;
                    break;
                } //형 3번선

                else if ((new_x >= 890 && new_x <= 940) && new_y == 190)
                {
                    read_bmp("fail.bmp", &data, &cols, &rows);
                    sleep(2);
                    fail = 1;
                    break;
                } //형 4번선

                else if ((new_y >= 70 && new_y <= 120) && new_x == 520)
                {
                    read_bmp("fail.bmp", &data, &cols, &rows);
                    sleep(2);
                    fail = 1;
                    break;
                } //형 5번선

                else if ((new_x >= 260 && new_x <= 360) && new_y == 570)
                {
                    read_bmp("fail.bmp", &data, &cols, &rows);
                    sleep(2);
                    fail = 1;
                    break;
                } //플레이어 뒤에 선

                else if ((new_x >= 280 && new_x <= 330) && new_y == 90)
                {
                    read_bmp("clear.bmp", &data, &cols, &rows);
                    fb_write(data, cols, rows);
                    printf("도착점에 도달하였습니다.\n");
                    sleep(2); // 2초간 output을 보여준 후 프로그램 종료
                    stage = 2;
                    stage1_end = 1;
                    return NULL;
                    break;
                } //성공 지점 도달;
                else
                {
                    printf("ok\n"); //이동중인 상태
                }
                usleep(125000);
            }
        }
    }
}

void show_problem(const char *correct_bmp, const char *select_bmp)
{
    // 문제 표시
    read_bmp(correct_bmp, &data, &cols, &rows);
    fb_write(data, cols, rows);
    usleep(200000); // 0.2초 대기

    // 선택 화면 표시
    read_bmp(select_bmp, &data, &cols, &rows);
    fb_write(data, cols, rows);

    // 정답 결정
    int correct_answer;
    if (strcmp(correct_bmp, "correct1.bmp") == 0)
        correct_answer = 2; // 문제 1의 정답은 2
    else if (strcmp(correct_bmp, "correct2.bmp") == 0)
        correct_answer = 3; // 문제 2의 정답은 3
    else if (strcmp(correct_bmp, "correct3.bmp") == 0)
        correct_answer = 1; // 문제 3의 정답은 1

    // 사용자 입력 대기 (버튼 처리)
    int button = -1;
    while (button != BUTTON_HOME && button != BUTTON_BACK && button != BUTTON_SEARCH)
    {
        button = get_button_press(); // 사용자 입력 버튼을 가져옴
        usleep(100000);              // 0.1초마다 확인
    }

    // 정답 확인 후 o.bmp 또는 x.bmp 출력
    if (button == correct_answer)
    {
        read_bmp("o.bmp", &data, &cols, &rows);
        pwmSetPercent(0, 0);
        pwmSetPercent(100, 1);
        pwmSetPercent(0, 2); //초록
    }
    else
    {
        read_bmp("x.bmp", &data, &cols, &rows);
        pwmSetPercent(0, 0);
        pwmSetPercent(0, 1);
        pwmSetPercent(100, 2);  //빨강
    }
    fb_write(data, cols, rows);
    sleep(1); // 1초 대기
}

int main(void)
{
    int conFD = open ("/dev/tty0", O_RDWR);
    ioctl(conFD, KDSETMODE, KD_GRAPHICS);  //커서 없애기

    pwmLedInit();
    ledLibInit();
    textlcdInit();
    buzzerInit();

    fndDisp(000000, 0); // 게임 시작 시 FND 0으로 초기화

    if (fb_init(&screen_width, &screen_height, &bits_per_pixel, &line_length) < 0)
    {
        printf("FrameBuffer Init Failed\r\n"); // TFT LCD 초기화
    }

    read_bmp("final_rule.bmp", &data, &cols, &rows); // 게임 시작 초기 화면 :: STRAT PAGE
    fb_write(data, cols, rows);
    system("sudo amixer sset 'Speaker' 70%");
    system("sudo aplay ./startbgm.wav");

    ///////// < 미로 게임 > /////////


    stage = 1; //미로게임

    pthread_create(&thread[0], NULL, thread_object_0, NULL); // 미로게임 타이머 스레드 생성
    pthread_create(&thread[1], NULL, thread_object_1, NULL); // 미로게임 스레드 생성

    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);  //스레드 종료 대기

    printf("while start!\r\n");
    while (!stage1_end)     //end flag가 떠도 main문에 묶일 수 있도록 하는 while문
        ;
    printf("while end!\r\n");
    /////////<같은 그림 찾기 게임>/////////
    
    if (fb_init(&screen_width, &screen_height, &bits_per_pixel, &line_length) < 0)
    {
        printf("FrameBuffer Init Failed\r\n"); // TFT LCD 초기화
    }
    fndDisp(000000, 0);


    printf("FIND SAME GAME START!\r\n");
    BGM_START(); //buzzer

    // 게임 시작 화면 출력
    read_bmp("start.bmp", &data, &cols, &rows);  // 안나옴 빼도될듯
    fb_write(data, cols, rows);
    usleep(3); // 3초 대기

    // 문제 1: correct1.bmp를 0.2초 동안 보여주고 select1.bmp 출력
    show_problem("correct1.bmp", "select1.bmp");

    // 문제 2: correct2.bmp를 0.2초 동안 보여주고 select2.bmp 출력
    show_problem("correct2.bmp", "select2.bmp");

    // 문제 3: correct3.bmp를 0.2초 동안 보여주고 select3.bmp 출력
    show_problem("correct3.bmp", "select3.bmp");

    read_bmp("output.bmp", &data, &cols, &rows);  //황희찬 사진
    fb_write(data, cols, rows);

    lcdtextwrite(" Embeddedsystem ", " finish... ", 1);
    lcdtextwrite(" Embeddedsystem ", " finish... ", 2);
    sleep(3);
    textlcdclear();
    pwmInactiveAll();
    ledLibExit();
    textlcdExit();
    close_bmp();
    fb_close(); // 프로그램 & 각 기능들 종료 (TFTLCD는 안꺼짐)

    close(conFD);

    return 0;
}
