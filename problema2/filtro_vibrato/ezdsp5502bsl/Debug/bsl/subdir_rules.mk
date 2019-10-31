################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
bsl/%.obj: ../bsl/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C5500 Compiler'
	"/home/ros-user/ti/ccs920/ccs/tools/compiler/c5500_4.4.1/bin/cl55" --include_path="/home/ros-user/ti/ccs920/ccs/tools/compiler/c5500_4.4.1/include" --include_path="/home/ros-user/Downloads/ezdsp5502_v1/C55xxCSL/include" --include_path="/home/ros-user/Downloads/ezdsp5502_v1/include" --symdebug:dwarf --diag_warning=225 --asm_source=mnemonic --memory_model=large --ptrdiff_size=32 --silicon_version=5502 --preproc_with_compile --preproc_dependency="bsl/$(basename $(<F)).d_raw" --obj_directory="bsl" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

bsl/ezdsp5502_i2c.obj: ../bsl/ezdsp5502_i2c.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C5500 Compiler'
	"/home/ros-user/ti/ccs920/ccs/tools/compiler/c5500_4.4.1/bin/cl55" --include_path="/home/ros-user/ti/ccs920/ccs/tools/compiler/c5500_4.4.1/include" --include_path="/home/ros-user/Downloads/ezdsp5502_v1/C55xxCSL/include" --include_path="/home/ros-user/Downloads/ezdsp5502_v1/include" --symdebug:dwarf --diag_warning=225 --asm_source=mnemonic --memory_model=large --ptrdiff_size=32 --silicon_version=5502 --preproc_with_compile --preproc_dependency="bsl/$(basename $(<F)).d_raw" --obj_directory="bsl" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


