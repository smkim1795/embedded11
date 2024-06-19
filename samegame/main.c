#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <pthread.h>
#include <string.h>

#include "libBitmap.h"
#include "bitmapFileHeader.h"
#include "gyro.h"
#include "button.h"
#include "fnd.h"  // FND 관련 헤더 파일 추가

#define MODE_STATIC_DIS 0
static char *data;
static int cols = 0, rows = 0;

void show_problem(const char* correct_bmp, const char* select_bmp) {
    // 문제 표시
    read_bmp(correct_bmp, &data, &cols, &rows);
    fb_write(data, cols, rows);
    usleep(200000);  // 0.2초 대기

    // 선택 화면 표시
    read_bmp(select_bmp, &data, &cols, &rows);
    fb_write(data, cols, rows);

    // 5초 동안 FND에 카운트 다운 표시
    fndDisp(5, MODE_STATIC_DIS);  // 5초 카운트 다운 시작

    // 버튼 입력 대기 (5초 안에 버튼을 누르면 다음 문제로)
    int button = -1;
    int time_left = 5;
    while (time_left > 0) {
        button = get_button_press();  // 사용자 입력 버튼을 가져옴
        if (button != -1) {
            break;  // 버튼 입력이 감지되면 루프 종료
        }
        sleep(1);  // 1초씩 대기
        time_left--;
    }

    if (button != -1) {
        printf("Button pressed: %d\n", button);
        // 정답 확인 후 o.bmp 또는 x.bmp 출력
        if ((strcmp(correct_bmp, "correct1.bmp") == 0 && button == 2) ||
            (strcmp(correct_bmp, "correct2.bmp") == 0 && button == 3) ||
            (strcmp(correct_bmp, "correct3.bmp") == 0 && button == 1)) {
            read_bmp("o.bmp", &data, &cols, &rows);
        } else {
            read_bmp("x.bmp", &data, &cols, &rows);
        }
    } else {
        printf("Time's up!\n");
        // 시간 초과 시 x.bmp 출력
        read_bmp("x.bmp", &data, &cols, &rows);
    }

    fb_write(data, cols, rows);
    usleep(1000000);  // 1초 대기
}

int main(void) {
    int screen_width;
    int screen_height;
    int bits_per_pixel;
    int line_length;

    // 프레임버퍼 초기화
    if (fb_init(&screen_width, &screen_height, &bits_per_pixel, &line_length) < 0) {
        fprintf(stderr, "Framebuffer initialization failed\n");
        return 1;
    }

    // FND 초기화
    if (!fndInit()) {
        fprintf(stderr, "Failed to initialize FND\n");
        fb_close();
        return 1;
    }

    // 게임 시작 화면 출력
    read_bmp("output.bmp", &data, &cols, &rows);
    fb_write(data, cols, rows);
    usleep(3000000);  // 3초 대기

    // 문제 1: correct1.bmp를 0.2초 동안 보여주고 select1.bmp 출력
    show_problem("correct1.bmp", "select1.bmp");

    // 문제 2: correct2.bmp를 0.2초 동안 보여주고 select2.bmp 출력
    show_problem("correct2.bmp", "select2.bmp");

    // 문제 3: correct3.bmp를 0.2초 동안 보여주고 select3.bmp 출력
    show_problem("correct3.bmp", "select3.bmp");

    fndExit();  // FND 종료
    fb_close();  // 프레임버퍼 닫기

    return 0;
}
