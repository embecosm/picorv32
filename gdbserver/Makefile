# Makefile for the GDB server

# Copyright (C) 2017 Embecosm Limited <www.embecosm.com>

# Contributor: Jeremy Bennett <jeremy.bennett@embecosm.com>
# Contributor: Ian Bolton <ian.bolton@embecosm.com>

# This file is part of the GDB server for the picorv32 RISC-V implementation

# The GDB server for the picorv32 RISC-V implementation is free software: you
# can redistribute it and/or modify it under the terms of the GNU General
# Public License as published by the Free Software Foundation, either version
# 3 of the License, or (at your option) any later version.

# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.

# You should have received a copy of the GNU General Public License along with
# this program.  If not, see <http://www.gnu.org/licenses/>.


# Tools we use

VERILATOR = verilator
VSHAREDIR ?= /usr/share/verilator
CPPFLAGS = -Iobj_dir -I$(VSHAREDIR)/include -I$(VSHAREDIR)/include/vltstd
CXXFLAGS = -Wall -Werror -std=c++11
CXX = g++
LD = g++

EXE = riscv-gdbserver
OBJS = Cpu.o             \
       GdbServer.o       \
       riscv-gdbserver.o \
       MpHash.o          \
       RspConnection.o   \
       RspPacket.o       \
       TraceFlags.o      \
       Utils.o
VMK = obj_dir/Vwrapper.mk
VOBJS = obj_dir/verilated.o       \
	obj_dir/verilated_vcd_c.o \
	obj_dir/verilated_dpi.o
VLIB = obj_dir/Vwrapper__ALL.a

$(EXE): $(VLIB) $(VOBJS) $(OBJS)
	$(LD) -o $@ $(OBJS) $(VLIB) $(VOBJS)

$(VOBJS): $(VMK)
	for f in $@; \
	do \
		sf=$$(basename $$f); \
		$(MAKE) -C obj_dir -f Vwrapper.mk $$sf; \
	done

$(VLIB): $(VMK)
	$(MAKE) -C obj_dir -f Vwrapper.mk

$(VMK): wrapper.v ../picorv32.v
	$(VERILATOR) -Wno-fatal --cc --top-module wrapper \
		-Wno-PINMISSING -Wno-WIDTH -Wno-CASEINCOMPLETE \
		-Wno-CASEOVERLAP \
		--trace -DGDBSERVER -CFLAGS -std=gnu++11 $^

# Clean up everything we've created.

.PHONY: clean
clean:
	$(RM) -r obj_dir
	$(RM) *.o
	$(RM) $(EXE)

