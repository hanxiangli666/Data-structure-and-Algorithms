# set 是 Python 的哈希集合，用于存储不重复元素，常用于去重和快速查询元素是否存在。
# 初始化集合
hashset = {1, 2, 3, 4}

# 是否为空，输出：False
print(len(hashset) == 0)

# 大小，输出：4
print(len(hashset))

# 查找元素
if 3 in hashset:
    print("Element 3 found.")
else:
    print("Element 3 not found.")
# 输出：Element 3 found.

# 插入新元素
hashset.add(5)

# 删除元素 2
# discard 不存在的元素不会报错
hashset.discard(2)

# 检查删除后
if 2 in hashset:
    print("Element 2 found.")
else:
    print("Element 2 not found.")
# 输出：Element 2 not found.

# 遍历集合，输出：
# 1
# 3
# 4
# 5
for element in hashset:
    print(element)