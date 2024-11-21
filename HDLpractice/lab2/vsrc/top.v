module top(
  input [7:0] a,
  input en,
  output reg [3:0] f,
  output sig,
  output [6:0] c
);
    integer i;
    always@(a or en)
    begin
        if(en) begin
            f = 0;
            sig = 0;
            for(i=0; i<8; i=i+1)
                if(a[i] == 1) begin
                    f[2:0] = i[2:0]; sig = 1;
                end
            end
            else begin 
                f = 0; sig = 0;
            end
    end 
    assign f[3] =sig;
    seg seg1(.a(f), .h(c));

endmodule
