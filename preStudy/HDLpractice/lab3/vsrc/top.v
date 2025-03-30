module top(
  input [3:0] a,
  input [3:0] b, 
  input [2:0] mode, 
  output reg [3:0] res,
  output reg cf,
  output reg of,
  output reg zf,
  output reg [6:0] h,
  output reg [6:0] sign
);

reg [3:0] add_res;
reg [3:0] sub_res;
reg [2:0] add_flag;
reg [2:0] sub_flag;

add add_inst(.a(a), .b(b), .sub(0), .res(add_res), .cf(add_flag[2]), .of(add_flag[1]), .zf(add_flag[0]));
add sub_inst(.a(a), .b(b), .sub(1), .res(sub_res), .cf(sub_flag[2]), .of(sub_flag[1]), .zf(sub_flag[0]));

always@(*) begin
    cf = 0;
    of = 0;
    zf = 0;
    case(mode)
        3'b000: begin 
            res = add_res;
            {cf, of, zf} = add_flag;
        end
        3'b001:begin 
        res[3] = sub_res[3];
            if(sub_res[3] == 1) begin
                res[2:0] = ~sub_res[2:0] + 1'b1;
            end
            else res[2:0] = sub_res[2:0];
                
            {cf, of, zf} = sub_flag;
        end
        3'b010: res = ~a;
        3'b011: res = a & b;
        3'b100: res = a | b;
        3'b101: res = a ^ b;
        3'b110: begin
            if (sub_res[3] == 1) res = 4'b0001;
            else res = 4'b0000;
        end 
        3'b111: begin
            {cf, of, zf} = sub_flag;
            if (zf == 1) res = 4'b0001;
            else res = 4'b0000;
        end 

    endcase
end

seg_sign disp_inst(.a(res), .h(h), .sign(sign));
endmodule
