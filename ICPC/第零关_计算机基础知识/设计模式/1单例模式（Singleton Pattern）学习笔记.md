# 单例模式（Singleton Pattern）学习笔记

---

## 一、核心思想

> **一句话理解** ：整个应用程序中，某个类 **只能有一个实例** ，并且提供一个全局入口来访问它。

 **类比帮助理解** ：

> 想象一家公司只有 **一台打印机** 。所有员工都通过同一个"打印队列"来使用它，而不是每人买一台。这台打印机就是"单例"——全公司唯一，人人可访问，不重复创建。

---

## 二、为什么需要单例模式？

| 问题场景     | 如果不用单例                 | 用了单例之后               |
| ------------ | ---------------------------- | -------------------------- |
| 数据库连接   | 每次操作都新建连接，浪费资源 | 全局共享一个连接，节省资源 |
| 配置文件读取 | 每个模块重复读取文件         | 只读一次，全局共享         |
| 日志写入     | 多个实例同时写文件，内容冲突 | 统一入口，有序写入         |

---

## 三、实现的三个关键点

```
┌─────────────────────────────────────────────────────┐
│                   单例模式三要素                      │
│                                                     │
│  1. 私有构造函数   →  外部无法直接 new 一个新实例       │
│  2. 全局访问点     →  提供 static 方法获取唯一实例      │
│  3. 并发安全       →  多线程下也只创建一个实例           │
└─────────────────────────────────────────────────────┘
```

### 3.1 私有构造函数

* 把类的构造函数设为 `private`，外部代码就**无法通过 `new` 关键字创建实例**
* 同时要 **禁用拷贝构造函数** ，防止通过复制来"偷"出第二个实例

```cpp
// 外部代码这样写会报错：
DatabaseManager db;               // ❌ 构造函数是私有的
DatabaseManager db2 = db;         // ❌ 拷贝构造函数被删除
```

### 3.2 全局访问点

* 提供一个 `public static` 的 `getInstance()` 方法
* 所有人都通过这个方法获取实例

```cpp
// 外部代码正确的使用方式：
DatabaseManager& db = DatabaseManager::getInstance();  // ✅
db.execute("SELECT * FROM users");
```

### 3.3 并发安全

* 多个线程同时调用 `getInstance()` 时，必须保证只创建**一个**实例
* 不同实现方式对并发安全的处理策略不同（见下文）

---

## 四、两种实现方式

### 4.1 饿汉式（Eager Initialization）

> **类比** ：餐厅开门前就把所有餐具摆好，不管今天有没有客人。

 **特点** ：

* 程序**启动时**就立即创建实例
* 实现最简单
* 天然线程安全（静态成员变量初始化由编译器/运行时保证）

```cpp
class DatabaseManager {
private:
    static DatabaseManager instance;  // 程序启动时就创建
    std::Connection connection;
  
    DatabaseManager() {
        connection = getConnection("mysql://...", "user", "password");
    }

public:
    // 禁止拷贝
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
  
    static DatabaseManager& getInstance() {
        return instance;  // 直接返回已创建的实例
    }
  
    std::string execute(const std::string& sql) { /* ... */ }
};

// 类外定义静态成员（触发构造）
DatabaseManager DatabaseManager::instance;
```

 **优缺点对比** ：

| 优点         | 缺点                               |
| ------------ | ---------------------------------- |
| 实现简单     | 程序启动时就占用内存，即使从未使用 |
| 天然线程安全 | 启动时间可能略微增加               |
| 无需加锁     |                                    |

---

### 4.2 懒汉式（Lazy Initialization）

> **类比** ：餐厅只在第一位客人到来时才开始摆餐具。节约了资源，但需要确保两个服务员不会同时摆同一套餐具（线程安全问题）。

 **特点** ：

* **第一次使用时**才创建实例（懒加载）
* 节省内存，提升启动速度
* 需要额外保证线程安全

```cpp
class DatabaseManager {
private:
    std::Connection connection;
  
    DatabaseManager() {
        connection = getConnection("mysql://...", "user", "password");
    }

public:
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
  
    static DatabaseManager* getInstance() {
        // C++11 起，静态局部变量的初始化是线程安全的
        // 第一次调用时才初始化，之后直接返回同一个实例
        static DatabaseManager instance;
        return &instance;
    }
  
    std::string execute(const std::string& sql) { /* ... */ }
};
```

 **优缺点对比** ：

| 优点             | 缺点                 |
| ---------------- | -------------------- |
| 未使用时不占内存 | 实现相对复杂         |
| 加快程序启动速度 | 需要特别考虑线程安全 |

---

### 4.3 两种方式该怎么选？

```
实例创建开销很大（如数据库连接）？
    ├── 是 → 程序肯定会用到它吗？
    │         ├── 是 → 饿汉式（启动时就准备好）
    │         └── 否 → 懒汉式（用到再创建，节省资源）
    └── 否 → 两种均可，饿汉式更简单
```

---

## 五、关于线程安全的边界

⚠️  **重要提醒** ：单例模式的线程安全 **只保证 `getInstance()` 方法本身** 。

```
单例模式保证的范围：
  ✅ 并发调用 getInstance() → 只会创建一个实例

单例模式不保证的范围：
  ❌ execute() 等业务方法的并发安全 → 需要自己实现（如加锁）
```

 **举例说明** ：

> 假设线程A和线程B同时调用 `db.execute("UPDATE balance SET amount=100")`，可能会出现数据竞争。这和单例无关，需要在 `execute` 方法内部加锁来解决。

---

## 六、典型应用场景

### 6.1 数据库连接管理器

```java
// 所有业务代码通过同一个实例操作数据库
DatabaseManager db = DatabaseManager.getInstance();
db.execute("SELECT * FROM orders");
```

 **为什么适合单例** ：数据库连接建立代价高，全局共享可以复用连接，避免资源浪费。

---

### 6.2 配置管理器

```java
public class DatabaseService {
    public void connect() {
        ConfigManager config = ConfigManager.getInstance();
        String dbUrl = config.get("database.url");  // 读配置
        // 建立数据库连接...
    }
}

public class EmailService {
    public void sendEmail() {
        ConfigManager config = ConfigManager.getInstance();  // 同一个实例！
        String apiKey = config.get("email.api.key");
        // 发送邮件...
    }
}
```

 **为什么适合单例** ：配置文件只需加载一次，全局共享同一份配置，避免重复 I/O。

---

### 6.3 日志记录器

```java
public class AppLogger {
    private static final AppLogger instance = new AppLogger();
    private FileWriter fileWriter;

    private AppLogger() {
        fileWriter = new FileWriter("application.log", true);
    }

    public static AppLogger getInstance() { return instance; }

    public void log(String level, String message) { /* 写入文件 */ }
}

// 各模块统一使用
AppLogger.getInstance().log("INFO", "用户登录成功");
AppLogger.getInstance().log("ERROR", "数据库连接失败");
```

 **为什么适合单例** ：

* 所有日志通过**同一个入口**有序写入，避免文件冲突
* Log4j、SLF4J 等主流日志框架都采用了类似思想

---

## 七、注意事项与常见误区

### ✅ 适合用单例的场景

* 资源昂贵，只需一份（数据库连接、线程池）
* 需要全局一致的状态（配置、日志）
* 需要协调多处访问同一资源（文件写入）

### ❌ 不适合/滥用的场景

```
滥用单例的后果：
  1. 引入全局状态   → 代码逻辑难以追踪
  2. 增加耦合度     → 模块之间依赖隐式的全局对象
  3. 难以单元测试   → 无法轻易替换为 Mock 对象
```

 **类比** ：

> 单例就像公司的"公共资源"（如会议室）。会议室适合共享，但如果把员工的私人办公桌也变成"共享单例"，反而会造成混乱。 **不是所有东西都适合全局共享** 。

### 使用前问自己

> * 这个对象**真的需要**全局唯一吗？
> * 如果有两个实例，会出现**具体的问题**吗？
> * 是否有更好的方式传递这个依赖（如依赖注入）？

---

## 八、总结

```
单例模式
├── 核心目标：全局唯一实例 + 全局访问点
├── 三要素：私有构造 + static方法 + 并发安全
├── 实现方式
│   ├── 饿汉式：启动即创建，简单安全
│   └── 懒汉式：用时才创建，节省资源
├── 典型场景：数据库连接、配置管理、日志记录
└── 注意事项：警惕滥用，避免全局状态污染代码
```
