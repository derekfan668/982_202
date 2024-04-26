@echo off

@echo *********************************************************************
@echo 			                AC98N OTP SDK 
@echo *********************************************************************
@echo %date%
cd /d %~dp0

isd_download.exe -dev f98 -otp sdk_otp.bin -output-fw output_otp.fw -package-only

