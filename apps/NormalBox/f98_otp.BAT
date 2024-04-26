SET C51INC=E:\Keil\C51\INC\Atmel\;E:\Keil\C51\INC\
SET C51LIB=E:\Keil\C51\LIB
SET CPU_TYPE=AT89F52
SET CPU_VENDOR=Atmel
SET UV2_TARGET=f98_otp
SET CPU_XTAL=0x016E3600
"E:\Keil\C51\BIN\AX51.EXE" @.\work\STARTU~2._IA
"E:\Keil\C51\BIN\C51.EXE" @.\work\adc.__i
"E:\Keil\C51\BIN\C51.EXE" @.\work\boost.__i
"E:\Keil\C51\BIN\C51.EXE" @.\work\burn.__i
"E:\Keil\C51\BIN\C51.EXE" @.\work\charge.__i
"E:\Keil\C51\BIN\C51.EXE" @.\work\io_wkup.__i
"E:\Keil\C51\BIN\C51.EXE" @.\work\pmu.__i
"E:\Keil\C51\BIN\C51.EXE" @.\work\pmu_wkup.__i
"E:\Keil\C51\BIN\C51.EXE" @.\work\power_hw.__i
"E:\Keil\C51\BIN\C51.EXE" @.\work\PULSE_~1.__I
"E:\Keil\C51\BIN\C51.EXE" @.\work\pwm_led.__i
"E:\Keil\C51\BIN\C51.EXE" @.\work\seg_led.__i
"E:\Keil\C51\BIN\C51.EXE" @.\work\uart.__i
"E:\Keil\C51\BIN\C51.EXE" @.\work\uart_key.__i
"E:\Keil\C51\BIN\C51.EXE" @.\work\VOUT_I~1.__I
"E:\Keil\C51\BIN\C51.EXE" @.\work\APP_CL~1.__I
"E:\Keil\C51\BIN\C51.EXE" @.\work\APP_CO~1.__I
"E:\Keil\C51\BIN\C51.EXE" @.\work\app_main.__i
"E:\Keil\C51\BIN\C51.EXE" @.\work\BOARD_~1.__I
"E:\Keil\C51\BIN\C51.EXE" @.\work\BOOST_~1.__I
"E:\Keil\C51\BIN\C51.EXE" @.\work\CHARGE~3.__I
"E:\Keil\C51\BIN\C51.EXE" @.\work\hall_det.__i
"E:\Keil\C51\BIN\C51.EXE" @.\work\key.__i
"E:\Keil\C51\BIN\C51.EXE" @.\work\ntc_det.__i
"E:\Keil\C51\BIN\C51.EXE" @.\work\POWER_~1.__I
"E:\Keil\C51\BIN\C51.EXE" @.\work\timer.__i
"E:\Keil\C51\BIN\C51.EXE" @.\work\usb_det.__i
"E:\Keil\C51\BIN\C51.EXE" @.\work\wireless.__i
"E:\Keil\C51\BIN\LX51.EXE" @.\work\f98_otp.lnp
"E:\Keil\UV4\KSPAWN.EXE" "E:\Keil\C51\BIN\C51.EXE" "@.\work\power_hw.__i"
"E:\Keil\UV4\KSPAWN.EXE" "E:\Keil\C51\BIN\C51.EXE" "@.\work\APP_CO~1.__I"
"E:\Keil\UV4\KSPAWN.EXE" "E:\Keil\C51\BIN\C51.EXE" "@.\work\app_main.__i"
"E:\Keil\UV4\KSPAWN.EXE" "E:\Keil\C51\BIN\C51.EXE" "@.\work\POWER_~1.__I"
"E:\Keil\UV4\KSPAWN.EXE" "E:\Keil\C51\BIN\LX51.EXE" "@.\work\f98_otp.lnp"
"E:\Keil\C51\BIN\OHX51.EXE" ".\work\f98_otp" HEX
"E:\Keil\UV4\KSPAWN.EXE" ./tools/load_otp.bat
