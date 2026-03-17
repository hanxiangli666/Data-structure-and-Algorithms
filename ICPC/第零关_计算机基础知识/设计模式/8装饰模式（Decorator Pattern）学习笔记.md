# 装饰模式（Decorator Pattern）学习笔记

---

## 一、要解决什么问题？

 **场景** ：想给一个已有的对象 **增加额外功能** ，但不想修改它的源代码。

**直觉做法一：直接修改原类**

```java
// ❌ 直接在 UserDaoImpl 里加缓存逻辑
class UserDaoImpl implements UserDao {
    public String getUserNameBy(int id) {
        // 缓存逻辑 + 数据库查询逻辑混在一起
        // 违反开闭原则，想去掉缓存又得改回来
    }
}
```

**直觉做法二：用继承扩展**

```java
// ❌ 创建子类来增加功能
class CachedUserDaoImpl extends UserDaoImpl {
    // 如果要同时加缓存+日志+监控，需要多层继承
    // 继承链复杂，组合不灵活
}
```

 **两种做法的共同问题** ：

| 问题         | 说明                                 |
| ------------ | ------------------------------------ |
| 违反开闭原则 | 修改已有代码容易引入 bug             |
| 功能耦合     | 增强功能和核心功能混在一起           |
| 组合困难     | 想同时叠加多种增强功能，继承层级爆炸 |

 **类比帮助理解** ：

> 装饰模式就像给咖啡 **加料** 。一杯黑咖啡是核心，你可以加牛奶变拿铁，再加糖变甜拿铁，再加冰变冰甜拿铁—— **每次加料都不改变原来的咖啡本身，只是套一层新的"包装"** 。而且你可以任意组合、随时撤掉某层加料，极其灵活。

---

## 二、与适配器模式的区别（重要！）

这两个模式都是"包装另一个对象"，但目的完全不同：

|                    | 适配器模式                         | 装饰器模式                       |
| ------------------ | ---------------------------------- | -------------------------------- |
| **目的**     | 转换接口（A → B）                 | 增强功能（A → 更强的A）         |
| **接口变化** | 输出接口与输入接口**不同**   | 输出接口与输入接口**相同** |
| **类比**     | 转换插头（把中国插头转成英国插头） | 咖啡加料（还是咖啡，但更丰富）   |

```
适配器：
  Chinese（speakChinese）  →  [TranslatorAdapter]  →  EnglishSpeaker（speakEnglish）
                                                        接口变了！

装饰器：
  UserDao（getUserName）  →  [CacheDecorator]  →  UserDao（getUserName）
                                                   接口没变，功能增强了！
```

---

## 三、核心思想

> **一句话理解** ：装饰器和被装饰对象实现 **同一个接口** ，装饰器在内部持有被装饰对象，在调用其方法的前后 **插入额外逻辑** ，对外表现得像一个"功能更强的同类对象"。

---

## 四、三个核心角色

```
┌──────────────────────────────────────────────────────────────┐
│                    装饰器模式三个角色                          │
│                                                              │
│  Component（组件接口）                                        │
│      定义被装饰对象和装饰器共同遵守的接口                       │
│      → UserDao 接口                                          │
│                                                              │
│  ConcreteComponent（具体组件）                                │
│      核心功能的实现类，被装饰的对象                            │
│      → UserDaoImpl 类                                        │
│                                                              │
│  Decorator（装饰器）                                          │
│      实现同一接口，持有被装饰对象，在调用前后插入额外逻辑        │
│      → UserDaoCacheDecorator 类                              │
└──────────────────────────────────────────────────────────────┘
```

---

## 五、基础案例：缓存装饰器

### 场景

`UserDaoImpl` 每次都查数据库，频繁查询同一用户时性能浪费。需要加缓存，但不改原有代码。

---

### Step 1：组件接口（Component）

```java
interface UserDao {
    String getUserNameBy(int id);
}
```

---

### Step 2：具体组件（ConcreteComponent）——核心功能

```java
class UserDaoImpl implements UserDao {
    @Override
    public String getUserNameBy(int id) {
        // 模拟数据库查询（每次调用都会执行）
        String userName = "User" + id;
        System.out.println("去数据库查询 id = " + id + " 用户名：" + userName);
        return userName;
    }
}
```

---

### Step 3：装饰器（Decorator）——增加缓存功能

```java
class UserDaoCacheDecorator implements UserDao {  // ← 实现同一接口！
    private final UserDao decoratedDao;           // ← 持有被装饰对象
    private final Map<Integer, String> cache = new HashMap<>();

    public UserDaoCacheDecorator(UserDao decoratedDao) {
        this.decoratedDao = decoratedDao;
    }

    @Override
    public String getUserNameBy(int id) {
        // 前置增强：先查缓存
        if (cache.containsKey(id)) {
            System.out.println("命中缓存，用户名：" + cache.get(id));
            return cache.get(id);
        }

        // 调用被装饰对象的原始方法
        String userName = decoratedDao.getUserNameBy(id);

        // 后置增强：存入缓存
        cache.put(id, userName);
        return userName;
    }
}
```

---

### Step 4：客户端使用

```java
public class Main {
    public static void main(String[] args) {
        // 用装饰器包裹原始对象
        UserDao dao = new UserDaoCacheDecorator(new UserDaoImpl());

        dao.getUserNameBy(1);
        // 输出：去数据库查询 id = 1 用户名：User1  ← 第一次，查数据库

        dao.getUserNameBy(1);
        // 输出：命中缓存，用户名：User1             ← 第二次，走缓存
    }
}
```

> **完全没有修改 `UserDaoImpl` 的任何代码** ，缓存功能就无感地插入进来了。

---

### 装饰器调用结构图

```
客户端
    ↓ 调用 getUserNameBy(1)
UserDaoCacheDecorator
    ├── [前置] 查 cache → 未命中
    ├── 调用 decoratedDao.getUserNameBy(1)
    │       ↓
    │   UserDaoImpl → 查数据库 → 返回 "User1"
    ├── [后置] 存入 cache
    └── 返回 "User1"

第二次调用：
UserDaoCacheDecorator
    ├── [前置] 查 cache → 命中！
    └── 直接返回 "User1"（不再调用底层）
```

---

## 六、进阶案例：中间件链（多层装饰器）

### 场景

HTTP 请求在到达业务逻辑前，需要依次经过： **日志记录 → 身份认证 → 业务处理** 。

这是装饰器模式最经典的应用之一，体现了**多个装饰器叠加**的威力。

---

### 组件接口

```java
interface Handler {
    void handle(Request request, Response response);
}
```

---

### 核心业务处理器（ConcreteComponent）

```java
class BusinessHandler implements Handler {
    public void handle(Request request, Response response) {
        System.out.println(">>> Entering BusinessHandler...");
        if ("/api/user".equals(request.getPath())) {
            response.setStatusCode(200);
            response.setBody("{ \"name\": \"Alice\" }");
        } else {
            response.setStatusCode(404);
            response.setBody("Not Found");
        }
        System.out.println("<<< Exiting BusinessHandler...");
    }
}
```

---

### 日志中间件（Decorator 1）

```java
class LoggingMiddleware implements Handler {
    private final Handler next;  // 持有下一个处理器

    public LoggingMiddleware(Handler next) { this.next = next; }

    public void handle(Request request, Response response) {
        System.out.println(">>> Entering LoggingMiddleware...");
        long start = System.currentTimeMillis();

        this.next.handle(request, response);  // 传递给下一层

        // 后置：记录耗时（在下一层执行完之后才执行）
        System.out.println("耗时：" + (System.currentTimeMillis() - start) + "ms");
        System.out.println("<<< Exiting LoggingMiddleware...");
    }
}
```

---

### 认证中间件（Decorator 2）

```java
class AuthenticationMiddleware implements Handler {
    private final Handler next;

    public AuthenticationMiddleware(Handler next) { this.next = next; }

    public void handle(Request request, Response response) {
        System.out.println(">>> Entering AuthenticationMiddleware...");
        String token = request.getHeader("Authorization");

        if ("valid-token".equals(token)) {
            System.out.println("认证成功，继续传递...");
            this.next.handle(request, response);  // 认证通过，传递给下一层
        } else {
            System.out.println("认证失败，拦截请求！");
            response.setStatusCode(401);
            response.setBody("Unauthorized");
            // ← 不调用 next.handle()，请求在此被拦截
        }
        System.out.println("<<< Exiting AuthenticationMiddleware...");
    }
}
```

---

### 组装与使用

```java
// 从内到外，一层层包裹
Handler business   = new BusinessHandler();
Handler withAuth   = new AuthenticationMiddleware(business);  // 业务外包认证
Handler withLogging = new LoggingMiddleware(withAuth);         // 认证外包日志

// 调用链：LoggingMiddleware → AuthenticationMiddleware → BusinessHandler
withLogging.handle(request, response);
```

---

### 执行顺序图解

```
外层先进，内层先出（像洋葱）：

        请求进入
            ↓
┌─── LoggingMiddleware ──────────────────────────────┐
│   [前] 记录请求开始时间                              │
│       ↓                                             │
│   ┌─── AuthenticationMiddleware ────────────────┐  │
│   │   [前] 验证 Token                            │  │
│   │       ↓ 认证通过                             │  │
│   │   ┌─── BusinessHandler ───────────────────┐ │  │
│   │   │   执行核心业务逻辑                      │ │  │
│   │   └───────────────────────────────────────┘ │  │
│   │   [后] （认证中间件无后置逻辑）               │  │
│   └──────────────────────────────────────────────┘  │
│   [后] 记录响应状态和耗时                            │
└─────────────────────────────────────────────────────┘
            ↓
        响应返回
```

 **认证失败时** ：认证中间件在"[前]"阶段就返回 401，不调用 `next.handle()`，业务逻辑完全没有被执行。

---

### 链式输出对比

```
✅ 认证成功：
>>> Entering LoggingMiddleware...
>>> Entering AuthenticationMiddleware...
认证成功，继续传递...
>>> Entering BusinessHandler...
<<< Exiting BusinessHandler...
<<< Exiting AuthenticationMiddleware...
耗时：1ms
<<< Exiting LoggingMiddleware...

❌ 认证失败：
>>> Entering LoggingMiddleware...
>>> Entering AuthenticationMiddleware...
认证失败，拦截请求！
<<< Exiting AuthenticationMiddleware...     ← BusinessHandler 没有被调用
耗时：0ms
<<< Exiting LoggingMiddleware...
```

---

## 七、多层装饰器的灵活性

装饰器最大的优势之一是 **自由组合** ：

```java
// 只要日志，不要认证
Handler chain1 = new LoggingMiddleware(business);

// 只要认证，不要日志
Handler chain2 = new AuthenticationMiddleware(business);

// 日志 + 认证
Handler chain3 = new LoggingMiddleware(new AuthenticationMiddleware(business));

// 日志 + 认证 + 压缩（将来新增）
Handler chain4 = new LoggingMiddleware(
                     new AuthenticationMiddleware(
                         new CompressionMiddleware(business)));
```

> 新增 `CompressionMiddleware` 只需写一个新类， **不需要修改任何现有类** ，完美符合开闭原则。

---

## 八、优缺点分析

### ✅ 优点

**1. 符合开闭原则**

增加新功能 = 写新装饰器类，原有代码一行不改。

**2. 灵活组合，运行时可变**

```java
// 想要缓存就加上，不想要就去掉，甚至运行时动态决定
UserDao dao = needCache
    ? new UserDaoCacheDecorator(new UserDaoImpl())
    : new UserDaoImpl();
```

**3. 职责分离，代码清晰**

```
UserDaoImpl          → 只管数据库查询
UserDaoCacheDecorator → 只管缓存逻辑
各司其职，互不干扰
```

---

### ❌ 缺点

 **类数量增多** ：每增加一种装饰功能，就要新建一个装饰器类。

 **调试复杂** ：多层装饰器叠加时，追踪一次调用的完整执行路径较复杂。

---

## 九、什么时候用装饰器模式？

```
需要给对象增加功能？
    ├── 能修改原类源码吗？
    │     ├── 不能（第三方库/历史遗留）→ ✅ 用装饰器
    │     └── 能 → 这个功能是核心逻辑吗？
    │               ├── 是 → 直接加进去
    │               └── 否（缓存/日志/认证等横切关注点）→ ✅ 用装饰器
    └── 功能需要灵活组合/随时撤销？→ ✅ 强烈推荐装饰器
```

---

## 十、总结

```
装饰器模式
├── 核心思想：装饰器和被装饰对象实现同一接口，
│           在调用前后插入额外逻辑，对外透明
├── 三个角色
│   ├── Component（接口）：统一定义
│   ├── ConcreteComponent：核心功能实现
│   └── Decorator：持有 Component，包裹并增强功能
├── vs 适配器模式
│   ├── 适配器：转换接口（接口变了）
│   └── 装饰器：增强功能（接口不变）
├── 多层装饰器 = 中间件链（洋葱模型）
│   外层先进后出，可以拦截请求
├── 优点：开闭原则、灵活组合、职责分离
├── 缺点：类数量增多，多层时调试复杂
└── 适用场景：需动态增强功能，且不想/不能修改原有代码
```

> **最终类比总结** ：装饰器模式就是 **给咖啡加料** 。黑咖啡（核心组件）是基础，加牛奶（装饰器1）变拿铁，再加糖（装饰器2）变甜拿铁，再加冰（装饰器3）变冰甜拿铁。每次加料都不改变原来的咖啡，只是套一层新包装。你可以自由选择加几层、加什么、顺序如何，最终喝到的还是"咖啡"（同一个接口），只是越来越丰富。
>
