################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../driver/share/Inc/driver/Kalman_filter/kalman_filter.c 

OBJS += \
./driver/share/Inc/driver/Kalman_filter/kalman_filter.o 

C_DEPS += \
./driver/share/Inc/driver/Kalman_filter/kalman_filter.d 


# Each subdirectory must supply rules for building sources it contributes
driver/share/Inc/driver/Kalman_filter/%.o driver/share/Inc/driver/Kalman_filter/%.su driver/share/Inc/driver/Kalman_filter/%.cyclo: ../driver/share/Inc/driver/Kalman_filter/%.c driver/share/Inc/driver/Kalman_filter/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DNUCLEO_F401RE -DSTM32 -DSTM32F401RETx -DSTM32F4 -c -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/share/Inc/CMSIS" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/share/Inc/driver/Kalman_filter" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/share/Inc/driver/button" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/share/Inc/driver/buzzer" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/share/Inc/driver/led" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/share/Inc/driver/rtos" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/share/Inc/driver/sensor" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/share/Inc/driver/serial" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/share/Inc/driver/stmx" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/share/Inc/driver/ucglib" -I"D:/TTCS_test/KiemTraGiuaKy_ThucTapCoSo_Nhom1/driver/share/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-driver-2f-share-2f-Inc-2f-driver-2f-Kalman_filter

clean-driver-2f-share-2f-Inc-2f-driver-2f-Kalman_filter:
	-$(RM) ./driver/share/Inc/driver/Kalman_filter/kalman_filter.cyclo ./driver/share/Inc/driver/Kalman_filter/kalman_filter.d ./driver/share/Inc/driver/Kalman_filter/kalman_filter.o ./driver/share/Inc/driver/Kalman_filter/kalman_filter.su

.PHONY: clean-driver-2f-share-2f-Inc-2f-driver-2f-Kalman_filter

