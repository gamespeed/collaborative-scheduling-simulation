%Dag generation

 % %��������
% % 1.1����N������,
% % //DAG
% % 2.1.����DAG�Ľڵ���P
% % 2.2.��P�У�ÿһ�е�һ��������ÿ���ڵ��CPUȨֵ��GPUȨֵ
% % 2.3.�������E��E<=P^2
% % 2.4.��E�����ݣ�ÿ�е�һ�������Ǳߵ���ʼ�ڵ㣬�ڶ����ǽ����ڵ㣬����������������Ȩֵ��

clear all
%�ڶ������
%�����뿼��CPU��GPUЭͬ�������������ĳһ��������Ҫͬһ�ڵ��е�CPU��GPUͬʱЭͬ����
%Ҳ�����߼���������������Ĳ�ͬ����CPU��GPU���߼����������������ٶȲ�ͬ��һ���������߼�������������������

%���������ɼ���ͬ����DAGͼ������������������

N = 10;%����1000������
% % 2.1.����DAG�Ľڵ���P
P = 10;  %����DAGͼ�Ľڵ�����
M = 100;%DAGͼ���ظ�����


%%2.2���ɵ�һ��������ÿ������ڵ��Ȩֵ������CPU�ļ�������GPU�ļ�����
P_CPU_logic_Mu = 1000;%��ֵΪ100
P_CPU_arith_Mu = 300;
P_GPU_logic_Mu = 300;
P_GPU_arith_Mu = 1000;
PSigma = 300;%����20
P_CPU_logic_Weight = abs(normrnd(P_CPU_logic_Mu,PSigma,[N,P]));%��ֵΪ100������50,����N�飬ÿ����P����
P_CPU_arith_Weight = abs(normrnd(P_CPU_arith_Mu,PSigma,[N,P]));
P_GPU_logic_Weight = abs(normrnd(P_GPU_logic_Mu,PSigma,[N,P]));
P_GPU_arith_Weight = abs(normrnd(P_GPU_arith_Mu,PSigma,[N,P]));

%%2.3.�������E��E<=P^2
E = 14;
% % 2.4.��E�����ݣ�ÿ�е�һ�������Ǳߵ���ʼ�ڵ㣬�ڶ����ǽ����ڵ㣬��������Ȩֵ
%EWeightUnit = 50;%��Ҫ���ݵ����ݵ�������
EMu = 100;%��ֵΪ50
ESigma = 30;%����20
EWeight = abs((normrnd(EMu,ESigma,[N,E])));
%������
Edge = [1,2;
        1,3;
		1,4;
        1,5;
        2,6;
        3,6;
        4,7;
		4,8;
		4,9;
		5,7;
		6,9;
		7,10;
		8,10;
		9,10;
        ];
Edge = Edge -1;

% %�ļ�����
%дDAG����

for fi = 1 : N
    DAGName0 ='DAG.dup.in';
    NameNum = num2str(fi);
    Txt = '.txt';
    DAGName = [DAGName0, NameNum, Txt];
    DAGFile = fopen(DAGName,'wt');
    fprintf(DAGFile, '%d\n', P*M);
	for mi = 1 : M
		for pi = 1 : P
			fprintf(DAGFile, '%f %f\n', P_CPU_logic_Weight(fi, pi), P_CPU_arith_Weight(fi, pi));
			fprintf(DAGFile, '%f %f\n', P_GPU_logic_Weight(fi, pi), P_GPU_arith_Weight(fi, pi));
		end
	end
    fprintf(DAGFile, '%d\n', E*M);
    
	for mi = 1 : M
		for ei = 1 : E
			fprintf(DAGFile, '%d %d %f\n', Edge(ei,1) + (mi-1)*P, Edge(ei,2) + (mi-1)*P,  EWeight(fi, ei));
		end
	end
%    fprintf(DAGFile, '\n');
    fclose(DAGFile);
end
