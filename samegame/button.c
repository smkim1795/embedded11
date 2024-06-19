#include "button.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>

#define INPUT_EVENT_PATH "/dev/input/event4"  // 이벤트 디바이스 경로 확인 필요

int get_button_press() {
    int button_pressed = 0;
    struct input_event ev;
    int fd;

    // input event 장치 열기
    fd = open(INPUT_EVENT_PATH, O_RDONLY);
    if (fd < 0) {
        perror("Cannot open input event device");
        return 0;
    }

    printf("Device opened successfully\n");

    while (1) {
        // input event 읽기
        if (read(fd, &ev, sizeof(struct input_event)) < 0) {
            perror("Error reading input event");
            close(fd);
            return 0;
        }

        // 버튼 이벤트 확인 (예: KEY_HOME, KEY_BACK, KEY_SEARCH)
        if (ev.type == EV_KEY && ev.value == 1) {
            printf("Key event detected: code=%d, value=%d\n", ev.code, ev.value);
            switch (ev.code) {
                case KEY_HOME:
                    button_pressed = BUTTON_HOME;
                    break;
                case KEY_BACK:
                    button_pressed = BUTTON_BACK;
                    break;
                case KEY_SEARCH:
                    button_pressed = BUTTON_SEARCH;
                    break;
                default:
                    // 다른 버튼 이벤트 처리 가능
                    break;
            }
            if (button_pressed != 0) {
                break;  // 버튼을 찾았으면 반복문 종료
            }
        }
    }

    // input event 장치 닫기
    close(fd);
    return button_pressed;
}
