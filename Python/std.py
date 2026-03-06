a = 10

# 输出：10
print(a)

# 串联输出（通过字符串拼接或逗号分隔）
# 输出：Hello, World!
print("Hello" + ", " + "World!")
# 使用 sep 指定分隔符
print("Hello", "World!", sep=", ")

s = "abc"
# 输出：abc 10
print(s, a)

# 格式化输出
# 输出：abc 10
print(f"{s} {a}")