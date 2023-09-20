# Fan Speed Enable     HDA20     "som240_1_c24"
set_property PACKAGE_PIN A12     [get_ports {fan_en_b}]
set_property IOSTANDARD LVCMOS33 [get_ports {fan_en_b}]
set_property SLEW SLOW           [get_ports {fan_en_b}]
set_property DRIVE 4             [get_ports {fan_en_b}]

# Raspi Camera Enable  HDA09     "som240_1_a15"
set_property PACKAGE_PIN F11     [get_ports {rpi_cam_en}]
set_property IOSTANDARD LVCMOS33 [get_ports {rpi_cam_en}]
set_property SLEW SLOW           [get_ports {rpi_cam_en}]
set_property DRIVE 4             [get_ports {rpi_cam_en}]

# Raspi Camera IIC-SCL HDA00_CC  "som240_1_d16"
set_property PACKAGE_PIN G11     [get_ports {som240_1_connector_hda_iic_switch_scl_io}]
set_property IOSTANDARD LVCMOS33 [get_ports {som240_1_connector_hda_iic_switch_scl_io}]
set_property SLEW SLOW           [get_ports {som240_1_connector_hda_iic_switch_scl_io}]
set_property DRIVE 4             [get_ports {som240_1_connector_hda_iic_switch_scl_io}]

# Raspi Camera IIC-SDA HDA01     "som240_1_d17"
set_property PACKAGE_PIN F10     [get_ports {som240_1_connector_hda_iic_switch_sda_io}]
set_property IOSTANDARD LVCMOS33 [get_ports {som240_1_connector_hda_iic_switch_sda_io}]
set_property SLEW SLOW           [get_ports {som240_1_connector_hda_iic_switch_sda_io}]
set_property DRIVE 4             [get_ports {som240_1_connector_hda_iic_switch_sda_io}]

# Raspi Camera MIPI HPA10_CC_P   "som240_1_c12"
set_property PACKAGE_PIN D7      [get_ports {som240_1_connector_mipi_csi_raspi_clk_p}]

# Raspi Camera MIPI HPA10_CC_N   "som240_1_c13"
set_property PACKAGE_PIN D6      [get_ports {som240_1_connector_mipi_csi_raspi_clk_n}]

# Raspi Camera MIPI HPA11_P      "som240_1_b10"
set_property PACKAGE_PIN E5      [get_ports {som240_1_connector_mipi_csi_raspi_data_p}]

# Raspi Camera MIPI HPA11_N      "som240_1_b11"
set_property PACKAGE_PIN D5      [get_ports {som240_1_connector_mipi_csi_raspi_data_n}]
