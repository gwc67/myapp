board_runner_args(stm32cubeprogrammer "--port=swd" "--reset-mode=hw")   

include(${ZEPHYR_BASE}/boards/common/stm32cubeprogrammer.board.cmake)


# 删除 stlink_gdbserver 相关行，改为 OpenOCD
board_runner_args(openocd --cmd-pre-init "source [find interface/stlink.cfg]")
board_runner_args(openocd --cmd-pre-init "source [find target/stm32f4x.cfg]")
include(${ZEPHYR_BASE}/boards/common/openocd.board.cmake)
