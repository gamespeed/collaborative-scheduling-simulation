open(FILE, "result_mkspan.txt");
my $record;
my $total_cpu;
my $total_gpu;
my $test1,$test2;
while($record=<FILE>)
{
	$total = $total + $record;
}
print "Makespan:".$total/100;
print "\n";
#system("pause");

$total_cpu = 0;
$total_gpu = 0;
open(FILE, "result_util.txt");
while($record=<FILE>)
{
#��������������Ϊ�˽�ÿ���е�CPU�����ʺ�GPU������ƥ�����
	if($record =~ /([0-1]\.\d+)\s([0-1]\.?\d*)/)
	{
		$total_cpu = $total_cpu + $1;
		$total_gpu = $total_gpu + $2;
		print $1;
		print "\n";
		print $2;
		print "\n";
	}
#	print $record;
#	$total = $total + $record;
#	($test1, $test2) = $record;
#	print $test1;
#	print "\n";
#	print $test2;
#	$record = <FILE>;
#	$total1 = $total1 + $record;
}
print "CPU:".$total_cpu/200;
print "\n";
print "GPU:".$total_gpu/200;
print "\n";
system("pause");