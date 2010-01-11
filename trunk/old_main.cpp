#include <stdio.h>

#define MAX_CPU_NUM 4
#define MAX_GPU_NUM 2

#define MAX_NODE_NUM 1000
#define MAX_TASK_NUM 1000
#define MAX_DEPENDENCY_NUM 100000

struct task
{
	int computation_cpu;
	int computation_gpu;
	int pre_num;	//未完成的前驱数量
	int assign_node;
	int ready_time;
	int begin_time;
	int finish_time;
};

struct node
{
	int cpu_speed[MAX_CPU_NUM];
	int cpu_ready_time[MAX_CPU_NUM];
	int eth_bandwidth;
	int gpu_speed[MAX_GPU_NUM];
	int gpu_ready_time[MAX_GPU_NUM];
	int gpu_bandwidth[MAX_GPU_NUM];
};

struct dependency
{
	int output_task;
	int input_task;
	int data_volume;
};

void read_data()
{
	fscanf
}

int main()
{

	return 0;
}
