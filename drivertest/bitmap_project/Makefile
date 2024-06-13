# 컴파일러와 아카이버 설정
CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

# 기본 타겟
all: libBitmap.a bitmap_project

# 실행 파일 빌드
bitmap_project: bitmap.c libBitmap.a bitmap.h libBitmap.h bitmapFileHeader.h
        $(CC) bitmap.c -lBitmap -L. -lpthread -o bitmap_project

# 정적 라이브러리 빌드
libBitmap.a: libBitmap.o
        $(AR) rc libBitmap.a libBitmap.o

# 오브젝트 파일 생성 규칙
libBitmap.o: libBitmap.c libBitmap.h bitmapFileHeader.h
        $(CC) -c libBitmap.c -o libBitmap.o

# 클린업
clean:
        rm -f libBitmap.o libBitmap.a bitmap_project