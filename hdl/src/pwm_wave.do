onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /tb_pwm/pwm_INST/cnt_width
add wave -noupdate /tb_pwm/pwm_INST/clk
add wave -noupdate /tb_pwm/pwm_INST/rst
add wave -noupdate -radix decimal /tb_pwm/pwm_INST/freq
add wave -noupdate -radix decimal /tb_pwm/pwm_INST/duty
add wave -noupdate /tb_pwm/pwm_INST/dead_time
add wave -noupdate /tb_pwm/pwm_INST/pwm_out
add wave -noupdate /tb_pwm/pwm_INST/pwm_out_n
add wave -noupdate -radix decimal /tb_pwm/pwm_INST/cnt
add wave -noupdate -radix decimal /tb_pwm/pwm_INST/cmp0
add wave -noupdate -radix decimal /tb_pwm/pwm_INST/cmp1
add wave -noupdate -radix decimal /tb_pwm/pwm_INST/cmp2
add wave -noupdate /tb_pwm/pwm_INST/slope
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
WaveRestoreZoom {0 ps} {1 ns}
