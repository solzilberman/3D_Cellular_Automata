x=10
y=77
z=92

packed = x | (y << 8) | (z << 16)
print(packed)

x_unp = packed & 0xFF
y_unp = (packed >> 8) & 0xFF
z_unp = (packed >> 16) & 0xFF
print(x_unp, y_unp, z_unp)

x = [1, 1, 1, 0,  0,  2, 0]
bx = x[0] | x[1] << 2 | x[2] << 4 | x[3] << 6 | x[4] << 8 | x[5] << 10 | x[6] << 12
print(bx)
ux = [bx & 0x3, (bx >> 2) & 0x3, (bx >> 4) & 0x3, (bx >> 6) & 0x3, (bx >> 8) & 0x3, (bx >> 10) & 0x3, (bx >> 12) & 0x3]
print(ux)