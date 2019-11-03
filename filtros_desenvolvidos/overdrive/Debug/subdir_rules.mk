################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C5500 Compiler'
	"/opt/ti/ccs910/ccs/tools/compiler/c5500_4.4.1/bin/cl55" -v5502 --memory_model=large -g --include_path="/home/raffaello/dev/workspace/UFBA/ENGG45/ccs_workspace/Filtro" --include_path="/home/raffaello/dev/workspace/UFBA/ENGG45/ezdsp5502_v1/C55xxCSL/include/" --include_path="/home/raffaello/dev/workspace/UFBA/ENGG45/ezdsp5502_v1/include" --include_path="/opt/ti/ccs910/ccs/tools/compiler/c5500_4.4.1/include" --define=c5502 --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


