open(FILE, "result_mkspan.txt");
my $record;
my $total_cpu;
my $total_gpu;
my $test1,$test2;
my $proc_num;
while($record=<FILE>)
{
	$total = $total + $record;
}
print "Makespan:".$total/100;
print "\n";
#system("pause");

$total_cpu = 0;
$total_gpu = 0;
$proc_num = 0;
open(FILE, "result_util.txt");
while($record=<FILE>)
{
#下面这行正则是为了将每行中的CPU利用率和GPU利用率匹配出来
	if($record =~ /([0-1]\.?\d*)\s([0-1]\.?\d*)/)
	{
		$proc_num = $proc_num +1;
		$total_cpu = $total_cpu + $1;
		$total_gpu = $total_gpu + $2;
#		print $1;
#		print "\n";
#		print $2;
#		print "\n";	
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
print "CPU:".$total_cpu/$proc_num;
print "\n";
print "GPU:".$total_gpu/$proc_num;
print "\n";
system("pause");