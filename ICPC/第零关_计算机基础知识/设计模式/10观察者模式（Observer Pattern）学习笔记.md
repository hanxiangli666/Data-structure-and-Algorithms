# 观察者模式（Observer Pattern）学习笔记

---

## 一、要解决什么问题？

 **场景** ：一个对象的状态发生变化时，需要自动通知多个其他对象做出响应。

 **如果不用观察者模式** ，有两种糟糕的做法：

```
❌ 做法一：观察者主动轮询（拉模式）
每个竞拍者不停地去问拍卖师："现在多少钱了？现在多少钱了？"
→ 资源浪费，实时性差，混乱不堪

❌ 做法二：发布者直接调用每个观察者
auction.notifyAlice();
auction.notifyBob();
auction.notifyCharlie();
→ 发布者和每个观察者强耦合，新增一个观察者要改发布者代码
```

 **类比帮助理解** ：

> 你订阅了一份报纸。报社（发布者）每天出新报纸时，自动送到所有订阅者（观察者）家门口。你不需要每天跑去报社问"今天有没有新报纸"，报社也不需要知道你叫什么名字、住哪里，只需要维护一份"订阅者名单"，出报时统一派送即可。

---

## 二、核心思想

> **一句话理解** ：发布者维护一个观察者列表，状态变化时 **自动通知所有注册过的观察者** ；观察者可以随时注册或注销，发布者完全不关心观察者的具体实现。

---

## 三、四个核心角色

```
┌──────────────────────────────────────────────────────────────┐
│                   观察者模式四个角色                           │
│                                                              │
│  Subject/Publisher（发布者接口）                              │
│      声明 attach / detach / notify 方法                      │
│                                                              │
│  ConcreteSubject（具体发布者）                                │
│      持有状态，状态变化时调用 notify()                        │
│      内部维护观察者列表                                       │
│                                                              │
│  Observer（观察者接口）                                       │
│      声明 update() 方法（统一的接收通知入口）                  │
│                                                              │
│  ConcreteObserver（具体观察者）                               │
│      实现 update()，定义收到通知后的具体行为                   │
└──────────────────────────────────────────────────────────────┘
```

 **交互流程** ：

```
ConcreteObserver ──register()──→ ConcreteSubject
                                        │
                              状态变化，触发 notify()
                                        │
                              遍历观察者列表，调用每个 observer.update()
                                        │
                ←──────────────────────┘
ConcreteObserver.update() 执行各自的响应逻辑
```

---

## 四、场景一：在线拍卖系统

### 结构设计

```
Bidder（观察者接口）
    ├── OnlineBidder（竞拍者，在屏幕显示最新报价）
    └── BidRecorder （记录器，记录出价历史）

Auction（发布者接口）
    └── LiveAuction（具体拍卖会，维护竞拍者列表）
```

---

### 发布者接口 & 具体发布者

```java
// 观察者接口
interface Bidder {
    void update(String itemName, int currentPrice, String leadingBidder);
}

// 发布者接口
interface Auction {
    void registerBidder(Bidder bidder);    // 注册观察者
    void unregisterBidder(Bidder bidder);  // 注销观察者
    void notifyBidders();                  // 通知所有观察者
}

// 具体发布者
class LiveAuction implements Auction {
    private List<Bidder> bidders = new ArrayList<>();  // 观察者列表
    private String itemName;
    private int currentPrice;
    private String leadingBidder;

    public LiveAuction(String itemName, int startingPrice) {
        this.itemName = itemName;
        this.currentPrice = startingPrice;
        this.leadingBidder = "None";
    }

    public void registerBidder(Bidder bidder)   { bidders.add(bidder); }
    public void unregisterBidder(Bidder bidder) { bidders.remove(bidder); }

    public void notifyBidders() {
        for (Bidder b : bidders) {
            b.update(itemName, currentPrice, leadingBidder);  // 广播通知
        }
    }

    // 业务方法：有人出价
    public void placeBid(String bidderName, int price) {
        if (price > currentPrice) {
            currentPrice = price;
            leadingBidder = bidderName;
            System.out.println(bidderName + " bids " + price);
            notifyBidders();  // 状态变化，自动通知
        }
    }
}
```

---

### 具体观察者

```java
// 竞拍者：在控制台显示最新报价
class OnlineBidder implements Bidder {
    private String name;
    public OnlineBidder(String name) { this.name = name; }

    public void update(String itemName, int price, String leader) {
        System.out.printf("  [%s notified] %s price: %d, leader: %s%n",
                          name, itemName, price, leader);
    }
}

// 记录器：记录每次出价历史
class BidRecorder implements Bidder {
    private List<String> history = new ArrayList<>();

    public void update(String itemName, int price, String leader) {
        String record = itemName + ": " + price + " (" + leader + ")";
        history.add(record);
        System.out.println("  [Recorder] recorded: " + record);
    }

    public void printHistory() {
        System.out.println("=== Bid History ===");
        history.forEach(r -> System.out.println("  " + r));
    }
}
```

---

### 客户端使用

```java
LiveAuction auction = new LiveAuction("Antique Vase", 1000);

OnlineBidder alice = new OnlineBidder("Alice");
OnlineBidder bob   = new OnlineBidder("Bob");
BidRecorder recorder = new BidRecorder();

auction.registerBidder(alice);
auction.registerBidder(bob);
auction.registerBidder(recorder);

auction.placeBid("Alice", 1500);
// Alice bids 1500
//   [Alice notified] Antique Vase price: 1500, leader: Alice
//   [Bob notified] Antique Vase price: 1500, leader: Alice
//   [Recorder] recorded: Antique Vase: 1500 (Alice)

auction.unregisterBidder(bob);  // Bob 退出，不再收通知
System.out.println("Bob left the auction");

auction.placeBid("Alice", 2000);
// Alice bids 2000
//   [Alice notified] Antique Vase price: 2000, leader: Alice  ← Bob 不再收到了
//   [Recorder] recorded: Antique Vase: 2000 (Alice)
```

 **关键点** ：

* 发布者 `LiveAuction` **不知道也不关心** `OnlineBidder` 和 `BidRecorder` 的实现
* 两种观察者各自做各自的事，互不影响
* Bob 注销后，立即停止接收通知

---

## 五、场景二：游戏成就系统（进阶——按事件类型订阅）

### 场景

玩家击杀怪物时，需要同时触发：成就系统、统计模块、音效系统……

 **基础观察者模式的局限** ：所有观察者收到 **所有通知** ，成就系统不关心"捡道具"事件，但也会被通知到。

 **改进** ：引入事件中心（`EventCenter`）， **按事件类型选择性订阅** 。

---

### 事件中心（进阶发布者）

```java
// 事件类型枚举
enum GameEventType { MONSTER_KILLED, ITEM_COLLECTED, LEVEL_COMPLETED }

// 事件数据封装
class GameEvent {
    private GameEventType type;
    private Map<String, String> data;
    // 构造、getter 省略...
}

// 观察者接口
interface GameEventListener {
    void onEvent(GameEvent event);
}

// 事件中心：按事件类型维护订阅列表
class EventCenter {
    // key = 事件类型，value = 订阅了这个类型的观察者列表
    private Map<GameEventType, List<GameEventListener>> listeners = new HashMap<>();

    public void subscribe(GameEventType type, GameEventListener listener) {
        listeners.computeIfAbsent(type, k -> new ArrayList<>()).add(listener);
    }

    public void unsubscribe(GameEventType type, GameEventListener listener) {
        if (listeners.containsKey(type)) listeners.get(type).remove(listener);
    }

    public void publish(GameEvent event) {
        List<GameEventListener> subs = listeners.getOrDefault(event.getType(), List.of());
        for (GameEventListener listener : subs) {
            listener.onEvent(event);  // 只通知订阅了这个类型的观察者
        }
    }
}
```

---

### 具体观察者（各游戏子系统）

```java
// 成就系统：只关心击杀事件
class AchievementSystem implements GameEventListener {
    private int killCount = 0;

    public void onEvent(GameEvent event) {
        killCount++;
        System.out.println("  [Achievement] Kill count: " + killCount);
        if (killCount == 1)  System.out.println("  [Achievement] Unlocked: First Blood!");
        if (killCount == 10) System.out.println("  [Achievement] Unlocked: Monster Hunter!");
    }
}

// 统计模块：关心多种事件
class StatisticsTracker implements GameEventListener {
    private Map<GameEventType, Integer> stats = new HashMap<>();

    public void onEvent(GameEvent event) {
        stats.merge(event.getType(), 1, Integer::sum);
        System.out.println("  [Stats] " + event.getType() + " total: " + stats.get(event.getType()));
    }
}

// 音效系统：关心多种事件，根据类型播放不同音效
class SoundSystem implements GameEventListener {
    public void onEvent(GameEvent event) {
        switch (event.getType()) {
            case MONSTER_KILLED -> System.out.println("  [Sound] Kill sound: " + event.getData("monsterName") + " defeated!");
            case ITEM_COLLECTED -> System.out.println("  [Sound] Pickup sound: Ding~");
            case LEVEL_COMPLETED -> System.out.println("  [Sound] Victory sound!");
        }
    }
}
```

---

### 客户端使用

```java
EventCenter eventCenter = new EventCenter();

AchievementSystem achievements = new AchievementSystem();
StatisticsTracker stats = new StatisticsTracker();
SoundSystem sound = new SoundSystem();

// 各模块按需订阅，只关心自己感兴趣的事件
eventCenter.subscribe(MONSTER_KILLED, achievements);  // 成就只关心击杀
eventCenter.subscribe(MONSTER_KILLED, stats);
eventCenter.subscribe(MONSTER_KILLED, sound);
eventCenter.subscribe(ITEM_COLLECTED, stats);         // 成就不关心捡道具
eventCenter.subscribe(ITEM_COLLECTED, sound);

// 击杀史莱姆
eventCenter.publish(new GameEvent(MONSTER_KILLED, Map.of("monsterName", "Slime")));
//   [Achievement] Kill count: 1
//   [Achievement] Unlocked: First Blood!
//   [Stats] MONSTER_KILLED total: 1
//   [Sound] Kill sound: Slime defeated!

// 捡到宝箱（成就系统不会收到通知！）
eventCenter.publish(new GameEvent(ITEM_COLLECTED, Map.of("itemName", "Treasure Chest")));
//   [Stats] ITEM_COLLECTED total: 1
//   [Sound] Pickup sound: Ding~
```

> **将来要加"连杀奖励"模块？** 只需新建一个实现 `GameEventListener` 的类，并订阅 `MONSTER_KILLED`，**一行已有代码都不用改** ✅

---

## 六、两个场景对比

|                      | 拍卖系统             | 游戏成就系统                   |
| -------------------- | -------------------- | ------------------------------ |
| **发布者类型** | 直接持有观察者列表   | 事件中心（中介）               |
| **通知范围**   | 所有注册的观察者     | 只通知订阅了该事件类型的观察者 |
| **耦合程度**   | 发布者知道观察者接口 | 发布者完全不知道观察者存在     |
| **适用场景**   | 简单的同类通知       | 多事件类型、精细化订阅         |

---

## 七、观察者模式 vs 发布-订阅模式

这是经常被混淆的两个概念：

```
观察者模式：
  Publisher ─────直接持有引用────→ Observer
  发布者知道观察者的存在
  同步调用，适合同一应用内

发布-订阅模式：
  Publisher ─→ [Message Broker] ─→ Subscriber
  发布者和订阅者完全不知道彼此
  通过消息中间件解耦，适合跨系统/分布式（RabbitMQ、Kafka）
```

 **类比** ：

> * 观察者模式：班主任（发布者）直接点名通知每位同学（观察者）
> * 发布-订阅：学校通过广播系统（Broker）广播通知，不认识任何一个学生

> 文中的 `EventCenter` 已经有发布-订阅的影子——它作为中介，让发布者和观察者不直接耦合。实践中两者界限并不绝对。

---

## 八、优缺点分析

### ✅ 优点

**1. 松耦合**

```java
// 发布者只认识接口，不认识任何具体类
for (Bidder b : bidders) {
    b.update(...);  // 不知道 b 是 OnlineBidder 还是 BidRecorder
}
```

**2. 符合开闭原则**

新增观察者：写新类 → 注册 → 完成。已有代码零修改。

**3. 支持广播通信**

一次状态变化，所有订阅者自动接收，无需逐个手动调用。

---

### ❌ 缺点

**1. 通知顺序不确定**

```
观察者按注册顺序接收通知，不应依赖这个顺序
（先注册的先收到，但这是实现细节，不是保证）
```

**2. 可能引发连锁反应**

```
ObserverA.update() 触发状态变化
    → 再次调用 notify()
        → ObserverA.update() 再次被调用
            → 无限循环！💥
```

**3. 内存泄漏风险**

```java
// ❌ 常见错误：注册了但忘记注销
auction.registerBidder(tempBidder);
// ... tempBidder 用完了，但没有调用 unregisterBidder
// auction 一直持有 tempBidder 的引用，无法被垃圾回收 → 内存泄漏
```

---

## 九、什么时候用观察者模式？

```
一个对象状态变化时，需要通知多个其他对象？
    ├── 这些对象之间需要松耦合吗？
    │     ├── 是 → ✅ 使用观察者模式
    │     └── 否（简单直接调用即可）→ 不需要
    └── 需要精细化订阅（按事件类型）？
              ├── 是 → 考虑带事件中心的变体（接近发布-订阅）
              └── 否 → 基础观察者模式即可
```

 **典型使用场景** ：

* 游戏子系统（成就、统计、UI、音效）响应玩家行为
* GUI 事件处理（按钮点击通知多个监听器）
* MVC 架构中 Model 变化通知 View 更新
* 实时数据推送（股票行情、消息推送）

---

## 十、总结

```
观察者模式
├── 核心思想：发布者维护观察者列表，状态变化时自动广播通知
├── 四个角色
│   ├── Subject/Publisher（发布者接口）：attach / detach / notify
│   ├── ConcreteSubject（具体发布者）：持有状态，变化时 notify
│   ├── Observer（观察者接口）：声明 update()
│   └── ConcreteObserver（具体观察者）：实现 update()，各自响应
├── 进阶变体：事件中心（按事件类型精细化订阅）
├── vs 发布-订阅模式
│   ├── 观察者：直接引用，同步，同应用内
│   └── 发布-订阅：中间 Broker，解耦，跨系统
├── 优点：松耦合、开闭原则、广播通信
├── 缺点：顺序不确定、可能连锁反应、内存泄漏风险
└── 适用场景：状态变化需通知多方，且不希望紧密耦合
```

> **最终类比总结** ：观察者模式就是 **报纸订阅服务** 。报社（发布者）出了新报纸（状态变化），不需要知道每位读者（观察者）是谁、在哪、怎么阅读，只需要按照订阅名单统一派送。读者可以随时订阅或取消订阅，互相之间毫无关系。报社专注出报，读者专注读报，完美分工，互不干扰。
>
