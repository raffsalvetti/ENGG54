################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
lnkx.out: ../lnkx.cmd $(GEN_CMDS)
	@echo 'Building file: $<'
	@echo 'Invoking: Linker'
	"C:/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -v5502 -g --diag_warning=225 --ptrdiff_size=32 --memory_model=large -z -m"aic3204.map" --warn_sections -i"C:/Texas Instruments/ccsv4/tools/compiler/c5500/lib" -i"C:/Texas Instruments/ccsv4/tools/compiler/c5500/include" -i"D:/CCS/ezdsp5502_v1/tests/aic3204/../../lib" -i"D:/CCS/ezdsp5502_v1/tests/aic3204/../../C55xxCSL/lib" --reread_libs --rom_model -o "$@" "$<" "../lnkx.cmd"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -v5502 -g --include_path="C:/Texas Instruments/ccsv4/tools/compiler/c5500/include" --include_path="D:/CCS/ezdsp5502_v1/tests/aic3204/../../C55xxCSL/include" --include_path="D:/CCS/ezdsp5502_v1/tests/aic3204/../../include" --diag_warning=225 --ptrdiff_size=32 --memory_model=large --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


