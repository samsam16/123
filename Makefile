# This is master makefile for this lab.  You should not edit this file, since
# changes you make will be ignored by the autograder.
#
# You can edit config.env to configure the build process.
#
# Here are the variables you can set in config.env:
#
# CMD_LINE_ARGS=<args>    :  This is passed to code.exe 
# GPROF=yes               :  Will enable gprof
# DEBUG=yes               :  Will enable debugging, which will hurt performance.
# OPTIMIZE=<gcc options>  :  Optimization flags for gcc.
# COMPILER=gcc-9          :  Choose the compiler you want.

default: inst_mix.csv pe.csv code.s

CLEANUP=inst_mix.csv pe.csv

COMPILER=gcc-9

include $(ARCHLAB_ROOT)/cse141.make

TAGS=--tag GPROF=$(GPROF) --tag OPTIMIZE=$(OPTIMIZE) --tag DEVEL_MODE=$(DEVEL_MODE)

ifeq ($(DEVEL_MODE),yes)
INST_MIX_CMD_LINE_ARGS=--stat runtime=ARCHLAB_WALL_TIME $(TAGS) --tag STATS=INST_MIX $(CMD_LINE_ARGS)
PE_CMD_LINE_ARGS=--stat runtime=ARCHLAB_WALL_TIME $(TAGS) --tag STATS=PE $(CMD_LINE_ARGS)
else
INST_MIX_CMD_LINE_ARGS=--stat-set inst_mix.cfg $(TAGS) --tag STATS=INST_MIX $(CMD_LINE_ARGS) 
PE_CMD_LINE_ARGS=--stat-set ipc.cfg $(TAGS) --tag STATS=ipc $(CMD_LINE_ARGS) 
endif

CUSTOM_CMD_LINE=--stat runtime=ARCHLAB_WALL_TIME $(CMD_LINE_ARGS)

inst_mix.csv: code.exe
	rm -f gmon.out
	./code.exe --stats-file $@ $(INST_MIX_CMD_LINE_ARGS)
	pretty-csv $@
	if [ -e gmon.out ]; then gprof $< > inst_mix.gprof; fi

pe.csv: code.exe
	rm -f gmon.out
	./code.exe --stats-file $@ $(PE_CMD_LINE_ARGS)
	pretty-csv $@
	if [ -e gmon.out ]; then gprof $< > pe.gprof; fi

