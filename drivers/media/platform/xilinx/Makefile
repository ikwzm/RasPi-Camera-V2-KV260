# SPDX-License-Identifier: GPL-2.0

#
# For in kernel tree variables
# 
xilinx-debug-video-objs += xilinx-debug-dma.o xilinx-debug-vip.o xilinx-debug-vipp.o

obj-$(CONFIG_VIDEO_XILINX_DEBUG) += xilinx-debug-video.o
obj-$(CONFIG_VIDEO_XILINX_DEBUG_CSI2RXSS) += xilinx-debug-csi2rxss.o
obj-$(CONFIG_VIDEO_XILINX_DEBUG_DEMOSAIC) += xilinx-debug-demosaic.o
obj-$(CONFIG_VIDEO_XILINX_DEBUG_GAMMA) += xilinx-debug-gamma.o
obj-$(CONFIG_VIDEO_XILINX_DEBUG_VPSS_CSC) += xilinx-debug-vpss-csc.o
obj-$(CONFIG_VIDEO_XILINX_DEBUG_VPSS_SCALER) += xilinx-debug-vpss-scaler.o

#
# For out of kernel tree variables
#
CONFIG_MODULES ?= CONFIG_VIDEO_XILINX_DEBUG=m             \
                  CONFIG_VIDEO_XILINX_DEBUG_CSI2RXSS=m    \
                  CONFIG_VIDEO_XILINX_DEBUG_DEMOSAIC=m    \
                  CONFIG_VIDEO_XILINX_DEBUG_GAMMA=m       \
                  CONFIG_VIDEO_XILINX_DEBUG_VPSS_CSC=m    \
                  CONFIG_VIDEO_XILINX_DEBUG_VPSS_SCALER=m \
                  $(END_OF_LINE)

HOST_ARCH ?= $(shell uname -m | sed -e s/arm.*/arm/ -e s/aarch64.*/arm64/)
ARCH      ?= $(shell uname -m | sed -e s/arm.*/arm/ -e s/aarch64.*/arm64/)

ifeq ($(ARCH), arm)
 ifneq ($(HOST_ARCH), arm)
   CROSS_COMPILE ?= arm-linux-gnueabihf-
 endif
endif
ifeq ($(ARCH), arm64)
 ifneq ($(HOST_ARCH), arm64)
   CROSS_COMPILE ?= aarch64-linux-gnu-
 endif
endif

ifdef KERNEL_SRC
  KERNEL_SRC_DIR := $(KERNEL_SRC)
else
  KERNEL_SRC_DIR ?= /lib/modules/$(shell uname -r)/build
endif

#
# For out of kernel tree rules
#
all:
	$(MAKE) -C $(KERNEL_SRC_DIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) $(CONFIG_MODULES) modules

modules_install:
	$(MAKE) -C $(KERNEL_SRC_DIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) $(CONFIG_MODULES) modules_install

clean:
	$(MAKE) -C $(KERNEL_SRC_DIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) clean

