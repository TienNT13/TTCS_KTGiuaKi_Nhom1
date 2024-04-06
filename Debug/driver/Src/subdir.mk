################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../driver/Src/Ucglib.c \
../driver/Src/stm32f401re_gpio.c \
../driver/Src/stm32f401re_rcc.c 

OBJS += \
./driver/Src/Ucglib.o \
./driver/Src/stm32f401re_gpio.o \
./driver/Src/stm32f401re_rcc.o 

C_DEPS += \
./driver/Src/Ucglib.d \
./driver/Src/stm32f401re_gpio.d \
./driver/Src/stm32f401re_rcc.d 


# Each subdirectory must supply rules for building sources it contributes
driver/Src/%.o: ../driver/Src/%.c driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DNUCLEO_F401RE -DSTM32 -DSTM32F401RETx -DSTM32F4 -c -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/Inc/CMSIS" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/Inc/driver/button" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/Inc/driver/buzzer" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/Inc/driver/led" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/Inc/driver/rtos" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/Inc/driver/sensor" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/Inc/driver/serial" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/Inc/driver/stmx" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/Inc/driver/ucglib" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

