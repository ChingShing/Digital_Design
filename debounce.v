module Light( in_key,out_key,clk,clr);  
  
 input in_key,clk,clr;  
 output out_key; 
 reg delay1,delay2,delay3;  
 always@( posedge clk)//CLK 50M  
 begin  
    if(clr)  
    begin  
        delay1  <= 0;  
        delay2  <= 0;  
        delay3  <= 0;  
    end  
    else  
    begin  
        delay1  <= ~in_key;   
        delay2  <= delay1;  
        delay3  <= delay2;  
    end  
 end  
   
 assign out_key = delay1&delay2&delay3;  
   
endmodule 