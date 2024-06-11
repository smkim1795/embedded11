#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "gyro.h"

#define ACCEL_THRESHOLD 5000
#define ACCEL_Z_THRESHOLD 9000
#define accel_t 500000

int main(void) {
    int first_accel[3];
    int second_accel[3];

    printf("Move Player");
    getAccel(first_accel);

    while (1) {
        getAccel(second_accel);

        if (first_accel[0] - second_accel[0] > ACCEL_THRESHOLD)
        {
            printf("Moving Right\n");
        }
        else if (second_accel[0] - first_accel[0] > ACCEL_THRESHOLD)
        {
            printf("Moving Left\n");
        }
        else if (first_accel[2] - second_accel[2] > ACCEL_Z_THRESHOLD)
        {
            printf("Moving Backward\n");
        }
        else if (second_accel[2] - first_accel[2] > ACCEL_THRESHOLD)
        {
            if (second_accel[2] - first_accel[2] < ACCEL_Z_THRESHOLD)
            {
                printf("Moving Forward\n");
            }
            else
            {
                printf("Accelerating Forward\n");
            }
        }
        else
        {
            printf("Stable\n");
        }

        usleep(accel_t); // wait for the next measurement
    }

    return 0;
}
