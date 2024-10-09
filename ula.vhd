library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity ULA is 
    port(

        z, bge, blt: out std_logic;   
        s : out std_logic_vector(15 downto 0);
	    op : in std_logic_vector(3 downto 0);
        a, b : in std_logic_vector(15 downto 0)
    );
end ULA; 

architecture behav_ULA of ULA is
begin
    s <=    std_logic_vector(to_signed(to_integer(signed(a)) + to_integer(signed(b)), s'length)) when op = "0000" else
            std_logic_vector(to_signed(to_integer(signed(a)) - to_integer(signed(b)), s'length)) when op = "0010" else
            std_logic_vector(to_signed(to_integer(signed(a)) * to_integer(signed(b)), s'length)) when op = "0011" else
            std_logic_vector(to_signed(to_integer(signed(a)) / to_integer(signed(b)), s'length)) when op = "0100" and to_integer(signed(b)) /= 0 else
             b; 
            -- b when op = "1101" or else 
            -- "0000000000000000";
    
    
    z <= '1' when to_integer(signed(a)) - to_integer(signed(b)) = 0 else '0';
    bge <= '1' when to_integer(signed(a)) >= to_integer(signed(b)) else '0';
    blt <= '1' when to_integer(signed(a)) < to_integer(signed(b)) else '0';
end behav_ULA;