// Top level Verilog wrapper for the GDB server.

// Copyright (C) 2017 Embecosm Limited <www.embecosm.com>

// Contributor: Jeremy Bennett <jeremy.bennett@embecosm.com>
// Contributor: Ian Bolton <ian.bolton@embecosm.com>

// This file is part of the GDB server for the picorv32 RISC-V implementation

// The GDB server for the picorv32 RISC-V implementation is free software: you
// can redistribute it and/or modify it under the terms of the GNU General
// Public License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// This wrapper reuses the memory model from the testbench.v file in the
// parent directory, which is licensed under the ISC license, which is
// compatible with the GPL version 3.  See the comments at the head of
// testbench.v for its licensing terms.


`timescale 1 ns / 1 ps

module wrapper (
   input clk,			// Clock supplied externally
   input resetn,		// Active low reset supplied externally
   output trap			// High when the processor is halted by trap
);

`ifdef GDBSERVER_VCD
   initial begin
      if ($test$plusargs("vcd")) begin
	 $dumpfile("gdbserver.vcd");
	 $dumpvars(0, wrapper);
      end
   end
`endif

   // Instantiate the CPU for a basic RV32I

   picorv32 #(.COMPRESSED_ISA (0),
	      .ENABLE_MUL     (0),
	      .ENABLE_DIV     (0)
	      ) cpu (.clk         (clk        ),
		     .resetn      (resetn     ),
		     .trap        (trap       ),
		     .mem_valid   (mem_valid  ),
		     .mem_instr   (mem_instr  ),
		     .mem_ready   (mem_ready  ),
		     .mem_addr    (mem_addr   ),
		     .mem_wdata   (mem_wdata  ),
		     .mem_wstrb   (mem_wstrb  ),
		     .mem_rdata   (mem_rdata  )
		     );

   // The memory model

   wire mem_valid;
   wire mem_instr;
   wire mem_ready;
   wire [31:0] mem_addr;
   wire [31:0] mem_wdata;
   wire [3:0]  mem_wstrb;
   wire [31:0] mem_rdata;

   reg [31:0]  x32 = 314159265;
   reg [31:0]  next_x32;

   always @(posedge clk) begin
      if (resetn) begin
	 next_x32 = x32;
	 next_x32 = next_x32 ^ (next_x32 << 13);
	 next_x32 = next_x32 ^ (next_x32 >> 17);
	 next_x32 = next_x32 ^ (next_x32 << 5);
	 x32 <= next_x32;
      end
   end

   reg [7:0] memory [0:4*1024*1024-1];

   assign mem_ready = x32[0] && mem_valid;

   assign mem_rdata[ 7: 0] = memory[mem_addr + 0];
   assign mem_rdata[15: 8] = memory[mem_addr + 1];
   assign mem_rdata[23:16] = memory[mem_addr + 2];
   assign mem_rdata[31:24] = memory[mem_addr + 3];

   always @(posedge clk) begin
      if (mem_valid && mem_ready) begin
	 // Log any writes to address 'h10000000 to stdout
	 if (mem_wstrb && mem_addr == 'h10000000) begin
	    $write("%c", mem_wdata[ 7: 0]);
	 end else begin
	    if (mem_wstrb[0]) memory[mem_addr + 0] <= mem_wdata[ 7: 0];
	    if (mem_wstrb[1]) memory[mem_addr + 1] <= mem_wdata[15: 8];
	    if (mem_wstrb[2]) memory[mem_addr + 2] <= mem_wdata[23:16];
	    if (mem_wstrb[3]) memory[mem_addr + 3] <= mem_wdata[31:24];
	 end
      end
   end

   // Tasks and functions to give visibility to Verilator

   function [7:0] readMem;
      /* verilator public */
      input [31:0] addr;
      readMem = memory[addr];
   endfunction

   task writeMem;
      /* verilator public */
      input [31:0] addr;
      input [7:0]  val;
      memory[addr] = val;
   endtask

endmodule
