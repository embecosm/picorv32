// GDB RSP server CPU model wrapper: top level program

// Copyright (C) 2017  Embecosm Limited <info@embecosm.com>

// Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

// This file is part of the RISC-V GDB server

// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.

// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License for more details.

// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <cstdint>

#include "Cpu.h"
#include "verilated.h"


int main(int   argc,
	 char *argv [])
{
  Verilated::commandArgs (argc, argv);
  Cpu *cpu = new Cpu;

  uint32_t addr = 0;
  uint8_t  val;

  while (!Verilated::gotFinish())
    {
      cpu->step ();
      val = (addr * 2) & 0xff;
      cpu->writeMem (addr, val);
      printf ("%d:\n", addr);
      printf ("wrote 0x%02x to 0x%08x\n", val, addr);
      val = cpu->readMem (addr - 1);
      printf ("read 0x%02x from 0x%08x\n", val, addr - 1);
      addr++;
    }

  delete cpu;
  exit (0);

}
