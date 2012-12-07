library ieee;
use ieee.std_logic_1164.all;

--------------------------------------------------------------------------------
entity  tb_pwm_bldc is

end entity ;
--------------------------------------------------------------------------------


architecture Bhv of tb_pwm_bldc is
  -----------------------------
  -- Port Signals 
  -----------------------------
signal   clk        : std_logic := '0';
signal   rst        : std_logic := '0';
signal   freq       : std_logic_vector(15 downto 0) := x"01F4";
signal   duty       : std_logic_vector(15 downto 0) := x"004B";
signal   dead_time  : std_logic_vector(3 downto 0)  := x"2";
signal   pwm_out    : std_logic := '0';
signal   pwm_out_n  : std_logic := '0';
signal   bldc_state : std_logic_vector(2 downto 0)  := "011";
signal   pwm_mode   : std_logic_vector(1 downto 0)  := "00";
signal   phase_a_h  : std_logic := '0';
signal   phase_a_l  : std_logic := '0';
signal   phase_b_h  : std_logic := '0';
signal   phase_b_l  : std_logic := '0';
signal   phase_c_h  : std_logic := '0';
signal   phase_c_l  : std_logic := '0';

  
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
      
  bldc_mux_INST: entity work.bldc_mux
   port map (
      clk        => clk,
      rst        => rst,
      pwm        => pwm_out,
      pwm_n      => pwm_out_n,
      bldc_state => bldc_state,
      pwm_mode   => pwm_mode,
      phase_a_h  => phase_a_h,
      phase_a_l  => phase_a_l,
      phase_b_h  => phase_b_h,
      phase_b_l  => phase_b_l,
      phase_c_h  => phase_c_h,
      phase_c_l  => phase_c_l);

	clock : PROCESS
  begin
     wait for 50 ns; clk  <= not clk;
  end PROCESS clock;
  
  StimuliProcess : process
  begin
    wait for 300 ns; rst  <= '1';
    wait for 300 ns; rst  <= '0';
    wait for 500000 ns; bldc_state <= "011";
    wait for 500000 ns; bldc_state <= "001";
    wait for 500000 ns; bldc_state <= "101";
    wait for 500000 ns; bldc_state <= "100";
    wait for 500000 ns; bldc_state <= "110";
    wait for 500000 ns; bldc_state <= "010";
    pwm_mode <= "01";
    wait for 500000 ns; bldc_state <= "011";
    wait for 500000 ns; bldc_state <= "001";
    wait for 500000 ns; bldc_state <= "101";
    wait for 500000 ns; bldc_state <= "100";
    wait for 500000 ns; bldc_state <= "110";
    wait for 500000 ns; bldc_state <= "010";
    pwm_mode <= "10";
    wait for 500000 ns; bldc_state <= "011";
    wait for 500000 ns; bldc_state <= "001";
    wait for 500000 ns; bldc_state <= "101";
    wait for 500000 ns; bldc_state <= "100";
    wait for 500000 ns; bldc_state <= "110";
    wait for 500000 ns; bldc_state <= "010";
    wait;
  end process StimuliProcess;
  
end architecture Bhv;
