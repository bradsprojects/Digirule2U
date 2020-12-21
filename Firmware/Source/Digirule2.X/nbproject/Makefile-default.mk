#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Digirule2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Digirule2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=config.c main.c cpu.c ui.c file.c misc.c comm.c monitor.c powerup.as

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/config.p1 ${OBJECTDIR}/main.p1 ${OBJECTDIR}/cpu.p1 ${OBJECTDIR}/ui.p1 ${OBJECTDIR}/file.p1 ${OBJECTDIR}/misc.p1 ${OBJECTDIR}/comm.p1 ${OBJECTDIR}/monitor.p1 ${OBJECTDIR}/powerup.o
POSSIBLE_DEPFILES=${OBJECTDIR}/config.p1.d ${OBJECTDIR}/main.p1.d ${OBJECTDIR}/cpu.p1.d ${OBJECTDIR}/ui.p1.d ${OBJECTDIR}/file.p1.d ${OBJECTDIR}/misc.p1.d ${OBJECTDIR}/comm.p1.d ${OBJECTDIR}/monitor.p1.d ${OBJECTDIR}/powerup.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/config.p1 ${OBJECTDIR}/main.p1 ${OBJECTDIR}/cpu.p1 ${OBJECTDIR}/ui.p1 ${OBJECTDIR}/file.p1 ${OBJECTDIR}/misc.p1 ${OBJECTDIR}/comm.p1 ${OBJECTDIR}/monitor.p1 ${OBJECTDIR}/powerup.o

# Source Files
SOURCEFILES=config.c main.c cpu.c ui.c file.c misc.c comm.c monitor.c powerup.as



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/Digirule2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F46K20
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/config.p1: config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/config.p1.d 
	@${RM} ${OBJECTDIR}/config.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O2 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file -Wl,-AINIT=0800h-0803h -Wl,-Pinit=INIT -Wl,-AISR=0808h-08FFh -Wl,-Pisr_text=ISR -Wl,-AFILES=F800h-FFFFh -Wl,-Pfiles=FILES  -ginhx032 -Wl,--data-init -mkeep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/config.p1 config.c 
	@-${MV} ${OBJECTDIR}/config.d ${OBJECTDIR}/config.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/config.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O2 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file -Wl,-AINIT=0800h-0803h -Wl,-Pinit=INIT -Wl,-AISR=0808h-08FFh -Wl,-Pisr_text=ISR -Wl,-AFILES=F800h-FFFFh -Wl,-Pfiles=FILES  -ginhx032 -Wl,--data-init -mkeep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/main.p1 main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cpu.p1: cpu.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/cpu.p1.d 
	@${RM} ${OBJECTDIR}/cpu.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O2 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file -Wl,-AINIT=0800h-0803h -Wl,-Pinit=INIT -Wl,-AISR=0808h-08FFh -Wl,-Pisr_text=ISR -Wl,-AFILES=F800h-FFFFh -Wl,-Pfiles=FILES  -ginhx032 -Wl,--data-init -mkeep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/cpu.p1 cpu.c 
	@-${MV} ${OBJECTDIR}/cpu.d ${OBJECTDIR}/cpu.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/cpu.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/ui.p1: ui.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ui.p1.d 
	@${RM} ${OBJECTDIR}/ui.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O2 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file -Wl,-AINIT=0800h-0803h -Wl,-Pinit=INIT -Wl,-AISR=0808h-08FFh -Wl,-Pisr_text=ISR -Wl,-AFILES=F800h-FFFFh -Wl,-Pfiles=FILES  -ginhx032 -Wl,--data-init -mkeep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/ui.p1 ui.c 
	@-${MV} ${OBJECTDIR}/ui.d ${OBJECTDIR}/ui.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/ui.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/file.p1: file.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/file.p1.d 
	@${RM} ${OBJECTDIR}/file.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O2 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file -Wl,-AINIT=0800h-0803h -Wl,-Pinit=INIT -Wl,-AISR=0808h-08FFh -Wl,-Pisr_text=ISR -Wl,-AFILES=F800h-FFFFh -Wl,-Pfiles=FILES  -ginhx032 -Wl,--data-init -mkeep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/file.p1 file.c 
	@-${MV} ${OBJECTDIR}/file.d ${OBJECTDIR}/file.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/file.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/misc.p1: misc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/misc.p1.d 
	@${RM} ${OBJECTDIR}/misc.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O2 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file -Wl,-AINIT=0800h-0803h -Wl,-Pinit=INIT -Wl,-AISR=0808h-08FFh -Wl,-Pisr_text=ISR -Wl,-AFILES=F800h-FFFFh -Wl,-Pfiles=FILES  -ginhx032 -Wl,--data-init -mkeep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/misc.p1 misc.c 
	@-${MV} ${OBJECTDIR}/misc.d ${OBJECTDIR}/misc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/misc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/comm.p1: comm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/comm.p1.d 
	@${RM} ${OBJECTDIR}/comm.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O2 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file -Wl,-AINIT=0800h-0803h -Wl,-Pinit=INIT -Wl,-AISR=0808h-08FFh -Wl,-Pisr_text=ISR -Wl,-AFILES=F800h-FFFFh -Wl,-Pfiles=FILES  -ginhx032 -Wl,--data-init -mkeep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/comm.p1 comm.c 
	@-${MV} ${OBJECTDIR}/comm.d ${OBJECTDIR}/comm.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/comm.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/monitor.p1: monitor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/monitor.p1.d 
	@${RM} ${OBJECTDIR}/monitor.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O2 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file -Wl,-AINIT=0800h-0803h -Wl,-Pinit=INIT -Wl,-AISR=0808h-08FFh -Wl,-Pisr_text=ISR -Wl,-AFILES=F800h-FFFFh -Wl,-Pfiles=FILES  -ginhx032 -Wl,--data-init -mkeep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/monitor.p1 monitor.c 
	@-${MV} ${OBJECTDIR}/monitor.d ${OBJECTDIR}/monitor.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/monitor.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/config.p1: config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/config.p1.d 
	@${RM} ${OBJECTDIR}/config.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O2 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file -Wl,-AINIT=0800h-0803h -Wl,-Pinit=INIT -Wl,-AISR=0808h-08FFh -Wl,-Pisr_text=ISR -Wl,-AFILES=F800h-FFFFh -Wl,-Pfiles=FILES  -ginhx032 -Wl,--data-init -mkeep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/config.p1 config.c 
	@-${MV} ${OBJECTDIR}/config.d ${OBJECTDIR}/config.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/config.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O2 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file -Wl,-AINIT=0800h-0803h -Wl,-Pinit=INIT -Wl,-AISR=0808h-08FFh -Wl,-Pisr_text=ISR -Wl,-AFILES=F800h-FFFFh -Wl,-Pfiles=FILES  -ginhx032 -Wl,--data-init -mkeep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/main.p1 main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/cpu.p1: cpu.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/cpu.p1.d 
	@${RM} ${OBJECTDIR}/cpu.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O2 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file -Wl,-AINIT=0800h-0803h -Wl,-Pinit=INIT -Wl,-AISR=0808h-08FFh -Wl,-Pisr_text=ISR -Wl,-AFILES=F800h-FFFFh -Wl,-Pfiles=FILES  -ginhx032 -Wl,--data-init -mkeep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/cpu.p1 cpu.c 
	@-${MV} ${OBJECTDIR}/cpu.d ${OBJECTDIR}/cpu.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/cpu.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/ui.p1: ui.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ui.p1.d 
	@${RM} ${OBJECTDIR}/ui.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O2 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file -Wl,-AINIT=0800h-0803h -Wl,-Pinit=INIT -Wl,-AISR=0808h-08FFh -Wl,-Pisr_text=ISR -Wl,-AFILES=F800h-FFFFh -Wl,-Pfiles=FILES  -ginhx032 -Wl,--data-init -mkeep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/ui.p1 ui.c 
	@-${MV} ${OBJECTDIR}/ui.d ${OBJECTDIR}/ui.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/ui.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/file.p1: file.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/file.p1.d 
	@${RM} ${OBJECTDIR}/file.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O2 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file -Wl,-AINIT=0800h-0803h -Wl,-Pinit=INIT -Wl,-AISR=0808h-08FFh -Wl,-Pisr_text=ISR -Wl,-AFILES=F800h-FFFFh -Wl,-Pfiles=FILES  -ginhx032 -Wl,--data-init -mkeep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/file.p1 file.c 
	@-${MV} ${OBJECTDIR}/file.d ${OBJECTDIR}/file.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/file.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/misc.p1: misc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/misc.p1.d 
	@${RM} ${OBJECTDIR}/misc.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O2 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file -Wl,-AINIT=0800h-0803h -Wl,-Pinit=INIT -Wl,-AISR=0808h-08FFh -Wl,-Pisr_text=ISR -Wl,-AFILES=F800h-FFFFh -Wl,-Pfiles=FILES  -ginhx032 -Wl,--data-init -mkeep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/misc.p1 misc.c 
	@-${MV} ${OBJECTDIR}/misc.d ${OBJECTDIR}/misc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/misc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/comm.p1: comm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/comm.p1.d 
	@${RM} ${OBJECTDIR}/comm.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O2 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file -Wl,-AINIT=0800h-0803h -Wl,-Pinit=INIT -Wl,-AISR=0808h-08FFh -Wl,-Pisr_text=ISR -Wl,-AFILES=F800h-FFFFh -Wl,-Pfiles=FILES  -ginhx032 -Wl,--data-init -mkeep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/comm.p1 comm.c 
	@-${MV} ${OBJECTDIR}/comm.d ${OBJECTDIR}/comm.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/comm.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/monitor.p1: monitor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/monitor.p1.d 
	@${RM} ${OBJECTDIR}/monitor.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O2 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file -Wl,-AINIT=0800h-0803h -Wl,-Pinit=INIT -Wl,-AISR=0808h-08FFh -Wl,-Pisr_text=ISR -Wl,-AFILES=F800h-FFFFh -Wl,-Pfiles=FILES  -ginhx032 -Wl,--data-init -mkeep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/monitor.p1 monitor.c 
	@-${MV} ${OBJECTDIR}/monitor.d ${OBJECTDIR}/monitor.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/monitor.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/powerup.o: powerup.as  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/powerup.o.d 
	@${RM} ${OBJECTDIR}/powerup.o 
	${MP_CC} -c $(MP_EXTRA_AS_PRE) -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O2 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file -Wl,-AINIT=0800h-0803h -Wl,-Pinit=INIT -Wl,-AISR=0808h-08FFh -Wl,-Pisr_text=ISR -Wl,-AFILES=F800h-FFFFh -Wl,-Pfiles=FILES  -ginhx032 -Wl,--data-init -mkeep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto   -o ${OBJECTDIR}/powerup.o  powerup.as 
	@-${MV} ${OBJECTDIR}/powerup.d ${OBJECTDIR}/powerup.o.d 
	@${FIXDEPS} ${OBJECTDIR}/powerup.o.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/powerup.o: powerup.as  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/powerup.o.d 
	@${RM} ${OBJECTDIR}/powerup.o 
	${MP_CC} -c $(MP_EXTRA_AS_PRE) -mcpu=$(MP_PROCESSOR_OPTION)   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O2 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file -Wl,-AINIT=0800h-0803h -Wl,-Pinit=INIT -Wl,-AISR=0808h-08FFh -Wl,-Pisr_text=ISR -Wl,-AFILES=F800h-FFFFh -Wl,-Pfiles=FILES  -ginhx032 -Wl,--data-init -mkeep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto   -o ${OBJECTDIR}/powerup.o  powerup.as 
	@-${MV} ${OBJECTDIR}/powerup.d ${OBJECTDIR}/powerup.o.d 
	@${FIXDEPS} ${OBJECTDIR}/powerup.o.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/Digirule2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=dist/${CND_CONF}/${IMAGE_TYPE}/Digirule2.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O2 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file -Wl,-AINIT=0800h-0803h -Wl,-Pinit=INIT -Wl,-AISR=0808h-08FFh -Wl,-Pisr_text=ISR -Wl,-AFILES=F800h-FFFFh -Wl,-Pfiles=FILES  -ginhx032 -Wl,--data-init -mkeep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -mrom=default,-fd30-ffff -mram=default,-ef4-eff,-f9c-f9c,-fd4-fd4,-fdb-fdf,-fe3-fe7,-feb-fef,-ffd-fff  $(COMPARISON_BUILD) -Wl,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -o dist/${CND_CONF}/${IMAGE_TYPE}/Digirule2.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/Digirule2.X.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/Digirule2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=dist/${CND_CONF}/${IMAGE_TYPE}/Digirule2.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O2 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file -Wl,-AINIT=0800h-0803h -Wl,-Pinit=INIT -Wl,-AISR=0808h-08FFh -Wl,-Pisr_text=ISR -Wl,-AFILES=F800h-FFFFh -Wl,-Pfiles=FILES  -ginhx032 -Wl,--data-init -mkeep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     $(COMPARISON_BUILD) -Wl,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -o dist/${CND_CONF}/${IMAGE_TYPE}/Digirule2.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
