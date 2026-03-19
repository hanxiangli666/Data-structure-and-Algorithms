# 深入理解 Session 和 Cookie 笔记

> **核心概览**：Cookie 和 Session 是 Web 开发中最基础的状态管理机制。Cookie 在浏览器端，Session 在服务器端，两者配合解决 HTTP 无状态问题。理解它们，不仅能搞懂登录鉴权，也能理解很多常见安全问题。

---

## 一、为什么需要 Cookie？——HTTP 的“失忆症”

### 1.1 问题：HTTP 是无状态协议

HTTP 协议本身不会记住你是谁。每次请求在服务器看来都像一个新访客。

> **类比**：HTTP 就像一个“失忆”的柜台服务员。你刚告诉他你是谁，转身再来一次，他又不认识你了。

### 1.2 不用 Cookie 会发生什么？（反例）

假设没有 Cookie（也没有其他状态机制），登录流程会变成这样：

1. 你在 `/login` 输入账号密码并登录成功。
2. 你点击“个人中心”跳到 `/profile`。
3. 服务器不知道你刚登录过，要求你再次登录。
4. 你再点“订单页”到 `/orders`，又要登录一次。

这显然不可用。用户每访问一个页面都要重复证明身份，体验会崩溃。

> 这就是 Cookie 出现的根本原因：让浏览器在后续请求里自动携带“身份线索”。

---

## 二、Cookie 是什么？如何工作？

### 2.1 Cookie 是什么

一句话：Cookie 是浏览器端的一组键值对，由服务器通过响应头下发，浏览器按规则自动携带。

```text
一个 cookie = name=value
```

服务器可设置多个 Cookie。

### 2.2 服务器设置 Cookie

服务器通过响应头 `Set-Cookie` 下发 Cookie：

```java
// Java Servlet 示例
public void doGet(HttpServletRequest request, HttpServletResponse response) {
    Cookie cookie1 = new Cookie("name1", "value1");
    response.addCookie(cookie1);

    Cookie cookie2 = new Cookie("name2", "value2");
    response.addCookie(cookie2);

    response.getWriter().println("html content");
}
```

### 2.3 完整交互流程

```text
浏览器                                        服务器
  |                                            |
  | ① 第一次请求（没有 Cookie）                 |
  | -----------------------------------------> |
  |                                            |
  | ② 响应 + Set-Cookie: name1=value1         |
  |         Set-Cookie: name2=value2           |
  | <----------------------------------------- |
  |                                            |
  | ③ 后续请求自动携带:                         |
  |    Cookie: name1=value1; name2=value2      |
  | -----------------------------------------> |
  |                                            |
  | ④ 服务器读取 Cookie，识别请求上下文         |
```

关键点：

- `Set-Cookie` 在响应里“种”Cookie。
- 浏览器后续请求会自动带上匹配域名/路径的 Cookie。
- 用户通常无感知。

### 2.4 Cookie 的局限性

| 局限 | 说明 |
|------|------|
| 容量有限 | 单个 Cookie 通常约 4KB，域名下数量也有限 |
| 消耗带宽 | 每次请求都会带上 Cookie Header |
| 安全风险 | 客户端可见，不适合存敏感业务数据 |

---

## 三、Cookie 安全属性与经典攻击

这部分是实际开发高频必备。

### 3.1 三个关键属性：HttpOnly、Secure、SameSite

| 属性 | 作用 | 解决的主要风险 |
|------|------|----------------|
| **HttpOnly** | 禁止 JavaScript 读取 Cookie（`document.cookie` 不可见） | 降低 XSS 窃取 Cookie 风险 |
| **Secure** | 只允许在 HTTPS 请求中发送 Cookie | 防止明文 HTTP 传输被窃听 |
| **SameSite** | 控制跨站请求是否携带 Cookie | 缓解 CSRF |

`SameSite` 常见取值：

- `Strict`：最严格，跨站请求基本不带 Cookie。
- `Lax`：较常用，部分顶级导航会带 Cookie，安全与可用性折中。
- `None`：允许跨站携带，但必须同时设置 `Secure`。

### 3.2 CSRF 是怎么发生的？（与 SameSite 的关系）

**CSRF（跨站请求伪造）**的核心是：

- 你已登录 `bank.com`，浏览器里有该站 Cookie。
- 你访问了攻击者页面 `evil.com`。
- 攻击者诱导你的浏览器向 `bank.com/transfer` 发请求。
- 如果浏览器自动带上 `bank.com` 的 Cookie，服务器可能误以为是你本人操作。

所以如果没有合适的 `SameSite` 策略和 CSRF Token 保护，就容易被利用。

### 3.3 XSS 与 HttpOnly 的关系

**XSS（跨站脚本）**是指页面被注入恶意 JavaScript。

如果 Session ID 存在 Cookie 且没设 `HttpOnly`，恶意脚本可直接读取：

```javascript
// 攻击者常见思路（示意）
const stolen = document.cookie;
```

拿到 Session ID 后，攻击者可能冒充用户会话。

设置 `HttpOnly` 后，脚本无法读取 Cookie，可显著降低“偷 Session ID”风险。

---

## 四、Session：服务器端的“记忆”

### 4.1 Session 是什么

一句话：Session 是存储在服务器端的会话数据对象，用于保存用户登录态和上下文。

> **类比**：
> - Cookie = 你手上的会员卡号
> - Session = 商场后台会员档案
>
> 浏览器带来卡号（Session ID），服务器据此查档案（Session 数据）。

### 4.2 Session + Cookie 的经典登录流程

```text
浏览器                                            服务器
  |                                                |
  | ① 提交账号密码                                  |
  | ---------------------------------------------> |
  |                                                |
  | ② 验证成功，创建 Session                        |
  |    { user_id: 12345, role: "admin", ... }      |
  |    生成 Session ID: abcd1234                    |
  |                                                |
  | ③ 响应 + Set-Cookie: sessionID=abcd1234        |
  | <--------------------------------------------- |
  |                                                |
  | ④ 后续请求自动携带 Cookie: sessionID=abcd1234   |
  | ---------------------------------------------> |
  |                                                |
  | ⑤ 服务器按 sessionID 找到 Session               |
  |    识别用户并返回业务数据                        |
```

核心要点：

- 浏览器只持有随机 Session ID。
- 用户敏感信息保存在服务器端 Session。
- 服务器通过 Session ID 查找会话状态。

### 4.3 Session 过期机制

Session 通常有过期时间，例如“30 分钟无操作过期”。过期后用户需重新登录。

### 4.4 Session 固定攻击（Session Fixation）

**问题**：如果登录成功后继续沿用“登录前的 Session ID”，攻击者可能提前构造或诱导受害者使用某个已知 Session ID。

攻击思路（简化）：

1. 攻击者先拿到一个合法但未登录的 Session ID。
2. 诱导受害者带着这个 Session ID 访问目标网站。
3. 受害者登录后，服务器若不更换 Session ID，该 ID 就被绑定到受害者登录态。
4. 攻击者继续用同一个 Session ID，可能劫持会话。

**防御关键**：登录成功后必须重新生成新的 Session ID（session rotation），并废弃旧 ID。

---

## 五、浏览器对第三方 Cookie 的限制

现代浏览器正在持续收紧第三方 Cookie，这直接影响广告追踪和部分旧认证方案。

### 5.1 什么是第三方 Cookie

- 第一方 Cookie：当前访问站点自己设置的 Cookie。
- 第三方 Cookie：页面中嵌入的其他域（广告、统计、社交插件）设置的 Cookie。

### 5.2 Safari 和 Chrome 的趋势

- **Safari**：长期通过 ITP（Intelligent Tracking Prevention）严格限制第三方 Cookie，默认环境下跨站跟踪能力很弱。
- **Chrome**：正在推进第三方 Cookie 逐步禁用与替代方案（Privacy Sandbox），很多用户环境已对第三方 Cookie 有限制。

### 5.3 为什么广告追踪系统受影响

传统广告网络依赖“跨站同一 Cookie”识别用户路径。第三方 Cookie 受限后：

- 难以跨站拼接完整行为链路。
- 重定向登录和跨站埋点方案需要改造。
- 行业转向一方数据、服务端跟踪和新隐私 API。

这也是现代 Web 认证与风控方案不断演进的背景。

---

## 六、Cookie 与 Session 速查

| 维度 | Cookie | Session |
|------|--------|---------|
| 存储位置 | 浏览器端 | 服务器端 |
| 常见内容 | Session ID、偏好项 | 用户会话数据（ID、角色、状态） |
| 安全性 | 较低（可见/可篡改风险） | 较高（用户不可直接访问） |
| 容量 | 小（约 4KB） | 由服务器资源决定 |
| 生命周期 | 由 Expires/Max-Age 控制 | 由服务端 TTL/策略控制 |
| 传输 | 自动随请求发送 | 不直接传输，通过 Session ID 引用 |

安全实践建议：

1. 敏感数据放 Session，不放明文 Cookie。
2. 认证 Cookie 默认加 `HttpOnly; Secure; SameSite=Lax`（按业务再细调）。
3. 登录成功后轮换 Session ID。
4. 对关键写操作叠加 CSRF Token 校验。

---

## 七、进阶：Session 的三层架构实现

这一节属于进阶内容。零基础读者先掌握前面“Cookie + Session 基本交互”即可。

### 7.1 为什么不能直接一个 HashMap 搞定

直接 `HashMap<sessionID, 用户数据>` 虽然简单，但会遇到：

1. 进程重启后会话全丢。
2. 大流量下内存压力与清理复杂。
3. 过期与淘汰策略难维护。
4. 不利于在内存/Redis/MySQL 间切换。

### 7.2 三层架构总览

```text
Handler -> Manager -> Provider -> Session
```

- **Session**：单用户会话对象，存键值与元信息。
- **Provider**：管理所有 Session（增删查改、过期清理）。
- **Manager**：全局配置与协调（TTL、Cookie 名、存储实现选择）。

### 7.3 示例接口

```java
interface Session {
    void set(String key, Object value);
    Object get(String key);
    void delete(String key);
}

interface Provider {
    Session sessionCreate(String sid);
    void sessionDestroy(String sid);
    Session sessionRead(String sid);
    void sessionUpdate(String sid);
    void sessionGC(long maxLifeTime);
}
```

### 7.4 分层的价值

| 层级 | 主要职责 | 可替换性 |
|------|----------|---------|
| Session | 单会话数据结构 | 可替换底层存储结构 |
| Provider | 会话容器与淘汰策略 | 可替换 TTL/LRU/LFU 策略 |
| Manager | 全局配置与路由 | 可替换环境参数与实现绑定 |

> 价值总结：通过解耦，系统可在不改业务代码的前提下升级存储与策略。
