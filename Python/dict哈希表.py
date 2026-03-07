# dict 是 Python 的哈希表实现，通过键值对存储数据，查找、插入和删除操作平均时间复杂度为 O(1)。
# 初始化字典
hashmap = {1: "one", 2: "two", 3: "three"}

# 是否为空，输出：False
print(len(hashmap) == 0)

# 大小，输出：3
print(len(hashmap))

# 查找键
# 输出：Key 2 -> two
if 2 in hashmap:
    print(f"Key 2 -> {hashmap[2]}")
else:
    print("Key 2 not found.")

# 获取键对应的值，不存在则返回 None
# 输出：None
print(hashmap.get(4))

# 插入新键值对
hashmap[4] = "four"

# 获取新插入的值，输出：four
print(hashmap[4])

# 删除键 3
del hashmap[3]

# 检查删除后
if 3 in hashmap:
    print(f"Key 3 -> {hashmap[3]}")
else:
    print("Key 3 not found.")
# 输出：Key 3 not found.

# 遍历字典
# 输出：
# 1 -> one
# 2 -> two
# 4 -> four
for k, v in hashmap.items():
    print(f"{k} -> {v}")