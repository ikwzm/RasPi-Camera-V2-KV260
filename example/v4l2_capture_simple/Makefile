#
# SPDX-License-Identifier: BSD-2-Clause
#
# (c) 2024 Ichiro Kawazome
#

CC            = gcc -g
TARGET_LIST   = v4l2_capture_mplane_mmap     \
                v4l2_capture_mplane_dma_heap \
                $(END_LIST)
INCLUDE_FILES = bmp.h proc_sample.h

all: $(TARGET_LIST)

clean:
	rm -f *.o $(TARGET_LIST)

define BUILD_TARGET
$(1): $(2) $(INCLUDE_FILES)
	$(CC) -o $(1) $(2)
endef

$(foreach TARGET, $(TARGET_LIST), \
    $(eval $(call BUILD_TARGET, $(TARGET), $(addsuffix .c, $(TARGET)))) \
)
