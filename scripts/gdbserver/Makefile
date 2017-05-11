RISCV_TOOLS_DIR = /opt/riscv32imc
RISCV_TOOLS_PREFIX = $(RISCV_TOOLS_DIR)/bin/riscv32-unknown-elf-
CSMITH_INCDIR = $(shell ls -d /usr/local/include/csmith-* | head -n1)
CC = $(RISCV_TOOLS_PREFIX)gcc
SHELL = /bin/bash

obj_dir/Vtestbench: testbench.v testbench.cpp Cpu.cpp picorv32.v
	verilator --exe -Wno-fatal --cc --top-module testbench \
		  -CFLAGS -std=gnu++11 $^
	$(MAKE) -C obj_dir -f Vtestbench.mk

.PHONY: clean
clean:
	$(RM) -r obj_dir

