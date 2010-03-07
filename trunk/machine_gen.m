%第二版代码
%本代码考虑CPU与GPU协同工作的情况，即某一项任务需要同一节点中的CPU和GPU同时协同工作
%也考虑逻辑运算与算术运算的不同，即CPU与GPU的逻辑运算和算术运算的速度不同，一项任务有逻辑运算量和算术运算量


%Machine Generation

%生成计算机节点的数据
%假定每个计算机节点均为4核心CPU+2块GPU

clear all

N = 10; %输出测试用例的数量为10
P = 2; %计算机节点数量

CPU_logic_Mu = 300; %CPU速度均值为100
CPU_arith_Mu = 100; %GPU速度均值为300
GPU_logic_Mu = 100;
GPU_arith_Mu = 300;
CPUBandMu = 50; %CPU带宽均值为50
GPUBandMu = 30; %GPU带宽均值为30

PSigma = 0; %方差为0

CPUSpeed_logic = abs(normrnd(CPU_logic_Mu,PSigma,[N,P]));
CPUSpeed_arith = abs(normrnd(CPU_arith_Mu,PSigma,[N,P]));
GPUSpeed_logic = abs(normrnd(GPU_logic_Mu,PSigma,[N,P]));
GPUSpeed_arith = abs(normrnd(GPU_arith_Mu,PSigma,[N,P]));
CPUBandwidth = abs(normrnd(CPUBandMu,PSigma,[N,P]));
GPUBandwidth = abs(normrnd(GPUBandMu,PSigma,[N,P]));

for fi = 1 : N
	MachineName0 ='Machine.in';
	NameNum = num2str(fi);
	Txt = '.txt';
	MachineName = [MachineName0, NameNum, Txt];
	MachineFile = fopen(MachineName,'wt');
	
	fprintf(MachineFile, '%d\n', P);
	for pi = 1 : P
		fprintf(MachineFile, '%d\n', 2);
		for ii = 1 : 2
			fprintf(MachineFile, '%f %f \n', CPUSpeed_logic(fi), CPUSpeed_arith(fi));
		end
		fprintf(MachineFile, '%d\n', 2);
		fprintf(MachineFile, '%f %f %f\n', GPUSpeed_logic(fi), GPUSpeed_arith(fi), GPUBandwidth(fi));
		fprintf(MachineFile, '%f %f %f\n', GPUSpeed_logic(fi), GPUSpeed_arith(fi), GPUBandwidth(fi));
		fprintf(MachineFile, '%f\n', CPUBandwidth(fi));
	end
	fclose(MachineFile);
end