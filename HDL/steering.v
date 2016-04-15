module steering(
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
                    SERVOMOTOR1,
                    SERVOMOTOR2,
                    SERVOMOTORPWM,
                    SAMPLEINT);

// APB Bus Interface
input PCLK,PENABLE, PSEL, PRESETN, PWRITE;
input wire [31:0] PWDATA;
input [7:0] PADDR;
output [31:0] PRDATA;
output PREADY, PSLVERR;
output reg SERVOMOTOR1, SERVOMOTOR2;
output SERVOMOTORPWM;
output SAMPLEINT;

reg active;

// Desired Position & Actual Position
reg signed [10:0] des_pos, act_pos;

parameter Pterm = 10;
parameter Iterm = 0.33;
parameter Dterm = 55;

 
assign BUS_WRITE_EN = (PENABLE && PWRITE && PSEL);
assign BUS_READ_EN = (!PWRITE && PSEL); //Data is ready during first cycle to make it availble on the bus when PENABLE is asserted

assign PREADY = 1'b1;
assign PSLVERR = 1'b0;

// Error values
integer E[0:5];

integer motor_power;

reg [31:0] counter = 0;

integer PID;

wire clk1k;

clk_div cd(PCLK, clk1k, SAMPLEINT);

initial
begin
    active = 0;
    des_pos = 512;
    act_pos = 0;
    motor_power = 0;
    E[0] = 0;
    E[1] = 0;
    E[2] = 0;
    E[3] = 0;
    E[4] = 0;
    E[5] = 0;
end

// Reads data from Software
// PWDATA[20] == 1 indicates the data is actual position
// PWDATA[20] == 0 indicates the data is desired position
always @(posedge PCLK)
begin
    if (BUS_WRITE_EN) 
        begin
        if (PWDATA[20] == 1)
        begin
            act_pos[10] <= 0;
            act_pos[9:0] <= PWDATA[9:0];
        end
        else if (active)
        begin
            des_pos[10] <= 0;
            des_pos[9:0] <= PWDATA[9:0];
        end
        
        if (PWDATA[22] == 1)
        begin
            active <= 1;
        end

        if (PWDATA[23] == 1)
        begin
            active <= 0;
        end
    end
end

always @(posedge clk1k)
begin
    // Shift register
    E[4] <= E[3];
    E[3] <= E[2];
    E[2] <= E[1];
    E[1] <= E[0];
    E[0] <= $signed(des_pos - act_pos);

    // Calculates PID feedback control
    PID <= Dterm*(E[0]*4 - E[1] - E[2] - E[3] - E[4]) + (Pterm*E[0]);

    // Set direction
    if (PID[31] == 1 & active)
    begin
        SERVOMOTOR1 <= 1;
        SERVOMOTOR2 <= 0;
        motor_power <= -PID;
    end
    else if (PID[31] == 0 & active)
    begin
        SERVOMOTOR1 <= 0;
        SERVOMOTOR2 <= 1;
        motor_power <= PID;
    end
    else 
    begin
        SERVOMOTOR1 <= 0;
        SERVOMOTOR2 <= 0;
        motor_power <= 0;
    end
end

pwmsteer ps(motor_power, SERVOMOTORPWM, PCLK);

endmodule



module pwmsteer(input wire [31:0] power, output reg pwm_out, input clk);
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

module clk_div(input clk, output reg sclk, output reg interrupt);
    reg [31:0] count;

    always @(posedge clk)
    begin
        if (count >= 100000)
        begin 
            count <= 0;
        end
        else
        begin
            count <= count + 1;
        end

        if (count <= 50000)
        begin
            sclk <= 1;
            interrupt <= 1;
        end
        else 
        begin 
            sclk <= 0; 
            interrupt <= 0;
        end
    end
endmodule
