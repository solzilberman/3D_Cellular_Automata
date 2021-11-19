x=10
y=77
z=92

packed = x | (y << 8) | (z << 16)
print(packed)

x_unp = packed & 0xFF
y_unp = (packed >> 8) & 0xFF
z_unp = (packed >> 16) & 0xFF
print(x_unp, y_unp, z_unp)