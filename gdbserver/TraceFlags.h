// GDB RSP server: definition

// Copyright (C) 2009, 2013  Embecosm Limited <info@embecosm.com>

// Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

// This file is part of the GDB server for the picorv32 RISC-V implementation

// The GDB server for the picorv32 RISC-V implementation is free software: you
// can redistribute it and/or modify it under the terms of the GNU Lesser
// General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License for more details.

// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#ifndef TRACE_FLAGS__H
#define TRACE_FLAGS__H


//! Class for trace flags

//! The flags themselves are bits in an unsigned integer

class TraceFlags
{
public:

  // Constructor and destructor
  TraceFlags (unsigned int  _Flags = TRACE_NONE);
  ~TraceFlags ();

  // Accessors
  bool traceRsp ();
  bool traceServer ();
  bool traceVcd ();


private:

  // Definition of flag values
  static const unsigned int TRACE_MASK   = 0x00000007;	//!< Trace flag mask

  static const unsigned int TRACE_NONE   = 0x00000000;	//!< Trace nothing
  static const unsigned int TRACE_RSP    = 0x00000001;	//!< Trace RSP packets
  static const unsigned int TRACE_SERVER = 0x00000002;	//!< Trace the server
  static const unsigned int TRACE_VCD    = 0x00000004;	//!< Generate VCD

  //! The trace flags
  unsigned int  flags;

};	// TraceFlags ()

#endif	// TRACE_FLAGS__H


// Local Variables:
// mode: C++
// c-file-style: "gnu"
// End:
