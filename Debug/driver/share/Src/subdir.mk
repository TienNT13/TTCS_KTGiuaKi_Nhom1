################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../driver/share/Src/Ucglib.c \
../driver/share/Src/stm32f401re_gpio.c \
../driver/share/Src/stm32f401re_rcc.c 

OBJS += \
./driver/share/Src/Ucglib.o \
./driver/share/Src/stm32f401re_gpio.o \
./driver/share/Src/stm32f401re_rcc.o 

C_DEPS += \
./driver/share/Src/Ucglib.d \
./driver/share/Src/stm32f401re_gpio.d \
./driver/share/Src/stm32f401re_rcc.d 


# Each subdirectory must supply rules for building sources it contributes
driver/share/Src/%.o driver/share/Src/%.su driver/share/Src/%.cyclo: ../driver/share/Src/%.c driver/share/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DNUCLEO_F401RE -DSTM32 -DSTM32F401RETx -DSTM32F4 -c -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/share/Inc/CMSIS" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/share/Inc/driver/Kalman_filter" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/share/Inc/driver/button" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/share/Inc/driver/buzzer" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/share/Inc/driver/led" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/share/Inc/driver/rtos" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/share/Inc/driver/sensor" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/share/Inc/driver/serial" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/share/Inc/driver/stmx" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/share/Inc/driver/ucglib" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/share/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-driver-2f-share-2f-Src

clean-driver-2f-share-2f-Src:
	-$(RM) ./driver/share/Src/Ucglib.cyclo ./driver/share/Src/Ucglib.d ./driver/share/Src/Ucglib.o ./driver/share/Src/Ucglib.su ./driver/share/Src/stm32f401re_gpio.cyclo ./driver/share/Src/stm32f401re_gpio.d ./driver/share/Src/stm32f401re_gpio.o ./driver/share/Src/stm32f401re_gpio.su ./driver/share/Src/stm32f401re_rcc.cyclo ./driver/share/Src/stm32f401re_rcc.d ./driver/share/Src/stm32f401re_rcc.o ./driver/share/Src/stm32f401re_rcc.su

.PHONY: clean-driver-2f-share-2f-Src

