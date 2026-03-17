# 策略模式（Strategy Pattern）学习笔记

---

## 一、要解决什么问题？

 **经典症状** ：一个方法里有一长串 `if-else` 或 `switch-case`，根据不同类型执行不同逻辑。

```java
// 😫 典型的 if-else 地狱（初期看起来还好）
public String export(String title, String content, String format) {
    if ("markdown".equals(format)) {
        return "# " + title + "\n\n" + content;
    } else if ("html".equals(format)) {
        return "<h1>" + title + "</h1>\n<p>" + content + "</p>";
    } else if ("plaintext".equals(format)) {
        // ...
    }
    // 产品经理：再加 LaTeX、JSON、Confluence...
    // 你：😭
}
```

 **两个核心问题** ：

| 问题                     | 说明                                                 |
| ------------------------ | ---------------------------------------------------- |
| **违反开闭原则**   | 每次新增格式都要打开这个方法往里塞代码，容易引入 bug |
| **方法越来越臃肿** | 所有逻辑挤在一起，难以阅读和维护                     |

 **类比帮助理解** ：

> 想象一个万能遥控器，把电视、空调、音响的所有控制逻辑都写在一个"按钮处理函数"里：`if (设备 == 电视) {...} else if (设备 == 空调) {...}`。每新买一件家电就要打开遥控器重新焊电路板。策略模式的思路是： **把每种设备的控制逻辑单独封装成一个"遥控模块"，插上哪个模块就控制哪个设备** ，遥控器本体完全不用改。

---

## 二、核心思想

> **一句话理解** ：把会变化的算法/行为抽取成独立的策略类，让它们可以互相替换，上下文（使用者）只持有策略接口引用，完全不知道也不关心具体用的是哪种策略。

---

## 三、三个核心角色

```
┌──────────────────────────────────────────────────────────────┐
│                   策略模式三个角色                             │
│                                                              │
│  Strategy（策略接口）                                         │
│      定义所有策略的通用行为（抽象方法）                         │
│      → ExportStrategy                                        │
│                                                              │
│  ConcreteStrategy（具体策略）                                 │
│      实现策略接口，封装具体的算法                              │
│      → MarkdownExporter / HtmlExporter / PlainTextExporter   │
│                                                              │
│  Context（上下文）                                            │
│      持有策略接口的引用，把具体工作委托给策略对象               │
│      → ReportExporter                                        │
└──────────────────────────────────────────────────────────────┘
```

---

## 四、场景一：报告导出器（消除 if-else）

### Step 1：定义策略接口

```java
// 把"导出格式"这个可变行为抽象为接口
interface ExportStrategy {
    String doExport(String title, String content);
}
```

---

### Step 2：每种格式独立封装为策略类

```java
// Markdown 格式
class MarkdownExporter implements ExportStrategy {
    public String doExport(String title, String content) {
        return "# " + title + "\n\n" + content;
    }
}

// HTML 格式
class HtmlExporter implements ExportStrategy {
    public String doExport(String title, String content) {
        return "<h1>" + title + "</h1>\n<p>" + content + "</p>";
    }
}

// 纯文本格式
class PlainTextExporter implements ExportStrategy {
    public String doExport(String title, String content) {
        return title.toUpperCase() + "\n"
             + "=".repeat(title.length()) + "\n"
             + content;
    }
}
```

---

### Step 3：上下文持有策略，委托执行

```java
class ReportExporter {
    private ExportStrategy strategy;  // 只认识接口，不认识具体类

    public ReportExporter(ExportStrategy strategy) {
        this.strategy = strategy;
    }

    // 运行时可以随时切换策略
    public void setStrategy(ExportStrategy strategy) {
        this.strategy = strategy;
    }

    public String doExport(String title, String content) {
        return strategy.doExport(title, content);  // 委托给策略
    }
}
```

---

### Step 4：客户端使用

```java
ReportExporter exporter = new ReportExporter(new MarkdownExporter());

exporter.doExport("Monthly Report", "Sales grew by 20% this month");
// 输出：# Monthly Report
//
//       Sales grew by 20% this month

// 需要 HTML 格式？一行代码切换，其他什么都不用改
exporter.setStrategy(new HtmlExporter());
exporter.doExport("Monthly Report", "Sales grew by 20% this month");
// 输出：<h1>Monthly Report</h1>
//       <p>Sales grew by 20% this month</p>
```

**将来新增 LaTeX 格式？**

```java
// 只需新建一个类，已有代码零修改 ✅
class LatexExporter implements ExportStrategy {
    public String doExport(String title, String content) {
        return "\\section{" + title + "}\n" + content;
    }
}
```

---

### 重构前后对比

```
重构前（if-else 地狱）：
ReportExporter
    └── export() ← 包含所有格式的逻辑，新增格式 = 修改这个方法

重构后（策略模式）：
ReportExporter（Context）
    └── strategy.doExport() ← 委托给策略

ExportStrategy（接口）
    ├── MarkdownExporter   ← 只管 Markdown
    ├── HtmlExporter       ← 只管 HTML
    ├── PlainTextExporter  ← 只管纯文本
    └── LatexExporter      ← 新增！完全独立，不影响任何已有代码
```

---

## 五、场景二：游戏 NPC 行为（运行时动态切换）

### 场景

不同 NPC 有不同的战斗风格，且可以在游戏运行中动态切换（如玩家使用"激怒"技能）。

---

### 实现

```java
// 策略接口
interface BattleStrategy {
    String decideAction(int myHp, int enemyHp);
}

// 激进型：不管不顾，全力输出
class AggressiveStrategy implements BattleStrategy {
    public String decideAction(int myHp, int enemyHp) {
        return "Full attack! Deal 30 damage";
    }
}

// 防御型：以防御为主
class DefensiveStrategy implements BattleStrategy {
    public String decideAction(int myHp, int enemyHp) {
        return "Raise shield, reduce 50% incoming damage";
    }
}

// 狡猾型：根据战场局势判断
class CunningStrategy implements BattleStrategy {
    public String decideAction(int myHp, int enemyHp) {
        if (enemyHp < 30) {
            return "Enemy is weak, go for the kill! Deal 40 damage";
        }
        return "Probe cautiously, deal 10 damage";
    }
}

// 上下文：游戏角色
class GameCharacter {
    private String name;
    private int hp;
    private BattleStrategy strategy;

    public GameCharacter(String name, int hp, BattleStrategy strategy) {
        this.name = name; this.hp = hp; this.strategy = strategy;
    }

    public void setStrategy(BattleStrategy strategy) {
        this.strategy = strategy;  // 运行时切换
    }

    public void takeTurn(int enemyHp) {
        System.out.println(name + ": " + strategy.decideAction(hp, enemyHp));
    }
}
```

---

### 动态切换演示

```java
GameCharacter warrior  = new GameCharacter("Warrior",  100, new AggressiveStrategy());
GameCharacter guardian = new GameCharacter("Guardian", 120, new DefensiveStrategy());
GameCharacter assassin = new GameCharacter("Assassin",  80, new CunningStrategy());

warrior.takeTurn(80);
// Warrior: Full attack! Deal 30 damage

guardian.takeTurn(80);
// Guardian: Raise shield, reduce 50% incoming damage

assassin.takeTurn(100);
// Assassin: Probe cautiously, deal 10 damage

assassin.takeTurn(25);
// Assassin: Enemy is weak, go for the kill! Deal 40 damage  ← 策略内部自适应

// 玩家使用"激怒"技能 → 运行时切换策略
System.out.println("Player used 'Enrage' on Guardian!");
guardian.setStrategy(new AggressiveStrategy());  // 一行代码，行为立刻改变
guardian.takeTurn(80);
// Guardian: Full attack! Deal 30 damage          ← 行为切换了！
```

---

## 六、与相似模式的区别

### 6.1 策略模式 vs 桥接模式

两者代码结构相似（都持有接口引用），但解决的问题不同：

|                      | 策略模式                 | 桥接模式                         |
| -------------------- | ------------------------ | -------------------------------- |
| **解决问题**   | 一个维度的算法替换       | 两个维度的独立变化               |
| **变化维度**   | 一个（如：导出格式）     | 两个（如：消息类型 × 发送渠道） |
| **上下文层次** | Context 本身不需要子类   | 抽象层有自己的子类层次           |
| **目标**       | 消除 if-else，可互换算法 | 避免 M×N 类爆炸                 |

 **一句话区分** ：

> 策略是"选一个算法"，桥接是"分离两个独立变化的维度"。

---

### 6.2 策略模式 vs 状态模式

两者结构几乎一模一样，语义完全不同：

|                      | 策略模式                           | 状态模式                                   |
| -------------------- | ---------------------------------- | ------------------------------------------ |
| **谁决定切换** | **外部客户端**决定用哪个策略 | **对象自己**根据内部状态自动切换     |
| **策略间关系** | 各策略互不知道对方                 | 状态之间通常互相知道（知道下一个状态是谁） |
| **典型场景**   | 玩家主动给 NPC 施加激怒效果        | 订单从"待支付"自动流转到"已支付"           |

 **类比区分** ：

> 策略模式：你（外部）帮服务员选今天穿哪件制服
> 状态模式：服务员自己根据"上班/下班/休假"状态，自动换装

---

## 七、优缺点分析

### ✅ 优点

**1. 消除条件分支**

```java
// ✅ 策略模式：用多态替代 if-else
return strategy.doExport(title, content);

// ❌ 没有策略模式：
if ("markdown") {...} else if ("html") {...} else if ("latex") {...} ...
```

**2. 符合开闭原则**

新增算法 = 新建类，已有代码零修改。

**3. 运行时动态切换**

```java
exporter.setStrategy(new HtmlExporter());  // 一行代码，行为立刻改变
```

**4. 每个策略类职责单一**

`MarkdownExporter` 只管 Markdown 格式，`HtmlExporter` 只管 HTML，互不干扰，易于测试和维护。

---

### ❌ 缺点

**1. 类数量增多**

每种算法一个类，策略多时类数量膨胀。

**2. 客户端必须了解有哪些策略**

```java
// 客户端要知道"有 Markdown、HTML、纯文本这几种策略"才能做选择
// 相比 if-else 把所有逻辑藏在一处，策略需要客户端主动了解
exporter.setStrategy(new MarkdownExporter());
```

---

## 八、什么时候用策略模式？

```
代码中出现大量 if-else / switch，根据类型选择不同行为？
    └── 这些行为会频繁新增或修改吗？
            ├── 是 → ✅ 策略模式（把每种行为独立封装）
            └── 否（分支固定且少）→ 直接 if-else 更简单

需要在运行时动态切换算法？
    └── ✅ 策略模式（setStrategy() 一行搞定）
```

---

## 九、总结

```
策略模式
├── 核心思想：把可变的算法/行为封装成独立策略类，可互相替换
├── 三个角色
│   ├── Strategy（接口）：定义通用行为
│   ├── ConcreteStrategy：每种算法独立实现
│   └── Context（上下文）：持有接口引用，委托执行
├── 两大核心能力
│   ├── 消除 if-else：用多态替代条件分支
│   └── 运行时切换：setStrategy() 动态替换算法
├── vs 桥接模式：策略=一维算法替换，桥接=两维度分离
├── vs 状态模式：策略=客户端选算法，状态=对象自动切换
├── 优点：消除条件分支、开闭原则、动态切换、职责单一
├── 缺点：类数量增多，客户端需了解可用策略
└── 适用场景：同一件事有多种做法，且做法会频繁扩展
```

> **最终类比总结** ：策略模式就像 **导航软件的路线选择** 。从 A 到 B，可以选"最快路线"、"最短路线"、"避开高速"……每种路线算法独立封装，导航软件（Context）只管调用当前选择的算法，你（客户端）随时可以切换策略。将来增加"避开收费站"算法，只需加一个新选项，导航软件本体完全不用改。
>
