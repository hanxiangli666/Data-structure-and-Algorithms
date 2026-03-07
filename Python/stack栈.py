# 使用 list 作为栈
# 虽然 Python 没有专门的栈类型，但可以使用 list 或 deque 来模拟栈。append 相当于压栈，pop 相当于出栈。
s = []

# 压栈
s.append(10)
s.append(20)
s.append(30)

# 是否为空，输出：False
print(len(s) == 0)

# 大小，输出：3
print(len(s))

# 栈顶元素，输出：30
print(s[-1])

# 出栈
s.pop()

# 新的栈顶元素，输出：20
print(s[-1])