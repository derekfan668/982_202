@echo off

@echo *********************************************************************
@echo 			                AC98N FLASH SDK 
@echo *********************************************************************
@echo %date%
cd /d %~dp0


isd_download.exe -tonorflash -dev f98 -boot 0x0 -div8 -wait 300 -otp otp.bin -app sdk_flash.bin -trim trim.bin -output-fw output_flash.fw -format all 

