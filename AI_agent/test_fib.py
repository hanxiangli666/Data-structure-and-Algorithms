import subprocess
import sys

def test_fib():
    # 测试前10个斐波那契数
    expected = [1, 1, 2, 3, 5, 8, 13, 21, 34, 55]
    for i, exp in enumerate(expected, start=1):
        result = subprocess.run([sys.executable, 'fib.py', str(i)], capture_output=True, text=True)
        if result.returncode != 0:
            print(f"错误: n={i}, stderr: {result.stderr}")
            return False
        output = result.stdout.strip()
        if int(output) != exp:
            print(f"失败: n={i}, 期望 {exp}, 实际 {output}")
            return False
        else:
            print(f"通过: n={i} -> {output}")
    print("所有测试通过")
    return True

if __name__ == "__main__":
    test_fib()