// sevenseg 模块为 4 位的 BCD 码至 7 段 LED 的译码器, 
//下面实例化 6 个 LED 数码管的各自译码器。	
	sevenseg LED8_minute_display_high ( minute_display_high, hex5 );
	sevenseg LED8_minute_display_low ( minute_display_low,hex4 );

	sevenseg LED8_second_display_high( second_display_high, hex3 );
	sevenseg LED8_second_display_low ( second_display_low,  hex2 );

	sevenseg LED8_msecond_display_high( msecond_display_high, hex1 );
	sevenseg LED8_msecond_display_low ( msecond_display_low, hex0 );

	always @ (*)
		begin
			if (key_reset == 0)
				begin
					assign led0 <= 1'b1;
					assign start <= 1'b1;
					assign counter_work <= 1'b1;
					assign display <= 1'b1;
				end
			else if (key_start_pause == 0)
					begin
						assign led1 <= 1'b1;
						assign counter_work <= ~counter_work;
						assign display <= ~display;
					end
			else if (key_display_stop == 0)
					begin
						assign led2 <= 1'b1;
						assign display <= ~display;
					end
		end



	always @ (posedge clk or posedge start)//每一个时钟上升沿或者复位开始触发下面的逻辑
 		begin
  			if (start)
  				begin
   					counter_50M <= 0;
   					assign start <= 1'b0;
   				end
  			else if (counter_50M == DELAY_TIME_MIN)
  					counter_50M <= 0
  				else
  					counter_50M <= counter_50M + 1;   				
 		end
  
	always @ (posedge counter_50M) 
		begin
			if(counter_work)//if false, are LEDs on?
				begin
					if(counter_50M[15:0] == DELAY_TIME_10MS)
						begin
							if(msecond_counter_high == 9)
								msecond_counter_high <= 0;
							else
								msecond_counter_high <= msecond_counter_high + 1;
							if(msecond_counter_low == 9)
								msecond_counter_low <= 0;
							else
								msecond_counter_low <= msecond_counter_low + 1;
						end

					else if(counter_50M[25:0] == DELAY_TIME_S)
						begin
							if(second_counter_high == 9)
								second_counter_high <= 0;
							else
								second_counter_high <= second_counter_high + 1;
							if(second_counter_low == 5)
								second_counter_low <= 0;
							else
								second_counter_low <= second_counter_low + 1;
						end
					else if(counter_50M == DELAY_TIME_MIN)
						begin
							if(minute_counter_high == 9)
								minute_counter_high <= 0;
							else
								minute_counter_high <= minute_counter_high + 1;
							if(minute_counter_low == 5)
								minute_counter_low <= 0;
							else
								minute_counter_low <= minute_counter_low + 1;
						end		
					else if(counter_50M == 0)
						begin
							minute_counter_high <= 0;
							minute_counter_low <= 0;
							second_counter_high <= 0;
							second_counter_low <= 0;
							msecond_counter_high <= 0;
							msecond_counter_low <= 0;
						end
					else if(display)
						begin
							minute_display_high <= minute_counter_high;
							minute_display_low <= minute_counter_low;
							second_display_high <= second_counter_high;
							second_display_low <= second_counter_low;
							msecond_display_high <= msecond_counter_high;
							msecond_display_low <= msecond_counter_low;
						end	
				end
		end

endmodule

//4bit 的 BCD 码至 7 段 LED 数码管译码器模块 
module sevenseg ( data, ledsegments);
	input [3:0] data;
	output ledsegments;
	reg [6:0] ledsegments;
	
	always @ (*) 
			case(data)
										// gfe_dcba  // 7 段 LED 数码管的位段编号
										// 654_3210  // DE1-SOC 板上的信号位编号
				0: ledsegments = 7'b100_0000;  // DE1-SOC 板上的数码管为共阳极接法。
				1: ledsegments = 7'b111_1001;
				2: ledsegments = 7'b010_0100;
				3: ledsegments = 7'b011_0000;
				4: ledsegments = 7'b001_1001;
				5: ledsegments = 7'b001_0010;  
				6: ledsegments = 7'b000_0010;
				7: ledsegments = 7'b111_1000;
				8: ledsegments = 7'b000_0000;
				9: ledsegments = 7'b001_0000;
				default: ledsegments = 7'b111_1111;   // 其它值时全灭。
			endcase
endmodule
