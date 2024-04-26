
copy ..\..\common\tools\burn_tool.exe .\tools\
copy ..\..\common\tools\fw_add.exe .\tools\
copy ..\..\common\tools\Hex.exe .\tools\
copy ..\..\common\tools\hextobin.exe .\tools\
copy ..\..\common\tools\isd_download.exe .\tools\
copy ..\..\common\tools\ufw_maker.exe .\tools\
copy ..\..\common\tools\otp.bin .\tools\
copy ..\..\common\tools\trim.bin .\tools\
copy ..\..\common\tools\isd_config.ini .\tools\

.\tools\Hex.exe -tohex -s 0x0000 -l 0x2FD0 -b 16 -f 0xFF -e .\work\f98_otp.hex .\work\f98_otp_memory.hex"
.\tools\hextobin.exe .\work\f98_otp_memory.hex .\tools\sdk_otp.bin .\work\otp_memory.ver

cd .\tools

download_otp.bat