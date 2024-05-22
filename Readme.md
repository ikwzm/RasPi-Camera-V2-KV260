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
* Setup video4linux    : Success
* Capture by camera    : Success


Build Bitstream file
------------------------------------------------------------------------------------

[Build raspi-camera-v2-kv260-w8x1-hp0.bin](./fpga/raspi-camera-v2-w8x1-hp0/Readme.md)

Install Bitstream and Device Tree
------------------------------------------------------------------------------------

### Decompress raspi-camera-v2-kv260-w8x1-hp0.bin.gz to raspi-camera-v2-kv260-w8x1-hp0.bin

```console
shell$ gzip -d fpga/raspi-camera-v2-w8x1-hp0/raspi-camera-v2-kv260-w8x1-hp0.bin.gz
```

### Copy fpga/raspi-camera-v2-w8x1-hp0/raspi-camera-v2-kv260-w8x1-hp0.bin to /lib/firmware

```console
shell$ sudo cp fpga/raspi-camera-v2-w8x1-hp0/raspi-camera-v2-kv260-w8x1-hp0.bin /lib/firmware
```

### Load Device Tree Overlay

```console
shell$ sudo devicetrees/dtbo-config -i raspi-camera-v2-kv260-w8x1-hp0 --dts devicetrees/raspi-camera-v2-kv260-w8x1-hp0.dts
```

```console
shell$ dmesg | tail -75
[   85.936129] fpga_manager fpga0: writing raspi-camera-v2-kv260-w8x1-hp0.bin to Xilinx ZynqMP FPGA Manager
[   86.401790] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /fpga-full/firmware-name
[   86.411905] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /fpga-full/resets
[   86.422212] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/overlay0
[   86.432060] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/overlay1
[   86.441893] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/afi0
[   86.451382] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/clocking0
[   86.461301] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/clocking1
[   86.471220] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/overlay2
[   86.481055] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/imx219_vana
[   86.491150] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/imx219_vdig
[   86.501275] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/imx219_vddl
[   86.511376] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/imx219_clk
[   86.521387] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/overlay3
[   86.531230] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/axi_iic_0
[   86.541160] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/iic_mux_0
[   86.551079] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/isa0_i2c0
[   86.561000] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/isa1_i2c1
[   86.570921] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/rpi_i2c2
[   86.580756] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/imx219
[   86.590417] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/imx219_0
[   86.600263] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/misc_clk_0
[   86.610272] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/mipi_csi2_rx_subsyst_0
[   86.621328] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/mipi_csi_portsmipi_csi2_rx_subsyst_0
[   86.633592] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/mipi_csi_port1mipi_csi2_rx_subsyst_0
[   86.645858] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/mipi_csirx_outmipi_csi2_rx_subsyst_0
[   86.658126] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/mipi_csi_port0mipi_csi2_rx_subsyst_0
[   86.670396] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/mipi_csi_inmipi_csi2_rx_subsyst_0
[   86.682401] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/misc_clk_1
[   86.692408] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/v_demosaic_0
[   86.702592] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/demosaic_portsv_demosaic_0
[   86.713998] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/demosaic_port1v_demosaic_0
[   86.725407] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/demo_outv_demosaic_0
[   86.736288] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/demosaic_port0v_demosaic_0
[   86.747684] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/v_demosaic_0mipi_csi2_rx_subsyst_0
[   86.759774] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/v_frmbuf_wr_0
[   86.770043] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/v_gamma_lut_0
[   86.780312] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/gamma_portsv_gamma_lut_0
[   86.791537] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/gamma_port1v_gamma_lut_0
[   86.802759] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/gamma_outv_gamma_lut_0
[   86.813808] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/gamma_port0v_gamma_lut_0
[   86.825032] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/v_gamma_lut_0v_demosaic_0
[   86.836342] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/v_proc_ss_csc
[   86.846610] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/csc_portsv_proc_ss_csc
[   86.857660] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/csc_port1v_proc_ss_csc
[   86.868710] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/csc_outv_proc_ss_csc
[   86.879596] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/csc_port0v_proc_ss_csc
[   86.890642] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/v_proc_ss_cscv_gamma_lut_0
[   86.902041] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/v_proc_ss_scaler
[   86.912570] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/scaler_portsv_proc_ss_scaler
[   86.924141] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/scaler_port1v_proc_ss_scaler
[   86.935711] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/sca_outv_proc_ss_scaler
[   86.946848] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/scaler_port0v_proc_ss_scaler
[   86.958419] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/v_proc_ss_scalerv_proc_ss_csc
[   86.970078] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/vcap_portsv_proc_ss_scaler
[   86.981473] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /__symbols__/v_frmbuf_wr_0v_proc_ss_scaler
[   87.061197] i2c i2c-3: Added multiplexed i2c bus 4
[   87.061436] i2c i2c-3: Added multiplexed i2c bus 5
[   87.061593] xilinx-frmbuf a0020000.v_frmbuf_wr: Xilinx AXI frmbuf DMA_DEV_TO_MEM
[   87.061717] xilinx-frmbuf a0020000.v_frmbuf_wr: Xilinx AXI FrameBuffer Engine Driver Probed!!
[   87.068997] i2c i2c-3: Added multiplexed i2c bus 6
[   87.069416] i2c i2c-3: Added multiplexed i2c bus 7
[   87.069431] pca954x 3-0074: registered 4 multiplexed busses for I2C switch pca9546
[   87.134212] xilinx-demosaic a0010000.v_demosaic: Xilinx Video Demosaic Probe Successful
[   87.187345] xilinx-gamma-lut a0030000.v_gamma_lut: Xilinx 8-bit Video Gamma Correction LUT registered
[   87.189493] xilinx-vpss-csc a0040000.v_proc_ss_csc: VPSS CSC 8-bit Color Depth Probe Successful
[   87.189822] xilinx-vpss-scaler a0080000.v_proc_ss_scaler: Num Hori Taps 6
[   87.189829] xilinx-vpss-scaler a0080000.v_proc_ss_scaler: Num Vert Taps 6
[   87.189834] xilinx-vpss-scaler a0080000.v_proc_ss_scaler: VPSS Scaler Probe Successful
[   87.190358] xilinx-video axi:vcap_v_proc_ss_scaler: Entity type for entity a0080000.v_proc_ss_scaler was not initialized!
[   87.190373] xilinx-video axi:vcap_v_proc_ss_scaler: Entity type for entity a0040000.v_proc_ss_csc was not initialized!
[   87.190383] xilinx-video axi:vcap_v_proc_ss_scaler: Entity type for entity a0030000.v_gamma_lut was not initialized!
[   87.190391] xilinx-video axi:vcap_v_proc_ss_scaler: Entity type for entity a0010000.v_demosaic was not initialized!
[   87.190399] xilinx-video axi:vcap_v_proc_ss_scaler: Entity type for entity 80002000.mipi_csi2_rx_subsystem was not initialized!
[   87.190409] xilinx-video axi:vcap_v_proc_ss_scaler: device registered
```

Setup video4linux
------------------------------------------------------------------------------------

### Requirement

```console
shell$ sudo apt install -y yavta
shell$ sudo apt install -y v4l-utils
```

### Run tools/setup.sh

```console
shell$ sudo sh tools/setup.sh -s -hd -v
# mipi_csi2_rx: 
#   entry  : 80002000.mipi_csi2_rx_subsystem
#   device : /dev/v4l-subdev4
# v_demosaic: 
#   entry  : a0010000.v_demosaic
#   device : /dev/v4l-subdev3
# v_gamma_lut: 
#   entry  : a0030000.v_gamma_lut
#   device : /dev/v4l-subdev2
# v_proc_ss_csc: 
#   entry  : a0040000.v_proc_ss_csc
#   device : /dev/v4l-subdev1
# v_proc_ss_scaler: 
#   entry  : a0080000.v_proc_ss_scaler
#   device : /dev/v4l-subdev0
### SONY IMX219 Sensor ###
media-ctl -d /dev/media0 --set-v4l2 '"imx219 6-0010":0 [fmt:SRGGB10_1X10/1920x1080]'
### MIPI CSI2-Rx ###
media-ctl -d /dev/media0 --set-v4l2 '"80002000.mipi_csi2_rx_subsystem":0 [fmt:SRGGB10_1X10/1920x1080 field:none]'
media-ctl -d /dev/media0 --set-v4l2 '"80002000.mipi_csi2_rx_subsystem":1 [fmt:SRGGB10_1X10/1920x1080 field:none]'
### Demosaic IP subdev2 ###
media-ctl -d /dev/media0 --set-v4l2 '"a0010000.v_demosaic":0 [fmt:SRGGB10_1X10/1920x1080 field:none]'
media-ctl -d /dev/media0 --set-v4l2 '"a0010000.v_demosaic":1 [fmt:RBG888_1X24/1920x1080 field:none]'
### Gamma LUT IP ###
media-ctl -d /dev/media0 --set-v4l2 '"a0030000.v_gamma_lut":0 [fmt:RBG888_1X24/1920x1080 field:none]'
media-ctl -d /dev/media0 --set-v4l2 '"a0030000.v_gamma_lut":1 [fmt:RBG888_1X24/1920x1080 field:none]'
yavta --no-query -w '0x0098c9c1 10' /dev/v4l-subdev2
Device /dev/v4l-subdev2 opened.
Control 0x0098c9c1 set to 10, is 10
yavta --no-query -w '0x0098c9c2 10' /dev/v4l-subdev2
Device /dev/v4l-subdev2 opened.
Control 0x0098c9c2 set to 10, is 10
yavta --no-query -w '0x0098c9c3 10' /dev/v4l-subdev2
Device /dev/v4l-subdev2 opened.
Control 0x0098c9c3 set to 10, is 10
### VPSS: Color Space Conversion (CSC) Only ###
media-ctl -d /dev/media0 --set-v4l2 '"a0040000.v_proc_ss_csc":0 [fmt:RBG888_1X24/1920x1080 field:none]'
media-ctl -d /dev/media0 --set-v4l2 '"a0040000.v_proc_ss_csc":1 [fmt:RBG888_1X24/1920x1080 field:none]'
### VPSS: Scaler Only with CSC ###
media-ctl -d /dev/media0 --set-v4l2 '"a0080000.v_proc_ss_scaler":0 [fmt:RBG888_1X24/1920x1080 field:none]'
media-ctl -d /dev/media0 --set-v4l2 '"a0080000.v_proc_ss_scaler":1 [fmt:RBG888_1X24/1920x1080 field:none]'
yavta -w '0x0098c9a1 90' /dev/v4l-subdev0
Device /dev/v4l-subdev0 opened.
Control 0x0098c9a1 set to 90, is 90
Unable to get format: Inappropriate ioctl for device (25).
yavta -w '0x0098c9a2 50' /dev/v4l-subdev0
Device /dev/v4l-subdev0 opened.
Control 0x0098c9a2 set to 50, is 50
Unable to get format: Inappropriate ioctl for device (25).
yavta -w '0x0098c9a3 35' /dev/v4l-subdev0
Device /dev/v4l-subdev0 opened.
Control 0x0098c9a3 set to 35, is 35
Unable to get format: Inappropriate ioctl for device (25).
yavta -w '0x0098c9a4 24' /dev/v4l-subdev0
Device /dev/v4l-subdev0 opened.
Control 0x0098c9a4 set to 24, is 24
Unable to get format: Inappropriate ioctl for device (25).
yavta -w '0x0098c9a5 40' /dev/v4l-subdev0
Device /dev/v4l-subdev0 opened.
Control 0x0098c9a5 set to 40, is 40
Unable to get format: Inappropriate ioctl for device (25).
v4l2-ctl --set-ctrl=analogue_gain=120
v4l2-ctl --set-ctrl=digital_gain=400
```

```console
shell$ sudo media-ctl -p
Media controller API version 6.1.72

Media device information
------------------------
driver          xilinx-video
model           Xilinx Video Composite Device
serial          
bus info        platform:axi:vcap_v_proc_ss_sca
hw revision     0x0
driver version  6.1.72

Device topology
- entity 1: vcap_v_proc_ss_scaler output 0 (1 pad, 1 link)
            type Node subtype V4L flags 0
            device node name /dev/video0
	pad0: Sink
		<- "a0080000.v_proc_ss_scaler":1 [ENABLED]

- entity 5: a0080000.v_proc_ss_scaler (2 pads, 2 links)
            type V4L2 subdev subtype Unknown flags 0
            device node name /dev/v4l-subdev0
	pad0: Sink
		[fmt:RBG888_1X24/1920x1080 field:none]
		<- "a0040000.v_proc_ss_csc":1 [ENABLED]
	pad1: Source
		[fmt:RBG888_1X24/1920x1080 field:none]
		-> "vcap_v_proc_ss_scaler output 0":0 [ENABLED]

- entity 8: a0040000.v_proc_ss_csc (2 pads, 2 links)
            type V4L2 subdev subtype Unknown flags 0
            device node name /dev/v4l-subdev1
	pad0: Sink
		[fmt:RBG888_1X24/1920x1080 field:none]
		<- "a0030000.v_gamma_lut":1 [ENABLED]
	pad1: Source
		[fmt:RBG888_1X24/1920x1080 field:none]
		-> "a0080000.v_proc_ss_scaler":0 [ENABLED]

- entity 11: a0030000.v_gamma_lut (2 pads, 2 links)
             type V4L2 subdev subtype Unknown flags 0
             device node name /dev/v4l-subdev2
	pad0: Sink
		[fmt:RBG888_1X24/1920x1080 field:none]
		<- "a0010000.v_demosaic":1 [ENABLED]
	pad1: Source
		[fmt:RBG888_1X24/1920x1080 field:none]
		-> "a0040000.v_proc_ss_csc":0 [ENABLED]

- entity 14: a0010000.v_demosaic (2 pads, 2 links)
             type V4L2 subdev subtype Unknown flags 0
             device node name /dev/v4l-subdev3
	pad0: Sink
		[fmt:SRGGB10_1X10/1920x1080 field:none]
		<- "80002000.mipi_csi2_rx_subsystem":1 [ENABLED]
	pad1: Source
		[fmt:RBG888_1X24/1920x1080 field:none]
		-> "a0030000.v_gamma_lut":0 [ENABLED]

- entity 17: 80002000.mipi_csi2_rx_subsystem (2 pads, 2 links)
             type V4L2 subdev subtype Unknown flags 0
             device node name /dev/v4l-subdev4
	pad0: Sink
		[fmt:SRGGB10_1X10/1920x1080 field:none]
		<- "imx219 6-0010":0 [ENABLED]
	pad1: Source
		[fmt:SRGGB10_1X10/1920x1080 field:none]
		-> "a0010000.v_demosaic":0 [ENABLED]

- entity 20: imx219 6-0010 (1 pad, 1 link)
             type V4L2 subdev subtype Sensor flags 0
             device node name /dev/v4l-subdev5
	pad0: Source
		[fmt:SRGGB10_1X10/1920x1080 field:none colorspace:srgb xfer:srgb ycbcr:601 quantization:full-range
		 crop.bounds:(8,8)/3280x2464
		 crop:(688,700)/1920x1080]
		-> "80002000.mipi_csi2_rx_subsystem":0 [ENABLED]

```

```console
shell$ sudo v4l2-ctl -d /dev/video0 --list-formats
ioctl: VIDIOC_ENUM_FMT
	Type: Video Capture Multiplanar

	[0]: 'RX24' (32-bit XBGR 8-8-8-8)
	[1]: 'XR24' (32-bit BGRX 8-8-8-8)
	[2]: 'RGB3' (24-bit RGB 8-8-8)
	[3]: 'BGR3' (24-bit BGR 8-8-8)
```

Capture by camera
------------------------------------------------------------------------------------

### Run tools/capture_hd_jpeg.sh

```console
shell$ sudo sh tools/capture_hd_jpeg.sh
Pipeline is live and does not need PREROLL ...
Pipeline is PREROLLED ...
Setting pipeline to PLAYING ...
0:00:00.243999940  1128   0x556d891800 INFO                 v4l2src gstv4l2src.c:550:gst_v4l2src_query_preferred_size:<v4l2src0> Detect input 0 as `a0080000.v_proc_ss_scaler`
New clock: GstSystemClock
0:00:00.244247883  1128   0x556d891800 WARN                    v4l2 gstv4l2object.c:4524:gst_v4l2_object_probe_caps:<v4l2src0:src> Failed to probe pixel aspect ratio with VIDIOC_CROPCAP: Invalid argument
0:00:00.244786360  1128   0x556d891800 DEBUG                v4l2src gstv4l2src.c:601:gst_v4l2src_negotiate:<v4l2src0> caps of src: video/x-raw(format:Interlaced), format=(string)BGRx, framerate=(fraction)[ 0/1, 2147483647/1 ], width=(int)[ 1, 16383 ], height=(int)[ 1, 8191 ], interlace-mode=(string)alternate; video/x-raw, format=(string)BGRx, framerate=(fraction)[ 0/1, 2147483647/1 ], width=(int)[ 1, 16383 ], height=(int)[ 1, 8191 ], interlace-mode=(string){ progressive, interleaved }; video/x-raw(format:Interlaced), format=(string)BGRx, framerate=(fraction)[ 0/1, 2147483647/1 ], width=(int)[ 1, 16383 ], height=(int)[ 1, 8191 ], interlace-mode=(string)alternate; video/x-raw(format:Interlaced), format=(string)xBGR, framerate=(fraction)[ 0/1, 2147483647/1 ], width=(int)[ 1, 16383 ], height=(int)[ 1, 8191 ], interlace-mode=(string)alternate; video/x-raw, format=(string)xBGR, framerate=(fraction)[ 0/1, 2147483647/1 ], width=(int)[ 1, 16383 ], height=(int)[ 1, 8191 ], interlace-mode=(string){ progressive, interleaved }; video/x-raw(format:Interlaced), format=(string)xBGR, framerate=(fraction)[ 0/1, 2147483647/1 ], width=(int)[ 1, 16383 ], height=(int)[ 1, 8191 ], interlace-mode=(string)alternate; video/x-raw(format:Interlaced), format=(string)BGR, framerate=(fraction)[ 0/1, 2147483647/1 ], width=(int)[ 1, 21844 ], height=(int)[ 1, 8191 ], interlace-mode=(string)alternate; video/x-raw, format=(string)BGR, framerate=(fraction)[ 0/1, 2147483647/1 ], width=(int)[ 1, 21844 ], height=(int)[ 1, 8191 ], interlace-mode=(string){ progressive, interleaved }; video/x-raw(format:Interlaced), format=(string)BGR, framerate=(fraction)[ 0/1, 2147483647/1 ], width=(int)[ 1, 21844 ], height=(int)[ 1, 8191 ], interlace-mode=(string)alternate; video/x-raw(format:Interlaced), format=(string)RGB, framerate=(fraction)[ 0/1, 2147483647/1 ], width=(int)[ 1, 21844 ], height=(int)[ 1, 8191 ], interlace-mode=(string)alternate; video/x-raw, format=(string)RGB, framerate=(fraction)[ 0/1, 2147483647/1 ], width=(int)[ 1, 21844 ], height=(int)[ 1, 8191 ], interlace-mode=(string){ progressive, interleaved }; video/x-raw(format:Interlaced), format=(string)RGB, framerate=(fraction)[ 0/1, 2147483647/1 ], width=(int)[ 1, 21844 ], height=(int)[ 1, 8191 ], interlace-mode=(string)alternate
0:00:00.245138425  1128   0x556d891800 DEBUG                v4l2src gstv4l2src.c:609:gst_v4l2src_negotiate:<v4l2src0> caps of peer: video/x-raw, framerate=(fraction)5/1, width=(int)1920, height=(int)1080, format=(string){ I420, YV12, YUY2, UYVY, Y41B, Y42B, YVYU, Y444, NV21, NV12, RGB, BGR, RGBx, xRGB, BGRx, xBGR, GRAY8 }
0:00:00.245329157  1128   0x556d891800 DEBUG                v4l2src gstv4l2src.c:615:gst_v4l2src_negotiate:<v4l2src0> intersect: video/x-raw, framerate=(fraction)5/1, width=(int)1920, height=(int)1080, format=(string)BGRx, interlace-mode=(string){ progressive, interleaved }; video/x-raw, framerate=(fraction)5/1, width=(int)1920, height=(int)1080, format=(string)xBGR, interlace-mode=(string){ progressive, interleaved }; video/x-raw, framerate=(fraction)5/1, width=(int)1920, height=(int)1080, format=(string)BGR, interlace-mode=(string){ progressive, interleaved }; video/x-raw, framerate=(fraction)5/1, width=(int)1920, height=(int)1080, format=(string)RGB, interlace-mode=(string){ progressive, interleaved }
0:00:00.245509169  1128   0x556d891800 DEBUG                v4l2src gstv4l2src.c:406:gst_v4l2src_fixate:<v4l2src0> Fixating caps video/x-raw, framerate=(fraction)5/1, width=(int)1920, height=(int)1080, format=(string)BGRx, interlace-mode=(string){ progressive, interleaved }; video/x-raw, framerate=(fraction)5/1, width=(int)1920, height=(int)1080, format=(string)xBGR, interlace-mode=(string){ progressive, interleaved }; video/x-raw, framerate=(fraction)5/1, width=(int)1920, height=(int)1080, format=(string)BGR, interlace-mode=(string){ progressive, interleaved }; video/x-raw, framerate=(fraction)5/1, width=(int)1920, height=(int)1080, format=(string)RGB, interlace-mode=(string){ progressive, interleaved }
0:00:00.245609620  1128   0x556d891800 DEBUG                v4l2src gstv4l2src.c:407:gst_v4l2src_fixate:<v4l2src0> Preferred size 1920x1080
0:00:00.245833593  1128   0x556d891800 DEBUG                v4l2src gstv4l2src.c:430:gst_v4l2src_fixate:<v4l2src0> sorted and normalized caps video/x-raw, framerate=(fraction)5/1, width=(int)1920, height=(int)1080, format=(string)BGRx, interlace-mode=(string){ progressive, interleaved }; video/x-raw, framerate=(fraction)5/1, width=(int)1920, height=(int)1080, format=(string)xBGR, interlace-mode=(string){ progressive, interleaved }; video/x-raw, framerate=(fraction)5/1, width=(int)1920, height=(int)1080, format=(string)BGR, interlace-mode=(string){ progressive, interleaved }; video/x-raw, framerate=(fraction)5/1, width=(int)1920, height=(int)1080, format=(string)RGB, interlace-mode=(string){ progressive, interleaved }
0:00:00.246073516  1128   0x556d891800 DEBUG                v4l2src gstv4l2src.c:488:gst_v4l2src_fixate:<v4l2src0> fixated caps video/x-raw, framerate=(fraction)5/1, width=(int)1920, height=(int)1080, format=(string)BGRx, interlace-mode=(string)progressive, colorimetry=(string)sRGB
0:00:00.246174988  1128   0x556d891800 INFO                 v4l2src gstv4l2src.c:647:gst_v4l2src_negotiate:<v4l2src0> fixated to: video/x-raw, framerate=(fraction)5/1, width=(int)1920, height=(int)1080, format=(string)BGRx, interlace-mode=(string)progressive, colorimetry=(string)sRGB
/GstPipeline:pipeline0/GstV4l2Src:v4l2src0.GstPad:src: caps = video/x-raw, framerate=(fraction)5/1, width=(int)1920, height=(int)1080, format=(string)BGRx, interlace-mode=(string)progressive, colorimetry=(string)sRGB
/GstPipeline:pipeline0/GstCapsFilter:capsfilter0.GstPad:src: caps = video/x-raw, framerate=(fraction)5/1, width=(int)1920, height=(int)1080, format=(string)BGRx, interlace-mode=(string)progressive, colorimetry=(string)sRGB
/GstPipeline:pipeline0/GstJpegEnc:jpegenc0.GstPad:sink: caps = video/x-raw, framerate=(fraction)5/1, width=(int)1920, height=(int)1080, format=(string)BGRx, interlace-mode=(string)progressive, colorimetry=(string)sRGB
/GstPipeline:pipeline0/GstCapsFilter:capsfilter0.GstPad:sink: caps = video/x-raw, framerate=(fraction)5/1, width=(int)1920, height=(int)1080, format=(string)BGRx, interlace-mode=(string)progressive, colorimetry=(string)sRGB
0:00:00.359657346  1128   0x556d891800 DEBUG                v4l2src gstv4l2src.c:1049:gst_v4l2src_create:<v4l2src0> ts: 0:07:38.392682000 now 0:07:38.392761343 delay 0:00:00.000079343
/GstPipeline:pipeline0/GstJpegEnc:jpegenc0.GstPad:src: caps = image/jpeg, sof-marker=(int)4, width=(int)1920, height=(int)1080, pixel-aspect-ratio=(fraction)1/1, framerate=(fraction)5/1, interlace-mode=(string)progressive, colorimetry=(string)sRGB
/GstPipeline:pipeline0/GstFileSink:filesink0.GstPad:sink: caps = image/jpeg, sof-marker=(int)4, width=(int)1920, height=(int)1080, pixel-aspect-ratio=(fraction)1/1, framerate=(fraction)5/1, interlace-mode=(string)progressive, colorimetry=(string)sRGB
Redistribute latency...
0:00:01.149041280  1128   0x556d836550 DEBUG                v4l2src gstv4l2src.c:814:gst_v4l2src_query:<v4l2src0> report latency min 0:00:00.200000000 max 0:00:06.400000000
0:00:01.149081361  1128   0x556d891980 DEBUG                v4l2src gstv4l2src.c:814:gst_v4l2src_query:<v4l2src0> report latency min 0:00:00.200000000 max 0:00:06.400000000
Got EOS from element "pipeline0".
Execution ended after 0:00:00.909666788
Setting pipeline to NULL ...
Freeing pipeline ...
```

