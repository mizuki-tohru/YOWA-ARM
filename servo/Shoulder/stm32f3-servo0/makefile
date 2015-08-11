# Use USB?
# USB_SUPPORT		= USE_USB

TCHAIN  = /usr/cross/bin/arm-m4-eabi
TOOLDIR = /bin
REMOVAL = /bin/rm
WSHELL  = /bin/bash
MSGECHO = /bin/echo

# OPTIMIZE Definition
OPTIMIZE		= 2

# FPU Definition
USING_FPU		= -mfloat-abi=softfp  -mfpu=fpv4-sp-d16
#USING_FPU		= -mfloat-abi=soft

STM32F37X = STM32F37X

# Apprication Version
APP_VER = W.I.P

SUBMODEL		= STM32F37x

#MPU_DENSITY	= STM32F37x
MPU_DENSITY		= STM32_FLASH
HSE_CLOCK 		= 8000000
USE_CLOCK		= USE_HSE
#USE_CLOCK		= USE_HSI
PERIF_DRIVER	= USE_STDPERIPH_DRIVER

# Use FreeRTOS?
#OS_SUPPORT		= BARE_METAL
OS_SUPPORT		= USE_FREERTOS

FREERTOS_DIR = ./FreeRTOS
#FREERTOS_DIR = ../FreeRTOSV8.0.1/FreeRTOS
#FREERTOS_DIR = ../FreeRTOSV7.2.0/FreeRTOS
FREERTOS_COM = $(FREERTOS_DIR)/Demo/Common/include

# Synthesis makefile Defines
DEFZ =	\
	$(USE_PHY) $(PERIF_DRIVER) $(USE_FPU) $(SUBMODEL) \
	$(OS_SUPPORT) $(USE_CLOCK) $(USING_PRINTF) $(USE_STM324x7I_EVAL)

SYNTHESIS_DEFS	= $(addprefix -D,$(DEFZ))						\
		 -DPACK_STRUCT_END=__attribute\(\(packed\)\) 			\
		 -DALIGN_STRUCT_END=__attribute\(\(aligned\(4\)\)\) 	\
		 -DMPU_SUBMODEL=\"$(SUBMODEL)\"						\
		 -DAPP_VERSION=\"$(APP_VER)\"							\
		 -DHSE_VALUE=$(HSE_CLOCK)UL							\
		 -DUART_DEFAULT_NUM=$(UART_DEFAULT_NUM) 

# TARGET definition
TARGET 	    = main
TARGET_ELF  = $(TARGET).elf
TARGET_SREC = $(TARGET).s19
TARGET_HEX  = $(TARGET).hex
TARGET_BIN  = $(TARGET).bin
TARGET_LSS  = $(TARGET).lss
TARGET_SYM  = $(TARGET).sym

# define Cortex-M4 LIBRARY PATH
FWLIB  		= ../STM32F37x_DSP_StdPeriph_Lib_V1.0.0/Libraries/STM32F37x_StdPeriph_Driver
CM4LIB 		= ../STM32F37x_DSP_StdPeriph_Lib_V1.0.0/Libraries/CMSIS
CM4_DEVICE 	= $(CM4LIB)/Device/ST/STM32F37x
CM4_CORE	= $(CM4LIB)/Include
BOARDLIB    = ../STM32F37x_DSP_StdPeriph_Lib_V1.0.0/Utilities/STM32_EVAL/STM32373C_EVAL
BOARDCOMMON = ../STM32F37x_DSP_StdPeriph_Lib_V1.0.0/Utilities/STM32_EVAL
FATFS		= ../STM32F407xGT6_FatFS_DISP_20120710
FATFSLIB	= $(FATFS)/lib/ff

USB_FS_LIB_DIR = ../STM32_USB-FS-Device_Lib_V4.0.0/Libraries/STM32_USB-FS-Device_Driver

# include PATH

INCPATHS	= 	./					\
			./inc					\
			$(FWLIB)/inc  			\
			$(CM4_DEVICE)/Include	\
			$(CM4_CORE)			\
			$(BOARDLIB)			\
			$(BOARDCOMMON)			\
			$(BOARDCOMMON)/Common	\
			$(FREERTOS_DIR)	\
			$(FREERTOS_DIR)/Source/include	\
			$(FREERTOS_DIR)/Source/portable/GCC/ARM_CM4F \
			$(FREERTOS_COM)         \
			$(USB_FS_LIB_DIR)/inc	\
			$(LIBNKF)

#			$(FREERTOS_DIR)/Source/portable/GCC/ARM_CM4F \
#			$(FREERTOS_DIR)/Source/portable/GCC/ARM_CM3 \

INCLUDES     = $(addprefix -I ,$(INCPATHS))

# Set library PATH
LIBPATHS     = $(FWLIB) $(CM4LIB)

LIBRARY_DIRS = $(addprefix -L,$(LIBPATHS))
# if you use math-library, put "-lm" 
# MATH_LIB	 =	-lm

# LinkerScript PATH
#LINKER_PATH =  $(FATFS)/lib/linker
LINKER_PATH =  ./linker
LINKER_DIRS = $(addprefix -L,$(LINKER_PATH)) 

# Object definition
OBJS 	 = $(CFILES:%.c=%.o) 
LIBOBJS  = $(LIBCFILES:%.c=%.o) $(SFILES:%.s=%.o)

# C code PATH
SOURCE  = ./src
CFILES = \
 $(SOURCE)/usb_istr.c	\
 $(SOURCE)/usb_prop.c	\
 $(SOURCE)/usb_desc.c	\
 $(SOURCE)/usb_endp.c	\
 $(SOURCE)/usb_pwr.c	\
 $(SOURCE)/stm32_it.c	\
 $(SOURCE)/hw_config.c	\
 $(SOURCE)/systick.c	\
 $(SOURCE)/encoder.c	\
 $(SOURCE)/dcmotor.c	\
 $(SOURCE)/dcmotor_controller.c	\
 $(SOURCE)/task_sequence.c	\
 $(SOURCE)/armangle.c	\
 $(SOURCE)/quaternion.c	\
 $(SOURCE)/xprintf.c	\
 $(SOURCE)/uart_support.c	\
 $(SOURCE)/main.c					
 						

ifeq ($(USING_PRINTF),USE_PRINTF)
CFILES += \
 $(FATFS)/src/syscalls.c
endif


#/*----- STARTUP code PATH -----*/
STARTUP_DIR = $(CM4_DEVICE)/Source/Templates/gcc_ride7
ifeq ($(OS_SUPPORT),USE_FREERTOS)
SFILES += \
	$(SOURCE)/startup_stm32f37x_rtos.s
else
SFILES += \
	$(SOURCE)/startup_stm32f37x_rtos.s
#	$(STARTUP_DIR)/startup_stm32f37x.s
endif

#/*----- STM32 library PATH -----*/
LIBCFILES = \
 $(FWLIB)/src/stm32f37x_syscfg.c		\
 $(FWLIB)/src/stm32f37x_dma.c			\
 $(FWLIB)/src/stm32f37x_dac.c			\
 $(FWLIB)/src/stm32f37x_tim.c			\
 $(FWLIB)/src/stm32f37x_exti.c			\
 $(FWLIB)/src/stm32f37x_gpio.c			\
 $(FWLIB)/src/stm32f37x_i2c.c			\
 $(FWLIB)/src/stm32f37x_rcc.c			\
 $(FWLIB)/src/stm32f37x_spi.c			\
 $(FWLIB)/src/stm32f37x_usart.c 		\
 $(FWLIB)/src/stm32f37x_adc.c			\
 $(FWLIB)/src/stm32f37x_pwr.c			\
 $(FWLIB)/src/stm32f37x_misc.c			\
 $(FREERTOS_DIR)/Source/tasks.c 						\
 $(FREERTOS_DIR)/Source/list.c 						\
 $(FREERTOS_DIR)/Source/queue.c 						\
 $(FREERTOS_DIR)/Source/timers.c 						\
 $(FREERTOS_DIR)/Source/portable/GCC/ARM_CM4F/port.c	\
 $(FREERTOS_DIR)/Source/portable/MemMang/heap_2.c	\
 $(USB_FS_LIB_DIR)/src/usb_sil.c	\
 $(USB_FS_LIB_DIR)/src/usb_mem.c	\
 $(USB_FS_LIB_DIR)/src/usb_regs.c	\
 $(USB_FS_LIB_DIR)/src/usb_int.c	\
 $(USB_FS_LIB_DIR)/src/usb_core.c	\
 $(USB_FS_LIB_DIR)/src/usb_init.c	\
 ./src/system_stm32f37x.c

# $(FREERTOS_DIR)/Source/portable/GCC/ARM_CM4F/port.c	\
# $(FREERTOS_DIR)/Source/portable/GCC/ARM_CM3/port.c	\

# $(FWLIB)/src/misc.c					\
# $(FWLIB)/src/stm32f37x_sdio.c          \
# $(BOARDLIB)/stm32373c_eval.c           \
# $(BOARDLIB)/stm32373c_eval_lcd.c           \

# TOOLCHAIN SETTING
CC 			= $(TCHAIN)-gcc
CPP 		= $(TCHAIN)-g++
OBJCOPY 	= $(TCHAIN)-objcopy
OBJDUMP 	= $(TCHAIN)-objdump
SIZE 		= $(TCHAIN)-size
AR 			= $(TCHAIN)-ar
LD 			= $(TCHAIN)-gcc
NM 			= $(TCHAIN)-nm
REMOVE		= $(REMOVAL) -f
REMOVEDIR 	= $(REMOVAL) -rf

# C and ASM FLAGS
CFLAGS  = -MD -mcpu=cortex-m4 -march=armv7e-m -mtune=cortex-m4
#CFLAGS  = -MD -mcpu=cortex-m3
CFLAGS += -mthumb -mlittle-endian $(ALIGNED_ACCESS)
CFLAGS += -mapcs-frame -mno-sched-prolog $(USING_FPU)
CFLAGS += -std=gnu99
CFLAGS += -gdwarf-2 -O$(OPTIMIZE) $(USE_LTO)
CFLAGS += -fno-strict-aliasing -fsigned-char
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -fno-schedule-insns2
CFLAGS += --param max-inline-insns-single=1000
CFLAGS += -fno-common -fno-hosted
CFLAGS += -Wall
CFLAGS += -Wa,-adhlns=$(subst $(suffix $<),.lst,$<) 
CFLAGS += $(SYNTHESIS_DEFS)  

ifeq ($(USB_SUPPORT),USE_USB)
#usb_conf.h
CFLAGS+=-DUSE_USB_OTG_FS=1
endif

# Linker FLAGS -mfloat-abi=softfp -msoft-float
LDFLAGS  = -mcpu=cortex-m4 -march=armv7e-m -mthumb
#LDFLAGS  = -mcpu=cortex-m3 -march=armv7e-m-mthumb
LDFLAGS += -u g_pfnVectors -Wl,-static -Wl,--gc-sections -nostartfiles
LDFLAGS += -Wl,-Map=$(TARGET).map
LDFLAGS += $(LIBRARY_DIRS) $(LINKER_DIRS) $(MATH_LIB)

ifeq ($(USE_EXT_SRAM),DATA_IN_ExtSRAM)
LDFLAGS +=-T$(LINKER_PATH)/$(MPU_DENSITY)_EXTRAM.ld
else
LDFLAGS +=-T$(LINKER_PATH)/$(MPU_DENSITY).ld
endif

# Object Copy generation FLAGS
OBJCPFLAGS = -O
OBJDUMPFLAGS = -h -S -C
DFLAGS = -w

# Build Object
all: gccversion build sizeafter

# Object Size Infomations
ELFSIZE = $(SIZE) -A -x $(TARGET).elf
sizeafter:
	@$(MSGECHO) 
	@$(MSGECHO) Size After:
	$(SIZE) $(TARGET).elf
	@$(SIZE) -A -x $(TARGET).elf
	
# Display compiler version information.
gccversion : 
	@$(CC) --version
	@$(MSGECHO) 


build: $(TARGET_ELF) $(TARGET_LSS) $(TARGET_SYM) $(TARGET_HEX) $(TARGET_SREC) $(TARGET_BIN)

.SUFFIXES: .o .c .s   

$(TARGET_LSS): $(TARGET_ELF)
	@$(MSGECHO)
	@$(MSGECHO) Disassemble: $@
	$(OBJDUMP) $(OBJDUMPFLAGS) $< > $@ 
$(TARGET_SYM): $(TARGET_ELF)
	@$(MSGECHO)
	@$(MSGECHO) Symbol: $@
	$(NM) -n $< > $@
$(TARGET).hex: $(TARGET).elf
	@$(MSGECHO)
	@$(MSGECHO) Objcopy: $@
	$(OBJCOPY) $(OBJCPFLAGS) ihex $^ $@    
$(TARGET).s19: $(TARGET).elf
	@$(MSGECHO)
	@$(MSGECHO) Objcopy: $@
	$(OBJCOPY) $(OBJCPFLAGS) srec $^ $@ 
$(TARGET).bin: $(TARGET).elf
	@$(MSGECHO)
	@$(MSGECHO) Objcopy: $@
	$(OBJCOPY) $(OBJCPFLAGS) binary $< $@ 
$(TARGET).elf: $(OBJS) stm32.a
	@$(MSGECHO) Link: $@
	$(LD) $(CFLAGS) $(LDFLAGS) $^ -o $@
	@$(MSGECHO)

stm32.a: $(LIBOBJS)
	@$(MSGECHO) Archive: $@
	$(AR) cr $@ $(LIBOBJS)    
	@$(MSGECHO)
.c.o:
	@$(MSGECHO) Compile: $<
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@
	@$(MSGECHO)
.s.o:
	@$(MSGECHO) Assemble: $<
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@
	@$(MSGECHO)

# Drop files into dust-shoot
clean:
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).bin
	$(REMOVE) $(TARGET).obj
	$(REMOVE) stm32.a
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).map
	$(REMOVE) $(TARGET).s19
	$(REMOVE) $(TARGET).obj
	$(REMOVE) $(TARGET).a90
	$(REMOVE) $(TARGET).sym
	$(REMOVE) $(TARGET).lnk
	$(REMOVE) $(TARGET).lss
	$(REMOVE) $(wildcard *.stackdump)
	$(REMOVE) $(OBJS)
	$(REMOVE) $(LIBOBJS)
	$(REMOVE) $(CFILES:.c=.lst)
	$(REMOVE) $(CFILES:.c=.d)
	$(REMOVE) $(LIBCFILES:.c=.lst)
	$(REMOVE) $(LIBCFILES:.c=.d)
	$(REMOVE) $(SFILES:.s=.lst)
	$(REMOVE) $(wildcard $(CM4_DEVICE)/*.d)
	$(REMOVE) $(wildcard $(CM4_DEVICE)/*.lst)
	$(REMOVEDIR) .dep
	@$(MSGECHO)

