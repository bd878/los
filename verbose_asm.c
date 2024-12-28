int test(int n)
{
	int i;
	int total = 0;

	for (i = 0; i < n; i++)
		total += i * i;
	
	return total;
}
