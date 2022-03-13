
The following steps must be performed after the OpenCL setup and vendor drivers installation.

<h4>1 Step</h4>
Download the code that is in the code/rDaemon folder.

<h4>2 Step</h4>
Configure the network interface used by the rDaemon. To do this, the macro with the name LOCAL_INTERFACE should be edited by placing the IP address that the rDaemon should use to receive the requests. This macro is in the file common.h.

<h4>3 Step</h4>
The code must be recompiled using the Makefile that is in the project.

<h4>3 Step</h4>
Run the executable file resulting from the build (eg./rDaemon). At this point rDaemon is ready to receive execution requests.


<h4>Additional Notes</h4>

1) It is not possible to have a configured rDaemon and rOpenCL client on the same machine, since it can create an requests loop.
2) In the case where the OpenCL kernel contains references to external files it is necessary to place those files at the root of the rDaemon project.
3) After compiling rDaemon, it can be run as a system service.
4) Does not work on Windows and MacOS.
5) Tested on Ubuntu LTS 20.04.
6) By default the port used in socket is 1050 to change is need change the macro with name PORT in common.h
7) This process must be done on each client machine that is intended to be used.
8) Any problem with instalation process or technical problem, contact rui.alves@ipb.pt

