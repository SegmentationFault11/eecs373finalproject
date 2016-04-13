module ir_gun(PCLK,ir_send,
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
assign BUS_READ_EN = (!PWRITE && PSEL); //Data is ready during first cycle to make it availble on the bus when PENABLE is asserted
assign PREADY = 1'b1;
assign PSLVERR = 1'b0;

input PCLK;
output reg ir_send;
reg [10:0] DATA_TO_SEND;
reg [31:0] count1 = 0;
reg [31:0] count2 = 0;
reg [31:0] count3 = 0;  

always @(posedge PCLK) begin

    if (BUS_WRITE_EN) begin
        DATA_TO_SEND[10:0] <= PWDATA[10:0];
        count3 <= 0;
    end

    if (count3 == 10)
        DATA_TO_SEND[10:0] <= 0;
    else if (count3 > 10)
        count3 <= 0;


    //counter1 reset
    if (count1 > 596*3)
        count1 <= 0;
    else
        count1 <= count1 + 1;
    //counter2 reset
    if (count2 > 30*70*596) begin
        count2 <= 0;
        if (count3 < 10)
            count3 <= count3 + 1;
    end
    else
        count2 <= count2 + 1;

//////GUN FIRE IMPLEMENTAION BIT BY BIT/////////
    //Start bit
    if (count2 < 30*6*596) begin 
        if (count1 < 596)
            ir_send <= 1;
        else
            ir_send <= 0;
    end
    else if (count2 < 30*8*596) begin
        ir_send <= 0;
    end
    //Bit 10
    else if (count2 < 30*13*596) begin
        if (DATA_TO_SEND[10] == 0) begin
            if (count2 < 30*10*596) begin 
                if (count1 < 596)
                    ir_send <= 1;
                else
                    ir_send <= 0;
            end
            else begin
                 ir_send <= 0;
            end
        end
        else begin
            if (count2 < 30*11*596) begin 
                if (count1 < 596)
                    ir_send <= 1;
                else
                    ir_send <= 0;
            end
            else begin
                 ir_send <= 0;
            end
        end
    end
    //Bit 9
    else if (count2 < 30*18*596) begin
        if (DATA_TO_SEND[9] == 0) begin
            if (count2 < 30*15*596) begin 
                if (count1 < 596)
                    ir_send <= 1;
                else
                    ir_send <= 0;
            end
            else begin
                 ir_send <= 0;
            end
        end
        else begin
            if (count2 < 30*16*596) begin 
                if (count1 < 596)
                    ir_send <= 1;
                else
                    ir_send <= 0;
            end
            else begin
                 ir_send <= 0;
            end
        end
    end
    //Bit 8
    else if (count2 < 30*23*596) begin
        if (DATA_TO_SEND[8] == 0) begin
            if (count2 < 30*20*596) begin 
                if (count1 < 596)
                    ir_send <= 1;
                else
                    ir_send <= 0;
            end
            else begin
                 ir_send <= 0;
            end
        end
        else begin
            if (count2 < 30*21*596) begin 
                if (count1 < 596)
                    ir_send <= 1;
                else
                    ir_send <= 0;
            end
            else begin
                 ir_send <= 0;
            end
        end
    end
    //Bit 7
    else if (count2 < 30*28*596) begin
        if (DATA_TO_SEND[7] == 0) begin
            if (count2 < 30*25*596) begin 
                if (count1 < 596)
                    ir_send <= 1;
                else
                    ir_send <= 0;
            end
            else begin
                 ir_send <= 0;
            end
        end
        else begin
            if (count2 < 30*26*596) begin 
                if (count1 < 596)
                    ir_send <= 1;
                else
                    ir_send <= 0;
            end
            else begin
                 ir_send <= 0;
            end
        end
    end
    //Bit 6
    else if (count2 < 30*33*596) begin
        if (DATA_TO_SEND[6] == 0) begin
            if (count2 < 30*30*596) begin 
                if (count1 < 596)
                    ir_send <= 1;
                else
                    ir_send <= 0;
            end
            else begin
                 ir_send <= 0;
            end
        end
        else begin
            if (count2 < 30*31*596) begin 
                if (count1 < 596)
                    ir_send <= 1;
                else
                    ir_send <= 0;
            end
            else begin
                 ir_send <= 0;
            end
        end
    end
    //Bit 5
    else if (count2 < 30*38*596) begin
        if (DATA_TO_SEND[5] == 0) begin
            if (count2 < 30*35*596) begin 
                if (count1 < 596)
                    ir_send <= 1;
                else
                    ir_send <= 0;
            end
            else begin
                 ir_send <= 0;
            end
        end
        else begin
            if (count2 < 30*36*596) begin 
                if (count1 < 596)
                    ir_send <= 1;
                else
                    ir_send <= 0;
            end
            else begin
                 ir_send <= 0;
            end
        end
    end
    //Bit 4
    else if (count2 < 30*43*596) begin
        if (DATA_TO_SEND[4] == 0) begin
            if (count2 < 30*40*596) begin 
                if (count1 < 596)
                    ir_send <= 1;
                else
                    ir_send <= 0;
            end
            else begin
                 ir_send <= 0;
            end
        end
        else begin
            if (count2 < 30*41*596) begin 
                if (count1 < 596)
                    ir_send <= 1;
                else
                    ir_send <= 0;
            end
            else begin
                 ir_send <= 0;
            end
        end
    end
    //Bit 3
    else if (count2 < 30*48*596) begin
        if (DATA_TO_SEND[3] == 0) begin
            if (count2 < 30*45*596) begin 
                if (count1 < 596)
                    ir_send <= 1;
                else
                    ir_send <= 0;
            end
            else begin
                 ir_send <= 0;
            end
        end
        else begin
            if (count2 < 30*46*596) begin 
                if (count1 < 596)
                    ir_send <= 1;
                else
                    ir_send <= 0;
            end
            else begin
                 ir_send <= 0;
            end
        end
    end
    //Bit 2
    else if (count2 < 30*53*596) begin
        if (DATA_TO_SEND[2] == 0) begin
            if (count2 < 30*50*596) begin 
                if (count1 < 596)
                    ir_send <= 1;
                else
                    ir_send <= 0;
            end
            else begin
                 ir_send <= 0;
            end
        end
        else begin
            if (count2 < 30*51*596) begin 
                if (count1 < 596)
                    ir_send <= 1;
                else
                    ir_send <= 0;
            end
            else begin
                 ir_send <= 0;
            end
        end
    end
    //Bit 1
    else if (count2 < 30*58*596) begin
        if (DATA_TO_SEND[1] == 0) begin
            if (count2 < 30*55*596) begin 
                if (count1 < 596)
                    ir_send <= 1;
                else
                    ir_send <= 0;
            end
            else begin
                 ir_send <= 0;
            end
        end
        else begin
            if (count2 < 30*56*596) begin 
                if (count1 < 596)
                    ir_send <= 1;
                else
                    ir_send <= 0;
            end
            else begin
                 ir_send <= 0;
            end
        end
    end
    //Bit 0
    else if (count2 < 30*63*596) begin
        if (DATA_TO_SEND[0] == 0) begin
            if (count2 < 30*60*596) begin 
                if (count1 < 596)
                    ir_send <= 1;
                else
                    ir_send <= 0;
            end
            else begin
                 ir_send <= 0;
            end
        end
        else begin
            if (count2 < 30*61*596) begin 
                if (count1 < 596)
                    ir_send <= 1;
                else
                    ir_send <= 0;
            end
            else begin
                 ir_send <= 0;
            end
        end
    end

    else begin
        ir_send <= 0;
    end 
end
endmodule
