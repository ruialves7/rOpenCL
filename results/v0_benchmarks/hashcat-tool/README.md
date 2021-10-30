The hashcat plays an especially important role in single-client rOpenCL testing, as it is the only test with the ability to use more than one GPU, allowing you to fully explore the 4 GPUs in the test environment and realize not only the overload of remote execution, as well as its effect on scalability.

Note that the hashcat has an integrated benchmark feature (a brute force attack on a single hash ). However, it involves very few data transfers between the host and the GPUs, as the hash space is known a priori and divided equally among the GPUs used. As an alternative, we used an * example that uses a dictionary with rules to break 28 passwords encrypted with the MD5 cipher, the scenarios tested are those presented in Table 0, involving different combinations of local and remote GPUs, as permitted by the test environment (see table 1 and 2) at the date.

*Jake, Hashcat Tutorial – The basics of cracking passwords with hashcat , https://laconicwolf.com/2018/09/29/hashcat-tutorial-the-basics-of-cracking-passwords-with-hashcat/, (accessed Outubro 3, 2020).


<h5> Scenarios (table 0)</h5>
<table>
  
  <tr>
    <td>Scenarios</td>
    <td>Local GPUs </td>
    <td>Remote GPUs</td>
  </tr>
  
  <tr>
    <td>C1</td>
    <td>0</td>
    <td>1</td>
  </tr>

  <tr>
    <td>C2</td>
    <td>0</td>
    <td>2</td>
  </tr>

  <tr>
    <td>C3</td>
    <td>0</td>
    <td>1</td>
  </tr>

  <tr>
    <td>C4</td>
    <td>0</td>
    <td>1</td>
  </tr>
  <tr>
    <td>C5</td>
    <td>0</td>
    <td>1</td>
  </tr>
  <tr>
    <td>C6</td>
    <td>0</td>
    <td>1</td>
  </tr>
  <tr>
    <td>C7</td>
    <td>0</td>
    <td>1</td>
  </tr>
  <tr>
    <td>C8</td>
    <td>0</td>
    <td>1</td>
  </tr>
  <tr>
    <td>C1</td>
    <td>0</td>
    <td>1</td>
  </tr>

</table>




<h5> Physical Hardware (table 1)</h5>
<table>
  
  <tr>
    <th>CPU</th>
    <th>RAM</th>
    <th>Network</th>
    <th>GPUs</th>
  </tr>
  
  <tr>
    <td>2 x AMD EPYC 7351 16-Core 2.4/2.9GHz</td>
    <td>256 GB ECC DDR4 2666 MHz</td>
    <td>10Gbps Ethernet</td>
    <td>2 x NVIDIA RTX 2080 Ti</td>
  </tr>
</table>


<h5> Virtual Hardware (table 2)</h5>
<table>
  
  <tr>
    <th>vCPU</th>
    <td>1 x AMD EPYC 7351 8-Core 2.4/2.9GHz</td>
  </tr>
  
  <tr>
    <th>vRAM</th>
    <td>16 GB ECC DDR4 2666 MHz</td>
  </tr>
  
  <tr>
    <th>vNIC</th>
    <td>10Gbps Ethernet</td>
  </tr>
  
  <tr>
    <th>vGPU</th>
    <td>1 ou 2 x NVIDIA RTX 2080 Ti</td>
  </tr>
  
  <tr>
    <th>Operating System</th>
    <td>Ubuntu 18.04.3 LTS</td>
  </tr>
  
  <tr>
    <th>POCL</th>
    <td>Version 1.3</td>
  </tr>
  
  
  <tr>
    <th>Intel OpenCL SDK</th>
    <td>Version 18.1.0.0920</td>
  </tr>
  
   <tr>
    <th>NVIDIA Driver</th>
    <td>Version 430.50</td>
  </tr>
  
</table>


The tests were run with a two servers (rDaemon) and a single client ( rOpenCL driver).  The client submitted requests  for 4 GPUs.
