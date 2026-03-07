from collections import deque
# 队列是一种操作受限制的数据结构：只允许在队尾插入元素，在队头删除元素。
# Python 没有专门的队列类型，但可以使用 deque 来模拟队列，append 相当于入队，popleft 相当于出队。

# 初始化队列
q = deque()

# 向队尾插入元素
q.append(10)
q.append(20)
q.append(30)

# 是否为空，输出：False
print(len(q) == 0)

# 大小，输出：3
print(len(q))

# 获取队头元素，不出队，输出：10
print(q[0])

# 队头元素出队
q.popleft()

# 新的队头元素，输出：20
print(q[0])