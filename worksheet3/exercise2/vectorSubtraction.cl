__kernel
void vectorSub( __global float *a, __global float *b, __global float *c )
{
	// The global id tells us the index of the vector for this thread.
	int gid = get_global_id(0);
	
	// Perform the subtraction.
	c[gid] = a[gid] - b[gid];
}
