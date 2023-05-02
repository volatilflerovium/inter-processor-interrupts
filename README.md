# Inter Processor Interrupts
C library for shared memory and messaging using inter processor interrupts. 

Design based on the Xilinx Vitis Unified Software Development Platform 
and targeted to the Xilinx Zynq UltraScale+ MPSoC.

# Introduction

This project is based on [Inter Processor Interrupts and Message Buffers](https://ohwr.org/project/soc-course/wikis/Inter-Processor-Interrupts-and-Message-Buffers)

The idea is to exchange interrup messages among the processors in the
system. Theses messages contain information about the sender, the 
memory addres and the size of the data at that memory address.

# Project organization

For this project we have deployed 2 A53 processors (one running baremetal and the
other freeRTOS), 1 R5 (running freeRTOS) and 1 microblaze (running baremetal).

The directory ipi_lib needs to be shared across all the cores.

Notice that despite the four A53 share the same IPI, our implementation
is able to route messages to a specific A53 core.

