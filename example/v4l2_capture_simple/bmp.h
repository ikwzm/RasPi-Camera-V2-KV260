/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * (c) 2024 Ichiro Kawazome
 */
#ifndef BMP_H
#define BMP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#pragma pack(push, 1)
typedef struct {
	uint16_t bfType;          // file type ("BM")
	uint32_t bfSize;          // file size (Byte Unit)
	uint16_t bfReserved1;     // reserved  (always 0)
	uint16_t bfReserved2;     // reserved  (always 0)
	uint32_t bfOffBits;       // offset to bitmap data
} BMP_FILE_HEADER;

typedef struct {
	uint32_t biSize;          // size of this structure (40bytes)
	int32_t  biWidth;         // width of bitmap
	int32_t  biHeight;        // height of bitmap
	uint16_t biPlanes;        // plaens of bitmap (always 1)
	uint16_t biBitCount;      // bits per pixel (if its 24, its 24)
	uint32_t biCompression;   // compression format (0 = no compression)
	uint32_t biSizeImage;     // image data size (in bytes)
	int32_t  biXPelsPerMeter; // horizontal resolution (pixel/m)
	int32_t  biYPelsPerMeter; // vertical resolution (pixel/m)
	uint32_t biClrUsed;       // number of used colors (0 = all)
	uint32_t biClrImportant;  // number of impotant colors (0 = all))
} BMP_INFO_HEADER;
#pragma pack(pop)

static void BMP_WRITE(const char *filename, int width, int height, uint8_t *data)
{
	BMP_FILE_HEADER file_header;
	BMP_INFO_HEADER info_header;
	FILE *          fp = fopen(filename, "wb");

	if (!fp) {
		perror("Failed to open file for bmp writing");
		exit(EXIT_FAILURE);
	}

	int      row_stride = (width * 3 + 3) & ~3;
	int      data_size  = row_stride * height;

	file_header.bfType          = 0x4D42; // "BM"
	file_header.bfSize          = sizeof(BMP_FILE_HEADER) + sizeof(BMP_INFO_HEADER) + data_size;
	file_header.bfReserved1     = 0;
	file_header.bfReserved2     = 0;
	file_header.bfOffBits       = sizeof(BMP_FILE_HEADER) + sizeof(BMP_INFO_HEADER);

	info_header.biSize          = sizeof(BMP_INFO_HEADER);
	info_header.biWidth         = width;
	info_header.biHeight        = height;
	info_header.biPlanes        = 1;
	info_header.biBitCount      = 24;
	info_header.biCompression   = 0; // 無圧縮
	info_header.biSizeImage     = data_size;
	info_header.biXPelsPerMeter = 0;
	info_header.biYPelsPerMeter = 0;
	info_header.biClrUsed       = 0;
	info_header.biClrImportant  = 0;

	fwrite(&file_header, sizeof(file_header), 1, fp);
	fwrite(&info_header, sizeof(info_header), 1, fp);

	for (int y = 0; y < height; y++) {
		fwrite(data + y * width * 3, 3, width, fp);
		fwrite("\0\0\0", 1, row_stride - width * 3, fp); // padding
	}

	fclose(fp);
}
#endif
