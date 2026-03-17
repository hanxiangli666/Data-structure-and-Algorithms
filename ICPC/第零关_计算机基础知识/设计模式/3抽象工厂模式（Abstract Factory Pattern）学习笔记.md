# 抽象工厂模式（Abstract Factory Pattern）学习笔记

---

## 一、核心思想

> **一句话理解** ：提供一个接口，用于创建 **一系列相关联的对象** ，而无需指定它们的具体类。

 **类比帮助理解** ：

> 想象一家家具店有两种风格：**现代风**和 **古典风** 。你选了现代风，店员就给你配套的现代风桌子 + 现代风椅子 + 现代风沙发—— **一整套风格统一的产品** 。你不需要知道每件家具怎么做的，只需要告诉店员"我要现代风"，剩下的工厂帮你搞定。
>
> 这里的"现代风工厂"就是 **具体工厂** ，"家具工厂"接口就是 **抽象工厂** 。

---

## 二、与工厂方法模式的区别

这是理解抽象工厂的 **最关键一步** 。

| 对比维度               | 工厂方法模式                         | 抽象工厂模式                       |
| ---------------------- | ------------------------------------ | ---------------------------------- |
| **实现方式**     | 子类**继承**父类，重写工厂方法 | 通过**构造函数传入**工厂对象 |
| **创建产品数量** | 每个工厂只创建**一种**产品     | 每个工厂创建**一族相关**产品 |
| **扩展方式**     | 新增子类                             | 新增工厂实现类                     |
| **依赖关系**     | 依赖继承                             | 依赖组合（更现代）                 |

 **一句话区分** ：

* 工厂方法：**一个工厂，一种产品**
* 抽象工厂：**一个工厂，一族产品**

---

## 三、用同一个 ILogger 例子对比两种模式

### 工厂方法的做法（回顾）

```java
// 父类暴露抽象方法，子类继承并重写
abstract class Application {
    public abstract ILogger createLogger();  // 子类决定创建哪种 Logger
}

class DevelopmentApplication extends Application {
    public ILogger createLogger() { return new ConsoleLogger(); }
}
```

### 抽象工厂的做法

```java
// Step 1：定义抽象工厂接口
interface ILoggerFactory {
    ILogger createLogger();
}

// Step 2：实现具体工厂类
class ConsoleLoggerFactory implements ILoggerFactory {
    public ILogger createLogger() { return new ConsoleLogger(); }
}

class FileLoggerFactory implements ILoggerFactory {
    public ILogger createLogger() { return new FileLogger(); }
}

class RemoteLoggerFactory implements ILoggerFactory {
    public ILogger createLogger() { return new RemoteLogger(); }
}

// Step 3：Application 不再需要继承，直接接收工厂对象
class Application {
    private ILogger logger;

    // ✅ 通过构造函数注入工厂（组合，而非继承）
    public Application(ILoggerFactory loggerFactory) {
        this.logger = loggerFactory.createLogger();
    }

    public void doSomething() {
        logger.log("Start doing something...");
    }
}
```

> ⚠️  **注意** ：上面这个 Logger 例子只有一个创建方法，用抽象工厂有点"大材小用"。抽象工厂真正的威力在于 **创建一族相关产品** ，下面的 UI 主题案例才是标准用法。

---

## 四、标准场景案例：UI 主题工厂

### 需求描述

应用程序需要支持**亮色/暗色**两套主题，且切换主题时，所有 UI 组件（按钮、复选框、文本）必须 **同步切换** ，保证风格统一。

```
亮色主题 → LightButton + LightCheckbox + LightText（必须配套）
暗色主题 → DarkButton  + DarkCheckbox  + DarkText （必须配套）
```

---

### Step 1：定义抽象产品接口

```java
// 三种 UI 组件的接口
interface IButton {
    void render();
}

interface ICheckbox {
    void render();
}

interface IText {
    void render();
}
```

---

### Step 2：定义抽象工厂接口

```java
// 🏭 抽象工厂：一次创建一整族 UI 组件
interface IThemeFactory {
    IButton   createButton();
    ICheckbox createCheckbox();
    IText     createText();
}
```

> **关键** ：一个工厂接口包含**多个**创建方法，这就是抽象工厂与工厂方法的核心区别。

---

### Step 3：实现具体产品

```java
// ☀️ 亮色主题组件
class LightButton implements IButton {
    public void render() { System.out.println("Render Light Button"); }
}
class LightCheckbox implements ICheckbox {
    public void render() { System.out.println("Render Light Checkbox"); }
}
class LightText implements IText {
    public void render() { System.out.println("Render Light Text"); }
}

// 🌙 暗色主题组件
class DarkButton implements IButton {
    public void render() { System.out.println("Render Dark Button"); }
}
class DarkCheckbox implements ICheckbox {
    public void render() { System.out.println("Render Dark Checkbox"); }
}
class DarkText implements IText {
    public void render() { System.out.println("Render Dark Text"); }
}
```

---

### Step 4：实现具体工厂

```java
// ☀️ 亮色主题工厂：负责生产整套亮色组件
class LightThemeFactory implements IThemeFactory {
    public IButton   createButton()   { return new LightButton(); }
    public ICheckbox createCheckbox() { return new LightCheckbox(); }
    public IText     createText()     { return new LightText(); }
}

// 🌙 暗色主题工厂：负责生产整套暗色组件
class DarkThemeFactory implements IThemeFactory {
    public IButton   createButton()   { return new DarkButton(); }
    public ICheckbox createCheckbox() { return new DarkCheckbox(); }
    public IText     createText()     { return new DarkText(); }
}
```

---

### Step 5：应用程序使用工厂

```java
class Application {
    private IButton   button;
    private ICheckbox checkbox;
    private IText     text;

    // ✅ 只依赖抽象接口，具体是亮色还是暗色，由传入的工厂决定
    public Application(IThemeFactory themeFactory) {
        this.button   = themeFactory.createButton();
        this.checkbox = themeFactory.createCheckbox();
        this.text     = themeFactory.createText();
    }

    public void start() {
        button.render();
        checkbox.render();
        text.render();
    }

    public static void main(String[] args) {
        // 切换主题只需换一个工厂对象，业务代码完全不变
        Application darkApp = new Application(new DarkThemeFactory());
        darkApp.start();
        // 输出：Render Dark Button
        //       Render Dark Checkbox
        //       Render Dark Text

        Application lightApp = new Application(new LightThemeFactory());
        lightApp.start();
        // 输出：Render Light Button
        //       Render Light Checkbox
        //       Render Light Text
    }
}
```

> **如果将来新增"高对比度主题"** ，只需：
>
> 1. 新增 `HighContrastButton` / `HighContrastCheckbox` / `HighContrastText`
> 2. 新增 `HighContrastThemeFactory`
> 3. `Application` 的代码**一行都不用改** ✅

---

## 五、四个关键角色总结

```
┌─────────────────────────────────────────────────────────────────┐
│                     抽象工厂模式四个角色                          │
│                                                                 │
│  抽象产品（AbstractProduct）                                     │
│      IButton / ICheckbox / IText                                │
│                                                                 │
│  具体产品（ConcreteProduct）                                     │
│      LightButton / LightCheckbox / LightText                    │
│      DarkButton  / DarkCheckbox  / DarkText                     │
│                                                                 │
│  抽象工厂（AbstractFactory）                                     │
│      IThemeFactory（含多个 create 方法）                         │
│                                                                 │
│  具体工厂（ConcreteFactory）                                     │
│      LightThemeFactory / DarkThemeFactory                       │
└─────────────────────────────────────────────────────────────────┘
```

---

## 六、结构图

```
IThemeFactory（抽象工厂）
    ├── LightThemeFactory（具体工厂）
    │       ├── createButton()   → LightButton
    │       ├── createCheckbox() → LightCheckbox
    │       └── createText()     → LightText
    │
    └── DarkThemeFactory（具体工厂）
            ├── createButton()   → DarkButton
            ├── createCheckbox() → DarkCheckbox
            └── createText()     → DarkText

Application（客户端）
    └── 依赖 IThemeFactory、IButton、ICheckbox、IText（全是接口）
        ← 完全不知道 Light/Dark 具体类的存在
```

---

## 七、优缺点分析

### ✅ 优点

**1. 降低耦合度**

```java
// Application 只依赖接口，从不出现具体类名
IButton button = themeFactory.createButton();   // ✅ 依赖接口
// 而不是：
LightButton button = new LightButton();          // ❌ 依赖具体类
```

> **类比** ：你去餐厅只需说"我要套餐A"，不需要知道厨师名字和具体食材来源。

**2. 保证产品族的兼容性**

一个工厂只产出同一套风格的组件， **从根本上杜绝了"亮色按钮 + 暗色文本"的混搭错误** 。

> **类比** ：家具店的"现代风套餐"保证你买到的所有家具都是现代风，不会给你混一把古典椅子。

**3. 符合"组合优于继承"原则**

工厂作为对象被**注入**到 Application，而非通过继承实现——更灵活，运行时也可以动态切换。

---

### ❌ 缺点

**类和接口数量多，代码复杂度高**

| 主题数量 | 需要的类/接口数量（3种组件）                                      |
| -------- | ----------------------------------------------------------------- |
| 1种主题  | 3个产品接口 + 1个工厂接口 + 3个具体产品 + 1个工厂 =**8个**  |
| 2种主题  | 3个产品接口 + 1个工厂接口 + 6个具体产品 + 2个工厂 =**12个** |
| N种主题  | 4 + N×4 个类/接口                                                |

> 对于简单场景，这种复杂度得不偿失。

---

## 八、使用时机判断

```
需要创建的对象之间有"配套关系"（必须同时切换）？
    ├── 是 → 考虑抽象工厂
    │         └── 业务是否足够复杂，值得引入这些类？
    │               ├── 是 → ✅ 使用抽象工厂
    │               └── 否 → 直接用简单 if/else 即可
    └── 否 → 工厂方法模式或更简单的方案
```

 **适合场景举例** ：

* 跨平台 UI 框架（Windows 组件 / macOS 组件）
* 多数据库支持（MySQL 连接/查询/事务 vs PostgreSQL 的一套）
* 多主题 / 多皮肤应用

---

## 九、三种工厂模式横向对比

|                        | 简单工厂       | 工厂方法 | 抽象工厂     |
| ---------------------- | -------------- | -------- | ------------ |
| **扩展方式**     | 改 if/else     | 加子类   | 加工厂实现类 |
| **创建产品种类** | 一种           | 一种     | 一族（多种） |
| **依赖关系**     | 直接依赖具体类 | 依赖继承 | 依赖组合     |
| **复杂度**       | 低             | 中       | 高           |
| **适用规模**     | 小型项目       | 中型框架 | 复杂业务系统 |

---

## 十、总结

```
抽象工厂模式
├── 核心思想：一个工厂负责创建一族相关产品，保证产品兼容
├── 与工厂方法的本质区别
│   ├── 工厂方法：继承 + 一种产品
│   └── 抽象工厂：组合 + 一族产品
├── 四个角色
│   ├── 抽象产品接口（多个）
│   ├── 具体产品实现（多个）
│   ├── 抽象工厂接口（含多个 create 方法）
│   └── 具体工厂实现（每个工厂负责一套产品）
├── 优点：解耦、保证产品族兼容性、符合组合优于继承
├── 缺点：类数量多，复杂度高
└── 适用场景：产品必须配套使用、需要在多套方案间整体切换
```

> **最终类比总结** ：抽象工厂就像 **装修公司的"整装套餐"** 。你选了北欧风套餐，公司给你配套的北欧风地板 + 北欧风家具 + 北欧风灯具，保证整体统一。你不需要每样单独挑，也不会出现"北欧地板配中式家具"的尴尬搭配。
>
