vlib pwm_work
vcom "pwm.vhd"
vcom "bldc_mux.vhd"
vcom "pwm_bldc_tb.vhd"
vsim work.tb_pwm_bldc
do pwm_bldc_wave.do
run 10ms