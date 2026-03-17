# 原型模式（Prototype Pattern）学习笔记

---

## 一、要解决什么问题？

 **场景** ：需要创建一个与现有对象相同或相似的新对象。

 **直觉做法** ：手动把每个字段复制过去：

```java
// 😫 手动复制的方式
List<String> admin2Permissions = new ArrayList<>(admin1.getPermissions());
User admin2 = new User("Admin2", admin2Permissions);
```

 **这样做的两个问题** ：

| 问题               | 具体表现                                                      |
| ------------------ | ------------------------------------------------------------- |
| **代码耦合** | 调用方必须了解 `User`类的所有字段才能复制，内部细节暴露在外 |
| **维护困难** | `User`新增字段后，所有手动复制的地方都要跟着改，容易遗漏    |

 **类比帮助理解** ：

> 手动复制就像你要复印一份文件，却被要求自己拿纸笔把每个字抄一遍，还得知道文件里有哪些内容。而原型模式就像直接按 **复印机的复印键** ——你不需要知道文件里写了什么，机器自己搞定复制的事。

---

## 二、核心思想

> **一句话理解** ：不让调用方来复制对象，而是 **让对象自己负责复制自己** 。

对象内部实现一个 `clone()` 方法，外部只需调用这个方法，完全不用关心内部细节。

---

## 三、完整实现拆解

### Step 1：定义克隆接口（Prototype）

```java
interface Cloneable<T> {
    T clone();  // 每个可被克隆的类都要实现这个方法
}
```

---

### Step 2：让产品类实现克隆（Concrete Prototype）

```java
class User implements Cloneable<User> {
    private String name;
    private List<String> permissions;

    public User(String name, List<String> permissions) {
        this.name = name;
        this.permissions = permissions;
    }

    // getter / setter 省略...

    @Override
    public User clone() {
        // ⚠️ 关键：必须创建新的 permissions 列表（深拷贝）
        // 如果直接 return new User(this.name, this.permissions);
        // 两个对象会共享同一个 permissions 列表，改一个会影响另一个！
        List<String> clonedPermissions = new ArrayList<>(this.permissions);
        return new User(this.name, clonedPermissions);
    }
}
```

---

### Step 3：调用方使用（Client）

```java
// 创建原始对象
List<String> permissions = new ArrayList<>();
permissions.add("view");
permissions.add("edit");
permissions.add("delete");
User admin1 = new User("Admin", permissions);

// ✅ 原型模式：直接克隆，不关心内部细节
User admin2 = admin1.clone();
admin2.setName("Admin2");

// 修改 admin2 的权限不会影响 admin1
admin2.getPermissions().add("publish");
// admin1 的权限仍然是 [view, edit, delete]
// admin2 的权限是 [view, edit, delete, publish]
```

---

## 四、深拷贝 vs 浅拷贝（重要！）

这是原型模式中**最容易出错**的地方，必须理解。

### 浅拷贝（错误做法）

```java
// ❌ 浅拷贝：新对象和原对象共享同一个 permissions 列表
public User clone() {
    return new User(this.name, this.permissions);  // 直接传同一个引用
}

User admin2 = admin1.clone();
admin2.getPermissions().add("publish");
// 💥 admin1 的 permissions 也被修改了！因为两者指向同一个列表对象
```

 **类比** ：

> 浅拷贝就像复印了一张纸，但纸上的"附件清单"还是共用同一个抽屉里的附件。你往那个抽屉加东西，两份文件都"变了"。

### 深拷贝（正确做法）

```java
// ✅ 深拷贝：创建全新的列表，彻底独立
public User clone() {
    List<String> clonedPermissions = new ArrayList<>(this.permissions);  // 新列表！
    return new User(this.name, clonedPermissions);
}
```

 **类比** ：

> 深拷贝就像复印了整份文件，包括附件也全部复印了一遍。两份文件完全独立，修改一份不影响另一份。

### 对比图

```
浅拷贝：
  admin1 ──→ name: "Admin"
             permissions ──→ ["view", "edit", "delete"]  ← 共用！
  admin2 ──→ name: "Admin2"                                    ↑
             permissions ─────────────────────────────────────┘

深拷贝：
  admin1 ──→ name: "Admin"
             permissions ──→ ["view", "edit", "delete"]  ← 各自独立
  admin2 ──→ name: "Admin2"
             permissions ──→ ["view", "edit", "delete"]  ← 全新副本
```

---

## 五、三个关键角色

```
┌──────────────────────────────────────────────────────────┐
│                   原型模式三个角色                         │
│                                                          │
│  Prototype（原型接口）                                    │
│      Cloneable<T> ── 声明 clone() 方法                   │
│                                                          │
│  ConcretePrototype（具体原型）                            │
│      User ── 实现 clone()，负责复制自身                   │
│                                                          │
│  Client（客户端）                                        │
│      调用 clone() 获取新对象，不关心复制细节               │
└──────────────────────────────────────────────────────────┘
```

---

## 六、进阶：原型管理器（Prototype Manager）

### 应用场景

当系统中有 **多种预设的原型对象** ，需要统一管理和按需克隆时，引入原型管理器（也叫原型注册表）。

 **类比** ：

> 就像公司有一个"合同模板库"——有标准合同模板、保密协议模板、合作协议模板。需要时从模板库取出对应模板，复制一份再填写，而不是每次从零开始写。

### 实现

```java
class PrototypeManager {
    private Map<String, User> prototypes = new HashMap<>();

    // 注册原型
    public void addPrototype(String key, User user) {
        prototypes.put(key, user);
    }

    // 获取克隆（注意：返回的是副本，不是原始对象）
    public User getPrototype(String key) {
        User prototype = prototypes.get(key);
        if (prototype != null) {
            return prototype.clone();  // 克隆后返回
        }
        return null;
    }
}
```

### 使用方式

```java
// 1. 创建管理器并注册预设原型
PrototypeManager manager = new PrototypeManager();

manager.addPrototype("readonly",   new User("readonly",   Arrays.asList("view")));
manager.addPrototype("admin",      new User("admin",      Arrays.asList("view", "edit", "delete")));
manager.addPrototype("superAdmin", new User("superAdmin", Arrays.asList("view", "edit", "delete", "publish")));

// 2. 按需克隆，然后修改个性化字段
User user1 = manager.getPrototype("readonly");
user1.setName("John");   // 普通只读用户

User user2 = manager.getPrototype("admin");
user2.setName("Tom");    // 管理员

User user3 = manager.getPrototype("superAdmin");
user3.setName("Alice");  // 超级管理员
```

 **结构图** ：

```
PrototypeManager（管理器）
    ├── "readonly"   → User{name="readonly",   permissions=[view]}
    ├── "admin"      → User{name="admin",      permissions=[view, edit, delete]}
    └── "superAdmin" → User{name="superAdmin", permissions=[view, edit, delete, publish]}

客户端调用 getPrototype("admin")
    → 管理器找到原型 → 调用 clone() → 返回副本
    → 客户端修改副本的 name
    → 原始原型不受影响
```

---

## 七、优缺点分析

### ✅ 优点

**1. 解耦**

```java
// ✅ 调用方完全不知道 User 有哪些字段
User newUser = admin1.clone();
newUser.setName("newUser");

// ❌ 如果 User 新增了字段，下面的代码就得跟着改
User newUser = new User(admin1.getName(), admin1.getAge(),
                        admin1.getEmail(), new ArrayList<>(admin1.getPermissions()));
```

**2. 简化复杂对象的创建**

对于初始化很复杂的对象（如需要多步计算才能构建），克隆比重新构造快得多。

**3. 性能提升**

```
创建对象的两种方式性能对比：

从头创建：构造函数 → 连接数据库 → 查询权限 → 组装对象  （慢，每次都要 I/O）
克隆创建：找到已有对象 → 内存复制                       （快，纯内存操作）
```

> **类比** ：从头创建就像每次都现场烹饪一道菜（从买菜开始）；克隆就像把成品菜打包复制一份，加热就能吃，省时省力。

---

### ❌ 缺点

**深拷贝实现复杂**

当对象结构复杂时（嵌套对象、对象互相引用），正确实现深拷贝需要格外小心：

```java
// 简单对象：只有基本类型和简单列表 → 深拷贝相对容易
class User {
    String name;
    List<String> permissions;
}

// 复杂对象：嵌套多层对象 → 每一层都需要深拷贝
class Order {
    User buyer;          // 需要 User.clone()
    List<Item> items;    // 需要 Item.clone()，Item 内部可能还有嵌套...
    Address address;     // 需要 Address.clone()
}
```

> **类比** ：复印一张纸很简单，但复印一本书（里面还夹着照片、信封、便利贴）就需要逐层处理，稍不注意就会漏复印某层内容。

---

## 八、什么时候用原型模式？

```
需要创建的新对象与现有对象非常相似？
    ├── 是 → 对象创建成本高吗？（I/O、网络、复杂计算）
    │         ├── 是 → ✅ 强烈推荐原型模式（克隆比重建快）
    │         └── 否 → 也可以用，主要受益是解耦和可维护性
    └── 否 → 直接 new 就行
```

---

## 九、总结

```
原型模式
├── 核心思想：对象自己负责复制自己（clone 方法）
├── 三个角色
│   ├── Prototype（接口）：声明 clone()
│   ├── ConcretePrototype：实现 clone()，含深拷贝逻辑
│   └── Client：调用 clone()，不关心复制细节
├── 关键概念
│   ├── 浅拷贝 ❌：共享引用，修改互相影响
│   └── 深拷贝 ✅：完全独立，修改互不影响
├── 进阶：原型管理器（统一存储和分发预设原型）
├── 优点：解耦、简化创建、性能好
├── 缺点：复杂嵌套对象的深拷贝难以正确实现
└── 适用场景：新对象与现有对象相似，或对象创建代价高
```

> **最终类比总结** ：原型模式就是给对象装上了**"自我复制"按钮**。需要相似对象时，不用从零重建，直接按按钮复制一份，再改改个性化的部分就行。核心难点在于确保复制出来的是"真正独立的副本"（深拷贝），而不是"表面看起来独立、实际上共用内部零件"的假副本（浅拷贝）。
>
