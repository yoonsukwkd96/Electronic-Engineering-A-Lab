onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /module_fifo_regs_no_flags_tb/r_RESET
add wave -noupdate /module_fifo_regs_no_flags_tb/r_CLOCK
add wave -noupdate /module_fifo_regs_no_flags_tb/r_WR_EN
add wave -noupdate /module_fifo_regs_no_flags_tb/r_WR_DATA
add wave -noupdate /module_fifo_regs_no_flags_tb/w_FULL
add wave -noupdate /module_fifo_regs_no_flags_tb/r_RD_EN
add wave -noupdate /module_fifo_regs_no_flags_tb/w_RD_DATA
add wave -noupdate /module_fifo_regs_no_flags_tb/w_EMPTY
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {3075 ps} 0}
quietly wave cursor active 1
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
WaveRestoreZoom {0 ps} {128 ns}
