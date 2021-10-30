Another important aspect of the analysis of the performance of rOpenCL is its comparison with the approaches presented. Six approaches were identified, and it was only possible to perform a comparison with three of them: clOpenCL, VCL and dOpenCL (version based on C++ Asio library). In fact, the source code for clusterCL and Hybrid Opencl is not available. In the case of Snucl, despite the availability of the code, it was not possible to execute the selected test example.

To make this comparison, three new virtual machines were used, with some changes to the test environment described in the previous tests. Thus, to evaluate clOpenCL, it was necessary to go back to version 4.8 of the kernel, the latter with support for the Open-MX communication library used by clOpenCL. The value of the MTU also had to be modified, from 1500 to 9000 bytes. Two of the three virtual machines were also associated with Gpus, with each of these machines having 2 Gpus.


**As in previous tests, these values may change due to updates. The new results will be presented in early 2022
