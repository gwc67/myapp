function CodeMetrics() {
	 this.metricsArray = {};
	 this.metricsArray.var = new Array();
	 this.metricsArray.fcn = new Array();
	 this.metricsArray.var["blinky.c:rtM_"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\IntelWin64\\blinky\\blinky.c",
	size: 16};
	 this.metricsArray.var["rtDW"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\IntelWin64\\blinky\\blinky.c",
	size: 24};
	 this.metricsArray.var["rtU"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\IntelWin64\\blinky\\blinky.c",
	size: 12};
	 this.metricsArray.var["rtY"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\IntelWin64\\blinky\\blinky.c",
	size: 2};
	 this.metricsArray.fcn["blinky.c:task_5ms_high"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\IntelWin64\\blinky\\blinky.c",
	stack: 19,
	stackTotal: 19};
	 this.metricsArray.fcn["blinky_initialize"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\IntelWin64\\blinky\\blinky.c",
	stack: 0,
	stackTotal: 0};
	 this.metricsArray.fcn["blinky_step"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\IntelWin64\\blinky\\blinky.c",
	stack: 4,
	stackTotal: 23};
	 this.metricsArray.fcn["floorf"] = {file: "D:\\Downloads\\app3\\matlab_2\\polyspace\\verifier\\cxx\\include\\include-libc\\bits\\mathcalls.h",
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
	 this.codeMetricsSummary = '<a href="javascript:void(0)" onclick="return postParentWindowMessage({message:\'gotoReportPage\', pageName:\'blinky_metrics\'});">Global Memory: 54(bytes) Maximum Stack: 19(bytes)</a>';
	}
CodeMetrics.instance = new CodeMetrics();
