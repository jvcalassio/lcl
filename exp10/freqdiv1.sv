module freqdiv1(input clkin, input [1:0]aa, output clkout);

integer cont;
initial cont=0;
integer max;
initial max = 50000000;

always @ (*)
	begin
		if(aa == 2'b01)
			begin
				max = 150000000;
			end
		else if(aa == 2'b10)
			begin
				max = 25000000;
			end
		else if(aa == 2'b11)
			begin
				max = 175000000;
			end
	end
	
always @(posedge clkin)
	begin			
		if(cont==max)
			begin
				cont<=0;
				clkout<=~clkout;
			end
		else
			begin
				cont<= cont+1;
			end
	end
	
endmodule

