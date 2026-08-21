function CodeMetrics() {
	 this.metricsArray = {};
	 this.metricsArray.var = new Array();
	 this.metricsArray.fcn = new Array();
	 this.metricsArray.var["blinky.c:rtM_"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\ARMCortex-M\\blinky\\blinky.c",
	size: 8};
	 this.metricsArray.var["rtDW"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\ARMCortex-M\\blinky\\blinky.c",
	size: 1504};
	 this.metricsArray.var["rtU"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\ARMCortex-M\\blinky\\blinky.c",
	size: 36};
	 this.metricsArray.var["rtY"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\ARMCortex-M\\blinky\\blinky.c",
	size: 16};
	 this.metricsArray.fcn["asinf"] = {file: "D:\\Downloads\\app3\\matlab_2\\polyspace\\verifier\\cxx\\include\\include-libc\\bits\\mathcalls.h",
	stack: 0,
	stackTotal: 0};
	 this.metricsArray.fcn["atan2f"] = {file: "D:\\Downloads\\app3\\matlab_2\\polyspace\\verifier\\cxx\\include\\include-libc\\bits\\mathcalls.h",
	stack: 0,
	stackTotal: 0};
	 this.metricsArray.fcn["blinky.c:imufilter_stepImpl"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\ARMCortex-M\\blinky\\blinky.c",
	stack: 41,
	stackTotal: 101};
	 this.metricsArray.fcn["blinky.c:quaternionBase_rotmat"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\ARMCortex-M\\blinky\\blinky.c",
	stack: 60,
	stackTotal: 60};
	 this.metricsArray.fcn["blinky.c:quaternion_quaternion"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\ARMCortex-M\\blinky\\blinky.c",
	stack: 36,
	stackTotal: 36};
	 this.metricsArray.fcn["blinky.c:quaternion_quaternion_o"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\ARMCortex-M\\blinky\\blinky.c",
	stack: 32,
	stackTotal: 32};
	 this.metricsArray.fcn["blinky_initialize"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\ARMCortex-M\\blinky\\blinky.c",
	stack: 13,
	stackTotal: 13};
	 this.metricsArray.fcn["blinky_step"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\ARMCortex-M\\blinky\\rtmodel.c",
	stack: 4,
	stackTotal: 108};
	 this.metricsArray.fcn["blinky_step0"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\ARMCortex-M\\blinky\\blinky.c",
	stack: 0,
	stackTotal: 0};
	 this.metricsArray.fcn["blinky_step1"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\ARMCortex-M\\blinky\\blinky.c",
	stack: 3,
	stackTotal: 104};
	 this.metricsArray.fcn["blinky_terminate"] = {file: "D:\\Downloads\\zephyr_new\\myapp\\new\\blinky\\src\\simulink\\ARMCortex-M\\blinky\\blinky.c",
	stack: 0,
	stackTotal: 0};
	 this.metricsArray.fcn["cosf"] = {file: "D:\\Downloads\\app3\\matlab_2\\polyspace\\verifier\\cxx\\include\\include-libc\\bits\\mathcalls.h",
	stack: 0,
	stackTotal: 0};
	 this.metricsArray.fcn["fabsf"] = {file: "D:\\Downloads\\app3\\matlab_2\\polyspace\\verifier\\cxx\\include\\include-libc\\bits\\mathcalls.h",
	stack: 0,
	stackTotal: 0};
	 this.metricsArray.fcn["floorf"] = {file: "D:\\Downloads\\app3\\matlab_2\\polyspace\\verifier\\cxx\\include\\include-libc\\bits\\mathcalls.h",
	stack: 0,
	stackTotal: 0};
	 this.metricsArray.fcn["memcpy"] = {file: "D:\\Downloads\\app3\\matlab_2\\polyspace\\verifier\\cxx\\include\\include-libc\\string.h",
	stack: 0,
	stackTotal: 0};
	 this.metricsArray.fcn["memset"] = {file: "D:\\Downloads\\app3\\matlab_2\\polyspace\\verifier\\cxx\\include\\include-libc\\string.h",
	stack: 0,
	stackTotal: 0};
	 this.metricsArray.fcn["sinf"] = {file: "D:\\Downloads\\app3\\matlab_2\\polyspace\\verifier\\cxx\\include\\include-libc\\bits\\mathcalls.h",
	stack: 0,
	stackTotal: 0};
	 this.metricsArray.fcn["sqrtf"] = {file: "D:\\Downloads\\app3\\matlab_2\\polyspace\\verifier\\cxx\\include\\include-libc\\bits\\mathcalls.h",
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
	 this.codeMetricsSummary = '<a href="javascript:void(0)" onclick="return postParentWindowMessage({message:\'gotoReportPage\', pageName:\'blinky_metrics\'});">Global Memory: 1564(bytes) Maximum Stack: 60(bytes)</a>';
	}
CodeMetrics.instance = new CodeMetrics();
