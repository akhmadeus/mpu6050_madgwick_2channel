
This is a simple implementation of a code reading raw data from two MPU6050s and converting it to Roll, Pitch and Yaw using Madgwick filter.

The code corresponding to readings from two IMUs belongs to Shuvashish Sarker: https://gitlab.com/shuvashish/batikkrom.com/blob/master/MuttipleMPU/MuttipleMPU.ino I've only rewritten it the way I needed. Code related to Madgwick filter is taken from its Example provided with Arduino IDE.

If this code doesn't suit your application, take a look at a much more elaborate implementation:
https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050

