// ============================================================== 
//
// This stopwatch is just to test the work of LED and KEY on DE1-SOC board.
// The counter is designed by a series mode. / asynchronous mode. 即异步进位
// use "=" to give value to hour_counter_high and so on. 异步操作/阻塞赋值方式 
//
// 3 key: key_reset/系统复位, key_start_pause/暂停计时, key_display_stop/暂停显示 
//
// ==============================================================
module stopwatch_01(clk,key_reset,key_start_pause,key_display_stop,
// 时钟输入 + 3 个按键;按键按下为 0 。板上利用施密特触发器做了一定消抖,效果待测试。
					hex0,hex1,hex2,hex3,hex4,hex5,
// 板上的 6 个 7 段数码管,每个数码管有 7 位控制信号。 
					led0,led1,led2,led3 );
// LED 发光二极管指示灯,用于指示/测试程序按键状态,若需要,可增加。 高电平亮。

	input clk,key_reset,key_start_pause,key_display_stop; 
	output [6:0] hex0,hex1,hex2,hex3,hex4,hex5;
	output led0,led1,led2,led3;
	reg led0,led1,led2,led3;
	
	parameter DELAY_TIME_10MS = 500000;
	parameter DELAY_TIME_S = 50000000;
	parameter DELAY_TIME_MIN = 3000000000;
// 定义三个常量参数。 500000 ->10ms; 50000000 ->1s; 3000000000 ->1min。

// 定义 6 个显示数据(变量)寄存器:
	reg [3:0] minute_display_high; 
	reg [3:0] minute_display_low; 
	reg [3:0] second_display_high; 
	reg [3:0] second_display_low; 
	reg [3:0] msecond_display_high; 
	reg [3:0] msecond_display_low;
	
// 定义 6 个计时数据(变量)寄存器:
	reg [3:0] minute_counter_high; 
	reg [3:0] minute_counter_low; 
	reg [3:0] second_counter_high; 
	reg [3:0] second_counter_low; 
	reg [3:0] msecond_counter_high; 
	reg [3:0] msecond_counter_low;
	
	reg [31:0] counter_50M;   // 计时用计数器, 每个 50MHz 的 clock 为 20ns。
// DE1-SOC 板上有 4 个时钟, 都为 50MHz,所以需要 500000 次 20ns 之后,才是 10ms。

	reg reset_1_time; 				 // 消抖动用状态寄存器 -- for reset KEY
	reg [31:0] counter_reset;		// 按键状态时间计数器
	reg start_1_time;					 //消抖动用状态寄存器 -- for counter/pause KEY
	reg [31:0] counter_start; 		 //按键状态时间计数器
	reg display_1_time;				  //消抖动用状态寄存器 -- for KEY_display_refresh/pause
	reg [31:0] counter_display;    //按键状态时间计数器

	reg display_work; // 显示刷新,即显示寄存器的值 实时 更新为 计数寄存器 的值。 
	reg counter_work; // 计数(计时)工作 状态,由按键 “计时/暂停” 控制。 	
	reg start;	      // 工作状态寄存器
	reg display;      // 工作状态寄存器
	
