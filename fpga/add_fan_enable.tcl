proc add_fan_enable { zynq_ultra_ps_e_0 port_name prefix} {

    variable slice_cell
    variable cell_name
    variable net_name_din
    variable net_name_dout

    if { $port_name eq "" } {
        set port_name FAN_EN
    }

    if { $prefix eq "" } {
	set prefix ttc0
    }
    
    append cell_name     $prefix    "_slice"
    append net_name_din  $cell_name "_din"
    append net_name_dout $cell_name "_dout"

    set_property -dict [list CONFIG.PSU__TTC0__WAVEOUT__ENABLE {1} ] $zynq_ultra_ps_e_0
    set_property -dict [list CONFIG.PSU__TTC0__WAVEOUT__IO {EMIO}  ] $zynq_ultra_ps_e_0

    set slice_cell  [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 $cell_name ]

    set_property -dict [ list CONFIG.DIN_FROM {2} CONFIG.DIN_TO {2} CONFIG.DIN_WIDTH {3} ] $slice_cell

    create_bd_port -dir O -from 0 -to 0 $port_name
    connect_bd_net -net $net_name_dout [get_bd_pins $slice_cell/Dout] [get_bd_ports $port_name] 
    connect_bd_net -net $net_name_din  [get_bd_pins $slice_cell/Din ] [get_bd_pins $zynq_ultra_ps_e_0/emio_ttc0_wave_o]
}
