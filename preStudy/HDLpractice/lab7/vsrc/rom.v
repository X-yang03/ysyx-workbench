module rom(
   input [7:0] addr,
   output [7:0] data
);
    reg [7:0] mem[255:0];
    
    assign data = mem[addr];
    
    //implement a map rom from ps2_code to ascii
    initial begin
        mem[8'h1C] = 8'h61; //a
        mem[8'h32] = 8'h62; //b
        mem[8'h21] = 8'h63; //c
        mem[8'h23] = 8'h64; //d
        mem[8'h24] = 8'h65; //e
        mem[8'h2B] = 8'h66; //f
        mem[8'h34] = 8'h67; //g
        mem[8'h33] = 8'h68; //h
        mem[8'h43] = 8'h69; //i
        mem[8'h3B] = 8'h6A; //j
        mem[8'h42] = 8'h6B; //k
        mem[8'h4B] = 8'h6C; //l
        mem[8'h3A] = 8'h6D; //m
        mem[8'h31] = 8'h6E; //n
        mem[8'h44] = 8'h6F; //o
        mem[8'h4D] = 8'h70; //p
        mem[8'h15] = 8'h71; //q
        mem[8'h2D] = 8'h72; //r
        mem[8'h1B] = 8'h73; //s
        mem[8'h2C] = 8'h74; //t
        mem[8'h3C] = 8'h75; //u
        mem[8'h2A] = 8'h76; //v
        mem[8'h1D] = 8'h77; //w
        mem[8'h22] = 8'h78; //x
        mem[8'h35] = 8'h79; //y
        mem[8'h1A] = 8'h7A; //z
        mem[8'h45] = 8'h30; //0
        mem[8'h16] = 8'h31; //1 
        mem[8'h1E] = 8'h32; //2 
        mem[8'h26] = 8'h33; //3 
        mem[8'h25] = 8'h34; //4 
        mem[8'h2E] = 8'h35; //5 
        mem[8'h36] = 8'h36; //6 
        mem[8'h3D] = 8'h37; //7 
        mem[8'h3E] = 8'h38; //8 
        mem[8'h46] = 8'h39; //9 
    end

endmodule
