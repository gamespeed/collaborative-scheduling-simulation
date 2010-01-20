open(FILE, "result.txt");
my $record;
my $total;
while($record=<FILE>)
{
	$total = $total + $record;
}
print $total/100;
system("pause");
