// microPainter
// Eyassu Shimelis and Austin Chun
// eshimelis@hmc.edu; achun@hmc.edu
// E155 Final Project: LED Light Painter (microPainter)


module microPainter(input logic clk, reset, aUnsync, bUnsync,
                    output logic state, read, debugA, debugB);

    logic aDelayed, bDelayed, a, b, countEnable, countDirection;

    // Synchronize encoder data
    sync syncA(clk, reset, aUnsync, a);
    sync syncB(clk, reset, bUnsync, b);

    // Produced delayed output
    clockDelay clockDelayA(clk, reset, a, aDelayed);
    clockDelay clockDelayB(clk, reset, b, bDelayed);

	assign countEnable = a^aDelayed^b^bDelayed;
	assign countDirection = a^bDelayed;

    logic [4:0] count;

    always @(posedge clk)
		begin
			if (~reset)
				count <= 0;

			else if (countEnable)
				begin
					if (countDirection) count <= count+1;
					else count <= count-1;
			end
		end
		
	// For debugging purposes
	assign debugA = countEnable;
	assign debugB = state;

	always @(posedge countEnable)
		begin
			if (~reset)
				read <= 0;
			else if (count == 0)
				read <= 1;
			else
				read <= 0;
	   end
		 
	always @(posedge a)
		begin
			if (~reset)
				state <= 0;
			else
				state <= countDirection;
		end

endmodule


// Asynchronous reset Flip Flop
module fflop(input logic clk, reset, d,
		       output logic q);

	always_ff @(posedge clk)
		if (~reset) q <= 0;
		else
			q <= d;	
endmodule


//  Synchronizer
module sync(input logic clk, reset, d,  // Unsynchronized input
			   output logic q);	          // Synchronized input

	// Logic to store intermediate value for second flop
	logic intermediate;
	
	// Instantiate flip flops with corresponding inputs
	fflop flopOne(clk, reset, d, intermediate);
	fflop flopTwo(clk, reset, intermediate, q);

endmodule


// One bit synchronizer
module clockDelay(input logic clk, reset, d,
		          output logic q);

	always_ff @(posedge clk)
		if (~reset) q <= 0;
		else
			q <= d;
endmodule
