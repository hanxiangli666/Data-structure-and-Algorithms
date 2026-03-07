a = 10

if a > 5:
    print("a > 5")
elif a == 5:
    print("a == 5")
else:
    print("a < 5")
# 输出：a > 5

# 输出：0 1 2 3 4 
for i in range(5):
    print(i, end=" ")
print()

num = 100
# 输出：100 50 25 12 6 3 1 
while num > 0:
    print(num, end=" ")
    num //= 2
print()