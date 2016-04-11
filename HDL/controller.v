module PS2controller( 
/*** APB3 BUS INTERFACE ***/
input PCLK, // clock
input PRESERN, // system reset
input PSEL, // peripheral select
input PENABLE, // distinguishes access phase
output wire PREADY, // peripheral ready signal
output wire PSLVERR, // error signal
input PWRITE, // distinguishes read and write cycles
input [31:0] PADDR, // I/O address
input wire [31:0] PWDATA, // data from processor to I/O device (32 bits)
output reg [31:0] PRDATA, // data to processor from I/O device (32-bits)

/*** I/O PORTS DECLARATION ***/
input DATA,
output wire COMMAND,
output wire ATTENTION,
output wire PS2_CLOCK
);

assign BUS_WRITE_EN = (PENABLE && PWRITE && PSEL);
assign BUS_READ_EN = (!PWRITE && PSEL);

assign PREADY = 1'b1;
assign PSLVERR = 1'b0;

wire CLK;
reg [31:0] BUTTONS = 0;
reg [31:0] OLD_BUTTONS = 0;

ps2Generator  g3(PCLK, CLK, ATTENTION, PS2_CLOCK);
cmdGenerator  g2(CLK, ATTENTION, COMMAND);
attGenerator  g1(CLK, ATTENTION);
clockDivider2 d1(PCLK, ATTENTION, CLK);

//BEGIN DATA -> register//
reg [10:0] buttonsOffset = 0;
reg [10:0] bitNumber = 0;

always @(posedge PS2_CLOCK) begin 
    if ((bitNumber >= 24 && bitNumber <= 47) || (bitNumber >= 64 && bitNumber <= 71)) begin
        BUTTONS[buttonsOffset] <= DATA;
        buttonsOffset <= buttonsOffset + 1;
    end

    if (bitNumber == 71) begin
        bitNumber <= 0;
        buttonsOffset <= 0;
        OLD_BUTTONS <= BUTTONS;
    end
    else begin
        bitNumber <= bitNumber + 1;
    end
end
//END DATA -> register//

reg [31:0] ps2InterruptCounter = 0;
reg temp = 0;
always @(posedge PCLK) begin
    if (BUS_READ_EN) begin
        PRDATA <= OLD_BUTTONS;
    end
end

endmodule


//PS2_CLOCK GENERATOR//////////////////////////////////////////////
module ps2Generator (CLOCK_IN, CLK, ATT, CLOCK_OUT);
    input CLOCK_IN, CLK, ATT;
    output reg CLOCK_OUT;

    always @(negedge CLOCK_IN) begin
        if (ATT == 0) begin
         CLOCK_OUT = CLK;   
    end
    else begin
        CLOCK_OUT = 1;
    end
    end
endmodule

//COMMAND GENERATOR//////////////////////////////////////////////
module cmdGenerator (CLOCK_IN, ATT, CLOCK_OUT);
    input CLOCK_IN, ATT;
    output reg CLOCK_OUT;
    reg [6:0] counter = 0;

    always @(negedge CLOCK_IN) begin
        if (ATT == 0) begin
            counter = counter + 1;
            if (counter == 0 || counter == 9 || counter == 14) begin
                CLOCK_OUT = 1;
            end
            else if (counter < 72)begin
                CLOCK_OUT = 0;
            end 
            else begin
                CLOCK_OUT = 1;
            end
        end
        else begin
            counter = 0;
        end         
    end
endmodule

//ATTENTION GENERATOR//////////////////////////////////////////////
module attGenerator (CLOCK_IN, CLOCK_OUT);
    input CLOCK_IN;
    output reg CLOCK_OUT;
    reg [13:0]counter = 0;

    always @(negedge CLOCK_IN) begin 
        if (counter < 2500) begin
            CLOCK_OUT = 1;
            counter = counter + 1;
        end
        else if (counter < 2572) begin
            CLOCK_OUT = 0;
            counter = counter + 1;
        end
        else begin
            CLOCK_OUT = 1;
            counter = 0;
        end
    end
endmodule

//CLOCK  DIVIDER PCLK -> PS2_CLOCK//////////////////////////////////////////////
module clockDivider2 (CLOCK_IN, ATT, CLOCK_OUT);
    input CLOCK_IN, ATT;
    output reg CLOCK_OUT;
    reg [15:0]counter = 0;

    always @(negedge CLOCK_IN) begin 

        if (counter  < 1000) begin
            CLOCK_OUT = 0;
            counter = counter + 1;
        end
        else if (counter < 2000) begin
            CLOCK_OUT = 1;
            counter = counter + 1;
        end
        else begin
            CLOCK_OUT = 1;
            counter = 0;
        end
    end
endmodule

 