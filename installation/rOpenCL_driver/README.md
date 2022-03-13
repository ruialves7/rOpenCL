The following Steps must be performed after the OpenCL setup and vendor drivers are installed.

It is necessary to install on each machine where you want to configure the rOpenCL client.

<h4>Step 1</h4>
Download the code contained in the code/rOpenCL_driver folder.

<h4>Step 2</h4>
Within the extracted project, a build name folder must be created in root project.

<h4>Step 3</h4>
Then, inside the previous folder (build) the "cmake .." command must be executed.

<h4>Step 4</h4>
At the end of the execution of the previous command, the "make" command must be executed, which must generate the librOpenCL.so file in the $PWD/lib/CL directory.

<h4>Step 5</h4>
The rOpenCL vendor directory should be added ( by default in /etc/OpenCL/vendors/) a file referencing librOpenCL.so that should have .icd extension.

<h4>Step 6</h4>
You need create a configuration file, named config.txt, with the following structure:

1024 -> Transmission-related options using UDP, which is currently unavailable
0-> Transmission-related options using UDP, which is currently unavailable
172.31.100.46 -> client interface used to send requests to the server(s).
1050 -> port used by server(s) to receive request(s) from client(s). It must be equal to the value defined in rDaemon in the macro PORT (common.h file).
172.31.100.47 -> list of server(s) available to receive request.

<h4>Step 7</h4>
After all the steps you will be able to see the remote devices using the clinfo tool.

<h4>Aditional Notes</h4>

1) The config.txt file is created in two places: /etc/OpenCL/rOpenCL/ and you need to create the rOpenCL folder inside the /etc/OpenCL folder and give it the appropriate permissions.
2) In addition, this file can also be created in ~/.rOpenCL/.
3) Does not work on Windows and MacOS.
4) Tested on Ubuntu LTS 20.04.
5) Any problem with instalation process or technical problem, pleaase contact rui.alves@ipb.pt 
