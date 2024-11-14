VERILATOR_FLAGS=--cc --exe --build -j 0 -Wall --trace
all:
	@echo "Write this Makefile by your self."

sim:
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@echo "Write this Makefile by your self."
	verilator $(VERILATOR_FLAGS)  csrc/main.cpp vsrc/top.v
   
	 obj_dir/Vtop

clean:
	rm -rf obj_dir
	rm -f wave.vcd
include ../Makefile
