%�ڶ������
%�����뿼��CPU��GPUЭͬ�������������ĳһ��������Ҫͬһ�ڵ��е�CPU��GPUͬʱЭͬ����
%Ҳ�����߼���������������Ĳ�ͬ����CPU��GPU���߼����������������ٶȲ�ͬ��һ���������߼�������������������


%Machine Generation

%���ɼ�����ڵ������
%�ٶ�ÿ��������ڵ��Ϊ4����CPU+2��GPU

clear all

N = 10; %�����������������Ϊ10
P = 2; %������ڵ�����

CPU_logic_Mu = 300; %CPU�ٶȾ�ֵΪ100
CPU_arith_Mu = 100; %GPU�ٶȾ�ֵΪ300
GPU_logic_Mu = 100;
GPU_arith_Mu = 300;
CPUBandMu = 50; %CPU�����ֵΪ50
GPUBandMu = 30; %GPU�����ֵΪ30

PSigma = 0; %����Ϊ0

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