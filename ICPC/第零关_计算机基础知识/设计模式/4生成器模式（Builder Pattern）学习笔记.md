# 生成器模式（Builder Pattern）学习笔记

---

## 一、要解决什么问题？

先看一个让人头疼的代码场景：

```java
// 😫 传统构造函数方式
Resume resume1 = new Resume("Tom", 23, "本科", "某奖项", null, null);
Resume resume2 = new Resume("Jack", 30, "硕士", null, "XX工程师", "A公司");
```

 **三个明显的痛点** ：

| 问题         | 具体表现                                                        |
| ------------ | --------------------------------------------------------------- |
| 参数列表过长 | 构造函数有 6 个参数，一眼看不出每个位置是什么含义               |
| 大量 null    | 可选参数不存在时必须传 null，代码丑且容易出错（万一顺序填错？） |
| 扩展困难     | 新增一个字段，所有调用构造函数的地方都要跟着改                  |

 **类比帮助理解** ：

> 传统构造函数就像去餐厅点餐时，服务员让你**按固定顺序**报出所有菜名，没有的也要说"不要不要不要不要"。而生成器模式就像 **点菜单勾选** ——你只需要在想要的菜旁边打勾，空着的自然就是不要，清晰又灵活。

---

## 二、核心思想

> **一句话理解** ：用一个专门的 `Builder` 类，通过**链式调用**一步一步设置对象的属性，最后调用 `build()` 生成最终对象。

---

## 三、完整实现拆解

### Step 1：产品类（Product）

```java
class Resume {
    private String name;           // 必选
    private int age;               // 必选
    private String education;      // 必选
    private String awards;         // 可选
    private String workExperience; // 可选
    private String companyName;    // 可选

    // 构造函数可以设为 private，强制只能通过 Builder 创建
    public Resume(String name, int age, String education,
                  String awards, String workExperience, String companyName) {
        this.name = name;
        this.age = age;
        this.education = education;
        this.awards = awards;
        this.workExperience = workExperience;
        this.companyName = companyName;
    }
}
```

---

### Step 2：生成器类（Builder）

```java
class ResumeBuilder {
    private String name;
    private int age;
    private String education;
    private String awards;
    private String workExperience;
    private String companyName;

    // 设置必选项
    public ResumeBuilder setBasicInfo(String name, int age, String education) {
        this.name = name;
        this.age = age;
        this.education = education;
        return this;  // ← 返回 this，支持链式调用！
    }

    // 设置可选项（不调用就是 null，无需显式传 null）
    public ResumeBuilder setAwards(String awards) {
        this.awards = awards;
        return this;
    }

    public ResumeBuilder setWorkExperience(String workExperience) {
        this.workExperience = workExperience;
        return this;
    }

    public ResumeBuilder setCompanyName(String companyName) {
        this.companyName = companyName;
        return this;
    }

    // 最终生成对象，并在此做参数校验
    public Resume build() {
        if (name == null || age == 0 || education == null) {
            throw new IllegalArgumentException("Name, age and education are required");
        }
        return new Resume(name, age, education, awards, workExperience, companyName);
    }
}
```

> **链式调用的关键** ：每个 `setXxx()` 方法都返回 `this`（当前 Builder 对象本身），所以可以不断地 `.setXxx().setXxx()...` 连续调用下去。

---

### Step 3：使用方式对比

```java
// ✅ 生成器模式：清晰、灵活
Resume resume1 = new ResumeBuilder()
        .setBasicInfo("Tom", 23, "本科")
        .setAwards("某奖项")          // 只设需要的
        .build();                      // 不需要工作经验，直接不写，不用传 null

Resume resume2 = new ResumeBuilder()
        .setBasicInfo("Jack", 30, "硕士")
        .setWorkExperience("XX工程师") // 每行清楚说明在设置什么
        .setCompanyName("A公司")
        .build();

// ❌ 传统方式：难以阅读
Resume resume1 = new Resume("Tom", 23, "本科", "某奖项", null, null);
```

---

## 四、更紧凑的写法：Builder 作为内部类

更常见的实践是将 `Builder` 放进产品类内部：

```java
class Resume {
    private String name;
    private int age;
    // ... 其他字段

    // ← 构造函数私有，外部无法直接 new Resume(...)
    private Resume(ResumeBuilder builder) {
        this.name = builder.name;
        this.age = builder.age;
        // ...
    }

    // ← 提供静态入口获取 Builder
    public static ResumeBuilder builder() {
        return new ResumeBuilder();
    }

    // ← Builder 作为静态内部类
    private static class ResumeBuilder {
        private String name;
        private int age;
        // ...

        public ResumeBuilder setBasicInfo(String name, int age, String education) {
            this.name = name;
            this.age = age;
            return this;
        }
        // ... 其他 set 方法

        public Resume build() {
            return new Resume(this);
        }
    }
}

// 使用时更简洁：
Resume resume = Resume.builder()
        .setBasicInfo("Tom", 23, "本科")
        .setAwards("某奖项")
        .build();
```

 **两种写法对比** ：

|          | 独立 Builder 类        | 内部类 Builder                        |
| -------- | ---------------------- | ------------------------------------- |
| 代码结构 | 两个独立文件           | 紧凑在一个文件                        |
| 封装性   | 产品类构造函数可能暴露 | 可完全封闭，强制走 Builder            |
| 常见度   | 简单场景               | 更常见（Lombok `@Builder`就是这种） |

---

## 五、两个关键角色

```
┌─────────────────────────────────────────────────┐
│              生成器模式两个角色                   │
│                                                 │
│  Product（产品）                                │
│      Resume —— 最终要创建的复杂对象              │
│                                                 │
│  Builder（生成器）                              │
│      ResumeBuilder —— 负责一步步构建产品         │
│          ├── setXxx() 方法（返回 this）          │
│          └── build() 方法（生成并返回产品）       │
└─────────────────────────────────────────────────┘
```

---

## 六、真实框架中的应用

### 6.1 MyBatis SQL 构建器

```java
// 用生成器模式构建 SQL，避免手动拼接字符串（容易 SQL 注入或语法错误）
SelectStatementProvider selectStatement = select(order.allColumns())
    .from(order)
    .where(name, isEqualTo("example"))
        .and(status, isEqualTo("finished"))
        .and(createTime, isGreaterThan(LocalDate.of(2025, 1, 1)))
    .build();

// 等价于：SELECT * FROM order WHERE name = 'example' 
//         AND status = 'finished' AND create_time > '2025-01-01'
```

> **为什么适合 Builder** ：SQL 有大量可选子句（WHERE / ORDER BY / LIMIT 等），用 Builder 按需拼接，比字符串拼接安全可靠得多。

---

### 6.2 OkHttp HTTP 请求构建器

```java
RequestBody body = RequestBody.create(
    "{\"name\":\"labuladong\",\"age\":30}",
    MediaType.get("application/json; charset=utf-8")
);

// HTTP 请求包含 URL、请求头、请求体等多个部分，天然适合 Builder
Request request = new Request.Builder()
        .url("https://api.example.com/users")
        .header("User-Agent", "My-Awesome-App")
        .post(body)
        .build();
```

---

## 七、优缺点分析

### ✅ 优点

**1. 代码可读性高**

```java
// 每行都在说"我在设置什么"，像读句子一样自然
new ResumeBuilder()
    .setBasicInfo("Tom", 23, "本科")  // 设置基本信息
    .setAwards("某奖项")              // 设置获奖情况
    .build()
```

**2. 可选参数灵活**

* 需要哪个就调哪个方法，不需要的直接跳过
* 彻底告别满屏的 `null`

**3. 构建逻辑集中，校验方便**

```java
public Resume build() {
    // ✅ 所有参数校验集中在一处
    if (name == null || age == 0) {
        throw new IllegalArgumentException("必选项不能为空");
    }
    return new Resume(...);
}
```

---

### ❌ 缺点

**1. 额外的类**

需要额外编写 Builder 类，增加了代码量。

**2. 字段重复定义**

Builder 类和产品类通常有相同的字段，造成一定冗余。

```java
class Resume       { String name; int age; ... }  // 产品类
class ResumeBuilder{ String name; int age; ... }  // Builder 类（字段几乎一样！）
```

> 💡 实际开发中，Java 的 Lombok 库提供了 `@Builder` 注解，可以自动生成 Builder 代码，彻底消除这个冗余问题。

---

## 八、什么时候该用生成器模式？

```
构造函数参数超过 4 个？
    ├── 是 → 有很多可选参数吗？
    │         ├── 是 → ✅ 强烈建议用 Builder
    │         └── 否 → 可以考虑用 Builder 提升可读性
    └── 否 → 参数少，直接构造函数即可
```

 **简单记忆** ：

> 参数 ≤ 3 个且全是必选 → 普通构造函数就够了
> 参数 > 4 个 或 有多个可选参数 → 考虑 Builder

---

## 九、总结

```
生成器模式
├── 核心思想：用 Builder 类链式设置属性，最后 build() 生成对象
├── 两个角色
│   ├── Product（产品）：最终要创建的复杂对象
│   └── Builder（生成器）：逐步构建，提供 setXxx() + build()
├── 链式调用的关键：每个 set 方法返回 this
├── 优点：可读性高、可选参数灵活、校验逻辑集中
├── 缺点：多一个 Builder 类、字段有冗余
└── 适用场景：构造函数参数多（>4）或可选参数多
```

> **最终类比总结** ：生成器模式就像 **网购时的商品定制页面** 。你不需要一口气在一个表单里填写所有参数，而是按区块勾选——基本款必填，颜色/尺寸/刻字等选填，最后点"加入购物车"（`build()`）生成订单。整个过程清晰、灵活，不需要为"我不想要刻字"这件事专门填一个"null"。
>
