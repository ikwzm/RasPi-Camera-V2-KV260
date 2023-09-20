Raspberry Pi Camera V2 for Kv260
====================================================================================

Overvier
------------------------------------------------------------------------------------

### Intrduction

This repository provides PL(Programmable Logic) for using the Raspberry Pi Camera-V2 with the KV260.
The PL and Device Tree provided in this repository is based on the blog shown at the following URL.

  * https://forxai.konicaminolta.com/blog/30
  * https://forxai.konicaminolta.com/blog/31

I would like to take this opportunity to thank them.

### Requirement

* Board: 
  - Kv260
* OS: 
  - https://github.com/ikwzm/ZynqMP-FPGA-Ubuntu22.04-Desktop

Current Status
------------------------------------------------------------------------------------

* Build Bitstream file : Success
* Install Device tree  : Success
* Setup video4linux    : Success?
* Capture by camera    : Failure


Build Bitstream file
------------------------------------------------------------------------------------

### Requirement

* Xilinx Vivado 2023.1

### Download RasPi-Camera-V2-KV260

```console
shell$ git clone --depth 1 --recursive https://github.com/ikwzm/RasPi-Camera-V2-KV260
```

### Create Project

```
Vivado > Tools > Run Tcl Script... > RasPi-Camera-V2-KV260/fpga/create_project.tcl
```

### Implementation

```
Vivado > Tools > Run Tcl Script... > RasPi-Camera-V2-KV260/fpga/implementation.tcl
```

### Convert from Bitstream File to Binary File

```console
vivado% cd RasPi-Camera-V2-KV260/fpga
vivado% bootgen -image raspi-camera-v2-kv260.bif -arch zynqmp -w -o ../raspi-camera-v2-kv260.bin
```

### Compress raspi-camera-v2-kv260.bin to raspi-camera-v2-kv260.bin.gz

```console
vivado% cd RasPi-Camera-V2-KV260
vivado% gzip raspi-camera-v2-kv260.bin
```

Install Device Tree
------------------------------------------------------------------------------------

### Decompress raspi-camera-v2-kv260.bin.gz to raspi-camera-v2-kv260.bin

```console
shell$ gzip -d raspi-camera-v2-kv260.bin.gz
```

### Copy raspi-camera-v2-kv260.bin to /lib/firmware

```console
shell$ sudo cp raspi-camera-v2-kv260.bin /lib/firmware
```

### Compile Device Tree Blob

```console
shell$ dtc -I dts -O dtb -@ -o raspi-camera-v2-kv260.dtb raspi-camera-v2-kv260.dts
raspi-camera-v2-kv260.dts:357.39-367.7: Warning (graph_child_address): /fragment@3/__overlay__/vcap_v_proc_ss_scaler/ports: graph node has single child node 'port@0', #address-cells/#size-cells are not necessary
```

### Load Device Tree

```console
shell$ sudo mkdir /config/device-tree/overlays/raspi-camera-v2-kv260
[sudo] password for fpga:
shell$ sudo cp raspi-camera-v2-kv260.dtb /config/device-tree/overlays/raspi-camera-v2-kv260/dtbo
```

```console
shell$ dmesg | tail -81
[  401.027191] fpga_manager fpga0: writing raspi-camera-v2-kv260.bin to Xilinx ZynqMP FPGA Manager
[  401.166090] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /fpga-full/firmware-name
[  401.176205] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /fpga-full/resets
[  401.186471] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/overlay0
[  401.196317] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/overlay1
[  401.206155] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/afi0
[  401.215641] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/clocking0
[  401.225562] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/clocking1
[  401.235483] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/overlay2
[  401.245319] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/imx219_vana
[  401.255412] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/imx219_vdig
[  401.265507] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/imx219_vddl
[  401.275607] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/imx219_clk
[  401.285613] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/overlay3
[  401.295451] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/axi_iic_0
[  401.305376] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/iic_mux_0
[  401.315297] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/isa0_i2c0
[  401.325221] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/isa1_i2c1
[  401.335141] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/rpi_i2c2
[  401.344975] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/imx219
[  401.354636] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/imx219_0
[  401.364470] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/misc_clk_0
[  401.374478] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/mipi_csi2_rx_subsyst_0
[  401.385530] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/mipi_csi_portsmipi_csi2_rx_subsyst_0
[  401.397794] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/mipi_csi_port1mipi_csi2_rx_subsyst_0
[  401.410060] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/mipi_csirx_outmipi_csi2_rx_subsyst_0
[  401.422327] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/mipi_csi_port0mipi_csi2_rx_subsyst_0
[  401.434597] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/mipi_csi_inmipi_csi2_rx_subsyst_0
[  401.446601] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/misc_clk_1
[  401.456609] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/v_demosaic_0
[  401.466791] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/demosaic_portsv_demosaic_0
[  401.478188] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/demosaic_port1v_demosaic_0
[  401.489589] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/demo_outv_demosaic_0
[  401.500472] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/demosaic_port0v_demosaic_0
[  401.511867] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/v_demosaic_0mipi_csi2_rx_subsyst_0
[  401.523958] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/v_frmbuf_wr_0
[  401.534227] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/v_gamma_lut_0
[  401.544499] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/gamma_portsv_gamma_lut_0
[  401.555719] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/gamma_port1v_gamma_lut_0
[  401.566942] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/gamma_outv_gamma_lut_0
[  401.577992] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/gamma_port0v_gamma_lut_0
[  401.589216] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/v_gamma_lut_0v_demosaic_0
[  401.600527] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/v_proc_ss_csc
[  401.610800] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/csc_portsv_proc_ss_csc
[  401.621852] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/csc_port1v_proc_ss_csc
[  401.632902] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/csc_outv_proc_ss_csc
[  401.643786] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/csc_port0v_proc_ss_csc
[  401.654838] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/v_proc_ss_cscv_gamma_lut_0
[  401.666234] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/v_proc_ss_scaler
[  401.676769] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/scaler_portsv_proc_ss_scaler
[  401.688353] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/scaler_port1v_proc_ss_scaler
[  401.699936] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/sca_outv_proc_ss_scaler
[  401.711081] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/scaler_port0v_proc_ss_scaler
[  401.722656] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/v_proc_ss_scalerv_proc_ss_csc
[  401.734312] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/vcap_portsv_proc_ss_scaler
[  401.745710] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/v_frmbuf_wr_0v_proc_ss_scaler
[  401.827819] platform 80002000.mipi_csi2_rx_subsystem: Fixed dependency cycle(s) with /axi/i2c@80030000/i2c_mux@74/i2c@2/sensor@10
[  401.830172] platform 80002000.mipi_csi2_rx_subsystem: Fixed dependency cycle(s) with /axi/v_demosaic@a0010000
[  401.840233] i2c i2c-3: Added multiplexed i2c bus 4
[  401.841826] i2c i2c-3: Added multiplexed i2c bus 5
[  401.843996] i2c i2c-3: Added multiplexed i2c bus 6
[  401.844661] xilinx-frmbuf a0020000.v_frmbuf_wr: Xilinx AXI frmbuf DMA_DEV_TO_MEM
[  401.848972] xilinx-frmbuf a0020000.v_frmbuf_wr: Xilinx AXI FrameBuffer Engine Driver Probed!!
[  401.855761] i2c i2c-3: Added multiplexed i2c bus 7
[  401.856020] pca954x 3-0074: registered 4 multiplexed busses for I2C switch pca9546
[  401.879064] platform a0010000.v_demosaic: Fixed dependency cycle(s) with /axi/v_gamma_lut@a0030000
[  401.882418] platform a0030000.v_gamma_lut: Fixed dependency cycle(s) with /axi/v_proc_ss_csc@a0040000
[  401.883376] platform a0040000.v_proc_ss_csc: Fixed dependency cycle(s) with /axi/v_proc_ss_scaler@a0080000
[  401.887400] platform a0080000.v_proc_ss_scaler: Fixed dependency cycle(s) with /axi/vcap_v_proc_ss_scaler
[  401.909018] xilinx-video axi:vcap_v_proc_ss_scaler: Entity type for entity 80002000.mipi_csi2_rx_subsystem was not initialized!
[  401.909044] xilinx-video axi:vcap_v_proc_ss_scaler: device registered
[  401.925335] xilinx-video axi:vcap_v_proc_ss_scaler: Entity type for entity a0010000.v_demosaic was not initialized!
[  401.925359] xilinx-demosaic a0010000.v_demosaic: Xilinx Video Demosaic Probe Successful
[  401.930522] xilinx-video axi:vcap_v_proc_ss_scaler: Entity type for entity a0030000.v_gamma_lut was not initialized!
[  401.930545] xilinx-gamma-lut a0030000.v_gamma_lut: Xilinx 10-bit Video Gamma Correction LUT registered
[  401.938025] xilinx-video axi:vcap_v_proc_ss_scaler: Entity type for entity a0040000.v_proc_ss_csc was not initialized!
[  401.938053] xilinx-vpss-csc a0040000.v_proc_ss_csc: VPSS CSC 8-bit Color Depth Probe Successful
[  401.939720] xilinx-video axi:vcap_v_proc_ss_scaler: Entity type for entity a0080000.v_proc_ss_scaler was not initialized!
[  401.939746] xilinx-vpss-scaler a0080000.v_proc_ss_scaler: Num Hori Taps 6
[  401.939753] xilinx-vpss-scaler a0080000.v_proc_ss_scaler: Num Vert Taps 6
[  401.939758] xilinx-vpss-scaler a0080000.v_proc_ss_scaler: VPSS Scaler Probe Successful
```

Setup video4linux
------------------------------------------------------------------------------------

### Requirement

```console
shell$ sudo apt install -y yavta
shell$ sudo apt install -y v4l-utils
```

### Download https://github.com/RyusukeYamano/Raspberry-Pi-Camera-v2-on-KV260

```console
shell$ git clone https://github.com/RyusukeYamano/Raspberry-Pi-Camera-v2-on-KV260
```

### Run setup_VGA.sh

```console
shell$ sudo sh setup_VGA.sh
### SONY IMX219 Sensor subdev1 ###
### MIPI CSI2-Rx Subsystem subdev0 ###
### Demosaic IP subdev2 ###
### Gamma LUT IP subdev3 ###
Unable to setup formats: Invalid argument (22)
Unable to setup formats: Invalid argument (22)
Device /dev/v4l-subdev1 opened.
Control 0x0098c9c1 set to 10, is 10
Device /dev/v4l-subdev1 opened.
Control 0x0098c9c2 set to 10, is 10
Device /dev/v4l-subdev1 opened.
Control 0x0098c9c3 set to 10, is 10
### VPSS: Color Space Conversion (CSC) Only ###
Device /dev/v4l-subdev2 opened.
Unable to get format: Inappropriate ioctl for device (25).
Device /dev/v4l-subdev2 opened.
Unable to get format: Inappropriate ioctl for device (25).
Device /dev/v4l-subdev2 opened.
Unable to get format: Inappropriate ioctl for device (25).
Device /dev/v4l-subdev2 opened.
Unable to get format: Inappropriate ioctl for device (25).
Device /dev/v4l-subdev2 opened.
Unable to get format: Inappropriate ioctl for device (25).
### VPSS: Scaler Only with CSC ###
### show pipeline ###
Media controller API version 6.1.47

Media device information
------------------------
driver          xilinx-video
model           Xilinx Video Composite Device
serial
bus info        platform:axi:vcap_v_proc_ss_sca
hw revision     0x0
driver version  6.1.47

Device topology
- entity 1: vcap_v_proc_ss_scaler output 0 (1 pad, 1 link)
            type Node subtype V4L flags 0
            device node name /dev/video0
        pad0: Sink
                <- "a0080000.v_proc_ss_scaler":1 [ENABLED]

- entity 5: 80002000.mipi_csi2_rx_subsystem (2 pads, 2 links)
            type V4L2 subdev subtype Unknown flags 0
            device node name /dev/v4l-subdev0
        pad0: Sink
                [fmt:SRGGB10_1X10/640x480 field:none]
                <- "imx219 6-0010":0 [ENABLED]
        pad1: Source
                [fmt:SRGGB10_1X10/640x480 field:none]
                -> "a0010000.v_demosaic":0 [ENABLED]

- entity 8: a0010000.v_demosaic (2 pads, 2 links)
            type V4L2 subdev subtype Unknown flags 0
            device node name /dev/v4l-subdev1
        pad0: Sink
                [fmt:SRGGB10_1X10/640x480 field:none]
                <- "80002000.mipi_csi2_rx_subsystem":1 [ENABLED]
        pad1: Source
                [fmt:RBG888_1X24/640x480 field:none]
                -> "a0030000.v_gamma_lut":0 [ENABLED]

- entity 11: a0030000.v_gamma_lut (2 pads, 2 links)
             type V4L2 subdev subtype Unknown flags 0
             device node name /dev/v4l-subdev2
        pad0: Sink
                [fmt:RBG888_1X24/640x480 field:none]
                <- "a0010000.v_demosaic":1 [ENABLED]
        pad1: Source
                [fmt:RBG888_1X24/640x480 field:none]
                -> "a0040000.v_proc_ss_csc":0 [ENABLED]

- entity 14: a0040000.v_proc_ss_csc (2 pads, 2 links)
             type V4L2 subdev subtype Unknown flags 0
             device node name /dev/v4l-subdev3
        pad0: Sink
                [fmt:RBG888_1X24/640x480 field:none]
                <- "a0030000.v_gamma_lut":1 [ENABLED]
        pad1: Source
                [fmt:RBG888_1X24/640x480 field:none]
                -> "a0080000.v_proc_ss_scaler":0 [ENABLED]

- entity 17: a0080000.v_proc_ss_scaler (2 pads, 2 links)
             type V4L2 subdev subtype Unknown flags 0
             device node name /dev/v4l-subdev4
        pad0: Sink
                [fmt:RBG888_1X24/640x480 field:none]
                <- "a0040000.v_proc_ss_csc":1 [ENABLED]
        pad1: Source
                [fmt:RBG888_1X24/640x480 field:none]
                -> "vcap_v_proc_ss_scaler output 0":0 [ENABLED]

- entity 20: imx219 6-0010 (1 pad, 1 link)
             type V4L2 subdev subtype Sensor flags 0
             device node name /dev/v4l-subdev5
        pad0: Source
                [fmt:SRGGB10_1X10/640x480 field:none colorspace:srgb xfer:srgb ycbcr:601 quantization:full-range
                 crop.bounds:(8,8)/3280x2464
                 crop:(1008,760)/1280x960]
                -> "80002000.mipi_csi2_rx_subsystem":0 [ENABLED]

ioctl: VIDIOC_ENUM_FMT
        Type: Video Capture Multiplanar

        [0]: 'RX24' (32-bit XBGR 8-8-8-8)
        [1]: 'XR24' (32-bit BGRX 8-8-8-8)
        [2]: 'RGB3' (24-bit RGB 8-8-8)
        [3]: 'BGR3' (24-bit BGR 8-8-8)
```

Capture by camera
------------------------------------------------------------------------------------

### Run cap_VGA.sh

```console
shell$ sudo sh cap_VGA.sh
Setting pipeline to PAUSED ...
Pipeline is live and does not need PREROLL ...
Pipeline is PREROLLED ...
Setting pipeline to PLAYING ...
0:00:00.251026092  1694   0x559509a180 WARN                    v4l2 gstv4l2object.c:4524:gst_v4l2_object_probe_caps:<v4l2src0:src> Failed to probe pixel aspect ratio with VIDIOC_CROPCAP: Invalid argument
New clock: GstSystemClock
0:00:00.251544830  1694   0x559509a180 WARN                 basesrc gstbasesrc.c:3127:gst_base_src_loop:<v4l2src0> error: Internal data stream error.
0:00:00.251634281  1694   0x559509a180 WARN                 basesrc gstbasesrc.c:3127:gst_base_src_loop:<v4l2src0> error: streaming stopped, reason not-negotiated (-4)
ERROR: from element /GstPipeline:pipeline0/GstV4l2Src:v4l2src0: Internal data stream error.
Additional debug info:
../libs/gst/base/gstbasesrc.c(3127): gst_base_src_loop (): /GstPipeline:pipeline0/GstV4l2Src:v4l2src0:
streaming stopped, reason not-negotiated (-4)
Execution ended after 0:00:00.001107666
Setting pipeline to NULL ...
Freeing pipeline ...
```

