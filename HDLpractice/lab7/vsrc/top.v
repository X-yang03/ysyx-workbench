module top(
    input ps2_clk,
    input ps2_data,
    input clk,
    input rst,
    input nextdata_n,
    output [7:0] data,
    output ready,
    output overflow,
    output [6:0] seg_data_l,
    output [6:0] seg_data_h
);
    ps2_keyboard ps2_keyboard_inst(
        .clk(clk),
        .resetn(~rst),
        .ps2_clk(ps2_clk),
        .ps2_data(ps2_data),
   //     .nextdata_n(nextdata_n),
        .data(data),
        .ready(ready),
        .overflow(overflow)
    );
    seg_hex seg0(.a(data[3:0]), .h(seg_data_l));
    seg_hex seg1(.a(data[7:4]), .h(seg_data_h));
endmodule
