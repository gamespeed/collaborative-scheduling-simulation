my $dag_num=0;
my $dag=0;
my $mac_num=0;
my $mac=0;
my $dag_file_name;
my $mac_file_name;

for($mac=1;$mac<=$mac_num;$mac_num++)
{
	$mac_file_name = "Machine.in$mac.txt";
	for($dag=1;$dag<=1000;$dag++)
	{
		$dag_file_name = "DAG.in$dag.txt";
		system("echo $file_name");
		system("echo $file_name >>result.txt");
		system("main_cpp_unprotected.exe $mac_file_name $dag_file_name >> result.txt");
	}
}