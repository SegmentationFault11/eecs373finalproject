################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hal/CortexM3/GNU/hal_assert.c 

S_UPPER_SRCS += \
../hal/CortexM3/GNU/hal.S \
../hal/CortexM3/GNU/hw_reg_access.S 

OBJS += \
./hal/CortexM3/GNU/hal.o \
./hal/CortexM3/GNU/hal_assert.o \
./hal/CortexM3/GNU/hw_reg_access.o 

C_DEPS += \
./hal/CortexM3/GNU/hal_assert.d 


# Each subdirectory must supply rules for building sources it contributes
hal/CortexM3/GNU/%.o: ../hal/CortexM3/GNU/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Assembler'
	arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m3 -specs=bare.specs  -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

hal/CortexM3/GNU/%.o: ../hal/CortexM3/GNU/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU C Compiler'
	arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -DACTEL_STDIO_THRU_UART -IC:\Users\machong\Desktop\coolcar\coolcar\SoftConsole\coolcar_MSS_MSS_CM3_0\coolcar_MSS_MSS_CM3_0_hw_platform -IC:\Users\machong\Desktop\coolcar\coolcar\SoftConsole\coolcar_MSS_MSS_CM3_0\coolcar_MSS_MSS_CM3_0_hw_platform\CMSIS -IC:\Users\machong\Desktop\coolcar\coolcar\SoftConsole\coolcar_MSS_MSS_CM3_0\coolcar_MSS_MSS_CM3_0_hw_platform\CMSIS\startup_gcc -IC:\Users\machong\Desktop\coolcar\coolcar\SoftConsole\coolcar_MSS_MSS_CM3_0\coolcar_MSS_MSS_CM3_0_hw_platform\drivers -IC:\Users\machong\Desktop\coolcar\coolcar\SoftConsole\coolcar_MSS_MSS_CM3_0\coolcar_MSS_MSS_CM3_0_hw_platform\drivers\mss_ace -IC:\Users\machong\Desktop\coolcar\coolcar\SoftConsole\coolcar_MSS_MSS_CM3_0\coolcar_MSS_MSS_CM3_0_hw_platform\drivers\mss_gpio -IC:\Users\machong\Desktop\coolcar\coolcar\SoftConsole\coolcar_MSS_MSS_CM3_0\coolcar_MSS_MSS_CM3_0_hw_platform\drivers\mss_i2c -IC:\Users\machong\Desktop\coolcar\coolcar\SoftConsole\coolcar_MSS_MSS_CM3_0\coolcar_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm -IC:\Users\machong\Desktop\coolcar\coolcar\SoftConsole\coolcar_MSS_MSS_CM3_0\coolcar_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm\drivers -IC:\Users\machong\Desktop\coolcar\coolcar\SoftConsole\coolcar_MSS_MSS_CM3_0\coolcar_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm\drivers\F2DSS_NVM -IC:\Users\machong\Desktop\coolcar\coolcar\SoftConsole\coolcar_MSS_MSS_CM3_0\coolcar_MSS_MSS_CM3_0_hw_platform\drivers\mss_pdma -IC:\Users\machong\Desktop\coolcar\coolcar\SoftConsole\coolcar_MSS_MSS_CM3_0\coolcar_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc -IC:\Users\machong\Desktop\coolcar\coolcar\SoftConsole\coolcar_MSS_MSS_CM3_0\coolcar_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc\drivers -IC:\Users\machong\Desktop\coolcar\coolcar\SoftConsole\coolcar_MSS_MSS_CM3_0\coolcar_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc\drivers\mss_rtc -IC:\Users\machong\Desktop\coolcar\coolcar\SoftConsole\coolcar_MSS_MSS_CM3_0\coolcar_MSS_MSS_CM3_0_hw_platform\drivers\mss_spi -IC:\Users\machong\Desktop\coolcar\coolcar\SoftConsole\coolcar_MSS_MSS_CM3_0\coolcar_MSS_MSS_CM3_0_hw_platform\drivers\mss_spi\drivers -IC:\Users\machong\Desktop\coolcar\coolcar\SoftConsole\coolcar_MSS_MSS_CM3_0\coolcar_MSS_MSS_CM3_0_hw_platform\drivers\mss_spi\drivers\mss_spi -IC:\Users\machong\Desktop\coolcar\coolcar\SoftConsole\coolcar_MSS_MSS_CM3_0\coolcar_MSS_MSS_CM3_0_hw_platform\drivers\mss_timer -IC:\Users\machong\Desktop\coolcar\coolcar\SoftConsole\coolcar_MSS_MSS_CM3_0\coolcar_MSS_MSS_CM3_0_hw_platform\drivers\mss_uart -IC:\Users\machong\Desktop\coolcar\coolcar\SoftConsole\coolcar_MSS_MSS_CM3_0\coolcar_MSS_MSS_CM3_0_hw_platform\drivers_config -IC:\Users\machong\Desktop\coolcar\coolcar\SoftConsole\coolcar_MSS_MSS_CM3_0\coolcar_MSS_MSS_CM3_0_hw_platform\drivers_config\mss_ace -IC:\Users\machong\Desktop\coolcar\coolcar\SoftConsole\coolcar_MSS_MSS_CM3_0\coolcar_MSS_MSS_CM3_0_hw_platform\hal -IC:\Users\machong\Desktop\coolcar\coolcar\SoftConsole\coolcar_MSS_MSS_CM3_0\coolcar_MSS_MSS_CM3_0_hw_platform\hal\CortexM3 -IC:\Users\machong\Desktop\coolcar\coolcar\SoftConsole\coolcar_MSS_MSS_CM3_0\coolcar_MSS_MSS_CM3_0_hw_platform\hal\CortexM3\GNU -O1 -ffunction-sections -fdata-sections -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


