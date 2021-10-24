# rOpenCL - remote OpenCL

The efficient exploitation of heterogeneous systems, and assurance of code portability, depends on open standards, such as OpenCL, which support co-processors of virtually any type. In addition to these standards, there are proprietary standards, device-oriented frameworks such as the NVIDIA CUDA framework for GPUs.

Shared by all these approaches is the fact that originally only provide for the use of local co-processors, connected to a single host system, not allowing the exploration of accelerators connected to other systems, accessible via the network, thereby limiting the potential for applications to accelerate.

The rOpenCL responds to this limitation. As show the figure bellow, through a middleware and services that together allow an OpenCL application (even pre-compiled) to transparently and efficiently explore the set of available accelerators in a distributed Linux system environment, using portable BSD socket-based communication.

![arquiteture-v6-1](https://user-images.githubusercontent.com/64751470/138589891-b0abe05f-4948-4aba-a8a5-19494cbf804b.png)


Performs the following benchmarks without changes:
<ul><li>Babel Stream</li><li>CL_Mem</li><li>CL_Peak</li><li>FinanceBench</li><li>PolyBench</li></ul>

Implements about 70% of OpenCL 1.2 functions.
For now it is only possible to use on Linux / Unix systems.

<br/><h3>more informations soon</h3>
