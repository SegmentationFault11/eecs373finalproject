module rearmotor(
					PCLK,
					PENABLE,
					PSEL,
					PRESETN,
					PWRITE,
					PREADY,
					PSLVERR,
					PADDR,
					PWDATA,
					PRDATA,
                    REARMOTORPWM,
                    REARMOTOR1,
                    REARMOTOR2);

// APB Bus Interface
input PCLK,PENABLE, PSEL, PRESETN, PWRITE;
input [31:0] PWDATA;
input [7:0] PADDR;
output [31:0] PRDATA;
output PREADY, PSLVERR;
output REARMOTORPWM;
output reg REARMOTOR1, REARMOTOR2;
 
assign BUS_WRITE_EN = (PENABLE && PWRITE && PSEL);
assign BUS_READ_EN = (!PWRITE && PSEL); //Data is ready during first cycle to make it availble on the bus when PENABLE is asserted

assign PREADY = 1'b1;
assign PSLVERR = 1'b0;

reg active;

reg [7:0] inval;
reg [7:0] motor_power;

always @(posedge PCLK)
begin
    if (BUS_WRITE_EN) 
    begin
        if (active)
        begin
            inval[7:0] <= PWDATA[7:0];
        end
        
        if (PWDATA[21] == 1)
        begin
            active <= 1;
        end

        if (PWDATA[22] == 1)
        begin
            active <= 0;
        end
    end

    if (active & inval > 128)
    begin
        REARMOTOR1 <= 1;
        REARMOTOR2 <= 0;
        motor_power <= (inval - 128)*2;
    end
    else if (active & inval < 128)
    begin
        REARMOTOR1 <= 0;
        REARMOTOR2 <= 1;
        motor_power <= (127 - inval)*2;
    end
    else
    begin
        REARMOTOR1 <= 0;
        REARMOTOR2 <= 0;
        motor_power <= 128;
    end
end

pwmrear pr(motor_power, REARMOTORPWM, PCLK);

endmodule


module pwmrear(input wire [31:0] power, output reg pwm_out, input clk);
    reg [18:0] counter;

    reg [31:0] pulse;

    always @(posedge clk)
    begin
        if (power > 255)
        begin
            pulse <= 200000;
        end
        else
        begin
            pulse <= 784*power; //784
        end

        if (counter >= 200000) // binary 200000 creating 500Hz frequency
        begin
            counter <= 0;
        end
        else
        begin
            counter <= counter + 1;
        end
        if (counter <= pulse)
        begin
            pwm_out <= 1;
        end
        else
        begin
            pwm_out <= 0;
        end
    end
endmodule