module f(input[2:0] in, output[1:0] out);
	always @(*)
	case(in)
		3'b000: out = 2'b00; //apagado continua apagado até reset
		3'b001: out = 2'b10; //verde p amarelo
		3'b010: out = 2'b11; //amarelo p vermelho
		3'b011: out = 2'b01; //vermelho p verde
		
		//casos quando A está ativo//
		3'b100: out = 2'b10;
		3'b101: out = 2'b10;
		3'b110: out = 2'b00;
		3'b111: out = 2'b10;
		
	endcase
endmodule
