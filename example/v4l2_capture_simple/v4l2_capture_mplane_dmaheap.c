/*********************************************************************************
 *
 *       Copyright (C) 2024 Ichiro Kawazome
 *       All rights reserved.
 * 
 *       Redistribution and use in source and binary forms, with or without
 *       modification, are permitted provided that the following conditions
 *       are met:
 * 
 *         1. Redistributions of source code must retain the above copyright
 *            notice, this list of conditions and the following disclaimer.
 * 
 *         2. Redistributions in binary form must reproduce the above copyright
 *            notice, this list of conditions and the following disclaimer in
 *            the documentation and/or other materials provided with the
 *            distribution.
 * 
 *       THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *       "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *       LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *       A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 *       OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *       SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *       LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *       DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *       THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *       (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *       OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 ********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <poll.h>
#include <linux/dma-heap.h>
#include <linux/videodev2.h>

#define  VIDEO_DEVICE_FILE		"/dev/video0"
#define  DMA_HEAP_DEVICE_FILE           "/dev/dma_heap/reserved"
#define  CAPTURE_IMAGE_WIDTH		1920
#define  CAPTURE_IMAGE_HEIGHT		1080
#define  CAPTURE_IMAGE_PIXELFORMAT	V4L2_PIX_FMT_RGB24
#define  CAPTURE_IMAGE_COUNT		10
#define  CAPTURE_NUM_BUFFERS		4

void proc_run(int buf_index, int plane, void* start, size_t size)
{
}

static int xioctl(int fd, int ioctl_code, void* parameter)
{
	int result;
	do {
		result = ioctl(fd, ioctl_code, parameter);
	} while ((result == -1) && (errno == EINTR));
	return result;
}

int main(int argc, char** argv)
{
	int                        video_fd    = 0;
        int                        dma_heap_fd = 0;
	int                        num_planes  = 0;
	int                        num_buffers = 0;
	struct v4l2_capability     v4l2_cap;
	struct v4l2_format         v4l2_fmt;
	struct v4l2_requestbuffers v4l2_reqbuf;
	struct v4l2_buffer         v4l2_buf;
	struct v4l2_plane          v4l2_planes[CAPTURE_NUM_BUFFERS][VIDEO_MAX_PLANES];
	enum   v4l2_buf_type       v4l2_buf_type;
	struct buffer {
		void*  start;
		size_t length;
	}                          buffers[CAPTURE_NUM_BUFFERS][VIDEO_MAX_PLANES];
	//
	// 1. Open Video Device
	//
	video_fd = open(VIDEO_DEVICE_FILE, O_RDWR);
	if (video_fd == -1) {
		perror(VIDEO_DEVICE_FILE);
		return EXIT_FAILURE;
	}
        dma_heap_fd = open(DMA_HEAP_DEVICE_FILE, O_RDWR);
	if (dma_heap_fd == -1) {
		perror(DMA_HEAP_DEVICE_FILE);
		return EXIT_FAILURE;
	}
	//
	// 2. Check Capabilities
	//
	if (xioctl(video_fd, VIDIOC_QUERYCAP, &v4l2_cap)) {
		perror("VIDIOC_QUERYCAP");
		return EXIT_FAILURE;
	}
	if (!(v4l2_cap.capabilities & V4L2_CAP_STREAMING)) {
		fprintf(stderr, "Not support streaming\n");
		return EXIT_FAILURE;
	}
	if (!(v4l2_cap.capabilities & V4L2_CAP_VIDEO_CAPTURE_MPLANE)) {
		fprintf(stderr, "Not support capture multiplane\n");
		return EXIT_FAILURE;
	}
	//
	// 3. Set Capture Image Format
	//
	memset(&v4l2_fmt, 0, sizeof(v4l2_fmt));
	v4l2_fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	v4l2_fmt.fmt.pix_mp.width       = CAPTURE_IMAGE_WIDTH;
	v4l2_fmt.fmt.pix_mp.height      = CAPTURE_IMAGE_HEIGHT;
	v4l2_fmt.fmt.pix_mp.pixelformat = CAPTURE_IMAGE_PIXELFORMAT;
	v4l2_fmt.fmt.pix_mp.num_planes  = 1;
	if (xioctl(video_fd, VIDIOC_S_FMT, &v4l2_fmt)) {
		perror("VIDIOC_S_FMT");
		return EXIT_FAILURE;
	}
	//
	// 4. Check Capture Image Format
	//
	memset(&v4l2_fmt, 0, sizeof(v4l2_fmt));
	v4l2_fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	v4l2_fmt.fmt.pix_mp.width       = CAPTURE_IMAGE_WIDTH;
	v4l2_fmt.fmt.pix_mp.height      = CAPTURE_IMAGE_HEIGHT;
	v4l2_fmt.fmt.pix_mp.pixelformat = CAPTURE_IMAGE_PIXELFORMAT;
	v4l2_fmt.fmt.pix_mp.num_planes  = 1;
	if (xioctl(video_fd, VIDIOC_G_FMT, &v4l2_fmt)) {
		perror("VIDIOC_G_FMT");
		return EXIT_FAILURE;
	}
	if ((v4l2_fmt.fmt.pix_mp.width       != CAPTURE_IMAGE_WIDTH      ) ||
	    (v4l2_fmt.fmt.pix_mp.height      != CAPTURE_IMAGE_HEIGHT     ) ||
	    (v4l2_fmt.fmt.pix_mp.pixelformat != CAPTURE_IMAGE_PIXELFORMAT)) {
		fprintf(stderr, "Not support format\n");
		return EXIT_FAILURE;
	}
	for (num_planes = 0; num_planes < VIDEO_MAX_PLANES; num_planes++)
		if (v4l2_fmt.fmt.pix_mp.plane_fmt[num_planes].sizeimage == 0)
			break;
	if (num_planes == 0) {
		fprintf(stderr, "num_planes is 0\n");
		return EXIT_FAILURE;
	}
	//
	// 5. Requst Buffers 
	//
	memset(&v4l2_reqbuf, 0, sizeof(v4l2_reqbuf));
	v4l2_reqbuf.count  = CAPTURE_NUM_BUFFERS;
	v4l2_reqbuf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	v4l2_reqbuf.memory = V4L2_MEMORY_DMABUF;
	if (xioctl(video_fd, VIDIOC_REQBUFS, &v4l2_reqbuf)) {
		perror("VIDIOC_REQBUFS");
		return EXIT_FAILURE;
	}
	if (v4l2_reqbuf.count < CAPTURE_NUM_BUFFERS) {
		fprintf(stderr, "Can not request buffer size\n");
		return EXIT_FAILURE;
	}
	//
	// 6. Prepare Buffers
	//
	num_buffers = v4l2_reqbuf.count;
	memset(&v4l2_planes[0][0], 0, sizeof(v4l2_planes));
	memset(&(buffers[0][0])  , 0, sizeof(buffers));
	for (int i = 0; i < num_buffers; i++) {
		memset(&v4l2_buf, 0, sizeof(v4l2_buf));
		v4l2_buf.type     = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
		v4l2_buf.memory   = V4L2_MEMORY_DMABUF;
		v4l2_buf.index    = i;
		v4l2_buf.length   = num_planes;
		v4l2_buf.m.planes = &v4l2_planes[i][0];
		if (xioctl(video_fd, VIDIOC_QUERYBUF, &v4l2_buf)) {
			perror("VIDIOC_QUERYBUF");
			return EXIT_FAILURE;
		}
		for (int plane = 0; plane < num_planes; plane++) {
			void*  dma_heap_start;
			size_t dma_heap_size = v4l2_planes[i][plane].length;
	                struct dma_heap_allocation_data alloc_data = {
	                        .len        = dma_heap_size,
	                        .fd         = 0,
	                        .fd_flags   = (O_RDWR | O_CLOEXEC),
	                        .heap_flags = 0,
	                };
			if (ioctl(dma_heap_fd, DMA_HEAP_IOCTL_ALLOC, &alloc_data)) {
				perror("DMA_HEAP_IOCTL_ALLOC");
				return EXIT_FAILURE;
			}
			v4l2_planes[i][plane].m.fd = alloc_data.fd;
			dma_heap_start = mmap(NULL,
				              dma_heap_size,
				              PROT_READ | PROT_WRITE,
				              MAP_SHARED,
				              alloc_data.fd,
					      0);
			buffers[i][plane].length = dma_heap_size;
			buffers[i][plane].start  = dma_heap_start;
                }
	}
	//
	// 7. Enqueue Buffers
	//
	for (int buf_index = 0; buf_index < num_buffers; buf_index++) {
		memset(&v4l2_buf, 0, sizeof(v4l2_buf));
		v4l2_buf.type     = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
		v4l2_buf.memory   = V4L2_MEMORY_DMABUF;
		v4l2_buf.index    = buf_index;
		v4l2_buf.length   = num_planes;
		v4l2_buf.m.planes = &v4l2_planes[buf_index][0];
		if (xioctl(video_fd, VIDIOC_QBUF, &v4l2_buf)) {
			perror("VIDIOC_QBUF");
			return EXIT_FAILURE;
		}
	}
	//
	// 8. Start Stream
	//
	v4l2_buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	if (xioctl(video_fd, VIDIOC_STREAMON, &v4l2_buf_type)) {
		perror("VIDIOC_STREAMON");
		return EXIT_FAILURE;
	}
	//
	// 9. Capture Images
	//
	for (int count = 0; count < CAPTURE_IMAGE_COUNT; count++) {
		//
		// 9.1 Wait Captured Buffer
		//
		struct pollfd     poll_fds[1];
		int               poll_result;
		poll_fds[0].fd     = video_fd;
		poll_fds[0].events = POLLIN;
		poll_result = poll(poll_fds, 1, 5000);
		if (poll_result == -1) {
			perror("Waiting for frame");
			return EXIT_FAILURE;
		}
		//
		// 9.2 Dequeue Captured Buffer
		//
		int               buf_index;
		struct v4l2_plane buf_plane[VIDEO_MAX_PLANES] = {{0}};
		memset(&v4l2_buf, 0, sizeof(v4l2_buf));
		v4l2_buf.type     = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
		v4l2_buf.memory   = V4L2_MEMORY_DMABUF;
		v4l2_buf.length   = num_planes;
		v4l2_buf.m.planes = &buf_plane[0];
		if (xioctl(video_fd, VIDIOC_DQBUF, &v4l2_buf)) {
			perror("VIDIOC_DQBUF");
			return EXIT_FAILURE;
		}
		buf_index = v4l2_buf.index;
		//
		// 9.3 Process Captured Buffer
		//
		for (int plane = 0; plane < num_planes; plane++) {
			void*  buf_start = buffers[buf_index][plane].start;
			size_t buf_size  = buffers[buf_index][plane].length;
			proc_run(buf_index, plane, buf_start, buf_size);
		}
		//
		// 9.4 Enqueue Captured Buffer
		//
		memset(&v4l2_buf, 0, sizeof(v4l2_buf));
		v4l2_buf.type     = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
		v4l2_buf.memory   = V4L2_MEMORY_DMABUF;
		v4l2_buf.index    = buf_index;
		v4l2_buf.length   = num_planes;
		v4l2_buf.m.planes = &v4l2_planes[buf_index][0];
		if (xioctl(video_fd, VIDIOC_QBUF, &v4l2_buf)) {
			perror("VIDIOC_QBUF");
			return EXIT_FAILURE;
		}
	}
	//
	// 10. Stop Stream
	//
	v4l2_buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	if (xioctl(video_fd, VIDIOC_STREAMOFF, &v4l2_buf_type)) {
		perror("VIDIOC_STREAMOFF");
		return EXIT_FAILURE;
	}
	//
	// 11. Close Video Device
	//
	if (close(video_fd) < 0) {
		perror("Filed to close file");
		return EXIT_FAILURE;
	}	
	//
	// 12. Done
	//
	return EXIT_SUCCESS;
}
