S        : INSTR [NEWLINE INSTR] #

INST     : push VALUE
         | pop
	     | dump
	     | assert VALUE
		 | add
		 | sub
		 | mul
		 | div
		 | mod
		 | print
		 | exit

COMMENT  : ';' .* NEWLINE

NEWLINE  : '\n'+

VALUE    : int8(N)
         | int16(N)
		 | int32(N)
		 | float(Z)
		 | double(Z)

N        : [-]?[0..9]
Z        : [-]?[0..9]+.[0..9]+
