%Dag generation

 % %生成数据
% % 1.1输入N组用例,
% % //DAG
% % 2.1.输入DAG的节点数P
% % 2.2.有P行，每一行第一个数据是每个节点的CPU权值和GPU权值
% % 2.3.输入边数E，E<=P^2
% % 2.4.有E行数据，每行第一个数据是边的起始节点，第二个是结束节点，第三个是数据量（权值）

clear all
%第二版代码
%本代码考虑CPU与GPU协同工作的情况，即某一项任务需要同一节点中的CPU和GPU同时协同工作
%也考虑逻辑运算与算术运算的不同，即CPU与GPU的逻辑运算和算术运算的速度不同，一项任务有逻辑运算量和算术运算量

%本代码生成几个同样的DAG图，用于增大总任务量

N = 10;%输入1000组用例
% % 2.1.输入DAG的节点数P
P = 10;  %生成DAG图的节点数量
M = 100;%DAG图的重复次数


%%2.2生成第一个数据是每个任务节点的权值，包括CPU的计算量和GPU的计算量
P_CPU_logic_Mu = 1000;%均值为100
P_CPU_arith_Mu = 300;
P_GPU_logic_Mu = 300;
P_GPU_arith_Mu = 1000;
PSigma = 300;%方差20
P_CPU_logic_Weight = abs(normrnd(P_CPU_logic_Mu,PSigma,[N,P]));%均值为100，方差50,生成N组，每组有P个数
P_CPU_arith_Weight = abs(normrnd(P_CPU_arith_Mu,PSigma,[N,P]));
P_GPU_logic_Weight = abs(normrnd(P_GPU_logic_Mu,PSigma,[N,P]));
P_GPU_arith_Weight = abs(normrnd(P_GPU_arith_Mu,PSigma,[N,P]));

%%2.3.输入边数E，E<=P^2
E = 14;
% % 2.4.有E行数据，每行第一个数据是边的起始节点，第二个是结束节点，第三个是权值
%EWeightUnit = 50;%需要传递的数据的数量级
EMu = 100;%均值为50
ESigma = 30;%方差20
EWeight = abs((normrnd(EMu,ESigma,[N,E])));
%建立边
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

% %文件操作
%写DAG部分

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
