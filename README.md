# BooleanDagScheduler
This is a DAG scheduler for Boolean Function Graph (e.g. MIG, AIG)

## Get Started

1. Dependency

`sudo apt-get install  coinor-cbc coinor-libcbc-dev`
`g++ main.cpp src/*.cpp ILP/ILP.cpp -lOsiClp -lCbc -lClp -o main`
## Basic Information
```
Verilog --> DAG with n --> cut n --> stages and schedule --> inst

```


输入（示例）分别是 工艺点（22/28/32/45/65/90）、内存类型（0: RRAM /1: SRAM）、内存大小（KB）、IO带宽（bit）、bank row、bank col、activate bank row、 activate bank col、 mat row、 mat col、activate mat row、activate mat col、SA mux

其中bank row、bank col、activate bank row、 activate bank col 四个一组， mat row、 mat col、activate mat row、activate mat col 四个一组，你可以手动设置，或者设置成0 0 0 0 就是自动，但这样自动设置的不一定是你想要的结构，然后sa_mux 设置成0自动就行
示例输入：
./nvsim 65 0 512 1024 2 2 2 2 0 0 0 0 0
或者用SRAM就是./nvsim 65 1 512 1024 2 2 2 2 0 0 0 0 0

nvsim
```
Memory Cell: RRAM (Memristor)
Cell Area (F^2)    : 4 (2Fx2F)
Cell Aspect Ratio  : 1
Cell Turned-On Resistance : 1Mohm
Cell Turned-Off Resistance: 10Mohm
Read Mode: Current-Sensing
  - Read Voltage: 0.4V
Reset Mode: Voltage
  - Reset Voltage: 2V
  - Reset Pulse: 10ns
Set Mode: Voltage
  - Set Voltage: 2V
  - Set Pulse: 10ns
Access Type: None Access Device

====================
DESIGN SPECIFICATION
====================
Design Target: Random Access Memory
Capacity   : 512KB
Data Width : 1024Bits (128Bytes)

=============
CONFIGURATION
=============
Bank Organization: 2 x 2
 - Row Activation   : 2 / 2
 - Column Activation: 2 / 2
Mat Organization: 2 x 2
 - Row Activation   : 2 / 2
 - Column Activation: 2 / 2
 - Subarray Size    : 256 Rows x 1024 Columns
Mux Level:
 - Senseamp Mux      : 16
 - Output Level-1 Mux: 1
 - Output Level-2 Mux: 1
Latency-Optimized
=============
   RESULT
=============
Area:
 - Total Area = 1.42312mm x 904.28um = 1.2869mm^2
 |--- Mat Area      = 711.562um x 452.14um = 321726um^2   (5.50809%)
 |--- Subarray Area = 344.764um x 226.07um = 77940.8um^2   (5.6841%)
 - Area Efficiency = 5.50809%
Timing:
 -  Read Latency = 1.23112ns
 |--- Mat Latency    = 1.19777ns
    |--- Subarray Latency   = 987.106ps
 - Write Latency = 20.6749ns
 |--- Mat Latency    = 20.6582ns
    |--- Subarray Latency   = 20.4475ns
 - Subarray Compute Latency = 1.37325ns
Power:
 -  Read Dynamic Energy = 499.379pJ
 |--- Mat Dynamic Energy    = 46.3709pJ per mat
    |--- Subarray Dynamic Energy   = 11.4598pJ per active subarray
 - Write Dynamic Energy = 1.13442nJ
 |--- Mat Dynamic Energy    = 205.132pJ per mat
    |--- Subarray Dynamic Energy   = 51.1502pJ per active subarray
 - Subarray Compute Dynamic Energy = 74.0249pJ per active subarray
 - Leakage Power = 90.0445mW
 |--- Mat Leakage Power    = 22.5111mW per mat
     |--- Subarray Leakage Power    = 5.48614mW per subarray

```