function CodeMetrics() {
	 this.metricsArray = {};
	 this.metricsArray.var = new Array();
	 this.metricsArray.fcn = new Array();
	 this.metricsArray.var["blinky.c:rtM_"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\IntelWin64\\blinky\\blinky.c",
	size: 16};
	 this.metricsArray.var["rtDW"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\IntelWin64\\blinky\\blinky.c",
	size: 28};
	 this.metricsArray.var["rtU"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\IntelWin64\\blinky\\blinky.c",
	size: 8};
	 this.metricsArray.var["rtY"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\IntelWin64\\blinky\\blinky.c",
	size: 4};
	 this.metricsArray.fcn["blinky.c:task_5ms_high"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\IntelWin64\\blinky\\blinky.c",
	stack: 24,
	stackTotal: 24};
	 this.metricsArray.fcn["blinky_initialize"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\IntelWin64\\blinky\\blinky.c",
	stack: 0,
	stackTotal: 0};
	 this.metricsArray.fcn["blinky_step"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\IntelWin64\\blinky\\blinky.c",
	stack: 4,
	stackTotal: 28};
	 this.metricsArray.fcn["fabsf"] = {file: "D:\\Downloads\\app3\\matlab_2\\polyspace\\verifier\\cxx\\include\\include-libc\\bits\\mathcalls.h",
	stack: 0,
	stackTotal: 0};
	 this.getMetrics = function(token) { 
		 var data;
		 data = this.metricsArray.var[token];
		 if (!data) {
			 data = this.metricsArray.fcn[token];
			 if (data) data.type = "fcn";
		 } else { 
			 data.type = "var";
		 }
	 return data; }; 
	 this.codeMetricsSummary = '<a href="javascript:void(0)" onclick="return postParentWindowMessage({message:\'gotoReportPage\', pageName:\'blinky_metrics\'});">Global Memory: 56(bytes) Maximum Stack: 24(bytes)</a>';
	}
CodeMetrics.instance = new CodeMetrics();
