# 桥接模式（Bridge Pattern）学习笔记

---

## 一、要解决什么问题？

### "类爆炸"问题

用继承实现多维度扩展时，类的数量会 **成倍增长** 。

以消息通知系统为例：

```
消息类型（2种）× 发送渠道（3种）= 6个类

Message（基类）
├── NormalEmailMessage   （普通 + 邮件）
├── NormalSmsMessage     （普通 + 短信）
├── NormalPushMessage    （普通 + 推送）
├── UrgentEmailMessage   （紧急 + 邮件）
├── UrgentSmsMessage     （紧急 + 短信）
└── UrgentPushMessage    （紧急 + 推送）
```

**产品经理说要加"定时消息"和"微信渠道"：**

```
3种类型 × 4种渠道 = 12个类  （翻倍！）
再加一种类型/渠道，继续翻倍……
```

 **还有大量重复代码** ：

* 所有邮件相关的类都要写发邮件的逻辑
* 所有紧急消息相关的类都要写加 `[紧急]` 前缀的逻辑

 **类比帮助理解** ：

> 想象你开了一家奶茶店，有 2 种甜度 × 3 种杯型 = 6 种组合。如果用继承，你需要 6 个"不同品种"的奶茶。再加一种甜度和一种杯型，就变 12 种。**桥接模式的思路是：甜度和杯型是两个独立的维度，分开管理！** 甜度有自己的档位，杯型有自己的规格，点单时自由组合，不需要为每个组合单独定义一种奶茶。

---

## 二、核心思想

> **一句话理解** ：识别系统中 **独立变化的多个维度** ，把它们分离成独立的类层次，再通过**组合（持有引用）**把两个层次"桥接"起来，实现自由组合。

 **M × N → M + N** ：

```
继承方案：2种类型 × 3种渠道 = 6个类
桥接方案：2种类型 + 3种渠道 = 5个类，却能覆盖所有 6 种组合
```

---

## 三、四个核心角色

```
┌──────────────────────────────────────────────────────────────┐
│                    桥接模式四个角色                            │
│                                                              │
│  Abstraction（抽象层基类）                                    │
│      Message ── 持有实现层引用（这就是"桥"）                  │
│                                                              │
│  RefinedAbstraction（精确抽象）                               │
│      NormalMessage / UrgentMessage ── 实现具体业务逻辑        │
│                                                              │
│  Implementor（实现层接口）                                    │
│      MessageSender ── 定义底层操作规范                        │
│                                                              │
│  ConcreteImplementor（具体实现）                              │
│      EmailSender / SmsSender / PushSender ── 实现底层操作    │
└──────────────────────────────────────────────────────────────┘
```

**两个独立的类层次，通过引用相连：**

```
抽象层（消息类型）            实现层（发送渠道）
Message ──────────────桥──→ MessageSender（接口）
├── NormalMessage            ├── EmailSender
└── UrgentMessage            ├── SmsSender
                             └── PushSender
```

---

## 四、完整代码实现

### Step 1：实现层接口（Implementor）

```java
// 定义底层操作：如何发送消息
interface MessageSender {
    void send(String content);
}
```

---

### Step 2：具体实现类（ConcreteImplementor）

```java
// 邮件渠道
class EmailSender implements MessageSender {
    public void send(String content) {
        System.out.println("发送邮件: " + content);
    }
}

// 短信渠道
class SmsSender implements MessageSender {
    public void send(String content) {
        System.out.println("发送短信: " + content);
    }
}

// App 推送渠道
class PushSender implements MessageSender {
    public void send(String content) {
        System.out.println("发送推送: " + content);
    }
}
```

---

### Step 3：抽象层基类（Abstraction）

```java
abstract class Message {
    // ← 这就是"桥"：持有实现层的引用
    protected MessageSender sender;

    public Message(MessageSender sender) {
        this.sender = sender;
    }

    // 子类决定"如何处理消息"
    public abstract void send(String content);
}
```

> **关键** ：`sender` 成员变量就是两个层次之间的 **桥** 。抽象层通过这个引用调用实现层，而不需要知道具体是哪种发送方式。

---

### Step 4：精确抽象类（RefinedAbstraction）

```java
// 普通消息：直接发送，不做特殊处理
class NormalMessage extends Message {
    public NormalMessage(MessageSender sender) { super(sender); }

    public void send(String content) {
        sender.send(content);              // 直接转发给实现层
    }
}

// 紧急消息：加上 [紧急] 前缀再发送
class UrgentMessage extends Message {
    public UrgentMessage(MessageSender sender) { super(sender); }

    public void send(String content) {
        sender.send("[紧急] " + content);  // 加工后再转发给实现层
    }
}
```

---

### Step 5：客户端自由组合

```java
public class Main {
    public static void main(String[] args) {
        // 普通消息 + 邮件
        new NormalMessage(new EmailSender()).send("你的订单已发货");
        // 输出：发送邮件: 你的订单已发货

        // 紧急消息 + 短信
        new UrgentMessage(new SmsSender()).send("服务器宕机了！");
        // 输出：发送短信: [紧急] 服务器宕机了！

        // 紧急消息 + 推送
        new UrgentMessage(new PushSender()).send("检测到异常登录");
        // 输出：发送推送: [紧急] 检测到异常登录
    }
}
```

---

## 五、扩展性演示

### 新增"定时消息"类型（只改抽象层）

```java
class ScheduledMessage extends Message {
    private String scheduledTime;

    public ScheduledMessage(MessageSender sender, String time) {
        super(sender);
        this.scheduledTime = time;
    }

    public void send(String content) {
        sender.send("[定时 " + scheduledTime + "] " + content);
    }
}
```

### 新增"微信"渠道（只改实现层）

```java
class WechatSender implements MessageSender {
    public void send(String content) {
        System.out.println("发送微信: " + content);
    }
}
```

### 两者自由组合，现有代码一行不改

```java
// 定时消息 + 微信
new ScheduledMessage(new WechatSender(), "2024-01-01 10:00").send("新年快乐！");
// 输出：发送微信: [定时 2024-01-01 10:00] 新年快乐！

// 定时消息 + 邮件（自动可用，无需额外代码！）
new ScheduledMessage(new EmailSender(), "2024-01-01 10:00").send("新年快乐！");
// 输出：发送邮件: [定时 2024-01-01 10:00] 新年快乐！
```

 **扩展后的类数量对比** ：

```
继承方案：3种类型 × 4种渠道 = 12个类
桥接方案：3种类型 + 4种渠道 = 7个类，覆盖全部 12 种组合
```

---

## 六、两个维度的调用关系图

```
客户端
    ↓ new UrgentMessage(new SmsSender())
  
UrgentMessage（精确抽象）
    ├── 字段：sender = SmsSender 实例
    └── send("服务器宕机了！")
            ↓ 调用 sender.send("[紧急] 服务器宕机了！")
        SmsSender（具体实现）
            └── 输出："发送短信: [紧急] 服务器宕机了！"
```

 **抽象层负责"怎么处理"** （加前缀、加时间戳…）
 **实现层负责"怎么发送"** （邮件、短信、推送…）
**两者通过 `sender` 这座"桥"连接，互不干扰**

---

## 七、与相似模式的区别

三个"包装"相关模式的本质区别：

| 模式             | 核心目的               | 接口变化                         | 设计时机       |
| ---------------- | ---------------------- | -------------------------------- | -------------- |
| **适配器** | 转换不兼容的接口       | 输入输出接口**不同**       | 事后补救       |
| **装饰器** | 给对象动态增加功能     | 接口**不变** ，功能增强    | 需要叠加功能时 |
| **桥接**   | 分离多个独立变化的维度 | 两个独立接口，**桥接**起来 | 系统设计之初   |

 **一句话记忆** ：

```
适配器 → 转换接口
装饰器 → 增强功能
桥接   → 分离维度
```

---

## 八、如何识别是否需要桥接模式？

 **关键问题** ：这个类是否存在 **两个（或以上）独立变化的维度** ？

```
发现一个类需要在多个方向扩展？
    └── 这些扩展方向是否相互独立？
            ├── 是 → 这两个维度的变化会产生类爆炸吗？
            │         ├── 是 → ✅ 使用桥接模式
            │         └── 否 → 普通继承即可
            └── 否（维度之间有依赖）→ 不适合桥接
```

 **更多适用场景举例** ：

| 系统     | 维度1                     | 维度2                         |
| -------- | ------------------------- | ----------------------------- |
| 图形渲染 | 形状（圆/方/三角）        | 渲染器（OpenGL/DirectX）      |
| 数据导出 | 数据类型（报表/图表）     | 导出格式（PDF/Excel/CSV）     |
| 设备驱动 | 设备类型（打印机/扫描仪） | 操作系统（Windows/Mac/Linux） |

---

## 九、优缺点分析

### ✅ 优点

**1. 根治类爆炸**

```
继承：M × N 个类
桥接：M + N 个类（节省量随规模指数增长）
```

**2. 两个维度独立扩展，互不干扰**

```
新增消息类型 → 只动抽象层 → 实现层完全不受影响
新增发送渠道 → 只动实现层 → 抽象层完全不受影响
```

**3. 体现"组合优于继承"**

运行时可以动态替换实现：

```java
// 运行时换渠道
message.sender = new WechatSender();  // 直接换，不需要重新创建
```

---

### ❌ 缺点

 **设计难度高** ：需要在**系统设计之初**就正确识别哪些维度是独立变化的。

* 识别错了（其实两个维度有耦合），会导致桥接方案反而更复杂
* 只有一个变化维度时，使用桥接是**过度设计**

---

## 十、总结

```
桥接模式
├── 核心思想：识别独立变化的多个维度，分离成独立类层次，用引用"桥接"
├── 解决问题：继承导致的类爆炸（M×N → M+N）
├── 四个角色
│   ├── Abstraction（抽象层基类）：持有实现层引用（"桥"）
│   ├── RefinedAbstraction（精确抽象）：实现具体业务逻辑
│   ├── Implementor（实现层接口）：定义底层操作规范
│   └── ConcreteImplementor（具体实现）：实现底层操作
├── vs 适配器/装饰器
│   ├── 适配器：转换接口（事后补救）
│   ├── 装饰器：增强功能（接口不变）
│   └── 桥接：分离维度（设计之初）
├── 优点：避免类爆炸、两维度独立扩展、组合优于继承
├── 缺点：需提前识别维度，设计难度较高
└── 适用场景：类在多个独立维度上都需要扩展
```

> **最终类比总结** ：桥接模式就像 **电源插头系统的标准化** 。插头形状（抽象层：中国三脚/美国两脚/欧洲两脚）和电压规格（实现层：110V/220V）是两个独立的维度。如果用继承，需要"中国三脚220V"、"中国三脚110V"、"美国两脚110V"……无数组合。但只要把插头形状和电压分开独立管理，用"转换接口"（桥）连起来，任意组合瞬间搞定，而且新增任何一种形状或电压都完全不影响另一侧。
>
