library IEEE;
use IEEE.std_logic_1164.all;


entity IMMGEN is 
port (

    -- imm4: in std_logic_vector(3 downto 0); 
    imm8: in std_logic_vector(7 downto 0); 
    
    -- out_imm4: out std_logic_vector(3 downto 0); 
    out_imm8: out std_logic_vector(15 downto 0)

);
end IMMGEN; 

architecture behav_IMMGEN of IMMGEN is 
begin

    -- out_imm4 <= "000000000000" & imm4 when imm4(3) = '0' else
    --             "111111111111" & imm4; 

    out_imm8 <= "00000000" & imm8(7) & imm8(6) & imm8(5) & imm8(4) & imm8(3) & imm8(2) & imm8(1) & imm8(0) when imm8(7) = '0' else 
                "11111111" & imm8(7) & imm8(6) & imm8(5) & imm8(4) & imm8(3) & imm8(2) & imm8(1) & imm8(0); 

end behav_IMMGEN; 