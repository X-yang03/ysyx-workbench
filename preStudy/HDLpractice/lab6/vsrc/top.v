module top(
    input [7:0] a,
    input clk,
    input rst,
    output [7:0] res,
    output [6:0] seg_left,
    output [6:0] seg_right
);

    reg [7:0] shift_reg;
    reg in;
    assign in = shift_reg[3] ^ shift_reg[2] ^ shift_reg[1] ^ shift_reg[0];
    assign res = shift_reg;
    always @(posedge clk or posedge rst) begin
        if (rst) begin
            if( a == 8'b0 )
                shift_reg <= 8'b00000001;
            else
                shift_reg <= a;
        end else
        begin
        shift_reg <= {in, shift_reg[7:1]};
    end
end
    seg_hex seg1(
        .a(shift_reg[3:0]),
        .h(seg_right)
    );
    seg_hex seg0(
        .a(shift_reg[7:4]),
        .h(seg_left)
    );
endmodule
