
obj_dir/Vtestbench: testbench.v testbench.cpp Cpu.cpp picorv32.v
	verilator -O3 --trace --exe -Wno-fatal --cc --top-module testbench \
		  -DGDBSERVER -DENABLE_COUNTERS -CFLAGS "-O3 -std=gnu++11" $^
	$(MAKE) -C obj_dir -f Vtestbench.mk

.PHONY: clean
clean:
	$(RM) -r obj_dir

