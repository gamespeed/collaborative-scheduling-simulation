#include <iostream.h>
#include <fstream.h>

#define MAX_CPU_NUM 4
#define MAX_GPU_NUM 2

#define MAX_NODE_NUM 1000
#define MAX_TASK_NUM 1000
#define MAX_DEPENDENCY_NUM 100000

class task
{
	public:
		task(int comp_cpu, int comp_gpu);
		void add_dependency();
		void del_dependency();
		int  check_dependency();
		int check_ready_time();
	private:
		int computation_cpu;
		int computation_gpu;
		int dependency;
		int assign_node;
		int ready_time;
		int begin_time;
		int finish_time;
}

class node
{
	public:
		node();
		void set_cpu_num(int m_cpu_num);
		void set_cpu_speed(int cpu_num, int m_cpu_speed);
		void set_eth_bandwidth(int m_eth_bandwidth);
		void set_gpu_speed(int gpu_num, int m_gpu_speed);
		void set_gpu_bandwidth(int gpu_num, int m_gpu_bandwidth);
		void assign(task mTask);
	private:
		int cpu_speed[MAX_CPU_NUM];
		int cpu_ready_time[MAX_CPU_NUM];
		int cpu_num;
		int eth_bandwidth;
		int gpu_speed[MAX_GPU_NUM];
		int gpu_ready_time[MAX_GPU_NUM];
		int gpu_bandwidth[MAX_GPU_NUM];
		int gpu_num;
}

class main
{
	public:
		main()
		{
			task_num = 0;
			node_num = 0;
		}
		~main()
		{}
		void readin_data();
	private:
		task *pTask[MAX_TASK_NUM];
		int task_num;
		node *pNode[MAX_NODE_NUM];
		int node_num;
}

task::task(int comp_cpu, int comp_gpu)
{
	computation_cpu = comp_cpu;
	computation_gpu = comp_gpu;
	dependency = 0;
	assign_node = -1;
	ready_time = 0;
	begin_time = -1;
	finish_time = -1;
}

void task::add_dependency()
{
	dependency++;
}

void task:del_dependency()
{
	if(dependency <= 0)
		cout<<"dependency below 0";
	dependency--;
}

int task::check_dependency()
{
	return dependency;
}

int task::check_ready_time()
{
	return ready_time;
}

node::node()
{
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
}

void node::set_cpu_num(int m_cpu_num)
{
	cpu_num = m_cpu_num;
}

void node::set_cpu_speed(int cpu_num, int m_cpu_speed)
{
	cpu_speed[cpu_num] = m_cpu_speed;
}

void node::set_eth_bandwidth(int m_eth_bandwidth)
{
	eth_bandwidth = m_eth_bandwidth;
}

void node::set_gpu_speed(int gpu_num, int m_gpu_speed)
{
	gpu_speed[gpu_num] = m_gpu_speed;
}

void node::set_gpu_bandwidth(int gpu_num, int m_gpu_bandwidth)
{
	gpu_bandwidth[gpu_num] = m_gpu_bandwidth;
}

void node::assign(task mTask)
{
	
}

void main::readin_data()
{
	istream i_file;
	i_file.open("data.in");
	i_file >> node_num;
	int temp_cpu_num, temp_gpu_num;
	int temp_cpu_speed, temp_gpu_speed;
	int temp_eth_bandwidth, temp_gpu_bandwidth;
	for(int i=0;i<node_num;i++)
	{
		pNode[i] = new node();
		i_file >> temp_cpu_num;
		pNode[i]->set_cpu_num(temp_cpu_num);
		for(int j=0;j<)
		
	}
}

int main()
{
	
}