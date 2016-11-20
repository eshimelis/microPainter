module testbench();

    logic clk, reset, aUnsync, bUnsync;
    microPainter mPainter(clk, reset, aUnsync, bUnsync);

    // Simulate clock
    always
        begin
            clk <= 1; # 5; clk <= 0; # 5;
        end

    // Simulate rotary encoder a phase output data
    always
        begin
            aUnsync <= 1; #253; clk <= 0; #268;
        end

    // Simulate rotary encoder b phase output data
    always
        begin
            aUnsync <= 1; #253; clk <= 0; #268;
        end

    initial
        begin
            #5;
            reset <= 1'b1;
            #20;
            reset <= 1'b0;
        end
endmodule
