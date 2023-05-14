struct input{
	float a;
	float b;
	int c;
};

program CALC_PROG{
	version ADD_VERS{
		float calc(input)=1;
	}=1;
}= 0x23451111;
