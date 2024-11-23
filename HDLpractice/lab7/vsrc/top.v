module top(
    input ps2_clk,
    input ps2_data,
    input clk,
    input resetn,
    input nextdata_n,
    output [7:0] data,
    output ready,
    output overflow
);
    ps2_keyboard ps2_keyboard_inst(
        .clk(clk),
        .resetn(resetn),
        .ps2_clk(ps2_clk),
        .ps2_data(ps2_data),
   //     .nextdata_n(nextdata_n),
        .data(data),
        .ready(ready),
        .overflow(overflow)
    );
endmodule
