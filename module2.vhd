--°° Implements a simple Nios II system for the DE2 board.
--°° Inputs: SW7°0 are parallel port inputs to the Nios II system.
--°° CLOCK_50 is the system clock.
--°° KEY0 is the active-low system reset.
--°° Outputs: LEDG7°0 are parallel port outputs from the Nios II system.
--°° SDRAM ports correspond to the signals in Figure 2; their names are those
--°° used in the DE2 User Manual.
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
USE ieee.std_logic_unsigned.all;
ENTITY module2 IS
PORT (
SW : IN STD_LOGIC_VECTOR(17 DOWNTO 0);
KEY : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
CLOCK_50 : IN STD_LOGIC;
LEDR : OUT STD_LOGIC_VECTOR(17 DOWNTO 0);

DRAM_CLK, DRAM_CKE : OUT STD_LOGIC;
DRAM_ADDR : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
DRAM_BA_0, DRAM_BA_1 : BUFFER STD_LOGIC;
DRAM_CS_N, DRAM_CAS_N, DRAM_RAS_N, DRAM_WE_N : OUT STD_LOGIC;
DRAM_DQ : INOUT STD_LOGIC_VECTOR(15 DOWNTO 0);
DRAM_UDQM, DRAM_LDQM : BUFFER STD_LOGIC ;

LCD_DATA : inout STD_LOGIC_VECTOR (7 downto 0);
LCD_ON,LCD_BLON,LCD_EN,LCD_RS,LCD_RW : out STD_LOGIC;

SRAM_LB_N,SRAM_UB_N,SRAM_CE_N,SRAM_OE_N ,SRAM_WE_N : OUT STD_LOGIC;
SRAM_ADDR :INOUT STD_LOGIC_VECTOR(17 downto 0);
SRAM_DQ : inout std_LOGIC_VECTOR (15 downto 0);

VGA_CLK, VGA_HS, VGA_VS, VGA_BLANK, VGA_SYNC : out std_LOGIC;
VGA_B, VGA_G, VGA_R :  out std_LOGIC_VECTOR (9 downto 0);

-- Bahar's audio VHDL 
I2C_SDAT : inout std_logic ;
I2C_SCLK : out std_logic ;

AUD_XCK : out std_logic;
CLOCK_27 : in std_logic;

AUD_ADCDAT : in std_logic;
AUD_ADCLRCK : in std_logic;
AUD_BCLK : in std_logic;
AUD_DACDAT  : out std_logic;
AUD_DACLRCK : in std_logic;

SD_CMD   : inout std_logic                     := 'X';             -- b_SD_cmd
SD_DAT   : inout std_logic                     := 'X';             -- b_SD_dat
SD_DAT3  : inout std_logic                     := 'X';             -- b_SD_dat3
SD_CLK : out   std_logic;
UART_RXD : in std_LOGIC := 'X';
UART_TXD : out std_LOGIC );

END module2;
ARCHITECTURE Structure OF module2 IS	
    component nios_system is
        port (
            clk_clk          : in    std_logic                     := 'X';             -- clk
            reset_reset_n    : in    std_logic                     := 'X';             -- reset_n
				buttons_export	  : in 	 std_logic_vector(3 downto 0)  := (others =>'X'); 				-- buttons 
            switches_export  : in    std_logic_vector(17 downto 0)  := (others => 'X'); -- export
            leds_export      : out   std_logic_vector(17 downto 0);                     -- export
            sdram_wire_addr  : out   std_logic_vector(11 downto 0);                    -- addr
            sdram_wire_ba    : out   std_logic_vector(1 downto 0);                     -- ba
            sdram_wire_cas_n : out   std_logic;                                        -- cas_n
            sdram_wire_cke   : out   std_logic;                                        -- cke
            sdram_wire_cs_n  : out   std_logic;                                        -- cs_n
            sdram_wire_dq    : inout std_logic_vector(15 downto 0) := (others => 'X'); -- dq
            sdram_wire_dqm   : out   std_logic_vector(1 downto 0);                     -- dqm
            sdram_wire_ras_n : out   std_logic;                                        -- ras_n
            sdram_wire_we_n  : out   std_logic;                                        -- we_n
            sdram_clk_clk    : out   std_logic;                                        -- clk
            lcd_data_DATA    : inout std_logic_vector(7 downto 0)  := (others => 'X'); -- DATA
            lcd_data_ON      : out   std_logic;                                        -- ON
            lcd_data_BLON    : out   std_logic;                                        -- BLON
            lcd_data_EN      : out   std_logic;                                        -- EN
            lcd_data_RS      : out   std_logic;                                        -- RS
            lcd_data_RW      : out   std_logic;                                         -- RW

           	sram_DQ              : inout std_logic_vector(15 downto 0) := (others => 'X'); -- DQ
            sram_ADDR            : out   std_logic_vector(17 downto 0);                    -- ADDR
            sram_LB_N            : out   std_logic;                                        -- LB_N
            sram_UB_N            : out   std_logic;                                        -- UB_N
            sram_CE_N            : out   std_logic;                                        -- CE_N
            sram_OE_N            : out   std_logic;                                        -- OE_N
            sram_WE_N            : out   std_logic; 
				
				vga_controller_ext_int_CLK   : out   std_logic;                                        -- CLK
            vga_controller_ext_int_HS    : out   std_logic;                                        -- HS
            vga_controller_ext_int_VS    : out   std_logic;                                        -- VS
            vga_controller_ext_int_BLANK : out   std_logic;                                        -- BLANK
            vga_controller_ext_int_SYNC  : out   std_logic;                                        -- SYNC
            vga_controller_ext_int_R     : out   std_logic_vector(9 downto 0);                     -- R
            vga_controller_ext_int_G     : out   std_logic_vector(9 downto 0);                     -- G
            vga_controller_ext_int_B     : out   std_logic_vector(9 downto 0); 
				
				-- Bahar's audio vhdl 
				audio_video_SDAT : inout std_logic                     := 'X';             -- SDAT
            audio_video_SCLK : out   std_logic;                                        -- SCLK
				
				clocks_audio_clk : out   std_logic;                                        -- clk
            clk_sec_27_clk   : in    std_logic                    := 'X' ;             -- clk
				
            audio_ADCDAT     : in    std_logic                     := 'X';             -- ADCDAT
            audio_ADCLRCK    : in    std_logic                     := 'X';             -- ADCLRCK
            audio_BCLK       : in    std_logic                     := 'X';             -- BCLK
            audio_DACDAT     : out   std_logic;                                        -- DACDAT
            audio_DACLRCK    : in    std_logic                     := 'X' ;             -- DACLRCK
				
								--rs232_RXD : in std_LOGIC ;
				--rs232_TXD : out std_LOGIC;
				
				sdcard_b_SD_cmd   : inout std_logic                     := 'X';             -- b_SD_cmd
            sdcard_b_SD_dat   : inout std_logic                     := 'X';             -- b_SD_dat
            sdcard_b_SD_dat3  : inout std_logic                     := 'X';             -- b_SD_dat3
            sdcard_o_SD_clock : out   std_logic ;
				
            rs232_RXD         : in    std_logic                     := 'X';             -- RXD
            rs232_TXD         : out   std_logic                                         -- TXD
				);
    end component nios_system;
SIGNAL DQM : STD_LOGIC_VECTOR(1 DOWNTO 0);
SIGNAL BA : STD_LOGIC_VECTOR(1 DOWNTO 0);
BEGIN
DRAM_BA_0 <= BA(0);
DRAM_BA_1 <= BA(1);
DRAM_UDQM <= DQM(1);
DRAM_LDQM <= DQM(0);
--°° Instantiate the Nios II system entity generated by the Qsys tool.
NiosII: nios_system
PORT MAP (
clk_clk => CLOCK_50,
reset_reset_n => KEY(0),
buttons_export => KEY, 
sdram_clk_clk => DRAM_CLK,
leds_export => LEDR,
switches_export => SW,
sdram_wire_addr => DRAM_ADDR,
sdram_wire_ba => BA,
sdram_wire_cas_n => DRAM_CAS_N,
sdram_wire_cke => DRAM_CKE,
sdram_wire_cs_n => DRAM_CS_N,
sdram_wire_dq => DRAM_DQ,
sdram_wire_dqm => DQM,
sdram_wire_ras_n => DRAM_RAS_N,
sdram_wire_we_n => DRAM_WE_N,
lcd_data_DATA => LCD_DATA,
lcd_data_ON => LCD_ON,
lcd_data_BLON => LCD_BLON,
lcd_data_EN => LCD_EN,
lcd_data_RS => LCD_RS,
lcd_data_RW  =>  LCD_RW,


sram_DQ              => SRAM_DQ,              --           sram_ext_int.DQ
sram_ADDR            => SRAM_ADDR,            --                       .ADDR
sram_LB_N            => SRAM_LB_N,            --                       .LB_N
sram_UB_N            => SRAM_UB_N,            --                       .UB_N
sram_CE_N            => SRAM_CE_N,            --                       .CE_N
sram_OE_N            => SRAM_OE_N,            --                       .OE_N
sram_WE_N            => SRAM_WE_N,            --                       .WE_N
vga_controller_ext_int_CLK   => VGA_CLK,   -- vga_controller_ext_int.CLK
vga_controller_ext_int_HS    => VGA_HS,    --                       .HS
vga_controller_ext_int_VS    => VGA_VS,    --                       .VS
vga_controller_ext_int_BLANK => VGA_BLANK, --                       .BLANK
vga_controller_ext_int_SYNC  => VGA_SYNC,  --                       .SYNC
vga_controller_ext_int_R     => VGA_R,     --                       .R
vga_controller_ext_int_G     => VGA_G,     --                       .G
vga_controller_ext_int_B     => VGA_B,

-- Bahar's Audio VHDL
audio_video_SDAT => I2C_SDAT, -- audio_video.SDAT
audio_video_SCLK => I2C_SCLK, --            .SCLK

clocks_audio_clk => AUD_XCK, -- clocks_audio.clk
clk_sec_27_clk   => CLOCK_27,    --   clk_sec_27.clk

audio_ADCDAT     => AUD_ADCDAT,     --       audio.ADCDAT
audio_ADCLRCK    => AUD_ADCLRCK,    --            .ADCLRCK
audio_BCLK       => AUD_BCLK,       --            .BCLK
audio_DACDAT     => AUD_DACDAT,     --            .DACDAT
audio_DACLRCK    => AUD_DACLRCK,     --            .DACLRCK
				--rs232_RXD => UART_RXD,
				--rs232_TXD => UART_TXD,
sdcard_b_SD_cmd   => SD_CMD,   --       sdcard.b_SD_cmd
sdcard_b_SD_dat   => SD_DAT,   --             .b_SD_dat
sdcard_b_SD_dat3  => SD_DAT3,  --             .b_SD_dat3
sdcard_o_SD_clock => SD_CLK,  --             .o_SD_clock

rs232_RXD         => UART_RXD,         --        rs232.RXD
rs232_TXD         => UART_TXD          --             .TXD
);
END Structure;