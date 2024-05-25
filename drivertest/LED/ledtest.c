#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define LED_DRIVER_NAME "/dev/periled"

int main()
{
    ledLibInit();

    // 모든 LED를 켜기
    for (int i = 0; i < 8; i++)
    {
        ledOnOff(i, 1);
    }

    //3초 대기
    sleep(3);

    // 모든 LED를 끄기
    for (int i = 0; i < 8; i++)
    {
        ledOnOff(i, 0);
    }

    ledLibExit();
    return 0;
}

