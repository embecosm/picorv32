
obj_dir/Vtestbench: testbench.v testbench.cpp Cpu.cpp picorv32.v
	verilator --exe -Wno-fatal --cc --top-module testbench \
		  -DGDBSERVER -CFLAGS "-O3 -std=gnu++11" $^
	$(MAKE) -C obj_dir -f Vtestbench.mk

.PHONY: clean
clean:
	$(RM) -r obj_dir

