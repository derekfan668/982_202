
copy ..\..\common\tools\burn_tool.exe .\tools\
copy ..\..\common\tools\fw_add.exe .\tools\
copy ..\..\common\tools\Hex.exe .\tools\
copy ..\..\common\tools\hextobin.exe .\tools\
copy ..\..\common\tools\isd_download.exe .\tools\
copy ..\..\common\tools\ufw_maker.exe .\tools\
copy ..\..\common\tools\otp.bin .\tools\
copy ..\..\common\tools\trim.bin .\tools\
copy ..\..\common\tools\isd_config.ini .\tools\

.\tools\Hex.exe -tohex -s 0x4000 -l 0x4000 -b 8 -f 0xFF -e .\work\f98_flash.hex .\work\f98_flash_memory.hex"
.\tools\hextobin.exe .\work\f98_flash_memory.hex  .\tools\sdk_flash.bin  .\work\flash_memory.ver

cd .\tools

download_flash.bat

