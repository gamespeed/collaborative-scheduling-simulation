%Machine Generation

%生成计算机节点的数据
%假定每个计算机节点均为4核心CPU+2块GPU

clear all

N = 10; %输出测试用例的数量为10
P = 10; %计算机节点数量为10

CPUMu = 100; %CPU速度均值为100
GPUMu = 300; %GPU速度均值为300
CPUBandMu = 50; %CPU带宽均值为50
GPUBandMu = 30; %GPU带宽均值为30

PSigma = 0; %方差为0

CPUSpeed = abs((normrnd(CPUMu,PSigma,[N,P])));
GPUSpeed = abs((normrnd(GPUMu,PSigma,[N,P])));
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
		fprintf(MachineFile, '%d\n', 4);
		fprintf(MachineFile, '%f %f %f %f \n', CPUSpeed(fi), CPUSpeed(fi), CPUSpeed(fi), CPUSpeed(fi));
		fprintf(MachineFile, '%d\n', 2);
		fprintf(MachineFile, '%f %f\n', GPUSpeed(fi), GPUBandwidth(fi));
		fprintf(MachineFile, '%f %f\n', GPUSpeed(fi), GPUBandwidth(fi));
		fprintf(MachineFile, '%f\n', CPUBandwidth(fi));
	end
	fclose(MachineFile);
end