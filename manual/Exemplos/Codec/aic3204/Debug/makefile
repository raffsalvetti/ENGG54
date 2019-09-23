################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

EMPTY := 
SPACE := $(EMPTY) $(EMPTY)
CG_TOOL_ROOT := C:/Texas Instruments/ccsv4/tools/compiler/c5500

ORDERED_OBJS += \
$(GEN_CMDS_QUOTED) \
"./main.obj" \
-l"csl5502x.lib" \
-l"ezdsp5502bsl.lib" \
-l"rts55x.lib" \
"../lnkx.cmd" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: aic3204.out

dependents:
	-cd D:/CCS/ezdsp5502_v1/lib/Debug && $(MAKE) clean all

# Tool invocations
aic3204.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS) D:/CCS/ezdsp5502_v1/lib/Debug/../ezdsp5502bsl.lib
	@echo 'Building target: $@'
	@echo 'Invoking: Linker'
	"C:/Texas Instruments/ccsv4/tools/compiler/c5500/bin/cl55" -v5502 -g --diag_warning=225 --ptrdiff_size=32 --memory_model=large -z -m"aic3204.map" --warn_sections -i"C:/Texas Instruments/ccsv4/tools/compiler/c5500/lib" -i"C:/Texas Instruments/ccsv4/tools/compiler/c5500/include" -i"D:/CCS/ezdsp5502_v1/tests/aic3204/../../lib" -i"D:/CCS/ezdsp5502_v1/tests/aic3204/../../C55xxCSL/lib" --reread_libs --rom_model -o "aic3204.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C5500_EXECUTABLE_OUTPUTS__QTD) "aic3204.out"
	-$(RM) ".\main.obj" 
	-$(RM) ".\main.pp" 
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
D:/CCS/ezdsp5502_v1/lib/Debug/../ezdsp5502bsl.lib:

-include ../makefile.targets
