################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
..\r_usb_basic/src/driver/comm/r_usb_cdataio.c \
..\r_usb_basic/src/driver/comm/r_usb_cintfifo.c \
..\r_usb_basic/src/driver/comm/r_usb_cinthandler_usbip0.c \
..\r_usb_basic/src/driver/comm/r_usb_cinthandler_usbip1.c \
..\r_usb_basic/src/driver/comm/r_usb_clibusbip.c \
..\r_usb_basic/src/driver/comm/r_usb_cscheduler.c \
..\r_usb_basic/src/driver/comm/r_usb_cstdapi.c \
..\r_usb_basic/src/driver/comm/r_usb_cstdfunction.c 

C_DEPS += \
./r_usb_basic/src/driver/comm/r_usb_cdataio.d \
./r_usb_basic/src/driver/comm/r_usb_cintfifo.d \
./r_usb_basic/src/driver/comm/r_usb_cinthandler_usbip0.d \
./r_usb_basic/src/driver/comm/r_usb_cinthandler_usbip1.d \
./r_usb_basic/src/driver/comm/r_usb_clibusbip.d \
./r_usb_basic/src/driver/comm/r_usb_cscheduler.d \
./r_usb_basic/src/driver/comm/r_usb_cstdapi.d \
./r_usb_basic/src/driver/comm/r_usb_cstdfunction.d 

OBJS += \
./r_usb_basic/src/driver/comm/r_usb_cdataio.obj \
./r_usb_basic/src/driver/comm/r_usb_cintfifo.obj \
./r_usb_basic/src/driver/comm/r_usb_cinthandler_usbip0.obj \
./r_usb_basic/src/driver/comm/r_usb_cinthandler_usbip1.obj \
./r_usb_basic/src/driver/comm/r_usb_clibusbip.obj \
./r_usb_basic/src/driver/comm/r_usb_cscheduler.obj \
./r_usb_basic/src/driver/comm/r_usb_cstdapi.obj \
./r_usb_basic/src/driver/comm/r_usb_cstdfunction.obj 


# Each subdirectory must supply rules for building sources it contributes
r_usb_basic/src/driver/comm/%.obj: ../r_usb_basic/src/driver/comm/%.c r_usb_basic/src/driver/comm/c.sub
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx  -MM -MP -output=dep="$(@:%.obj=%.d)" -MT="$(@:%.obj=%.obj)" -MT="$(@:%.obj=%.d)" -lang=c99   -include="C:\PROGRA~2\Renesas\RX\2_3_0/include","C:\Workspace\e2studio\git\MT01\r_lvd_rx","C:\Workspace\e2studio\git\MT01\r_lvd_rx\src","C:/Workspace/e2studio/git/MT01/r_tfat_driver_rx/src","C:/Workspace/e2studio/git/MT01/r_tfat_driver_rx","C:/Workspace/e2studio/git/MT01/r_usb_basic","C:\Workspace\e2studio\git\MT01\fsystem","C:/Workspace/e2studio/git/MT01/r_usb_basic/src/HW/inc","C:/Workspace/e2studio/git/MT01/r_usb_basic/src/driver/inc","C:/Workspace/e2studio/git/MT01/r_usb_hmsc/src/inc","C:/Workspace/e2studio/git/MT01/r_usb_hmsc","C:/Workspace/e2studio/git/MT01/r_bsp","C:/Workspace/e2studio/git/MT01/r_dtc_rx","C:/Workspace/e2studio/git/MT01/r_config","C:/Workspace/e2studio/git/MT01/r_dtc_rx/src","C:/Workspace/e2studio/git/MT01/r_dtc_rx/src/targets/rx63n","C:/Workspace/e2studio/git/MT01/r_bsp/board/rdkrx63n","C:/Workspace/e2studio/git/MT01/r_bsp/mcu/rx63n/register_access","C:/Workspace/e2studio/git/MT01/r_bsp/mcu/rx63n","C:\Workspace\e2studio\git\MT01\r_cmt_rx","C:\Workspace\e2studio\git\MT01\r_cmt_rx\src","C:/Workspace/e2studio/git/MT01/FreeRTOS/Source/include","C:/Workspace/e2studio/git/MT01/FreeRTOS/Source/portable/Renesas/RX600","C:/Workspace/e2studio/git/MT01/src/include","C:\Workspace\e2studio\git\MT01\src\cnc","C:\Workspace\e2studio\git\MT01\r_config","C:\Workspace\e2studio\git\MT01\r_byteq","C:\Workspace\e2studio\git\MT01\r_byteq\src","C:\Workspace\e2studio\git\MT01\r_rspi_rx","C:\Workspace\e2studio\git\MT01\r_rspi_rx\src","C:\Workspace\e2studio\git\MT01\r_mtu_rx","C:\Workspace\e2studio\git\MT01\r_mtu_rx\src","C:\Workspace\e2studio\git\MT01\r_tmr_rx","C:\Workspace\e2studio\git\MT01\r_tmr_rx\src","C:\Workspace\e2studio\git\MT01\r_flash_api_rx","C:\Workspace\e2studio\git\MT01\r_flash_api_rx\src","C:\Workspace\e2studio\git\MT01\r_vee","C:\Workspace\e2studio\git\MT01\r_vee\src","C:\Workspace\e2studio\git\MT01\src\cnc\macros","C:\Workspace\e2studio\git\MT01\r_s12ad_rx","C:\Workspace\e2studio\git\MT01\r_s12ad_rx\src","C:\Workspace\e2studio\git\MT01\r_spi_flash","C:\Workspace\e2studio\git\MT01\r_spi_flash\src","C:\Workspace\e2studio\git\MT01\r_flash_loader_rx","C:\Workspace\e2studio\git\MT01\r_flash_loader_rx\src","C:\Workspace\e2studio\git\MT01\r_crc_rx","C:\Workspace\e2studio\git\MT01\r_crc_rx\src","C:\Workspace\e2studio\git\MT01\fsystem_spi","C:\Workspace\e2studio\git\MT01\spiffs"  -debug -show=source,conditionals,definitions,expansions -listfile="$(basename $(notdir $<)).lst" -isa=rxv1 -optimize=0 -fpu -alias=noansi -nologo -nomessage  -define=__RX,COMPACTAXP=0,EASYMAK=0,MOBILE=1,FREE_RTOS_PP   "$<"
	ccrx -lang=c99 -output=obj="$(@:%.d=%.obj)"  -include="C:\PROGRA~2\Renesas\RX\2_3_0/include","C:\Workspace\e2studio\git\MT01\r_lvd_rx","C:\Workspace\e2studio\git\MT01\r_lvd_rx\src","C:/Workspace/e2studio/git/MT01/r_tfat_driver_rx/src","C:/Workspace/e2studio/git/MT01/r_tfat_driver_rx","C:/Workspace/e2studio/git/MT01/r_usb_basic","C:\Workspace\e2studio\git\MT01\fsystem","C:/Workspace/e2studio/git/MT01/r_usb_basic/src/HW/inc","C:/Workspace/e2studio/git/MT01/r_usb_basic/src/driver/inc","C:/Workspace/e2studio/git/MT01/r_usb_hmsc/src/inc","C:/Workspace/e2studio/git/MT01/r_usb_hmsc","C:/Workspace/e2studio/git/MT01/r_bsp","C:/Workspace/e2studio/git/MT01/r_dtc_rx","C:/Workspace/e2studio/git/MT01/r_config","C:/Workspace/e2studio/git/MT01/r_dtc_rx/src","C:/Workspace/e2studio/git/MT01/r_dtc_rx/src/targets/rx63n","C:/Workspace/e2studio/git/MT01/r_bsp/board/rdkrx63n","C:/Workspace/e2studio/git/MT01/r_bsp/mcu/rx63n/register_access","C:/Workspace/e2studio/git/MT01/r_bsp/mcu/rx63n","C:\Workspace\e2studio\git\MT01\r_cmt_rx","C:\Workspace\e2studio\git\MT01\r_cmt_rx\src","C:/Workspace/e2studio/git/MT01/FreeRTOS/Source/include","C:/Workspace/e2studio/git/MT01/FreeRTOS/Source/portable/Renesas/RX600","C:/Workspace/e2studio/git/MT01/src/include","C:\Workspace\e2studio\git\MT01\src\cnc","C:\Workspace\e2studio\git\MT01\r_config","C:\Workspace\e2studio\git\MT01\r_byteq","C:\Workspace\e2studio\git\MT01\r_byteq\src","C:\Workspace\e2studio\git\MT01\r_rspi_rx","C:\Workspace\e2studio\git\MT01\r_rspi_rx\src","C:\Workspace\e2studio\git\MT01\r_mtu_rx","C:\Workspace\e2studio\git\MT01\r_mtu_rx\src","C:\Workspace\e2studio\git\MT01\r_tmr_rx","C:\Workspace\e2studio\git\MT01\r_tmr_rx\src","C:\Workspace\e2studio\git\MT01\r_flash_api_rx","C:\Workspace\e2studio\git\MT01\r_flash_api_rx\src","C:\Workspace\e2studio\git\MT01\r_vee","C:\Workspace\e2studio\git\MT01\r_vee\src","C:\Workspace\e2studio\git\MT01\src\cnc\macros","C:\Workspace\e2studio\git\MT01\r_s12ad_rx","C:\Workspace\e2studio\git\MT01\r_s12ad_rx\src","C:\Workspace\e2studio\git\MT01\r_spi_flash","C:\Workspace\e2studio\git\MT01\r_spi_flash\src","C:\Workspace\e2studio\git\MT01\r_flash_loader_rx","C:\Workspace\e2studio\git\MT01\r_flash_loader_rx\src","C:\Workspace\e2studio\git\MT01\r_crc_rx","C:\Workspace\e2studio\git\MT01\r_crc_rx\src","C:\Workspace\e2studio\git\MT01\fsystem_spi","C:\Workspace\e2studio\git\MT01\spiffs"  -debug -show=source,conditionals,definitions,expansions -listfile="$(basename $(notdir $<)).lst" -isa=rxv1 -optimize=0 -fpu -alias=noansi -nologo -nomessage  -define=__RX,COMPACTAXP=0,EASYMAK=0,MOBILE=1,FREE_RTOS_PP   "$<"
	@echo 'Finished scanning and building: $<'
	@echo.
