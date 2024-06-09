#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include "temperature.h"

#define TEMP_THRESHOLD_HIGH 100  // 고온 임계값 설정
#define TEMP_THRESHOLD_LOW -50   // 저온 임계값 설정

int main(int argc, char **argv) { 
    char *buffer; 
    int file;

    file = spi_init("/dev/spidev1.0");  // SPI 초기화
    if (file < 0) {
        perror("SPI 초기화 실패");
        return 1;
    }

    buffer = (char *)spi_read_lm74(file);  // 온도 센서에서 데이터 읽기
    if (!buffer) {
        perror("SPI 장치에서 읽기 실패");
        close(file);
        return 1;
    }
    close(file);

    int value = 0;  // 13비트 빅 엔디안 싸인드 인트를 16비트 리틀 엔디안 싸인드 인트로 변환
    value = (buffer[1] >> 3);  // 마지막 3비트 버리기
    value += (buffer[0]) << 5;  // 상위 8비트를 값에 추가

    if (buffer[0] & 0x80) {  // buffer[0]의 MSB가 1이면 음수값임
        for (int i = 31; i > 12; i--) {
            value |= (1 << i);  // 2의 보수를 위해 비트를 1로 설정
        }
    }

    double temp = (double)value * 0.0625;  // 각 비트가 0.0625도를 나타냄

    if (temp > TEMP_THRESHOLD_HIGH || temp < TEMP_THRESHOLD_LOW) {
        printf("온도 이상 감지됨\n");
    }

    free(buffer);
    return 0;
}
