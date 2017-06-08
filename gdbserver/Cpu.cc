// GDB RSP server CPU model wrapper: definition

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

#include <cstdint>

#include "Cpu.h"
#include "verilated_vcd_c.h"
#include "Vwrapper_wrapper.h"
#include "Vwrapper_picorv32.h"

//! Constructor. Instantiate the Verilator model and initialize the clock.

Cpu::Cpu (TraceFlags * traceFlags) :
  mTraceFlags (traceFlags),
  mCpuTime (0),
  mPrevPc (0)
{
  mCpu = new Vwrapper;

  // Open VCD file if requested

  if (mTraceFlags->traceVcd ())
    {
      Verilated::traceEverOn (true);
      mTfp = new VerilatedVcdC;
      mCpu->trace (mTfp, 99);
      mTfp->open ("gdbserver.vcd");
    }

  // Reset the processor for 100 cycles

  mCpu->resetn = 0;

  for (int i = 0; i < 100; i++)
    oneCycle ();

}	// Cpu::Cpu ()


//! Destructor. Delete the Verilator model.

Cpu::~Cpu ()
{
  // Close VCD file if requested

  if (mTraceFlags->traceVcd ())
    mTfp->close ();

  delete mCpu;

}	// Cpu::~Cpu ()


//! Step one instruction execution

bool
Cpu::step ()
{
  // If we are trapped, we need to release the trap and re-execute the
  // instruction, otherwise we just execute the next instruction.

  uint32_t prevPc;

  if (1 == mCpu->trap)
    {
      // The trap happened on the instruction we want to re-execute. Clear the
      // trap, set the new PC and assert

      prevPc = mCpu->wrapper->cpu->readPrevPc ();
      mCpu->wrapper->cpu->clearTrap ();
      mCpu->wrapper->cpu->writePc (mPrevPc);
      mCpu->resetn = 1;			// Not in reset
      oneCycle ();
    }

  // Now step whole clock cycles (not just one edge) until the PC changes.

  // TODO: This breaks if we have zero-overhad loops. Need to check this in
  // RISC-V.

  prevPc = readProgramAddr ();
  mCpu->resetn = 1;			// Not in reset

  do
  {
    oneCycle ();
  }
  while ((prevPc == readProgramAddr ()) && (mCpu->trap == 0));

  return mCpu->trap == 1;

}	// Cpu::step ()


//! Read from memory

uint8_t
Cpu::readMem (uint32_t addr) const
{
  uint8_t res = mCpu->wrapper->readMem (addr);
  return res;

}	// Cpu::readMem ()


//! Write to memory

void
Cpu::writeMem (uint32_t addr,
	       uint8_t  val)
{
  mCpu->wrapper->writeMem (addr, val);

}	// Cpu::writeMem ()


//! Read a register

uint32_t
Cpu::readReg (unsigned int regno) const
{
  return mCpu->wrapper->cpu->readReg (regno);

}	// Cpu::readReg ()


//! Write a register

void
Cpu::writeReg (unsigned int regno,
	       uint32_t     val)
{
  mCpu->wrapper->cpu->writeReg (regno, val);

}	// Cpu::writeReg ()


//! Read the PC

uint32_t
Cpu::readProgramAddr () const
{
  return  mCpu->wrapper->cpu->readPc ();

}	// Cpu::readProgramAddr ()


//! Write the PC. To do this we need to trap the processor (which flushes the
//! pipeline and then clock in a new PC.

void
Cpu::writeProgramAddr (uint32_t val)
{
  // Trap the processor and cycle for it to take effect.

  mCpu->wrapper->cpu->assertTrap ();
  mCpu->resetn = 1;			// Not in reset
  oneCycle ();

  // Clear the trap, write the PC and cycle for it to take effect.

  mCpu->wrapper->cpu->clearTrap ();
  mCpu->wrapper->cpu->writePc (val);
  mCpu->resetn = 1;			// Not in reset
  oneCycle ();

}	// Cpu::writeProgramAddr ()


//! Flush the VCD if we are tracing

void
Cpu::flushVcd (void)
{
  if (mTraceFlags->traceVcd ())
    mTfp->flush ();

}	// Cpu::flushVcd ()


//! Provide a time stamp (needed for $time)

//! @return  The time in nanoseconds

double
Cpu::timeStamp (void)
{
  return static_cast<double> (mCpuTime);

}	// timeStamp ()


//! Helper to step one full clock cycle

void
Cpu::oneCycle ()
{
  mCpu->clk = 0;
  mCpu->eval ();
  if (mTraceFlags->traceVcd ())
    {
      mCpuTime += 5;			// in ns
      mTfp->dump (mCpuTime);
    }

  mCpu->clk = 1;
  mCpu->eval ();
  if (mTraceFlags->traceVcd ())
    {
      mCpuTime += 5;			// in ns
      mTfp->dump (mCpuTime);
    }

}	// oneCycle ()


// Local Variables:
// mode: C++
// c-file-style: "gnu"
// show-trailing-whitespace: t
// End:
