module top(
    input ps2_clk,
    input ps2_data,
    input clk,
    input rst,
    input nextdata_n,
    output [7:0] data,
    output [7:0] ascii,
    output [7:0] key_cnt,

    output ready,
    output overflow,
    output [6:0] seg_data_l,
    output [6:0] seg_data_h,
    output [6:0] seg_ascii_l, 
    output [6:0] seg_ascii_h,
    output [6:0] seg_key_l,
    output [6:0] seg_key_h
);
    ps2_keyboard ps2_keyboard_inst(
        .clk(clk),
        .resetn(~rst),
        .ps2_clk(ps2_clk),
        .ps2_data(ps2_data),
   //     .nextdata_n(nextdata_n),
        .data(data),
        .key_cnt(key_cnt),
        .ready(ready),
        .overflow(overflow)
    );
    seg_hex seg0(.a(data[3:0]), .en(ready), .h(seg_data_l));
    seg_hex seg1(.a(data[7:4]), .en(ready), .h(seg_data_h));
    rom mem(
        .addr(data),
        .data(ascii)
    );
    seg_hex seg2(.a(ascii[3:0]), .en(ready), .h(seg_ascii_l));
    seg_hex seg3(.a(ascii[7:4]), .en(ready), .h(seg_ascii_h));

    seg_hex seg4(.a(key_cnt[3:0]), .en(1), .h(seg_key_l)); 
    seg_hex seg5(.a(key_cnt[7:4]), .en(1), .h(seg_key_h));
endmodule
