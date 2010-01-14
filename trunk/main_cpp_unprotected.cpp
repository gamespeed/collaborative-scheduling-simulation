/*
第二版代码
本代码考虑CPU与GPU协同工作的情况，即某一项任务需要同一节点中的CPU和GPU同时协同工作
也考虑逻辑运算与算术运算的不同，即CPU与GPU的逻辑运算和算术运算的速度不同，一项任务有逻辑运算量和算术运算量
*/

#include <iostream>
#include <fstream>
#include <windows.h>

#define MAX_CPU_NUM 4
#define MAX_GPU_NUM 2

#define MAX_NODE_NUM 1000
#define MAX_TASK_NUM 1000
#define MAX_DEPENDENCY_NUM 100000

using namespace std;

class task
{
	public:
		task();
		void assign(int node_num, int proc, int proc_num);
		float computation_cpu_logic;
		float computation_cpu_arith;
		float computation_gpu_logic;
		float computation_gpu_arith;
		float data_volume;
		int dependency;
		int assign_node;
		float ready_time;
		float data_ready_time;
		float begin_time;
		float finish_time;
};

class node
{
	public:
		node();

		float cpu_speed_logic[MAX_CPU_NUM];
		float cpu_speed_arith[MAX_CPU_NUM];
		int cpu_num;
		float eth_bandwidth;
		float gpu_speed_logic[MAX_GPU_NUM];
		float gpu_speed_arith[MAX_GPU_NUM];
		float gpu_bandwidth[MAX_GPU_NUM];
		int gpu_num;
		float cpu_ready_time[MAX_CPU_NUM];
		float gpu_ready_time[MAX_GPU_NUM];
		float cpu_utilized_time;
		float gpu_utilized_time;
//		float node_ready_time;
};

class dependency
{
	public:
		dependency();
		int output_task;
		int input_task;
		float data_volume;
};

class main_class
{
	public:
		main_class();
		~main_class();
		void readin_data(char *mac_in, char *dag_in);
		void schedule();
		float eth_transfer_time(int task_n, int node_n);
		void assign(int task_n, int node_n, int proc, int proc_n);
		float exe_time(int task_n, int node_n, int proc_cpu, int proc_gpu);
		void get_ready_time(int task_n);
	private:
		task *pTask[MAX_TASK_NUM];
		int task_num;
		node *pNode[MAX_NODE_NUM];
		int node_num;
		dependency *pDependency[MAX_DEPENDENCY_NUM];
		int dependency_num;
};

task::task()
{
	computation_cpu_logic = 0;
	computation_cpu_arith = 0;
	computation_gpu_logic = 0;
	computation_gpu_arith = 0;
	data_volume = 0;
	dependency = 0;
	assign_node = -1;
	ready_time = 0;
	data_ready_time = 0;
	begin_time = -1;
	finish_time = -1;
//	latest_pre_finish_time = 0;
//	latest_pre = -1;
}
/* 
void task::add_dependency()
{
	dependency++;
}

void task::del_dependency()
{
	if(dependency <= 0)
		cout<<"dependency below 0";
	dependency--;
}

int task::check_dependency()
{
	return dependency;
}

float task::check_ready_time()
{
	return ready_time;
} */

node::node()
{
	cpu_num = 0;
	gpu_num = 0;
	for(int i=0;i<MAX_CPU_NUM;i++)
	{
		cpu_speed_logic[i] = 0;
		cpu_speed_arith[i] = 0;
		cpu_ready_time[i] = 0;
	}
	for(int i=0;i<MAX_GPU_NUM;i++)
	{
		gpu_speed_logic[i] = 0;
		gpu_speed_arith[i] = 0;
		gpu_bandwidth[i] = 0;
		gpu_ready_time[i] = 0;
	}
	eth_bandwidth = 0;
	cpu_utilized_time = 0;
	gpu_utilized_time = 0;
//	node_ready_time = 0;
}

dependency::dependency()
{
	output_task = -1;
	input_task = -1;
	data_volume = 0;
}

main_class::main_class()
{
	task_num = 0;
	node_num = 0;
	dependency_num = 0;
	for(int i=0;i<MAX_TASK_NUM;i++)
	{
		pTask[i] = NULL;
	}
	for(int i=0;i<MAX_NODE_NUM;i++)
	{
		pNode[i] = NULL;
	}
	for(int i=0;i<MAX_DEPENDENCY_NUM;i++)
	{
		pDependency[i] = NULL;
	}
}

main_class::~main_class()
{
	for(int i=0;i<MAX_TASK_NUM;i++)
	{
		delete pTask[i];
	}
	for(int i=0;i<MAX_NODE_NUM;i++)
	{
		delete pNode[i];
	}
	for(int i=0;i<MAX_DEPENDENCY_NUM;i++)
	{
		delete pDependency[i];
	}
}

void main_class::readin_data(char *mac_in, char *dag_in)
{	//从文件中读取测试数据
	ifstream i_file;
	i_file.open(mac_in);
//	i_file.open("Machine.in1.txt");
//	i_file.open("data.in", ifstream::in);
	
	i_file >> node_num;
//	cout << "node_num:" << node_num << endl;
	for(int i=0;i<node_num;i++)
	{
		pNode[i] = new node();
		i_file >> pNode[i]->cpu_num;
		for(int j=0;j<pNode[i]->cpu_num;j++)
		{
			i_file >> pNode[i]->cpu_speed_logic[j] >> pNode[i]->cpu_speed_arith[j];
//			cout << pNode[i]->cpu_speed[j] << " ";
		}
		i_file >> pNode[i]->gpu_num;
//		cout << pNode[i]->gpu_num << endl;
		for(int j=0;j<pNode[i]->gpu_num;j++)
		{
			i_file >> pNode[i]->gpu_speed_logic[j] >> pNode[i]->gpu_speed_arith[j] >> pNode[i]->gpu_bandwidth[j];
		}
		i_file >> pNode[i]->eth_bandwidth;
	}
	
	i_file.close();
//	i_file.open("DAG.in1.txt");
//	i_file.open("DAG.dup.in1.txt");
	i_file.open(dag_in);
	
	i_file >> task_num;
//	cout << "task_num" << task_num << endl;
	for(int i=0;i<task_num;i++)
	{
		pTask[i] = new task();
		i_file >> pTask[i]->computation_cpu_logic >> pTask[i]->computation_cpu_arith;
		i_file >> pTask[i]->computation_gpu_logic >> pTask[i]->computation_gpu_arith;
	}
	
	i_file >> dependency_num;
//	cout << "dependency_num:" << dependency_num << endl;
	for(int i=0;i<dependency_num;i++)
	{
		pDependency[i] = new dependency();
		i_file >> pDependency[i]->output_task >> pDependency[i]->input_task >> pDependency[i]->data_volume;
		
		//修改相关数据
		int temp_input_task = pDependency[i]->input_task;
		pTask[temp_input_task]->dependency++;
		pTask[temp_input_task]->data_volume += pDependency[i]->data_volume;
//		cout << "readin_data" << endl;
//		cout << "dependency++ " << pTask[temp_input_task]->dependency << endl;
	}
	
	i_file.close();
}
/* 
float main_class::gpu_exe_time(int task_n, int node_n, int proc, int proc_n)
{	//GPU的执行时间尚需讨论
	return 0;
}
 */

//找出task_n所有前驱节点都完成的时间
void main_class::get_ready_time(int task_n)
{
	float max_ready_time;
	int temp_output_task;
	max_ready_time = 0;
	for (int i=0;i<dependency_num;i++)
	{
		if (pDependency[i]->input_task == task_n)
		{
			temp_output_task = pDependency[i]->output_task;
			if (pTask[temp_output_task]->finish_time > max_ready_time)
				max_ready_time = pTask[temp_output_task]->finish_time;
		}
	}
	pTask[task_n]->ready_time = max_ready_time;
	return ;
}

float main_class::exe_time(int task_n, int node_n, int proc_cpu, int proc_gpu)
{	//返回计算消耗的时间长度
	//这里做一个简化，认为数据到达本节点后，CPU可以立即开始计算，同时数据通过PCI向GPU传输，
	//GPU得到数据后马上开始计算并在计算后立即传回数据
	//总的计算时间取决于CPU计算时间和GPU的数据传输+计算，二者取最大值
	//这里假设一个任务需要一个节点中的一个CPU和一个GPU同时进行计算
	float cpu_time,gpu_time;
	
	cpu_time = pTask[task_n]->computation_cpu_logic / pNode[node_n]->cpu_speed_logic[proc_cpu]
				+ pTask[task_n]->computation_cpu_arith / pNode[node_n]->cpu_speed_arith[proc_cpu];
	
	if (pNode[node_n]->cpu_speed_logic[proc_cpu]==0 || pNode[node_n]->cpu_speed_arith[proc_cpu]==0)
		cout << "Divided by zero" << endl;
	
	gpu_time = 2 * pTask[task_n]->data_volume / pNode[node_n]->gpu_bandwidth[proc_gpu]
				+ pTask[task_n]->computation_gpu_logic / pNode[node_n]->gpu_speed_logic[proc_gpu]
				+ pTask[task_n]->computation_gpu_arith / pNode[node_n]->gpu_speed_arith[proc_gpu];
	
	if(pNode[node_n]->gpu_bandwidth[proc_gpu]==0 || pNode[node_n]->gpu_speed_logic[proc_gpu]==0 || pNode[node_n]->gpu_speed_arith[proc_gpu]==0)
		cout << "Divided by zero" << endl;
	
	if (cpu_time > gpu_time)
		return cpu_time;
	else
		return gpu_time;
}

float main_class::eth_transfer_time(int task_n, int node_n)
{	//返回所有前驱节点的数据通过网络传输到计算节点的时间点
	//这里允许运行结束的前驱节点马上向后继节点传输数据，而不是等待所有前驱都完成后才开始数据传输
	float max_time, temp_max_time;
	max_time = pTask[task_n]->ready_time;
	for(int i=0;i<dependency_num;i++)
	{
		if(pDependency[i]->input_task == node_n)
		{
			int temp_output_task = pDependency[i]->output_task;
			int temp_output_node = pTask[temp_output_task]->assign_node;
			float temp_bandwidth = (pNode[node_n]->eth_bandwidth > pNode[temp_output_node]->eth_bandwidth)?pNode[temp_output_node]->eth_bandwidth : pNode[node_n]->eth_bandwidth;
			
			if(temp_bandwidth==0)
				cout << "Divided by zero" << endl;
			
			//两个主机之间的带宽去两者带宽的较小者
			if (pTask[temp_output_task]->finish_time < 0)
				cout << "Have not finish error, cannot execute  main:eth_transfer_time" << endl;
				//上面这句是为了检错
			if (node_n != temp_output_node)
			{
				temp_max_time = pTask[temp_output_task]->finish_time + pDependency[i]->data_volume / temp_bandwidth;
			}else
			{	//同一节点内数据传输时间为零
				temp_max_time = pTask[temp_output_task]->finish_time + 0;
			}
			if(temp_max_time > max_time)
			{	
				max_time = temp_max_time;
			}
		}
	}
//	cout << "task:" << task_n << " node:" << node_n << "transfer time:" << max_time << endl;
	return max_time;
}

void main_class::assign(int task_n, int node_n, int proc_cpu, int proc_gpu)
{
	int temp_task_n;
	
	if (pTask[task_n]->dependency != 0)
		cout << "Dependency error, cannot execute  main:assign" << endl;
	
	//这里似乎需要简化，因为任务结束时间不可能比数据到达时间更早
	if (pTask[task_n]->data_ready_time >= pTask[task_n]->ready_time)
	{
		pTask[task_n]->begin_time = pTask[task_n]->data_ready_time;
	}else
	{
		cout << "This is for test use main_class::assign" << endl;
		pTask[task_n]->begin_time = pTask[task_n]->ready_time;
	}
	
	//考虑任务等待处理器的情况
	if(pNode[node_n]->cpu_ready_time[proc_cpu] > pTask[task_n]->begin_time)
	{
		pTask[task_n]->begin_time = pNode[node_n]->cpu_ready_time[proc_cpu];
	}
	if(pNode[node_n]->gpu_ready_time[proc_gpu] > pTask[task_n]->begin_time)
	{
		pTask[task_n]->begin_time = pNode[node_n]->gpu_ready_time[proc_gpu];
	}
		
	pTask[task_n]->assign_node = node_n;
	pTask[task_n]->finish_time = pTask[task_n]->begin_time + exe_time(task_n, node_n, proc_cpu, proc_gpu);

	//CPU执行
	pNode[node_n]->cpu_ready_time[proc_cpu] = pTask[task_n]->finish_time;
	pNode[node_n]->cpu_utilized_time += pTask[task_n]->finish_time - pTask[task_n]->begin_time;

	//GPU执行
	pNode[node_n]->gpu_ready_time[proc_gpu] = pTask[task_n]->finish_time;
	pNode[node_n]->gpu_utilized_time += pTask[task_n]->finish_time - pTask[task_n]->begin_time;

//	cout << "task " << task_n << "begin_time:" << pTask[task_n]->begin_time << endl;
//	cout << "task " << task_n << "finish_time:" << pTask[task_n]->finish_time << endl;
//	cout << "task " << task_n << "assigned node:" << node_n << (proc_n>0?" CPU":" GPU") << "core:" << proc_n << endl;
	
	//处理dependency
	for (int i=0;i<dependency_num;i++)
	{
		if (pDependency[i]->output_task == task_n)
		{	//找到task_n的后续节点
			temp_task_n = pDependency[i]->input_task;
			pTask[temp_task_n]->dependency --;
			if (pTask[temp_task_n]->dependency == 0)
			{
				get_ready_time(temp_task_n);
			}
		}
	}
}

void main_class::schedule()
{
	int task_count;
	task_count = task_num;
	
	float earlist_ready_time;
	int earlist_ready_task;
	int earlist_ready_node = 0, earlist_ready_node_cpu = 0, earlist_ready_node_gpu = 0;
	int earlist_ready_cpu_proc = 0;	//临时变量
	float earlist_ready_cpu_time = pNode[0]->cpu_ready_time[0];	//初始化
	int earlist_ready_gpu_proc = 0;	//临时变量
	float earlist_ready_gpu_time = pNode[0]->gpu_ready_time[0];
	float earlist_ready_node_time;	//临时变量
	
 	while(task_count > 0)
	{	//找出ready_time最早的任务

		earlist_ready_time = -1;
		earlist_ready_task = -1;
		for (int i=0;i<task_num;i++)
		{
//			cout << "task " << i << "dependency: " <<  pTask[i]->dependency << endl;
			if (pTask[i]->dependency == 0 && pTask[i]->finish_time < 0)
			{//ready且未完成的任务
				if (earlist_ready_task < 0)
				{
					earlist_ready_time = pTask[i]->ready_time;
					earlist_ready_task = i;
				}
				if (earlist_ready_time > pTask[i]->ready_time)
				{
					earlist_ready_time = pTask[i]->ready_time;
					earlist_ready_task = i;
				}
			}
		}
		
		if(earlist_ready_task<0)
			cout << "schedule task error: earlist_ready_task=" << earlist_ready_task << endl;
		
		//分别找出ready_time最早的node
		//node的ready_time等于其中最早CPU与最早GPU的ready_time的较大值，即最早何时能同时拿出一个CPU和一个GPU
		//下面的代码需要修改
		earlist_ready_node = 0, earlist_ready_node_cpu = 0, earlist_ready_node_gpu = 0;
		
		earlist_ready_cpu_proc = 0;	//临时变量
		earlist_ready_cpu_time = pNode[0]->cpu_ready_time[0];	//初始化
		
		earlist_ready_gpu_proc = 0;	//临时变量
		earlist_ready_gpu_time = pNode[0]->gpu_ready_time[0];
		
//		earlist_ready_node_time;	//临时变量
		earlist_ready_node_time = earlist_ready_cpu_time>earlist_ready_gpu_time?earlist_ready_cpu_time:earlist_ready_gpu_time;
		for (int i=0;i<node_num;i++)
		{
			for (int j=0;j<pNode[i]->cpu_num;j++)
			{
				if (pNode[i]->cpu_ready_time[j] < earlist_ready_cpu_time)
				{
					earlist_ready_cpu_proc = j;
					earlist_ready_cpu_time = pNode[i]->cpu_ready_time[j];
				}
			}
			for (int j=0;j<pNode[i]->gpu_num;j++)
			{
				if (pNode[i]->gpu_ready_time[j] < earlist_ready_gpu_time)
				{
					earlist_ready_gpu_proc = j;
					earlist_ready_gpu_time = pNode[i]->gpu_ready_time[j];
				}
			}
			if(earlist_ready_cpu_time<earlist_ready_node_time && earlist_ready_gpu_time<earlist_ready_node_time)
			{
				earlist_ready_node = i;
				earlist_ready_node_cpu = earlist_ready_cpu_proc;
				earlist_ready_node_gpu = earlist_ready_gpu_proc;
				if(earlist_ready_cpu_time > earlist_ready_gpu_time)
					earlist_ready_node_time = earlist_ready_cpu_time;
				else
					earlist_ready_node_time = earlist_ready_gpu_time;
			}
		}
		pTask[earlist_ready_task]->data_ready_time = eth_transfer_time(earlist_ready_task, earlist_ready_node);
		assign(earlist_ready_task, earlist_ready_node, earlist_ready_node_cpu, earlist_ready_node_gpu);
		
//		cout << "assign task " << earlist_ready_task << " on node " << earlist_ready_node << 
//			" CPU:" << earlist_ready_node_cpu << " GPU:" << earlist_ready_node_gpu << " Time:" << pTask[earlist_ready_task]->begin_time <<
//			" - " << pTask[earlist_ready_task]->finish_time << endl;
		
		task_count--;
//		cout << "Schedule " << task_num - task_count << " tasks" << endl;
	}
	
	float last_task_time = pTask[0]->finish_time;
	int last_task = 0;
	for (int i=0;i<task_num;i++)
	{
//		cout << "task " << i << "finish_time:" << pTask[i]->finish_time << endl;
		if (pTask[i]->finish_time > last_task_time)
		{
			last_task_time = pTask[i]->finish_time;
			last_task = i;
		}
	}
	for (int i=0;i<node_num;i++)
	{
//		cout << "Node:" << i << " CPU Utilization Rate:" << pNode[i]->cpu_utilized_time / pNode[i]->cpu_num / last_task_time << endl;
//		cout << "Node:" << i << " GPU Utilization Rate:" << pNode[i]->gpu_utilized_time / pNode[i]->gpu_num / last_task_time << endl;
		cout << pNode[i]->cpu_utilized_time / pNode[i]->cpu_num / last_task_time << " ";
		cout << pNode[i]->gpu_utilized_time / pNode[i]->gpu_num / last_task_time << endl;
	}
//	cout << "The makespan :" << last_task_time << endl;
	cout << last_task_time << endl;
}

int main(int argc, char *argv[])
//int main()
{
	if (argc != 3)
		cout << "Parameter number error" << endl;
	main_class *mMain = new main_class();
	mMain->readin_data(argv[1], argv[2]);
//	cout << "readin_data success" << endl;
	mMain->schedule();
//	cout << "schedule success" << endl;
	delete mMain;
//	system("pause");
	return 0;
}
