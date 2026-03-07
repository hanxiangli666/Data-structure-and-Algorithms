from collections import deque
# deque 是 collections 模块提供的双端队列，可以高效地在两端插入和删除元素。
# 初始化双端队列
lst = deque([1, 2, 3, 4, 5])

# 检查是否为空，输出：False
print(len(lst) == 0)

# 获取大小，输出：5
print(len(lst))

# 在头部插入 0，尾部插入 6
lst.appendleft(0)
lst.append(6)

# 获取头部和尾部元素，输出：0 6
print(lst[0], lst[-1])

# 删除头部和尾部元素
lst.popleft()
lst.pop()

# 在索引 2 处插入 99
lst.insert(2, 99)

# 删除索引 1 处的元素
del lst[1]

# 遍历双端队列
# 输出：1 99 3 4 5
for val in lst:
    print(val, end=" ")
print()