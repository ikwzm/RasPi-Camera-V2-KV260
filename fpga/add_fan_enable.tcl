proc add_fan_enable { zynq_ultra_ps_e_0 port_name prefix} {

    if { $port_name eq "" } {
        set _port_name FAN_EN
    } else {
	set _port_name $port_name
    }

    if { $prefix eq "" } {
	set _prefix ttc0
    } else {
	set _prefix $prefix
    }
    
    set _cell_name     "${_prefix}_slice"
    set _net_name_din  "${_cell_name}_din"
    set _net_name_dout "${_cell_name}_dout"

    set_property -dict [list CONFIG.PSU__TTC0__WAVEOUT__ENABLE {1} ] $zynq_ultra_ps_e_0
    set_property -dict [list CONFIG.PSU__TTC0__WAVEOUT__IO {EMIO}  ] $zynq_ultra_ps_e_0

    set _slice_cell    [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 $_cell_name ]

    set_property -dict [ list CONFIG.DIN_FROM {2} CONFIG.DIN_TO {2} CONFIG.DIN_WIDTH {3} ] $_slice_cell

    create_bd_port -dir O -from 0 -to 0 $_port_name
    connect_bd_net -net $_net_name_dout [get_bd_pins $_slice_cell/Dout] [get_bd_ports $_port_name] 
    connect_bd_net -net $_net_name_din  [get_bd_pins $_slice_cell/Din ] [get_bd_pins $zynq_ultra_ps_e_0/emio_ttc0_wave_o]
}
