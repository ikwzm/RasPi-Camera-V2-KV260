#/bin/bash
__copyright__='Copyright (C) 2023 ikwzm'
__version__='0.3'
__license__='BSD-2-Clause'
__author__='ikwzm'
__author_email__='ichiro_k@ca2.so-net.ne.jp'
__url__='https://github.com/ikwzm/RasPi-Camera-V2-KV260'

set -e
script_name=$0
verbose=0
debug_level=0
dry_run=0
command_list=""
image_size="1920x1080"
sensor_format="SRGGB10_1X10"
target_format="RBG888_1X24"

#
# Video4Linux Devices
#
v4l2_media_device="/dev/media0"
v4l2_video_device="/dev/video0"

run_command()
{
    if [ $dry_run -ne 0 ] || [ $verbose -ne 0 ]; then
	echo "$1"
    fi
    if [ $dry_run -eq 0 ]; then
        eval "$1"
        if [ $? -ne 0 ]; then
	    if [ $verbose -eq 0 ]; then
                echo "$1"
            fi
	    exit 1
        fi
    fi
}    

get_v4l2_subdev()
{
    local entry=$(eval echo \"\$v4l2_entry_${1}\")
    local command="media-ctl -e '${entry}'"
    local subdev=$(eval "${command}")
    if [ ! -e "$subdev" ]; then
	echo $command  >&2
	echo $subdev   >&2
	exit 1
    fi
    echo $subdev
}

#
# IMX219 Subsystem
#
v4l2_entry_imx219="imx219 6-0010"
v4l2_device_imx219=$(get_v4l2_subdev "imx219")

#
# MIPI CSI2-Rx Subsystem
#
v4l2_entry_mipi_csi2_rx="80002000.mipi_csi2_rx_subsystem"
v4l2_device_mipi_csi2_rx=$(get_v4l2_subdev "mipi_csi2_rx")

#
# Demosaic IP
#
v4l2_entry_v_demosaic="a0010000.v_demosaic"
v4l2_device_v_demosaic=$(get_v4l2_subdev "v_demosaic")

#
# Gamma LUT IP
#
v4l2_entry_v_gamma_lut="a0030000.v_gamma_lut"
v4l2_device_v_gamma_lut=$(get_v4l2_subdev "v_gamma_lut")

#
# VPSS: Color Space Conversion (CSC) Only
#
v4l2_entry_v_proc_ss_csc="a0040000.v_proc_ss_csc"
v4l2_device_v_proc_ss_csc=$(get_v4l2_subdev "v_proc_ss_csc")

#
# VPSS: Scaler Only with CSC
#
v4l2_entry_v_proc_ss_scaler="a0080000.v_proc_ss_scaler"
v4l2_device_v_proc_ss_scaler=$(get_v4l2_subdev "v_proc_ss_scaler")


set_v4l2()
{
    local entry=$1
    local port=$2
    local value=$3
    run_command "media-ctl -d ${v4l2_media_device} --set-v4l2 '\"${entry}\":${port} [${value}]'"
}

setup_imx219()
{
    local image_size=$1
    local format=$2

    if [ $verbose -gt 0 ] || [ $debug_level -gt 0 ]; then
        echo "### SONY IMX219 Sensor ###"
    fi
    set_v4l2 "${v4l2_entry_imx219}" 0 "fmt:${format}/${image_size}"
}

setup_mipi_csi2_rx()
{
    local image_size=$1
    local format=$2
    if [ $verbose -gt 0 ] || [ $debug_level -gt 0 ]; then
        echo "### MIPI CSI2-Rx ###"
    fi
    set_v4l2 "${v4l2_entry_mipi_csi2_rx}" 0 "fmt:${format}/${image_size} field:none"
    set_v4l2 "${v4l2_entry_mipi_csi2_rx}" 1 "fmt:${format}/${image_size} field:none"
}

setup_demosaic()
{
    local image_size=$1
    local src_format=$2
    local dst_format=$3
    if [ $verbose -gt 0 ] || [ $debug_level -gt 0 ]; then
        echo "### Demosaic IP subdev2 ###"
    fi
    set_v4l2 "${v4l2_entry_v_demosaic}" "0" "fmt:${src_format}/${image_size} field:none"
    set_v4l2 "${v4l2_entry_v_demosaic}" "1" "fmt:${dst_format}/${image_size} field:none"
}

setup_gamma_lut()
{
    local image_size=$1
    local format=$2
    if [ $verbose -gt 0 ] || [ $debug_level -gt 0 ]; then
        echo "### Gamma LUT IP ###"
    fi
    set_v4l2 "${v4l2_entry_v_gamma_lut}" 0 "fmt:${format}/${image_size} field:none"
    set_v4l2 "${v4l2_entry_v_gamma_lut}" 1 "fmt:${format}/${image_size} field:none"
    # Red   gain min 1 max 40 step 1 default 10 current 40
    run_command "yavta --no-query -w '0x0098c9c1 10' ${v4l2_device_v_gamma_lut}"
    # Blue  gain min 1 max 40 step 1 default 10 current 40
    run_command "yavta --no-query -w '0x0098c9c2 10' ${v4l2_device_v_gamma_lut}"
    # Green gain min 1 max 40 step 1 default 10 current 40
    run_command "yavta --no-query -w '0x0098c9c3 10' ${v4l2_device_v_gamma_lut}"
}

setup_proc_ss_csc()
{
    local image_size=$1
    local format=$2
    if [ $verbose -gt 0 ] || [ $debug_level -gt 0 ]; then
        echo "### VPSS: Color Space Conversion (CSC) Only ###"
    fi
    set_v4l2 "${v4l2_entry_v_proc_ss_csc}" 0 "fmt:${format}/${image_size} field:none"
    set_v4l2 "${v4l2_entry_v_proc_ss_csc}" 1 "fmt:${format}/${image_size} field:none"
}    

setup_proc_ss_scaler()
{
    local image_size=$1
    local format=$2
    if [ $verbose -gt 0 ] || [ $debug_level -gt 0 ]; then
        echo "### VPSS: Scaler Only with CSC ###"
    fi
    set_v4l2 "${v4l2_entry_v_proc_ss_scaler}" 0 "fmt:${format}/${image_size} field:none"
    set_v4l2 "${v4l2_entry_v_proc_ss_scaler}" 1 "fmt:${format}/${image_size} field:none"
    # CSC Brightness' min 0 max 100 step 1 default 50 current 80
    run_command "yavta -w '0x0098c9a1 90' ${v4l2_device_v_proc_ss_scaler}"
    # CSC Contrast'   min 0 max 100 step 1 default 50 current 55
    run_command "yavta -w '0x0098c9a2 50' ${v4l2_device_v_proc_ss_scaler}"
    # CSC Red Gain'   min 0 max 100 step 1 default 50 current 35 
    run_command "yavta -w '0x0098c9a3 35' ${v4l2_device_v_proc_ss_scaler}"
    # CSC Green Gain' min 0 max 100 step 1 default 50 current 24 
    run_command "yavta -w '0x0098c9a4 24' ${v4l2_device_v_proc_ss_scaler}"
    # CSC Blue Gain'  min 0 max 100 step 1 default 50 current 40 
    run_command "yavta -w '0x0098c9a5 40' ${v4l2_device_v_proc_ss_scaler}"
}

do_setup()
{
    local image_size=$1
    local target_format=$2
    
    setup_imx219         ${image_size} ${sensor_format}
    setup_mipi_csi2_rx   ${image_size} ${sensor_format}
    setup_demosaic       ${image_size} ${sensor_format} ${target_format}
    setup_gamma_lut      ${image_size} ${target_format}
    setup_proc_ss_csc    ${image_size} ${target_format}
    setup_proc_ss_scaler ${image_size} ${target_format}

    ## set sensor gain ?
    run_command "v4l2-ctl --set-ctrl=analogue_gain=120"
    run_command "v4l2-ctl --set-ctrl=digital_gain=400"
}

do_show_pipeline()
{
    echo "### show pipeline ###"
    run_command "media-ctl -p"
    run_command "v4l2-ctl -d ${v4l2_video_device} --list-formats"
}

do_show_device()
{
    echo "# mipi_csi2_rx: "
    echo "#   entry  : $v4l2_entry_mipi_csi2_rx"
    echo "#   device : $v4l2_device_mipi_csi2_rx"
    echo "# v_demosaic: "
    echo "#   entry  : $v4l2_entry_v_demosaic"
    echo "#   device : $v4l2_device_v_demosaic"
    echo "# v_gamma_lut: "
    echo "#   entry  : $v4l2_entry_v_gamma_lut"
    echo "#   device : $v4l2_device_v_gamma_lut"
    echo "# v_proc_ss_csc: "
    echo "#   entry  : $v4l2_entry_v_proc_ss_csc"
    echo "#   device : $v4l2_device_v_proc_ss_csc"
    echo "# v_proc_ss_scaler: "
    echo "#   entry  : $v4l2_entry_v_proc_ss_scaler"
    echo "#   device : $v4l2_device_v_proc_ss_scaler"
}

do_print()
{
    do_show_pipeline
}

while [ $# -gt 0 ]; do
    case "$1" in
	-v|--verbose)
	    verbose=1
	    shift
	    ;;
	-d|--debug)
	    debug_level=1
	    shift
	    ;;
	-n|--dry-run)
	    dry_run=1
	    shift
	    ;;
	-h|--help)
	    command_list="$command_list help"
	    shift
	    ;;
	-s|--setup)
	    command_list="$command_list setup"
	    shift
	    ;;
	-p|--print)
	    command_list="$command_list print"
	    shift
	    ;;
	--vga)
	    image_size="640x480"
	    shift
	    ;;
	--hd)
	    image_size="1920x1080"
	    shift
	    ;;
	*)
	    shift
	    ;;
    esac
done

if [ -z "${command_list}" ]; then
    command_list="help"
fi

if [ $verbose -gt 0 ] || [ $debug_level -gt 0 ]; then
    do_show_device
fi

set $command_list
while [ $# -gt 0 ]; do
    case "$1" in
	"help"   )
	    do_help
	    shift
	    ;;
	"setup")
            do_setup ${image_size} ${target_format}
	    shift
	    ;;
	"print")
            do_print
	    shift
	    ;;
    esac
done

