# Reset the simulation
restart -froce -nowave

# Add waves
add wave /testbench/mPainter/clk
add wave /testbench/mPainter/reset
add wave /testbench/mPainter/aUnsync
add wave /testbench/mPainter/bUnsync
add wave /testbench/mPainter/aDelayed
add wave /testbench/mPainter/bDelayed
add wave /testbench/mPainter/a
add wave /testbench/mPainter/b
add wave /testbench/mPainter/countEnable
add wave /testbench/mPainter/countDirection
add wave /testbench/mPainter/count

# Reset encoder values
force /testbench/mPainter/aUnsync 0
force /testbench/mPainter/bUnsync 0

# Start simulation
run 33
force /testbench/mPainter/aUnsync 1
run 30
force /testbench/mPainter/bUnsync 1
run 30
force /testbench/mPainter/aUnsync 0
run 30
force /testbench/mPainter/bUnsync 0
run 30
force /testbench/mPainter/aUnsync 1
run 30
force /testbench/mPainter/bUnsync 1
run 30
force /testbench/mPainter/aUnsync 0
run 30
force /testbench/mPainter/bUnsync 0
run 30
force /testbench/mPainter/aUnsync 1
run 30
force /testbench/mPainter/bUnsync 1
run 30
force /testbench/mPainter/aUnsync 0
run 30
force /testbench/mPainter/bUnsync 0
run 30
force /testbench/mPainter/aUnsync 1
run 30
force /testbench/mPainter/bUnsync 1
run 30
force /testbench/mPainter/aUnsync 0
run 30
force /testbench/mPainter/bUnsync 0
run 30
force /testbench/mPainter/aUnsync 1
run 30
force /testbench/mPainter/bUnsync 1
run 30
force /testbench/mPainter/aUnsync 0
run 30
force /testbench/mPainter/bUnsync 0
run 30
force /testbench/mPainter/aUnsync 1
run 30
force /testbench/mPainter/bUnsync 1
run 30
force /testbench/mPainter/aUnsync 0
run 30
force /testbench/mPainter/bUnsync 0
run 30
force /testbench/mPainter/aUnsync 1
run 30
force /testbench/mPainter/bUnsync 1
run 30
force /testbench/mPainter/aUnsync 0
run 30
force /testbench/mPainter/bUnsync 0
run 300
force /testbench/mPainter/bUnsync 1
run 30
force /testbench/mPainter/aUnsync 1
run 30
force /testbench/mPainter/bUnsync 0
run 30
force /testbench/mPainter/aUnsync 0
run 30
force /testbench/mPainter/bUnsync 1
run 30
force /testbench/mPainter/aUnsync 1
run 30
force /testbench/mPainter/bUnsync 0
run 30
force /testbench/mPainter/aUnsync 0
run 30
force /testbench/mPainter/bUnsync 1
run 30
force /testbench/mPainter/aUnsync 1
run 30
force /testbench/mPainter/bUnsync 0
run 30
force /testbench/mPainter/aUnsync 0
run 30
force /testbench/mPainter/bUnsync 1
run 30
force /testbench/mPainter/aUnsync 1
run 30
force /testbench/mPainter/bUnsync 0
run 30
force /testbench/mPainter/aUnsync 0
run 30
force /testbench/mPainter/bUnsync 1
run 30
force /testbench/mPainter/aUnsync 1
run 30
force /testbench/mPainter/bUnsync 0
run 30
force /testbench/mPainter/aUnsync 0
run 30
force /testbench/mPainter/bUnsync 1
run 30
force /testbench/mPainter/aUnsync 1
run 30
force /testbench/mPainter/bUnsync 0
run 30
force /testbench/mPainter/aUnsync 0
run 30
force /testbench/mPainter/bUnsync 1
run 30
force /testbench/mPainter/aUnsync 1
run 30
force /testbench/mPainter/bUnsync 0
run 30
force /testbench/mPainter/aUnsync 0
run 30
force /testbench/mPainter/bUnsync 1
run 30
force /testbench/mPainter/aUnsync 1
run 30
force /testbench/mPainter/bUnsync 0
run 30
force /testbench/mPainter/aUnsync 0
run 30
force /testbench/mPainter/bUnsync 1
run 30
force /testbench/mPainter/aUnsync 1
run 30
force /testbench/mPainter/bUnsync 0
run 30
force /testbench/mPainter/aUnsync 0
run 30
force /testbench/mPainter/bUnsync 1
run 30
force /testbench/mPainter/aUnsync 1
run 30
force /testbench/mPainter/bUnsync 0
run 30
force /testbench/mPainter/aUnsync 0
run 30
force /testbench/mPainter/bUnsync 1
run 30
force /testbench/mPainter/aUnsync 1
run 30
force /testbench/mPainter/bUnsync 0
run 30
force /testbench/mPainter/aUnsync 0
run 30
force /testbench/mPainter/bUnsync 1
run 30
force /testbench/mPainter/aUnsync 1
run 30
force /testbench/mPainter/bUnsync 0
run 30
force /testbench/mPainter/aUnsync 0

# View wave
view wave
