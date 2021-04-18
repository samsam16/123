default: fom.csv

USER_CFLAGS=-I$(CANELA_ROOT)
BUILD=build
include $(BUILD)/config.mk
include $(ARCHLAB_ROOT)/lab.make

code.out: $(BUILD)/config

%.exe : $(BULID)%.o main.o 
	$(CXX) $^ $(LDFLAGS) -o $@

fom.csv: code.exe
	./code.exe --stats-file $@ --stat-set default.cfg
	csv-pretty.py < $@

test.csv: code.exe
	./code.exe --stats-file $@
	csv-pretty.py < $@

