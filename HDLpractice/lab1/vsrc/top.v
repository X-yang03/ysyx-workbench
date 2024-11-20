module top(
  input a[7:0],
  input b[1:0],
  output f[1:0]
);
  assign f = (b == 2'b00) ? a[1:0]:
             (b == 2'b01) ? a[3:2]:
             (b == 2'b10) ? a[5:4]:
             a[7:6];

endmodule
