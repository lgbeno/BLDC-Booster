onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /tb_pwm_bldc/clk
add wave -noupdate /tb_pwm_bldc/rst
add wave -noupdate -radix hexadecimal /tb_pwm_bldc/freq
add wave -noupdate -radix hexadecimal /tb_pwm_bldc/duty
add wave -noupdate /tb_pwm_bldc/dead_time
add wave -noupdate /tb_pwm_bldc/pwm_out
add wave -noupdate /tb_pwm_bldc/pwm_out_n
add wave -noupdate -radix hexadecimal /tb_pwm_bldc/bldc_state
add wave -noupdate /tb_pwm_bldc/phase_a_h
add wave -noupdate /tb_pwm_bldc/phase_a_l
add wave -noupdate /tb_pwm_bldc/phase_b_h
add wave -noupdate /tb_pwm_bldc/phase_b_l
add wave -noupdate /tb_pwm_bldc/phase_c_h
add wave -noupdate /tb_pwm_bldc/phase_c_l
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {0 ps} 0}
quietly wave cursor active 0
configure wave -namecolwidth 150
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ps
update
WaveRestoreZoom {0 ps} {3923025710 ps}
