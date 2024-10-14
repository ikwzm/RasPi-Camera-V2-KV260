/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * (c) 2024 Ichiro Kawazome
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <poll.h>
#include <linux/dma-buf.h>
#include <linux/videodev2.h>
#include <inttypes.h>

#include "u-dma-buf-ioctl.h"
#define  VIDEO_DEVICE_FILE		"/dev/video0"
#define  U_DMA_BUF_DEVICE_NAME          "udmabuf0"
#define  U_DMA_BUF_DEVICE_FILE          "/dev/udmabuf0"
#define  CAPTURE_IMAGE_WIDTH		1920
#define  CAPTURE_IMAGE_HEIGHT		1080
#define  CAPTURE_IMAGE_PIXELFORMAT	V4L2_PIX_FMT_RGB24
#define  CAPTURE_IMAGE_COUNT		10
#define  CAPTURE_NUM_BUFFERS		4

#include "proc_sample.h"

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
	int                        video_fd     = 0;
        int                        u_dma_buf_fd = 0;
        size_t                     u_dma_buf_size;
        void*                      u_dma_buf_start;
        uint64_t                   u_dma_buf_offset;
	int                        num_planes   = 0;
	int                        num_buffers  = 0;
        int                        export_mmap  = 1;
	int                        export_sync  = 0; // O_SYNC;
	int                        skip_sync    = 0;
	uint64_t                   buffer_offset= 0;
	size_t                     plane_bytes  = 0;
	size_t                     buffer_bytes = 0;
        size_t                     align_size   = 0x1000;
	struct v4l2_capability     v4l2_cap;
	struct v4l2_format         v4l2_fmt;
	struct v4l2_requestbuffers v4l2_reqbuf;
	struct v4l2_buffer         v4l2_buf;
	enum   v4l2_buf_type       v4l2_buf_type;
	struct v4l2_plane          v4l2_planes[CAPTURE_NUM_BUFFERS][VIDEO_MAX_PLANES];
	struct buffer {
		void*  start;
		size_t length;
	}                          buffers[CAPTURE_NUM_BUFFERS][VIDEO_MAX_PLANES];
	proc_init(CAPTURE_IMAGE_COUNT, CAPTURE_IMAGE_WIDTH, CAPTURE_IMAGE_HEIGHT);
	//
	// 1. Open Device
	//
	video_fd = open(VIDEO_DEVICE_FILE, O_RDWR);
	if (video_fd == -1) {
		perror(VIDEO_DEVICE_FILE);
		return EXIT_FAILURE;
	}
        u_dma_buf_fd = open(U_DMA_BUF_DEVICE_FILE, O_RDWR);
	if (u_dma_buf_fd == -1) {
		perror(U_DMA_BUF_DEVICE_FILE);
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
			size_t buf_length = v4l2_planes[i][plane].length;
			buffers[i][plane].length = buf_length;
			if (buf_length >= plane_bytes)
				plane_bytes = buf_length;
			
		}
	}
        plane_bytes  = ((plane_bytes+align_size-1)/align_size)*align_size;
	buffer_bytes = num_planes*plane_bytes;
        if (ioctl(u_dma_buf_fd, U_DMA_BUF_IOCTL_GET_SIZE, &u_dma_buf_size)) {
		perror("U_DMA_BUF_IOCTL_GET_SIZE");
		return EXIT_FAILURE;
	}
	if (num_buffers*buffer_bytes > u_dma_buf_size) {
		perror("u_dma_buf_size is small");
		return EXIT_FAILURE;
	}
	u_dma_buf_size  = num_buffers*buffer_bytes;
        if (export_mmap == 0) {
		u_dma_buf_start = mmap(NULL,
				       u_dma_buf_size,
				       PROT_READ | PROT_WRITE,
				       MAP_SHARED,
				       u_dma_buf_fd,
				       0);
		if (u_dma_buf_start == MAP_FAILED) {
			perror("mmap");
			return EXIT_FAILURE;
		}
        	// {
        	// 	uint64_t* word_ptr = u_dma_buf_start;
        	// 	size_t    words    = u_dma_buf_size/sizeof(uint64_t);
        	// 	for(int i = 0; i < words; i++){
        	// 		word_ptr[i] = 0;
        	// 	}
        	// }
        }
	buffer_offset = 0;
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
			void*  buf_start;
			size_t buf_size = v4l2_planes[i][plane].length;
                        u_dma_buf_ioctl_export_args export_args = {0};
                        export_args.size   = plane_bytes;
                        export_args.offset = buffer_offset;
                        SET_U_DMA_BUF_IOCTL_FLAGS_EXPORT_FD_FLAGS(&export_args,O_RDWR | O_CLOEXEC | export_sync);
			if (ioctl(u_dma_buf_fd, U_DMA_BUF_IOCTL_EXPORT, &export_args)) {
				perror("U_DMA_BUF_IOCTL_EXPORT");
				return EXIT_FAILURE;
			}
                        if (export_mmap == 0) {
        			buf_start = u_dma_buf_start + buffer_offset;
                        } else {
                                buf_start = mmap(NULL, buf_size, PROT_READ | PROT_WRITE, MAP_SHARED, export_args.fd, 0);
                                if (buf_start == MAP_FAILED) {
                                        perror("mmap");
                                        return EXIT_FAILURE;
                                }
                        }
			buffers[i][plane].start  = buf_start;
			buffers[i][plane].length = buf_size;
			v4l2_planes[i][plane].m.fd = export_args.fd;
			buffer_offset += plane_bytes;
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
		proc_start(count);
		//
		// 9.1 Wait Captured Buffer
		//
		proc_wait_start(count);
		struct pollfd     poll_fds[1];
		int               poll_result;
		poll_fds[0].fd     = video_fd;
		poll_fds[0].events = POLLIN;
		poll_result = poll(poll_fds, 1, 5000);
		if (poll_result == -1) {
			perror("Waiting for frame");
			return EXIT_FAILURE;
		}
		proc_wait_done(count);
		//
		// 9.2 Dequeue Captured Buffer
		//
		proc_dequeue_start(count);
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
		proc_dequeue_done(count);
		//
		// 9.3 Process Captured Buffer
		//
		proc_run_start(count);
		if (skip_sync == 0) {
			for (int plane = 0; plane < num_planes; plane++) {
				int dma_buf_fd = v4l2_planes[buf_index][plane].m.fd;
				const struct dma_buf_sync dma_buf_sync = {
					.flags = DMA_BUF_SYNC_READ | DMA_BUF_SYNC_START
				};
				if (ioctl(dma_buf_fd, DMA_BUF_IOCTL_SYNC, &dma_buf_sync)) {
					perror("DMA_BUF_IOCTL_SYNC START");
					return EXIT_FAILURE;
				}
			}
		}
		for (int plane = 0; plane < num_planes; plane++) {
			void*  buf_start = buffers[buf_index][plane].start;
			size_t buf_size  = buffers[buf_index][plane].length;
			proc_run(count, buf_index, plane, buf_start, buf_size);
		}
		if (skip_sync == 0) {
			for (int plane = 0; plane < num_planes; plane++) {
				int dma_buf_fd = v4l2_planes[buf_index][plane].m.fd;
				const struct dma_buf_sync dma_buf_sync = {
					.flags = DMA_BUF_SYNC_READ | DMA_BUF_SYNC_END
				};
				if (ioctl(dma_buf_fd, DMA_BUF_IOCTL_SYNC, &dma_buf_sync)) {
					perror("DMA_BUF_IOCTL_SYNC END");
					return EXIT_FAILURE;
				}
			}
		}
		proc_run_done(count);
		//
		// 9.4 Enqueue Captured Buffer
		//
		proc_enqueue_start(count);
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
		proc_enqueue_done(count);
		proc_done(count);
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
	// 11. Unmap Buffers
	//
	for (int i = 0; i < CAPTURE_NUM_BUFFERS; i++) {
		for (int plane = 0; plane < num_planes; plane++) {
			void*  start = buffers[i][plane].start;
			size_t size  = buffers[i][plane].length;
			if ((start != NULL) && (start != MAP_FAILED)) {
				if (munmap(start, size)) {
					perror("munmap");
					return EXIT_FAILURE;
				}
			}
		}
	}
	//
	// 12. Close Video Device
	//
	if (close(video_fd) < 0) {
		perror("Filed to close file");
		return EXIT_FAILURE;
	}	
	//
	// 13. Done
	//
	proc_complete();
	return EXIT_SUCCESS;
}
