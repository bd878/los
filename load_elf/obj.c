int add5(int num)
{
	return num + 5;
}

int add10(int num)
{
	num = add5(num);
	return add5(num);
}

static int var = 5;

const char *get_hello(void)
{
	return "hello, world!";
}

int get_var(void)
{
	return var;
}

void set_var(int num)
{
	var = num;
}

