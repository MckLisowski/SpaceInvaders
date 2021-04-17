#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <linux/vt.h>
#include <linux/kd.h>
#include <linux/input.h>
#include <time.h>
#include <pthread.h>
#include "functions.h"
#include "Rocket.h"
#include "Bullet.h"


extern struct JoY JoYAct;
extern int gRawKeys[6];


void SystemInit();
void SystemExit();
void PrintDiagnosticInfo();


int mode = 0;
int stage = 0;
volatile int flash_ligth=0;
volatile long globalTimer_ms=0;
long startTime_ms;
int frame_count = 0;
pthread_t tID;


char *fbname;
int FrameBufferFD, inputEventsFD;
struct fb_fix_screeninfo fix_info;
struct fb_var_screeninfo var_info;
void *framebuffer;

#endif /* SRC_MAIN_H_ */
