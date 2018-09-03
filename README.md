
This is a simple implementation of a code reading raw data from two MPU6050s and converting it to Roll, Pitch and Yaw using Madgwick filter.

The code corresponding to readings from two IMUs belongs to Shuvashish Sarker: https://gitlab.com/shuvashish/batikkrom.com/blob/master/MuttipleMPU/MuttipleMPU.ino I've only rewritten it the way I needed. Code related to Madgwick filter is taken from its Example provided with Arduino IDE.

Couple of advices:
This code cannot be directly expanded to have more MPU6050s than two. To have more MPU6050s, consider buying a multiplexer and address all the MPU6050s with the same address but multiplexe the SDA channel
Attention: MPU6050 has no magnetometer, so the yaw (heading) readings suffer a large drift. In fact, it is a 5-reliable-axis IMU.

If this code doesn't suit your application, take a look at a much more elaborated implementation:
https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050

