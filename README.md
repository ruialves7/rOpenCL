# rOpenCL - remote OpenCL

<h4>rOpenCl: What is it?</h4>

The efficient exploitation of heterogeneous systems, and assurance of code portability, depends on open standards, such as OpenCL, which support co-processors of virtually any type. In addition to these standards, there are proprietary standards, device-oriented frameworks such as the NVIDIA CUDA framework for GPUs.

Shared by all these approaches is the fact that originally only provide for the use of local co-processors, connected to a single host system, not allowing the exploration of accelerators connected to other systems, accessible via the network, thereby limiting the potential for applications to accelerate.

![arquiteture-v6-1](https://user-images.githubusercontent.com/64751470/138589891-b0abe05f-4948-4aba-a8a5-19494cbf804b.png)

The rOpenCL responds to this limitation. As show the figure above, through a middleware and services that together allow an OpenCL application (even pre-compiled) to transparently and efficiently explore the set of available accelerators in a distributed Linux system environment, using portable BSD socket-based communication.

<h4>rOpenCl: Coverage</h4>
At its current stage, rOpenCL only supports version 1.2 of OpenCL, a "limitation" inherited from the implementations of most OpenCL vendors, which stick to this version. Also, not all OpenCL functions can be performed remotely by the rOpenCL. The choice of functions to be supported by rOpenCL was conditioned by the development context (oriented to high-performance computing, not graphical acceleration) and expected execution environment, thus, rOpenCL only performs 70% of Opencl 1.2 functions correctly and stably (more information check the project’s Coverage folder), although, version 3.0, released on April 27, 2020, only requires vendors to support the features of version 1.2, making all functions of 2.x versions optional. In this way, the rOpenCL, ends up being aligned with what the latest version of the specification considers fundamental.

<h4>rOpenCl: Results</h4>

Performs the following benchmarks without changes:
<ul><li>Babel Stream</li><li>CL_Mem</li><li>CL_Peak</li><li>FinanceBench</li><li>PolyBench</li></ul>

<h4>rOpenCl: Publications</h4>


<h4>rOpenCl: Authors</h4>
For now it is only possible to use on Linux / Unix systems.

<br/><h3>more informations soon</h3>
