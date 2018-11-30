module intermitente(input clk, input logic a, output logic verde, output logic vermelho);

integer count;
initial count = 0;

integer auxcount;
initial auxcount = 0;

always @ (posedge clk)
begin

	if(a == 1)begin
	
		if(count == 25000000) begin
				count = 0;
				auxcount = auxcount+1;
		end
		
		else begin
			count = count + 1;
		end
		
		if(auxcount <=7)begin
			verde = 1;
			vermelho = 0;
		end
		
		else if(auxcount >7)begin
			if(auxcount[0]==0 && auxcount[1]==0) begin
				vermelho = ~vermelho;
			end
			verde = 0;
		end
	
		if (auxcount==14)begin
			auxcount = 0;
		end
		
	end
	
	else if(a==0) begin
		vermelho = 0;
		verde = 0;
	end
	
end
endmodule
				
		