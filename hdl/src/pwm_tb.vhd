library ieee;
use ieee.std_logic_1164.all;

--------------------------------------------------------------------------------
entity  tb_pwm is

end entity ;
--------------------------------------------------------------------------------


architecture Bhv of tb_pwm is
  -----------------------------
  -- Port Signals 
  -----------------------------
signal   clk       : std_logic := '0';
signal   rst       : std_logic := '0';
signal   freq      : std_logic_vector(15 downto 0) := x"01F4";
signal   duty      : std_logic_vector(15 downto 0) := x"00FA";
signal   dead_time : std_logic_vector(3 downto 0)  := x"2";
signal   pwm_out   : std_logic := '0';
signal   pwm_out_n : std_logic := '0';

  
begin  -- architecture Bhv

  -----------------------------
  -- component instantiation 
  -----------------------------
  pwm_INST: entity work.pwm
   generic map(
      cnt_width => 15)
   port map (
      clk       => clk,
      rst       => rst,
      freq      => freq,
      duty      => duty,
      dead_time => dead_time,
      pwm_out   => pwm_out,
      pwm_out_n => pwm_out_n);

	clock : PROCESS
  begin
     wait for 50 ns; clk  <= not clk;
  end PROCESS clock;
  
  StimuliProcess : process
  begin
    wait for 300 ns; rst  <= '1';
    wait for 300 ns; rst  <= '0';
    wait;
  end process StimuliProcess;
  
end architecture Bhv;
