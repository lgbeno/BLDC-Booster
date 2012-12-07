--BLDC MUX controls the 6 states of the BLDC commutation in 3 different pwm modes
-- as well as a 4th mode which disables all outputs

library ieee ;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity bldc_mux is 
port(  
  clk:        in  std_logic;
  rst:        in  std_logic;
  pwm:        in  std_logic;
  pwm_n:      in  std_logic;
  bldc_state: in  std_logic_vector(2 downto 0);
  pwm_mode:   in  std_logic_vector(1 downto 0);
  
  phase_a_h:  out std_logic;
  phase_a_l:  out std_logic;
  phase_b_h:  out std_logic;
  phase_b_l:  out std_logic;
  phase_c_h:  out std_logic;
  phase_c_l:  out std_logic
);
end bldc_mux;

architecture rtl of bldc_mux is        
  begin
    process(clk, rst)
    begin
      if (rst = '1') then
        phase_a_h <= '0';
        phase_a_l <= '0';
        phase_b_h <= '0';
        phase_b_l <= '0';
        phase_c_h <= '0';
        phase_c_l <= '0';
      elsif (clk='1') then
        --Bipolar mode muxing
        if pwm_mode = "00" then
          if bldc_state = "011" then
              phase_a_h <= pwm;
              phase_a_l <= pwm_n;
              phase_b_h <= '0';
              phase_b_l <= '0';
              phase_c_h <= pwm_n;
              phase_c_l <= pwm;
          elsif bldc_state = "001" then
              phase_a_h <= '0';
              phase_a_l <= '0';
              phase_b_h <= pwm;
              phase_b_l <= pwm_n;
              phase_c_h <= pwm_n;
              phase_c_l <= pwm;
          elsif bldc_state = "101" then
              phase_a_h <= pwm_n;
              phase_a_l <= pwm;
              phase_b_h <= pwm;
              phase_b_l <= pwm_n;
              phase_c_h <= '0';
              phase_c_l <= '0';
          elsif bldc_state = "100" then
              phase_a_h <= pwm_n;
              phase_a_l <= pwm;
              phase_b_h <= '0';
              phase_b_l <= '0';
              phase_c_h <= pwm;
              phase_c_l <= pwm_n;
          elsif bldc_state = "110" then
              phase_a_h <= '0';
              phase_a_l <= '0';
              phase_b_h <= pwm_n;
              phase_b_l <= pwm;
              phase_c_h <= pwm;
              phase_c_l <= pwm_n;
          elsif bldc_state = "010" then
              phase_a_h <= pwm;
              phase_a_l <= pwm_n;
              phase_b_h <= pwm_n;
              phase_b_l <= pwm;
              phase_c_h <= '0';
              phase_c_l <= '0';
          else
              phase_a_h <= '0';
              phase_a_l <= '0';
              phase_b_h <= '0';
              phase_b_l <= '0';
              phase_c_h <= '0';
              phase_c_l <= '0';
          end if;
        --Uni-polar mode with lowside switching
        elsif pwm_mode = "01" then
          if bldc_state = "011" then
              phase_a_h <= '1';
              phase_a_l <= '0';
              phase_b_h <= '0';
              phase_b_l <= '0';
              phase_c_h <= '0';
              phase_c_l <= pwm;
          elsif bldc_state = "001" then
              phase_a_h <= '0';
              phase_a_l <= '0';
              phase_b_h <= '1';
              phase_b_l <= '0';
              phase_c_h <= '0';
              phase_c_l <= pwm;
          elsif bldc_state = "101" then
              phase_a_h <= '0';
              phase_a_l <= pwm;
              phase_b_h <= '1';
              phase_b_l <= '0';
              phase_c_h <= '0';
              phase_c_l <= '0';
          elsif bldc_state = "100" then
              phase_a_h <= '0';
              phase_a_l <= pwm;
              phase_b_h <= '0';
              phase_b_l <= '0';
              phase_c_h <= '1';
              phase_c_l <= '0';
          elsif bldc_state = "110" then
              phase_a_h <= '0';
              phase_a_l <= '0';
              phase_b_h <= '0';
              phase_b_l <= pwm;
              phase_c_h <= '1';
              phase_c_l <= '0';
          elsif bldc_state = "010" then
              phase_a_h <= '1';
              phase_a_l <= '0';
              phase_b_h <= '0';
              phase_b_l <= pwm;
              phase_c_h <= '0';
              phase_c_l <= '0';
          else
              phase_a_h <= '0';
              phase_a_l <= '0';
              phase_b_h <= '0';
              phase_b_l <= '0';
              phase_c_h <= '0';
              phase_c_l <= '0';
          end if;
        --Uni-polar mode with highside 
        elsif pwm_mode = "10" then
          if bldc_state = "011" then
              phase_a_h <= pwm;
              phase_a_l <= '0';
              phase_b_h <= '0';
              phase_b_l <= '0';
              phase_c_h <= '0';
              phase_c_l <= '1';
          elsif bldc_state = "001" then
              phase_a_h <= '0';
              phase_a_l <= '0';
              phase_b_h <= pwm;
              phase_b_l <= '0';
              phase_c_h <= '0';
              phase_c_l <= '1';
          elsif bldc_state = "101" then
              phase_a_h <= '0';
              phase_a_l <= '1';
              phase_b_h <= pwm;
              phase_b_l <= '0';
              phase_c_h <= '0';
              phase_c_l <= '0';
          elsif bldc_state = "100" then
              phase_a_h <= '0';
              phase_a_l <= '1';
              phase_b_h <= '0';
              phase_b_l <= '0';
              phase_c_h <= pwm;
              phase_c_l <= '0';
          elsif bldc_state = "110" then
              phase_a_h <= '0';
              phase_a_l <= '0';
              phase_b_h <= '0';
              phase_b_l <= '1';
              phase_c_h <= pwm;
              phase_c_l <= '0';
          elsif bldc_state = "010" then
              phase_a_h <= pwm;
              phase_a_l <= '0';
              phase_b_h <= '0';
              phase_b_l <= '1';
              phase_c_h <= '0';
              phase_c_l <= '0';
          else
              phase_a_h <= '0';
              phase_a_l <= '0';
              phase_b_h <= '0';
              phase_b_l <= '0';
              phase_c_h <= '0';
              phase_c_l <= '0';
          end if;
        -- Disable mux output, all fets should be off
        elsif pwm_mode = "11" then
          phase_a_h <= '0';
          phase_a_l <= '0';
          phase_b_h <= '0';
          phase_b_l <= '0';
          phase_c_h <= '0';
          phase_c_l <= '0';
        end if;
      end if;
    end process;
end rtl;