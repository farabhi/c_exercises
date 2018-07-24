# c_mvm_l2
This is my response to one of the assignments in HPC class. I try to calculate FLOPS for the machine the program is running on. I'm doing that by performing matrix-vector multiplication in C. Matrix values come from .mtx file, vector values are created based on hard coded formula. Timer is set around critical piece of code to use its reporting for FLOPS calculation. For sanity check I calculate l2 norm of the multiplication result.
<p><b>Build:</b> use makefile provided.</p>
<p><b>Run:</b> specify data file as a command line argument.</p>
<p>Something like that:</p>
```
make
/mvm.o data
```
