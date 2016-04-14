module ir_receiver(PCLK,ir_recv,start_bit,GOT_HIT,
                    PENABLE,
					PSEL,
					PRESETN,
					PWRITE,
					PREADY,
					PSLVERR,
					PADDR,
					PWDATA,
					PRDATA);

// APB Bus Interface
input PENABLE, PSEL, PRESETN, PWRITE;
input  [31:0] PWDATA;
input  [7:0] PADDR;
output reg [31:0] PRDATA;
output PREADY, PSLVERR;
assign BUS_WRITE_EN = (PENABLE && PWRITE && PSEL);
assign BUS_READ_EN = (!PWRITE && PSEL); 
assign PREADY = 1'b1;
assign PSLVERR = 1'b0;

input PCLK;
input ir_recv;
reg [31:0] count3 = 0;
output reg start_bit;
reg [4:0] bit_pos = 10;
reg [10:0] DATA_RECEIVED = 0;
reg [31:0] count4 = 0;
wire ir_recv_sync;
wire ir_temp;
output reg GOT_HIT = 0;
reg [3:0] count5;
reg [31:0] got_hit_counter = 12000000;
reg [10:0] DATA_WHEN_HIT = 0;

d_flip_flop d1(PCLK,ir_recv,ir_temp);
d_flip_flop d2(PCLK,ir_temp,ir_recv_sync);

//////////SEND DATA///////////
always @(posedge PCLK) begin

    if (got_hit_counter > 15000099)
        got_hit_counter <= 0;
        
    if (got_hit_counter < 15000100) begin
        got_hit_counter <= got_hit_counter + 1;
        GOT_HIT <= 0;
    end

    if (DATA_RECEIVED[10:9] == 2'b10 && DATA_RECEIVED[1:0] == 2'b10 && got_hit_counter > 15000000) begin
        DATA_WHEN_HIT[10:0] <= DATA_RECEIVED[10:0];
        GOT_HIT <= 1;
        got_hit_counter <= 0;
    end

    ///////APB BUS/////
    if (BUS_READ_EN) begin
        PRDATA[10:0] <=  DATA_WHEN_HIT[10:0];
        PRDATA[31:11] <= 0;
        DATA_RECEIVED[10:0] <= 0;
    end
 
    //////////GET START BIT///////////
    if (ir_recv_sync == 0) begin
        count3 <= count3 + 1;
    end
    else begin
        if (count3 != 0) begin
            if (count3 > 85000 && count3 < 115000) begin
                start_bit <= 1;
            end
            count3 <= 0;
        end
    end

    //////////GET DATA//////////////
    if (start_bit == 1) begin
        if (ir_recv_sync == 0) 
            count4 <= count4 + 1;
        else begin
            if (count4 != 0) begin
                if (count4 >= 40000) 
                    DATA_RECEIVED[bit_pos] <= 1;
                else
                    DATA_RECEIVED[bit_pos] <= 0;
                if (bit_pos == 0) begin
                    start_bit <= 0;
                    bit_pos <= 10;
                end
                else
                    bit_pos <= bit_pos - 1;
                count4 <= 0;
            end
        end
    end
end
endmodule
/////////////////////////////////////////// 
module d_flip_flop(clk,data_in,data_out);
    input clk,data_in;
    output reg data_out;
    always @(posedge clk) begin
        data_out <= data_in;
    end
endmodule
///////////////////////////////////////////
