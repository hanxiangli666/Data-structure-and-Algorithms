# 工厂方法模式（Factory Method Pattern）学习笔记

---

## 一、核心思想

> **一句话理解** ：父类定义"创建对象的步骤框架"，但 **把"创建哪种对象"的决定权交给子类** 。

 **类比帮助理解** ：

> 想象一家连锁咖啡店总部（父类）规定了"制作饮品的流程"：准备杯子 → 制作饮品 → 加盖 → 交给顾客。但 **具体做什么饮品** ，由各地分店（子类）自己决定——北京分店做拿铁，上海分店做抹茶拿铁。总部的流程不变，分店各自"工厂"出不同的产品。

---

## 二、为什么需要工厂方法模式？

 **问题场景** ：业务逻辑完全相同，但依赖的对象需要根据环境/条件而变化。

以文中的日志场景为例：

```
同一套业务代码（doSomething）
    ├── 本地开发环境  → 打印到控制台（方便调试）
    ├── 测试环境      → 写入文件（方便查历史）
    └── 生产环境      → 发送到远程服务器（实时监控）
```

 **如果不用工厂方法** ，业务代码里会充斥大量 `if/else`：

```java
// ❌ 糟糕的做法：业务逻辑和对象创建混在一起
public void doSomething() {
    ILogger logger;
    if (env.equals("dev")) {
        logger = new ConsoleLogger();
    } else if (env.equals("prod")) {
        logger = new RemoteLogger("http://...");
    }
    logger.log("...");  // 每次业务方法都要重复这段判断！
}
```

 **使用工厂方法后** ：业务代码只管调用，对象的创建细节完全隔离。

---

## 三、四个关键角色

```
┌──────────────────────────────────────────────────────────────┐
│                    工厂方法模式的四个角色                      │
│                                                              │
│  Product（产品接口）        ←  ILogger                        │
│  ConcreteProduct（具体产品）←  ConsoleLogger / FileLogger / RemoteLogger │
│  Creator（创建者父类）      ←  Application（含抽象工厂方法）   │
│  ConcreteCreator（具体创建者子类）← DevelopmentApp / TestApp / ProdApp  │
└──────────────────────────────────────────────────────────────┘
```

---

## 四、完整案例拆解

### Step 1：定义产品接口（Product）

```java
interface ILogger {
    void log(String message);  // 所有日志器都必须实现这个方法
}
```

> **类比** ：这是"饮品"的定义——所有饮品都必须能"被喝"，但具体是什么饮品不限。

---

### Step 2：实现具体产品（ConcreteProduct）

```java
// 控制台日志 → 适合本地开发，直接打印，即时可见
class ConsoleLogger implements ILogger {
    public void log(String message) {
        System.out.println("CONSOLE: " + message);
    }
}

// 文件日志 → 适合测试环境，保留历史记录
class FileLogger implements ILogger {
    private String filePath;
    public FileLogger(String filePath) { this.filePath = filePath; }
  
    public void log(String message) {
        System.out.println("WRITE TO " + filePath + ": " + message);
    }
}

// 远程日志 → 适合生产环境，附带时间戳等元信息，发到监控服务器
class RemoteLogger implements ILogger {
    private String remoteServer;
    public RemoteLogger(String remoteServer) { this.remoteServer = remoteServer; }
  
    public void log(String message) {
        String payload = "{\"message\":\"" + message 
                       + "\",\"timestamp\":\"" + System.currentTimeMillis() + "\"}";
        System.out.println("SEND TO " + remoteServer + ": " + payload);
    }
}
```

---

### Step 3：定义创建者父类（Creator）

```java
abstract class Application {

    private ILogger logger;
  
    public Application() {
        this.logger = createLogger();  // 调用工厂方法（具体实现交给子类）
    }

    // ✅ 业务方法：只依赖 ILogger 接口，完全不关心具体是哪种 Logger
    public void doSomething() {
        logger.log("Start doing something...");
    }

    // 🏭 抽象工厂方法：子类必须实现，决定创建哪种 Logger
    public abstract ILogger createLogger();
}
```

> **关键点** ：`doSomething()` 永远不需要修改，无论将来新增多少种日志方式。

---

### Step 4：实现具体创建者子类（ConcreteCreator）

```java
// 开发环境 → 工厂生产 ConsoleLogger
class DevelopmentApplication extends Application {
    @Override
    public ILogger createLogger() {
        return new ConsoleLogger();
    }
}

// 测试环境 → 工厂生产 FileLogger
class TestingApplication extends Application {
    @Override
    public ILogger createLogger() {
        return new FileLogger("application.log");
    }
}

// 生产环境 → 工厂生产 RemoteLogger
class ProductionApplication extends Application {
    @Override
    public ILogger createLogger() {
        return new RemoteLogger("http://remote-server.com");
    }
}
```

---

### Step 5：调用方（Main）

```java
class Main {
    public static void main(String[] args) {
        String env = System.getenv("ENV");
      
        Application app;
        if (env.equals("dev"))        app = new DevelopmentApplication();
        else if (env.equals("test"))  app = new TestingApplication();
        else if (env.equals("prod"))  app = new ProductionApplication();
        else throw new IllegalArgumentException("Invalid environment: " + env);

        app.doSomething();  // 调用方式完全相同，内部行为自动适配
    }
}
```

---

## 五、结构总览图

```
ILogger（产品接口）
    ├── ConsoleLogger（具体产品）
    ├── FileLogger（具体产品）
    └── RemoteLogger（具体产品）

Application（创建者父类）
    ├── doSomething() → 业务逻辑，只用 ILogger 接口
    └── createLogger() → 抽象工厂方法 ← 子类重写这里
            ├── DevelopmentApplication → 返回 ConsoleLogger
            ├── TestingApplication     → 返回 FileLogger
            └── ProductionApplication → 返回 RemoteLogger
```

---

## 六、优缺点分析

### ✅ 优点

| 优点               | 解释                                                  | 类比                                     |
| ------------------ | ----------------------------------------------------- | ---------------------------------------- |
| **解耦**     | `Application`只依赖 `ILogger`接口，不关心具体实现 | 总部只规定"要有饮品"，不管分店做什么口味 |
| **扩展性强** | 新增环境只需加新子类，不改现有代码                    | 新开城市分店，不影响其他分店             |
| **单一职责** | 每个子类只负责创建一种 Logger，职责清晰               | 每个分店只专注自己的配方                 |

### ❌ 缺点

| 缺点                 | 解释                                                   |
| -------------------- | ------------------------------------------------------ |
| **类数量膨胀** | 每增加一种 Logger，就要增加一个对应的 Application 子类 |
| **复杂度上升** | 类的层级变多，理解和维护成本增加                       |

 **举例说明类数量膨胀问题** ：

> 如果将来要支持 5 种日志方式（控制台、文件、远程、数据库、消息队列），就需要 5 个 Logger 类 + 5 个 Application 子类 =  **10 个类** 。类的数量和功能数量成倍增长。

---

## 七、实际使用场景

### 适合使用的场景

```
✅ UI 框架开发
   → 框架需要通用渲染逻辑（父类），
     但具体渲染什么组件由应用开发者决定（子类）

✅ 需要"继承 + 扩展"的框架设计
   → 工厂方法在继承体系中可以很好地解耦父子类
```

### 不适合/不推荐的场景

```
❌ 日志库（实际开发中）
   → Log4j、SLF4J 等成熟框架已通过配置文件解决，无需如此复杂

❌ 普通应用开发
   → 现代软件推崇"组合优于继承"，
     工厂方法依赖继承，与这一原则相悖
```

> **"组合优于继承"是什么意思？**
> 类比：与其让"上海分店"继承"咖啡店总部"的所有规定，不如让上海分店 **持有一个"饮品制作器"对象** ，想换口味直接换对象就行，不用改继承关系。组合更灵活、耦合更低。

---

## 八、总结

```
工厂方法模式
├── 核心思想：父类定框架，子类决定创建哪种对象
├── 四个角色
│   ├── Product（产品接口）
│   ├── ConcreteProduct（具体产品）
│   ├── Creator（含抽象工厂方法的父类）
│   └── ConcreteCreator（重写工厂方法的子类）
├── 优点：解耦、扩展性强、单一职责
├── 缺点：类数量膨胀、复杂度上升
└── 适用场景：UI 框架、需要继承体系的框架设计
    不适合：普通应用开发（推荐用组合替代继承）
```

> **最终类比总结** ：工厂方法就像 **连锁品牌的加盟模式** 。总部（父类）规定统一的运营流程，各加盟商（子类）自行决定卖什么产品。流程稳定、产品灵活，但加盟商一多，管理复杂度也随之上升。
>
