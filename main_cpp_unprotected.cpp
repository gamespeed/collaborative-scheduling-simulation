//本代码没有考虑CPU与GPU同时协同工作的情况，也没有考虑逻辑运算和算术运算的不同


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
		task(float comp_cpu=0, float comp_gpu=0);
		void assign(int node_num, int proc, int proc_num);
		float computation_cpu;
		float computation_gpu;
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

		float cpu_speed[MAX_CPU_NUM];
		int cpu_num;
		float eth_bandwidth;
		float gpu_speed[MAX_GPU_NUM];
		float gpu_bandwidth[MAX_GPU_NUM];
		int gpu_num;
		float cpu_ready_time[MAX_CPU_NUM];
		float gpu_ready_time[MAX_GPU_NUM];
		float cpu_utilized_time;
		float gpu_utilized_time;
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
		void readin_data();
		void schedule();
		float eth_transfer_time(int task_n, int node_n);
		void assign(int task_n, int node_n, int proc, int proc_n);
		float exe_time(int task_n, int node_n, int proc, int proc_n);
		void get_ready_time(int task_n);
	private:
		task *pTask[MAX_TASK_NUM];
		int task_num;
		node *pNode[MAX_NODE_NUM];
		int node_num;
		dependency *pDependency[MAX_DEPENDENCY_NUM];
		int dependency_num;
};

task::task(float comp_cpu, float comp_gpu)
{
	computation_cpu = comp_cpu;
	computation_gpu = comp_gpu;
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
		cpu_speed[i] = 0;
		cpu_ready_time[i] = 0;
	}
	for(int i=0;i<MAX_GPU_NUM;i++)
	{
		gpu_speed[i] = 0;
		gpu_bandwidth[i] = 0;
		gpu_ready_time[i] = 0;
	}
	eth_bandwidth = 0;
	cpu_utilized_time = 0;
	gpu_utilized_time = 0;
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

void main_class::readin_data()
{	//从文件中读取测试数据
	ifstream i_file;
	i_file.open("Machine.in1.txt");
//	i_file.open("data.in", ifstream::in);
	
	i_file >> node_num;
//	cout << "node_num:" << node_num << endl;
	for(int i=0;i<node_num;i++)
	{
		pNode[i] = new node();
		i_file >> pNode[i]->cpu_num;
		for(int j=0;j<pNode[i]->cpu_num;j++)
		{
			i_file >> pNode[i]->cpu_speed[j];
//			cout << pNode[i]->cpu_speed[j] << " ";
		}
		i_file >> pNode[i]->gpu_num;
//		cout << pNode[i]->gpu_num << endl;
		for(int j=0;j<pNode[i]->gpu_num;j++)
		{
			i_file >> pNode[i]->gpu_speed[j] >> pNode[i]->gpu_bandwidth[j];
		}
		i_file >> pNode[i]->eth_bandwidth;
	}
	
	i_file.close();
	i_file.open("DAG.in1.txt");
	
	i_file >> task_num;
//	cout << "task_num" << task_num << endl;
	for(int i=0;i<task_num;i++)
	{
		pTask[i] = new task();
		i_file >> pTask[i]->computation_cpu >> pTask[i]->computation_gpu;
	}
	
	i_file >> dependency_num;
//	cout << "dependency_num:" << dependency_num << endl;
	for(int i=0;i<dependency_num;i++)
	{
		pDependency[i] = new dependency();
		i_file >> pDependency[i]->output_task >> pDependency[i]->input_task >> pDependency[i]->data_volume;
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
 
float main_class::exe_time(int task_n, int node_n, int proc, int proc_n)
{	//返回计算消耗的时间长度
	if (proc==0)	//CPU execution
	{
		return pTask[task_n]->computation_cpu / pNode[node_n]->cpu_speed[proc_n];
	}
	else		//GPU execution
	{
//		return gpu_exe_time(task_n, node_n, proc, proc_n);
//现在只是简单将GPU传输时间与计算时间相加，以后可以通过gpu_exe_time函数做的复杂些
		return pTask[task_n]->computation_gpu / pNode[node_n]->gpu_speed[proc_n] + pTask[task_n]->data_volume / pNode[node_n]->gpu_bandwidth[proc_n];
	}
}

float main_class::eth_transfer_time(int task_n, int node_n)
{	//返回所有前驱节点的数据通过网络传输到计算节点的时间点
	float max_time, temp_max_time;
	max_time = pTask[task_n]->ready_time;
	for(int i=0;i<dependency_num;i++)
	{
		if(pDependency[i]->input_task == node_n)
		{
			int temp_output_task = pDependency[i]->output_task;
			int temp_output_node = pTask[temp_output_task]->assign_node;
			float temp_bandwidth = (pNode[node_n]->eth_bandwidth > pNode[temp_output_node]->eth_bandwidth) ? 
										pNode[temp_output_node]->eth_bandwidth : pNode[node_n]->eth_bandwidth;
			if (pTask[temp_output_task]->finish_time < 0)
				cout << "Have not finish error, cannot execute  main:eth_transfer_time" << endl;
				//上面这句是为了检错
			if (task_n != temp_output_task)
			{
				temp_max_time = pTask[temp_output_task]->finish_time + pDependency[i]->data_volume / temp_bandwidth;
			}else
			{	//同一节点内数据传输时间为零
				temp_max_time = 0;
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

void main_class::assign(int task_n, int node_n, int proc, int proc_n)
{
	if (pTask[task_n]->dependency != 0)
		cout << "Dependency error, cannot execute  main:assign" << endl;
	
	//这里似乎需要简化，因为任务结束时间不可能比数据到达时间更早
	if (pTask[task_n]->data_ready_time > pTask[task_n]->ready_time)
	{
		pTask[task_n]->begin_time = pTask[task_n]->data_ready_time;
	}else
	{
		pTask[task_n]->begin_time = pTask[task_n]->ready_time;
	}
	
	//考虑任务等待处理器的情况
	if(proc == 0)
	{
		if (pNode[node_n]->cpu_ready_time[proc_n] > pTask[task_n]->begin_time)
			pTask[task_n]->begin_time = pNode[node_n]->cpu_ready_time[proc_n];
	}else
	{
		if (pNode[node_n]->gpu_ready_time[proc_n] > pTask[task_n]->begin_time)
			pTask[task_n]->begin_time = pNode[node_n]->gpu_ready_time[proc_n];
	}
	
	pTask[task_n]->assign_node = node_n;
	pTask[task_n]->finish_time = pTask[task_n]->begin_time + exe_time(task_n, node_n, proc, proc_n);
	if (proc == 0)
	{	//CPU执行
		pNode[node_n]->cpu_ready_time[proc_n] = pTask[task_n]->finish_time;
		pNode[node_n]->cpu_utilized_time += pTask[task_n]->finish_time - pTask[task_n]->begin_time;
	}else
	{	//GPU执行
		pNode[node_n]->gpu_ready_time[proc_n] = pTask[task_n]->finish_time;
		pNode[node_n]->gpu_utilized_time += pTask[task_n]->finish_time - pTask[task_n]->begin_time;
	}
//	cout << "task " << task_n << "begin_time:" << pTask[task_n]->begin_time << endl;
//	cout << "task " << task_n << "finish_time:" << pTask[task_n]->finish_time << endl;
//	cout << "task " << task_n << "assigned node:" << node_n << (proc_n>0?" CPU":" GPU") << "core:" << proc_n << endl;
	
	//处理dependency
	for (int i=0;i<dependency_num;i++)
	{
		if (pDependency[i]->output_task == task_n)
		{	//找到task_n的后续节点
			int temp_task_n = pDependency[i]->input_task;
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
	
 	while(task_count > 0)
	{	//找出ready_time最早的任务
		float earlist_ready_time;
		int earlist_ready_task;
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
		
		//分别找出CPU和GPU的ready_time最早的node
		int earlist_ready_cpu_node = 0;
		int earlist_ready_cpu_proc = 0;
		float earlist_ready_cpu_time = pNode[0]->cpu_ready_time[0];
		int earlist_ready_gpu_node = 0;
		int earlist_ready_gpu_proc = 0;
		float earlist_ready_gpu_time = pNode[0]->gpu_ready_time[0];
		for (int i=0;i<node_num;i++)
		{
			for (int j=0;j<pNode[i]->cpu_num;j++)
			{
				if (pNode[i]->cpu_ready_time[j] < earlist_ready_cpu_time)
				{
					earlist_ready_cpu_node = i;
					earlist_ready_cpu_proc = j;
					earlist_ready_cpu_time = pNode[i]->cpu_ready_time[j];
				}
			}
			for (int j=0;j<pNode[i]->gpu_num;j++)
			{
				if (pNode[i]->gpu_ready_time[j] < earlist_ready_gpu_time)
				{
					earlist_ready_gpu_node = i;
					earlist_ready_gpu_proc = j;
					earlist_ready_gpu_time = pNode[i]->gpu_ready_time[j];
				}
			}
		}
		
		if (exe_time(earlist_ready_task, earlist_ready_cpu_node, 0, earlist_ready_cpu_proc) < exe_time(earlist_ready_task, earlist_ready_gpu_node, 1, earlist_ready_gpu_proc))
			assign(earlist_ready_task, earlist_ready_cpu_node, 0, earlist_ready_cpu_proc);//CPU更快
		else
			assign(earlist_ready_task, earlist_ready_gpu_node, 1, earlist_ready_gpu_proc);//GPU更快
		task_count--;
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
		cout << "Node:" << i << " CPU Utilization Rate:" << pNode[i]->cpu_utilized_time / pNode[i]->cpu_num / last_task_time << endl;
		cout << "Node:" << i << " GPU Utilization Rate:" << pNode[i]->gpu_utilized_time / pNode[i]->gpu_num / last_task_time << endl;
	}
	cout << "The makespan :" << last_task_time << endl;
}

//int main(int argc, char *argv[])
int main()
{
//	if (argc != 3)
//		cout << "Parameter number error" << endl;
	main_class *mMain = new main_class();
	mMain->readin_data();
//	cout << "readin_data success" << endl;
	mMain->schedule();
//	cout << "schedule success" << endl;
	delete mMain;
	system("pause");
	return 0;
}
