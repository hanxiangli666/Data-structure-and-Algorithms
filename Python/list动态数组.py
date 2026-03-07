nums = [0] * 10

# 输出：False
print(len(nums) == 0)
# 输出：10
print(len(nums))

# 在列表尾部插入一个元素 20
nums.append(20)
# 输出：11
print(len(nums))

# 得到列表最后一个元素，输出：20
print(nums[-1])

# 删除列表的最后一个元素
nums.pop()
# 输出：10
print(len(nums))

# 索引访问与修改
nums[0] = 11
# 输出：11
print(nums[0])

# 在索引 3 处插入一个元素 99
nums.insert(3, 99)

# 删除索引 2 处的元素
nums.pop(2)

# 交换 nums[0] 和 nums[1]
nums[0], nums[1] = nums[1], nums[0]

# 遍历列表
# 输出示例：
# 0 11 99 0 0 0 0 0 0 0
for num in nums:
    print(num, end=" ")
print()