%Machine Generation

%���ɼ�����ڵ������
%�ٶ�ÿ��������ڵ��Ϊ4����CPU+2��GPU

clear all

N = 10; %�����������������Ϊ10
P = 10; %������ڵ�����Ϊ10

CPUMu = 100; %CPU�ٶȾ�ֵΪ100
GPUMu = 300; %GPU�ٶȾ�ֵΪ300
CPUBandMu = 50; %CPU�����ֵΪ50
GPUBandMu = 30; %GPU�����ֵΪ30

PSigma = 0; %����Ϊ0

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