################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src/commutate.obj: C:/lgbeno/BLDC-Booster/firmware/src/commutate.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.1/bin/cl430" -vmsp --abi=eabi -g --include_path="C:/lgbeno/BLDC-Booster/firmware/include" --include_path="C:/lgbeno/BLDC-Booster/firmware/config" --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.1/include" --advice:power=all --define=__MSP430G2553__ --diag_warning=225 --display_error_number --diag_wrap=off --printf_support=minimal --preproc_with_compile --preproc_dependency="src/commutate.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/hall.obj: C:/lgbeno/BLDC-Booster/firmware/src/hall.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.1/bin/cl430" -vmsp --abi=eabi -g --include_path="C:/lgbeno/BLDC-Booster/firmware/include" --include_path="C:/lgbeno/BLDC-Booster/firmware/config" --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.1/include" --advice:power=all --define=__MSP430G2553__ --diag_warning=225 --display_error_number --diag_wrap=off --printf_support=minimal --preproc_with_compile --preproc_dependency="src/hall.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/pwm.obj: C:/lgbeno/BLDC-Booster/firmware/src/pwm.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.1/bin/cl430" -vmsp --abi=eabi -g --include_path="C:/lgbeno/BLDC-Booster/firmware/include" --include_path="C:/lgbeno/BLDC-Booster/firmware/config" --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.1/include" --advice:power=all --define=__MSP430G2553__ --diag_warning=225 --display_error_number --diag_wrap=off --printf_support=minimal --preproc_with_compile --preproc_dependency="src/pwm.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/serial.obj: C:/lgbeno/BLDC-Booster/firmware/src/serial.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.1/bin/cl430" -vmsp --abi=eabi -g --include_path="C:/lgbeno/BLDC-Booster/firmware/include" --include_path="C:/lgbeno/BLDC-Booster/firmware/config" --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.1/include" --advice:power=all --define=__MSP430G2553__ --diag_warning=225 --display_error_number --diag_wrap=off --printf_support=minimal --preproc_with_compile --preproc_dependency="src/serial.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/timer.obj: C:/lgbeno/BLDC-Booster/firmware/src/timer.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.1/bin/cl430" -vmsp --abi=eabi -g --include_path="C:/lgbeno/BLDC-Booster/firmware/include" --include_path="C:/lgbeno/BLDC-Booster/firmware/config" --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.1/include" --advice:power=all --define=__MSP430G2553__ --diag_warning=225 --display_error_number --diag_wrap=off --printf_support=minimal --preproc_with_compile --preproc_dependency="src/timer.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


