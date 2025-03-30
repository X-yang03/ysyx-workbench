module seg_hex(
  input  [3:0] a, // 4-bit input, a[3] is the enable bit, a[2:0] is the data bit
  input en,
  output reg [6:0] h
);
    always@(a) begin
     if(en) begin
        case(a)
            4'b0000: h = 7'b1000000; // 0
            4'b0001: h = 7'b1111001; // 1
            4'b0010: h = 7'b0100100; // 2
            4'b0011: h = 7'b0110000; // 3
            4'b0100: h = 7'b0011001; // 4
            4'b0101: h = 7'b0010010; // 5
            4'b0110: h = 7'b0000010; // 6
            4'b0111: h = 7'b1111000; // 7
            4'b1000: h = 7'b0000000; // 8
            4'b1001: h = 7'b0010000; // 9
            4'b1010: h = 7'b0001000; // A
            4'b1011: h = 7'b0000011; // B
            4'b1100: h = 7'b1000110; // C
            4'b1101: h = 7'b0100001; // D
            4'b1110: h = 7'b0000110; // E
            4'b1111: h = 7'b0001110; // F
            default: h = 7'b1111111; // default to all off
        endcase
    end
    else 
        h = 7'b1111111; // all off
    end
endmodule
