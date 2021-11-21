from itertools import product
arr = []
for roll in product([-1,0,1], repeat = 3):
    arr.append(roll)
print(len(arr))

with open("moore.txt", "w") as f:
    for a in arr:
        print("{", end ="",file=f)
        for b in a:
            print(b, end = ",",file=f)
        print("},",end ="",file=f)
        print("",file=f)