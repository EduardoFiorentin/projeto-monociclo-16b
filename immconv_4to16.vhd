library IEEE;
use IEEE.std_logic_1164.all;


entity IMMCONV_4to16 is 
port (

    imm4: in std_logic_vector(3 downto 0); 
    
    out_imm16: out std_logic_vector(15 downto 0)

);
end IMMCONV_4to16; 

architecture behav_IMMCONV_4to16 of IMMCONV_4to16 is 
begin

    -- out_imm4 <= "000000000000" & imm4 when imm4(3) = '0' else
    --             "111111111111" & imm4; 

    out_imm16 <=  "000000000000" & imm4(3) & imm4(2) & imm4(1) & imm4(0);

end behav_IMMCONV_4to16; 