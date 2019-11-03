################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src/%.obj: ../src/%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C5500 Compiler'
	"/home/fe/ti/ccsv8/tools/compiler/c5500_4.4.1/bin/cl55" -v5502 --memory_model=large -g --include_path="/home/fe/workspace_v8/flange3" --include_path="/home/fe/workspace_v8/flange3/inc" --include_path="/home/fe/ti/ccsv8/tools/compiler/c5500_4.4.1/include" --define=c5502 --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="src/$(basename $(<F)).d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


