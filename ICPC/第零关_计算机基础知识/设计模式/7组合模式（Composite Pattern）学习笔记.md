# 组合模式（Composite Pattern）学习笔记

---

## 一、要解决什么问题？

 **场景** ：处理的对象天然具有 **树形结构** ，既有"叶子"（最小单元），又有"容器"（可以包含其他对象）。

| 树形结构例子 | 叶子节点     | 组合节点                  |
| ------------ | ------------ | ------------------------- |
| 文件系统     | 文件         | 文件夹                    |
| 图形编辑器   | 圆形、正方形 | 形状组                    |
| 公司组织架构 | 员工         | 部门                      |
| HTML 文档    | 文本节点     | `<div>`、`<ul>`等标签 |

 **如果不用组合模式** ，代码里会到处出现这种判断：

```java
// ❌ 客户端需要区分两种类型，到处写 if-else
if (node instanceof File) {
    totalSize += ((File) node).size();
} else if (node instanceof Folder) {
    totalSize += ((Folder) node).calculateTotalSize(); // 方法名还不一样！
}
```

 **类比帮助理解** ：

> 想象你是一个快递公司，要计算一个包裹的总重量。这个包裹里可能直接装着物品（叶子），也可能装着更小的盒子，小盒子里还有更小的盒子（组合）。组合模式就是给每个"东西"（无论是物品还是盒子）都装上一个 **统一的"称重按钮"** 。按一下，物品返回自身重量，盒子自动把里面所有东西的重量加起来返回——你根本不用关心里面是物品还是盒子。

---

## 二、核心思想

> **一句话理解** ：让"组合对象"和"独立对象"实现 **同一个接口** ，这样客户端可以用统一的方式处理它们，无需区分。

---

## 三、三个核心角色

```
┌──────────────────────────────────────────────────────────────┐
│                    组合模式三个角色                            │
│                                                              │
│  Component（组件接口）                                        │
│      为所有节点定义统一接口（叶子和容器都实现它）               │
│                                                              │
│  Leaf（叶子节点）                                             │
│      最小单元，没有子节点                                      │
│      实现组件接口中的方法（直接处理自身）                       │
│                                                              │
│  Composite（组合节点）                                        │
│      容器，可包含若干子节点（叶子或其他组合节点）               │
│      实现组件接口中的方法（委托给子节点，聚合结果）              │
└──────────────────────────────────────────────────────────────┘
```

 **树形结构示意** ：

```
           Composite（根）
          /      |       \
       Leaf    Leaf    Composite
                       /      \
                    Leaf    Composite
                              |
                            Leaf
```

> 关键：无论是 `Leaf` 还是 `Composite`，对外都长得一样（实现相同接口），客户端无法也无需区分它们。

---

## 四、场景一：文件系统

### 结构设计

```
FSNode（组件接口）
    ├── File（叶子）    ← 不含子节点，直接返回自身数据
    └── Folder（组合）  ← 含子节点，递归委托给子节点计算
```

---

### Step 1：组件接口（Component）

```java
interface FSNode {
    String getName();
    int count();      // 包含的文件总数
    long size();      // 磁盘占用总大小
    String tree(String indent);  // 打印目录树
}
```

---

### Step 2：叶子节点——文件（Leaf）

```java
class File implements FSNode {
    private final String name;
    private final long size;

    public File(String name, long size) {
        this.name = name;
        this.size = size;
    }

    public String getName() { return name; }

    public int count() {
        return 1;  // 文件本身就是 1 个文件，直接返回
    }

    public long size() {
        return size;  // 直接返回自身大小
    }

    public String tree(String indent) {
        return indent + name + " (" + size + " bytes)\n";
    }
}
```

---

### Step 3：组合节点——文件夹（Composite）

```java
class Folder implements FSNode {
    private final String name;
    private final List<FSNode> children = new ArrayList<>();  // 子节点列表

    public Folder(String name) { this.name = name; }

    public void add(FSNode node) { children.add(node); }
    public void remove(FSNode node) { children.remove(node); }

    public String getName() { return name; }

    public int count() {
        int total = 0;
        for (FSNode child : children) {
            total += child.count();  // 递归委托给子节点
        }
        return total;
    }

    public long size() {
        long total = 0;
        for (FSNode child : children) {
            total += child.size();  // 递归委托给子节点
        }
        return total;
    }

    public String tree(String indent) {
        StringBuilder sb = new StringBuilder();
        sb.append(indent).append("+ ").append(name).append("/\n");
        for (FSNode child : children) {
            sb.append(child.tree(indent + "  "));  // 递归，增加缩进
        }
        return sb.toString();
    }
}
```

> **关键模式** ：组合节点的每个方法都是 `for (FSNode child : children) { child.方法() }` ——把操作 **委托给子节点** ，然后 **聚合结果** 。

---

### Step 4：客户端使用

```java
Folder root = new Folder("root");
root.add(new File("README.md", 1024));
root.add(new File("LICENSE", 512));

Folder documents = new Folder("documents");
documents.add(new File("index.html", 2048));
documents.add(new File("config.txt", 1536));
root.add(documents);

// ✅ 客户端只调用接口方法，不关心内部是文件还是文件夹
System.out.println(root.tree(""));
System.out.println("Total files: " + root.count());  // 4
System.out.println("Total size: " + root.size());    // 5120 bytes
```

 **输出** ：

```
+ root/
  README.md (1024 bytes)
  LICENSE (512 bytes)
  + documents/
    index.html (2048 bytes)
    config.txt (1536 bytes)

Total files: 4
Total size: 5120 bytes
```

---

### 递归调用过程图解

以 `root.count()` 为例：

```
root.count()
    ├── README.md.count()   → 1
    ├── LICENSE.count()     → 1
    └── documents.count()
            ├── index.html.count()  → 1
            └── config.txt.count()  → 1
            └── 返回 2
    └── 返回 1 + 1 + 2 = 4
```

> 组合节点自动递归，客户端只需要调用一次顶层的 `count()`，整棵树就自动协作完成计算。

---

## 五、场景二：图形编辑器

### 结构设计

```
Shape（组件接口）
    ├── Circle（叶子）  ← 直接移动自身坐标
    ├── Square（叶子）  ← 直接移动自身坐标
    └── Group（组合）   ← 移动时递归移动所有子形状
```

---

### 核心实现

```java
// 组件接口
interface Shape {
    void move(int dx, int dy);
    void display(String indent);
}

// 叶子：圆形
class Circle implements Shape {
    private int x, y;
    private final int radius;

    public Circle(int x, int y, int radius) { this.x = x; this.y = y; this.radius = radius; }

    public void move(int dx, int dy) { x += dx; y += dy; }  // 直接修改自身坐标

    public void display(String indent) {
        System.out.println(indent + "Circle at (" + x + ", " + y + ")");
    }
}

// 组合：形状组
class Group implements Shape {
    private final List<Shape> children = new ArrayList<>();

    public void add(Shape shape) { children.add(shape); }

    public void move(int dx, int dy) {
        for (Shape child : children) {
            child.move(dx, dy);  // 递归移动所有子形状
        }
    }

    public void display(String indent) {
        System.out.println(indent + "Group {");
        for (Shape child : children) {
            child.display(indent + "  ");  // 递归显示，增加缩进
        }
        System.out.println(indent + "}");
    }
}
```

---

### 客户端使用

```java
Group canvas = new Group();
canvas.add(new Circle(10, 20, 15));

Group subGroup = new Group();
subGroup.add(new Square(100, 110, 40));
subGroup.add(new Circle(200, 210, 25));
canvas.add(subGroup);

canvas.display("");
// Group {
//   Circle at (10, 20)
//   Group {
//     Square at (100, 110)
//     Circle at (200, 210)
//   }
// }

// ✅ 移动整个画布，所有子元素自动跟着移动
canvas.move(100, -50);
canvas.display("");
// Group {
//   Circle at (110, -30)      ← 自动更新
//   Group {
//     Square at (200, 60)     ← 自动更新
//     Circle at (300, 160)    ← 自动更新
//   }
// }
```

> 客户端只调用一次 `canvas.move(100, -50)`，整棵树上的所有形状都自动移动——这就是组合模式的威力。

---

## 六、两个场景横向对比

|                    | 文件系统                          | 图形编辑器               |
| ------------------ | --------------------------------- | ------------------------ |
| **组件接口** | `FSNode`                        | `Shape`                |
| **叶子节点** | `File`                          | `Circle`,`Square`    |
| **组合节点** | `Folder`                        | `Group`                |
| **统一操作** | `count()`,`size()`,`tree()` | `move()`,`display()` |
| **委托方式** | 递归汇总子节点数值                | 递归传播操作到子节点     |

---

## 七、组合模式 vs 普通树结构

|                      | 普通树                  | 组合模式的树           |
| -------------------- | ----------------------- | ---------------------- |
| **节点类型**   | 可以不同                | 叶子和容器实现相同接口 |
| **客户端处理** | 需要 `instanceof`判断 | 统一调用接口方法       |
| **扩展性**     | 新增类型要改客户端代码  | 新增类型只需实现接口   |

---

## 八、优缺点分析

### ✅ 优点

**1. 简化客户端代码**

```java
// ✅ 组合模式：完全不需要 if-else 判断类型
long total = root.size();

// ❌ 不用组合模式：
if (node instanceof File) {
    total += ((File)node).size();
} else if (node instanceof Folder) {
    for (FSNode child : ((Folder)node).getChildren()) { /* 递归处理... */ }
}
```

**2. 易于扩展**

想新增一个"快捷方式"（Shortcut）类型？只需实现 `FSNode` 接口，客户端代码 **一行不用改** 。

---

### ❌ 缺点

**接口设计需要权衡**

叶子节点和组合节点的能力不同（叶子没有 `add()`/`remove()` 子节点的能力），但共用同一个接口，有时会导致：

* 接口里出现叶子节点"用不上"的方法（如 `add()`）
* 或者叶子节点调用这些方法时只能抛出异常

---

## 九、什么时候用组合模式？

```
处理的对象能组织成树形结构？
    ├── 是 → 需要对叶子节点和组合节点做相同操作？
    │         ├── 是 → ✅ 使用组合模式，统一接口
    │         └── 否 → 普通树结构即可
    └── 否 → 不需要组合模式
```

 **典型适用场景** ：

* 文件系统的遍历、统计
* UI 组件树（每个组件都有 `render()`）
* 公司组织架构（统计人数、计算薪资）
* 权限系统（权限组可以包含子权限或子权限组）

---

## 十、总结

```
组合模式
├── 核心思想：叶子和容器实现同一接口，客户端统一处理
├── 三个角色
│   ├── Component（接口）：定义统一操作
│   ├── Leaf（叶子）：直接处理自身，无子节点
│   └── Composite（组合）：持有子节点列表，递归委托操作
├── 关键机制：组合节点的方法 = 遍历子节点调用相同方法 + 聚合结果
├── 优点：消除 if-else 类型判断，易于扩展新节点类型
├── 缺点：叶子和组合接口一致性的权衡
└── 适用场景：任何天然具有树形结构、需要统一操作的场景
```

> **最终类比总结** ：组合模式就像 **俄罗斯套娃的"统一遥控器"** 。每个套娃（无论是最小的实心娃娃还是可以装其他娃娃的空心娃娃）都有同一个"旋转按钮"。按空心娃娃的旋转按钮，它会自动让里面所有的娃娃都旋转；按实心娃娃的旋转按钮，它自己转。你（客户端）根本不需要关心按的是哪种娃娃， **统一按按钮，结果自然正确** 。
>
