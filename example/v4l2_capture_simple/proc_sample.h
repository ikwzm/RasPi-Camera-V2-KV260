/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * (c) 2024 Ichiro Kawazome
 */
#ifndef  PROC_SAMPLE_H
#define  PROC_SAMPLE_H

#include <time.h>
#include "bmp.h"

#define  BMP_FILE_NAME "sample.bmp"
struct proc_time_info {
	struct timespec start;
	struct timespec done;
	long long       elapsed;
};
static  long long proc_time_calc_elapsed(struct proc_time_info* t)
{
	long time_sec  = t->done.tv_sec  - t->start.tv_sec; 
	long time_nsec = t->done.tv_nsec - t->start.tv_nsec;
	t->elapsed = 1000*1000*1000*time_sec + time_nsec;
}
struct  proc_info {
	uint8_t*        buffer;
	int             width;
	int             height;
	struct timespec_table {
		struct proc_time_info total;
		struct proc_time_info wait;
		struct proc_time_info dequeue;
		struct proc_time_info run;
		struct proc_time_info enqueue;
	} time;
};
static struct proc_info* proc_info_list = NULL;
static int               proc_info_size = 0;
static void proc_init(int num, int width, int height)
{
	size_t buffer_size = width*height*3;
	if ((buffer_size % sizeof(uint64_t)) != 0) {
		perror("Illegal buffer size for bmp writing");
		exit(EXIT_FAILURE);
	}
	proc_info_list = (struct proc_info*)malloc(num*sizeof(struct  proc_info));
	if (proc_info_list == NULL) {
		perror("Failed to malloc for bmp writing");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < num ; i++) {
		proc_info_list[i].width  = width;
		proc_info_list[i].height = height;
		if ((proc_info_list[i].buffer = malloc(buffer_size)) == NULL) {
			perror("Failed to malloc for bmp writing");
			exit(EXIT_FAILURE);
		}
	}
	proc_info_size = num;
}
static void proc_run(int proc_index, int buf_index, int plane, void* start, size_t size)
{
	uint64_t* src   = (uint64_t*)start;
	uint64_t* dst   = (uint64_t*)proc_info_list[proc_index].buffer;
        size_t    words = size/sizeof(uint64_t);
	for (int i = 0; i < words; i++)
		dst[i] = src[i];
}
static void proc_start(int proc_index)
{
	clock_gettime(CLOCK_MONOTONIC, &(proc_info_list[proc_index].time.total.start));
}
static void proc_wait_start(int proc_index)
{
	clock_gettime(CLOCK_MONOTONIC, &(proc_info_list[proc_index].time.wait.start));
}
static void proc_wait_done(int proc_index)
{
	clock_gettime(CLOCK_MONOTONIC, &(proc_info_list[proc_index].time.wait.done));
}
static void proc_dequeue_start(int proc_index)
{
	clock_gettime(CLOCK_MONOTONIC, &(proc_info_list[proc_index].time.dequeue.start));
}
static void proc_dequeue_done(int proc_index)
{
	clock_gettime(CLOCK_MONOTONIC, &(proc_info_list[proc_index].time.dequeue.done));
}
static void proc_run_start(int proc_index)
{
	clock_gettime(CLOCK_MONOTONIC, &(proc_info_list[proc_index].time.run.start));
}
static void proc_run_done(int proc_index)
{
	clock_gettime(CLOCK_MONOTONIC, &(proc_info_list[proc_index].time.run.done));
}
static void proc_enqueue_start(int proc_index)
{
	clock_gettime(CLOCK_MONOTONIC, &(proc_info_list[proc_index].time.enqueue.start));
}
static void proc_enqueue_done(int proc_index)
{
	clock_gettime(CLOCK_MONOTONIC, &(proc_info_list[proc_index].time.enqueue.done));
}
static void proc_done(int proc_index)
{
	clock_gettime(CLOCK_MONOTONIC, &(proc_info_list[proc_index].time.total.done));
}
static double proc_calc_time(struct proc_time_info time) {
	long time_sec  = time.done.tv_sec  - time.start.tv_sec; 
	long time_nsec = time.done.tv_nsec - time.start.tv_nsec;
	return time_sec + time_nsec*1e-9;
}
static void proc_complete()
{
	struct proc_info proc_info = proc_info_list[proc_info_size-1];
	BMP_WRITE(BMP_FILE_NAME, proc_info.width, proc_info.height, proc_info.buffer);
	for (int i = 0; i < proc_info_size; i++) {
		proc_time_calc_elapsed(&proc_info_list[i].time.total);
		proc_time_calc_elapsed(&proc_info_list[i].time.wait);
		proc_time_calc_elapsed(&proc_info_list[i].time.dequeue);
		proc_time_calc_elapsed(&proc_info_list[i].time.run);
		proc_time_calc_elapsed(&proc_info_list[i].time.enqueue);
	}
	double total_time   = 0.0;
	double wait_time    = 0.0;
	double dequeue_time = 0.0;
	double run_time     = 0.0;
	double enqueue_time = 0.0;
	for (int i = 0; i < proc_info_size; i++) {
		total_time   += (double)(proc_info_list[i].time.total.elapsed  )/(1000.0*1000.0*1000.0);
		wait_time    += (double)(proc_info_list[i].time.wait.elapsed   )/(1000.0*1000.0*1000.0);
		dequeue_time += (double)(proc_info_list[i].time.dequeue.elapsed)/(1000.0*1000.0*1000.0);
		run_time     += (double)(proc_info_list[i].time.run.elapsed    )/(1000.0*1000.0*1000.0);
		enqueue_time += (double)(proc_info_list[i].time.enqueue.elapsed)/(1000.0*1000.0*1000.0);
	}
	printf("Frame :\n");
	printf("  Width   : %d\n", proc_info_list[0].width );
	printf("  Height  : %d\n", proc_info_list[0].height);
	printf("Frames    : %d\n", proc_info_size);
	printf("Proc Time : # Average Per Frame\n");
	printf("  Total   : %.9f #[Second]\n"  , total_time   / proc_info_size);
	printf("  Wait    : %.9f #[Second]\n"  , wait_time    / proc_info_size);
	printf("  Dequeue : %.9f #[Second]\n"  , dequeue_time / proc_info_size);
	printf("  Run     : %.9f #[Second]\n"  , run_time     / proc_info_size);
	printf("  Enqueue : %.9f #[Second]\n"  , enqueue_time / proc_info_size);
	printf("FPS       : %.9f #[Frames Per Second]\n", 1.0/(total_time/proc_info_size));
}
#endif
