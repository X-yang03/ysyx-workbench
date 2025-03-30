module add(
  input [3:0] a,
  input [3:0] b, 
  input sub,
  output reg [3:0] res,
  output reg cf,
  output reg of,
  output reg zf
);
    reg [3:0] temp;
    always@(*) begin
       case(sub)
           1'b0: begin  
               {cf, res} = a + b; 
               of = (a[3] & b[3] & ~res[3]) | (~a[3] & ~b[3] & res[3]); 
           end
           1'b1: begin 
               temp = ~b + 1'b1;
               {cf, res} = a + temp;
               of = (a[3] & temp[3] & ~res[3]) | (~a[3] & ~temp[3] & res[3]);
           end
        endcase
        zf = ~(|res);
    end
endmodule
    
