// GDB RSP server CPU model wrapper: declaration

// Copyright (C) 2017  Embecosm Limited <info@embecosm.com>

// Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>
// Contributor Ian Bolton <ian.bolton@embecosm.com>

// This file is part of the GDB server for the picorv32 RISC-V implementation

// The GDB server for the picorv32 RISC-V implementation is free software: you
// can redistribute it and/or modify it under the terms of the GNU General
// Public License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License for more details.

// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef CPU__H
#define CPU__H

#include <cstdint>

#include "TraceFlags.h"
#include "Vwrapper.h"
#include "verilated.h"

class Cpu final
{
 public:

  // Constructor and destructor

  Cpu (TraceFlags * traceFlags);
  ~Cpu ();

  // Accessors

  bool step (void);
  uint8_t readMem (uint32_t addr) const;
  void writeMem (uint32_t addr,
		 uint8_t  val);
  uint32_t readReg (unsigned int regno) const;
  void writeReg (unsigned int regno,
		 uint32_t     val);
  uint32_t readProgramAddr () const;
  void writeProgramAddr (uint32_t addr);

  // VCD related functions

  void flushVcd (void);

  // Verilog support functions

  double timeStamp (void);


 private:

  //! Top level Verilator model.

  Vwrapper * mCpu;

  //! Trace flags

  TraceFlags * mTraceFlags;

  //! VCD trace file pointer

  VerilatedVcdC * mTfp;

  //! VCD time. This will be in ns and we have a 100MHz device

  vluint64_t  mCpuTime;

  //! Previous value of the program counter

  uint32_t mPrevPc;

  //! Helper to step one full clock cycle

  void  oneCycle ();
};

#endif

// Local Variables:
// mode: C++
// c-file-style: "gnu"
// show-trailing-whitespace: t
// End:
