system("del result.txt");
system("del result_mkspan.txt");
system("del result_util.txt");

my $dag_num=10;
my $dag=0;
my $mac_num=10;
my $mac=0;
my $dag_file_name;
my $mac_file_name;

for($mac=1;$mac<=$mac_num;$mac++)
{
#	$mac_file_name = "Machine.in"+$mac+".txt";
	for($dag=1;$dag<=$dag_num;$dag++)
	{
		$mac_file_name = "Machine.in".$mac.".txt";
		$dag_file_name = "DAG.dup.in".$dag.".txt";
#		print $mac_file_name;
#		system("echo $file_name");
#		system("echo $file_name >>result.txt");
		print "Processing Machine $mac  DAG $dag\n";
#		system("main_cpp_unprotected.exe $mac_file_name $dag_file_name >> result.txt");
		system("main_cpp_mkspan.exe $mac_file_name $dag_file_name >> result_mkspan.txt");
		system("main_cpp_util.exe $mac_file_name $dag_file_name >> result_util.txt");
	}
}

