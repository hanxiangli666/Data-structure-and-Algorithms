# 适配器模式（Adapter Pattern）学习笔记

---

## 一、要解决什么问题？

 **场景** ：你手上有一个功能完好的类/库，但它的接口和你系统期望的接口"对不上"。

| 常见情况                             | 例子                                         |
| ------------------------------------ | -------------------------------------------- |
| 重写了某个模块，新接口与旧系统不兼容 | 新版支付模块接口变了，旧业务代码还在用老接口 |
| 想用第三方库，但接口格式不符合需求   | 第三方地图 SDK 返回的坐标格式和系统不一样    |
| 历史遗留代码无法修改                 | 老系统的类不能动，但要接入新模块             |

 **直觉做法** ：直接修改被集成的类的源码。

 **为什么不行** ：

* 第三方库没有源码权限
* 修改历史代码可能引发大量连锁问题
* 违背"对修改关闭"的开闭原则

 **类比帮助理解** ：

> 你去英国旅行，带了中国的三脚插头，但英国插座是三孔方形的。你不能去改酒店的插座（被适配者），也不能去改你的充电器（客户端）。 **最简单的办法就是买一个转换插头（适配器）** ，一头插中国插头，一头插英国插座，问题解决。

---

## 二、核心思想

> **一句话理解** ：在两个不兼容的接口之间，插入一个**适配器**作为"转换插头"，让它们可以协同工作，而不需要修改任何一方的源代码。

---

## 三、基础案例拆解

### 场景

系统期望输出英文（`EnglishSpeaker`），但现有的 `Chinese` 类只会说中文。

---

### Step 1：被适配者（Adaptee）——已有的、接口不兼容的类

```java
// 已有的类，功能完好，但接口"对不上"
class Chinese {
    public void speakChinese() {
        System.out.println("你好世界！");
    }
}
```

---

### Step 2：目标接口（Target）——系统期望的接口

```java
// 系统期望所有说话者都实现这个接口
interface EnglishSpeaker {
    void speakEnglish();
}
```

---

### Step 3：适配器（Adapter）——连接两者的"翻译官"

```java
// 适配器：实现目标接口，内部持有被适配者
class TranslatorAdapter implements EnglishSpeaker {
    private Chinese adaptee;  // 持有被适配者

    public TranslatorAdapter(Chinese adaptee) {
        this.adaptee = adaptee;
    }

    @Override
    public void speakEnglish() {
        adaptee.speakChinese();                    // 调用被适配者的方法
        System.out.println("Translated: Hello World!");  // 转换为目标接口期望的格式
    }
}
```

---

### Step 4：客户端使用

```java
public class Main {
    public static void main(String[] args) {
        Chinese chinese = new Chinese();
        EnglishSpeaker adapter = new TranslatorAdapter(chinese);
      
        // 客户端只认识 EnglishSpeaker 接口
        // 完全不知道背后是 Chinese 类在工作
        adapter.speakEnglish();
        // 输出：你好世界！
        //       Translated: Hello World!
    }
}
```

---

## 四、四个核心角色

```
┌──────────────────────────────────────────────────────────────┐
│                    适配器模式四个角色                          │
│                                                              │
│  Target（目标接口）     ←  EnglishSpeaker                    │
│      客户端期望调用的接口                                     │
│                                                              │
│  Adaptee（被适配者）    ←  Chinese                           │
│      已有功能，但接口不兼容                                   │
│                                                              │
│  Adapter（适配器）      ←  TranslatorAdapter                 │
│      实现 Target 接口，内部调用 Adaptee 方法                  │
│      是连接两者的"转换插头"                                   │
│                                                              │
│  Client（客户端）       ←  Main                              │
│      只和 Target 接口交互，不知道 Adaptee 的存在              │
└──────────────────────────────────────────────────────────────┘
```

 **调用流程** ：

```
Client → Target接口 → Adapter（转换逻辑）→ Adaptee（真正干活）
  ↑                       ↑
只认识这个接口        实际在这里做接口转换
```

---

## 五、两种实现方式

### 5.1 对象适配器（推荐）

通过**组合**持有被适配者的实例：

```java
class TranslatorAdapter implements EnglishSpeaker {
    private Chinese adaptee;  // 组合：持有被适配者对象

    public TranslatorAdapter(Chinese adaptee) {
        this.adaptee = adaptee;
    }

    public void speakEnglish() {
        adaptee.speakChinese();  // 通过对象引用调用
        System.out.println("Translated: Hello World!");
    }
}
```

---

### 5.2 类适配器（了解即可）

通过**继承**被适配者：

```java
// 同时继承被适配者 + 实现目标接口
class TranslatorAdapter extends Chinese implements EnglishSpeaker {
    public void speakEnglish() {
        super.speakChinese();  // 通过继承调用
        System.out.println("Translated: Hello World!");
    }
}
```

---

### 5.3 两种方式对比

| 对比维度           | 对象适配器               | 类适配器                  |
| ------------------ | ------------------------ | ------------------------- |
| **实现方式** | 组合（持有实例）         | 继承（extends）           |
| **灵活性**   | 高（运行时可换被适配者） | 低（编译时绑定）          |
| **多重限制** | 无（Java 支持多接口）    | 受限（Java 不支持多继承） |
| **推荐度**   | ✅ 主流做法              | 不常用                    |

> **现代原则** ："组合优于继承"，因此**对象适配器**是更推荐的方式。

---

## 六、真实场景案例

### 6.1 电源适配器（贴近生活）

 **场景** ：家用插座只提供 220V 交流电，但不同设备需要不同规格的直流电。

```
角色对应：
  Target（目标接口）  → SmartChargeTarget（智能充电协议）
  Adaptee（被适配者）→ HouseholdSocket（220V家用插座）
  Adapter（适配器）  → SmartPowerAdapter（智能电源适配器）
  Client（客户端）   → Device（各种电子设备）
```

 **核心逻辑** ：

```java
// 适配器：接收 220V 交流电，协商输出设备所需的直流电
class SmartPowerAdapter implements SmartChargeTarget {
    private HouseholdSocket adaptee;  // 220V 插座

    public Voltage supplyVoltage(List<Voltage> deviceSupportedVoltages) {
        Voltage bestMatch = findBestVoltageMatch(deviceSupportedVoltages);
        if (bestMatch != null) {
            return convert(bestMatch);  // 220V AC → 目标直流电压
        }
        return null;  // 无法适配，拒绝供电
    }
}
```

 **三种结果** ：

```
iPhone 17（支持 15V/9V/5V）+ 全能适配器 → ✅ 协商到 15V DC 快充
Nokia   （只支持 5V）     + 全能适配器 → ✅ 协商到 5V DC 兼容充电
Gaming Device（需要 15V） + 老适配器   → ❌ 协商失败，拒绝供电（保护设备）
```

---

### 6.2 Java I/O 中的字符流适配器

 **场景** ：文件读取底层是字节流，但程序员想按字符来读取文本文件。

```
角色对应：
  Target（目标接口）  → Readable 接口（按字符读取）
  Adaptee（被适配者）→ InputStream（只能读字节）
  Adapter（适配器）  → InputStreamReader（字节流 → 字符流）
  Client（客户端）   → 程序员/业务代码
```

```java
// InputStream 读取字节流（Adaptee）
InputStream is = new FileInputStream("test.txt");

// InputStreamReader 是适配器，将字节流适配为字符流（Adapter）
InputStreamReader reader = new InputStreamReader(is, StandardCharsets.UTF_8);

// 现在可以按字符读取，完全不用管底层字节的事（Client 只和 Readable 打交道）
int character;
while ((character = reader.read()) != -1) {
    System.out.print((char) character);
}
```

> **Java 标准库本身就大量使用了适配器模式** ，`InputStreamReader` 是最经典的例子之一。

---

## 七、优缺点分析

### ✅ 优点

**1. 解耦，增强灵活性**

```java
// 客户端只依赖 Target 接口
// 想换被适配者？换一个 Adapter 就行，客户端代码不用动
EnglishSpeaker adapter = new TranslatorAdapter(new Chinese());
// 将来换成：
EnglishSpeaker adapter = new FrenchTranslatorAdapter(new French());
// 客户端调用 adapter.speakEnglish() 的代码完全不用改
```

**2. 符合单一职责原则**

接口转换的逻辑集中在适配器里，业务代码保持干净。

**3. 不修改现有代码**

被适配者（第三方库、历史遗留代码）完全不需要改动。

---

### ❌ 缺点

**增加代码复杂性**

每接入一个不兼容的类，就需要写一个适配器，类的数量增多。

> 如果可以直接修改被适配者的接口（例如它是自己团队维护的内部类），那直接改接口比写适配器更简单。

---

## 八、什么时候用适配器模式？

```
想使用某个现有类，但它的接口和你的系统不兼容？
    ├── 能修改那个类的源码吗？
    │     ├── 能 → 直接改接口更简单，不需要适配器
    │     └── 不能（第三方库 / 历史遗留 / 不该改）
    │               └── ✅ 使用适配器模式
    └── 无需接口转换 → 不需要适配器
```

---

## 九、总结

```
适配器模式
├── 核心思想：在不兼容的接口之间插入"转换插头"（适配器）
├── 四个角色
│   ├── Target（目标接口）：客户端期望的接口
│   ├── Adaptee（被适配者）：已有功能，接口不兼容
│   ├── Adapter（适配器）：实现 Target，内部调用 Adaptee
│   └── Client（客户端）：只和 Target 打交道
├── 两种实现
│   ├── 对象适配器（组合）← 推荐，灵活
│   └── 类适配器（继承）  ← 不推荐，受限
├── 真实案例：电源转换插头、Java InputStreamReader
├── 优点：解耦、单一职责、不修改现有代码
├── 缺点：增加类的数量，提升复杂度
└── 适用场景：接口不兼容且无法修改源码时
```

> **最终类比总结** ：适配器模式就是现实中的 **转换插头** 。你的设备（客户端）和墙上的插座（被适配者）各有自己的标准，谁都不需要改变，只需要一个小小的转换头（适配器）坐在中间，负责把两边的"语言"互相翻译，一切就能正常工作。
>
