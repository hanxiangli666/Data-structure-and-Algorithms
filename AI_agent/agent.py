import json
import subprocess
from openai import OpenAI

client = OpenAI(
    api_key="sk-564fde221f4b44beb00e09865b5f615b",
    base_url="https://api.deepseek.com"
)

# --- 工具描述 ---
tools = [
    {
        "type": "function",
        "function": {
            "name": "read_file",
            "description": "读取指定文件的内容",
            "parameters": {
                "type": "object",
                "properties": {
                    "path": {"type": "string", "description": "文件路径"}
                },
                "required": ["path"]
            }
        }
    },
    {
        "type": "function",
        "function": {
            "name": "write_file",
            "description": "将内容写入指定文件（会覆盖已有内容）",
            "parameters": {
                "type": "object",
                "properties": {
                    "path": {"type": "string", "description": "文件路径"},
                    "content": {"type": "string", "description": "要写入的内容"}
                },
                "required": ["path", "content"]
            }
        }
    },
    {
        "type": "function",
        "function": {
            "name": "run_command",
            "description": "执行一条 shell 命令并返回输出结果",
            "parameters": {
                "type": "object",
                "properties": {
                    "command": {"type": "string", "description": "要执行的命令"}
                },
                "required": ["command"]
            }
        }
    }
]

# --- 工具实现 ---
def read_file(path):
    try:
        with open(path, "r") as f:
            return f.read()
    except FileNotFoundError:
        return f"错误：文件 {path} 不存在"

def write_file(path, content):
    with open(path, "w") as f:
        f.write(content)
    return f"已写入 {path}"

def run_command(command):
    try:
        result = subprocess.run(
            command, shell=True, capture_output=True, text=True, timeout=10
        )
        output = result.stdout
        # shell 惯例：返回码 0 表示成功，非 0 表示出错
        if result.returncode != 0:
            output += f"\n[错误] {result.stderr}"
        return output or "(无输出)"
    except subprocess.TimeoutExpired:
        return "[错误] 命令执行超时（10秒）"

tool_functions = {
    "read_file": read_file,
    "write_file": write_file,
    "run_command": run_command,
}

# --- Agent 主循环 ---
SYSTEM_PROMPT = """你是一个编程助手。你可以读写文件和执行命令来帮用户完成编程任务。
工作流程：先理解需求，写代码，然后运行验证。如果发现错误就修复并重新运行，直到确认正确。"""

messages = [{"role": "system", "content": SYSTEM_PROMPT}]
print("编程 Agent 已启动，输入任务开始，输入 q 退出\n")

while True:
    user_input = input("你: ")
    if user_input.strip() == "q":
        break

    messages.append({"role": "user", "content": user_input})

    # Agent 循环
    while True:
        response = client.chat.completions.create(
            model="deepseek-reasoner",
            messages=messages,
            tools=tools
        )
        assistant_message = response.choices[0].message

        if not assistant_message.tool_calls:
            break

        messages.append(assistant_message)
        for tool_call in assistant_message.tool_calls:
            args = json.loads(tool_call.function.arguments)
            func = tool_functions[tool_call.function.name]
            result = func(**args)
            # 打印工具调用过程，方便观察 Agent 的行为
            print(f"  [工具] {tool_call.function.name}({args})")
            # 只打印前 200 个字符，避免大文件输出刷屏
            print(f"  [结果] {result[:200]}")
            messages.append({
                "role": "tool",
                "tool_call_id": tool_call.id,
                "content": result
            })

    messages.append({"role": "assistant", "content": assistant_message.content})
    print(f"AI: {assistant_message.content}\n")