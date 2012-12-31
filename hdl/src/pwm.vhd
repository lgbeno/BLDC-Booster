library ieee ;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity pwm is 
port(  
  clk:       in  std_logic;
  rst:       in  std_logic;
  freq:      in  std_logic_vector(15 downto 0);
  duty:      in  std_logic_vector(15 downto 0);
  dead_time: in  std_logic_vector(3 downto 0);
  pwm_out:   out std_logic;
  pwm_out_n: out std_logic
);
end pwm;

architecture rtl of pwm is         
  signal cnt:    std_logic_vector(15 downto 0);
  signal cmp0:   std_logic_vector(15 downto 0);
  signal cmp1:   std_logic_vector(15 downto 0);
  signal cmp2:   std_logic_vector(15 downto 0);
  signal cnt_en: std_logic;
  signal slope:  std_logic;
  begin
    process(clk, rst)
    begin
      if (rst = '1') then
        cnt       <= x"0001";
        cmp0      <= x"03e8";
        cmp1      <= x"00ff";
        cmp2      <= x"00ff";
        cnt_en    <= '1';
        slope     <= '1';
        pwm_out   <= '0';
        pwm_out_n <= '0';
      elsif (clk='1') then
      
        -- Load new registers at the beginning of the cycle
        if (cnt = x"0002") then
          cmp2  <= freq - 1;
          cmp1  <= duty;
          cmp0  <= duty - dead_time(3 downto 0);
          slope <= '1';
          if slope = '0' then
            cnt_en <= '0';
          end if;
        -- Flip the slope of the counter to start counting down
        elsif (cnt = cmp2) then
          slope  <= '0';
          if slope = '1' then
            cnt_en <= '0';
          end if;
        end if;   
        
        -- Compare counter against the low side threshold
        if (cnt < cmp0) then
          pwm_out <= '1';
        else
          pwm_out <= '0';
        end if;
        
        -- Compare counter against the high side threshold
        if (cnt >= cmp1) then
          pwm_out_n <= '1';
        else
          pwm_out_n <= '0';
        end if;  
        
        --Increment (or decrement) the counter based on the slope register
        if (cnt_en = '1') then
          if (slope = '1') then
            cnt <= cnt + '1';
          else
            cnt <= cnt - '1';
          end if;
        else
          cnt_en <= '1';
        end if;
      end if;
    end process;
end rtl;