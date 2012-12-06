vlib pwm_work
vcom "pwm.vhd"
vcom "pwm_tb.vhd"
vsim work.tb_pwm
do pwm_wave.do
run 1ms