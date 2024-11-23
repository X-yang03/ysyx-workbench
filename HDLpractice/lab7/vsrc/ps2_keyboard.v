module ps2_keyboard(

    input clk,resetn,ps2_clk,ps2_data,
//    input nextdata_n,
    output [7:0] data,
    output reg [7:0] key_cnt,
    output reg ready,
    output reg overflow
);

    reg [9:0] buffer;        // ps2_data bits
    reg [7:0] fifo[7:0];     // fifo
    reg [2:0] w_ptr, r_ptr;  // write pointer, read pointer
    reg [3:0] count;  // count ps2_data bits
    reg [2:0] ps2_clk_sync;

    always @(posedge clk) begin
        ps2_clk_sync <=  {ps2_clk_sync[1:0],ps2_clk};
    end

    wire sampling = ps2_clk_sync[2] & ~ps2_clk_sync[1];
    //detect if there is a falling edge on ps2_clk

    always @(posedge clk) begin
        if (resetn == 0) begin // reset
            count <= 0; w_ptr <= 0; r_ptr <= 0; ready <= 0; overflow <= 0; key_cnt <= 0;
        end
        else begin
            if(ready) begin // read to output next data
               // if(nextdata_n == 0) begin
                    r_ptr <= r_ptr + 3'b1;
                    if(w_ptr == (r_ptr +3'b1)) ready <= 0;
               // endt
            end
            if (sampling) begin
              if (count == 4'd10) begin
                if ((buffer[0] == 0) &&  // start bit
                    (ps2_data)       &&  // stop bit
                    (^buffer[9:1])) begin      // odd  parity
                    fifo[w_ptr] <= buffer[8:1];  // store ps2_data
                    if(buffer[8:1] == 8'hF0) key_cnt <= key_cnt +1; // loose a key

                    w_ptr <= w_ptr + 3'b1;
                    ready <= 1;
                    overflow <= overflow | ((w_ptr + 3'b1) == r_ptr);
                    $display("receive %x", buffer[8:1]);
                end
                count <= 0;     // for next
              end else begin
                buffer[count] <= ps2_data;  // store ps2_data
                count <= count + 3'b1;
              end
            end
        end
    end
    assign data = fifo[r_ptr];

endmodule
