module top(
  input [7:0] a,
  input [1:0] b,
  output [1:0] f
);
  assign f = (b == 2'b00) ? a[1:0]:
             (b == 2'b01) ? a[3:2]:
             (b == 2'b10) ? a[5:4]:
             a[7:6];

endmodule
