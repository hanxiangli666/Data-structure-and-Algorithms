# OAuth 2.0 与 PKCE 完整笔记

---

## 一、背景：授权码流程的隐含前提

标准 OAuth 2.0 授权码流程中有一步：

```text
客户端用 授权码 + client_secret -> 换 Access Token
```

隐含前提是：客户端可以安全保管 `client_secret`。

- 服务器端 Web 应用：通常可以。
- 移动 App、SPA、桌面应用：通常不可以。

---

## 二、先看威胁：没有 PKCE 时，授权码如何被截获并利用

在讲 PKCE 前，先看完整攻击链路。

```text
用户                客户端(App/SPA)             授权服务器(OP)             攻击者App
 |                        |                          |                          |
 | 1. 发起登录             |                          |                          |
 |----------------------->|                          |                          |
 |                        | 2. 跳转授权页             |                          |
 |                        |------------------------->|                          |
 |                        |                          |                          |
 | 3. 用户同意授权         |                          |                          |
 |-------------------------(浏览器交互)---------------------------------------> |
 |                        |                          |                          |
 |                        | 4. 回调 redirect_uri?code=AUTH_CODE               |
 |                        |<-------------------------|                          |
 |                        |                          | 5. 攻击者截获 code        |
 |                        |                          |-------------------------> |
 |                        |                          |                          |
 |                        |                          | 6. 攻击者用 code 换 token  |
 |                        |                          |<------------------------- |
 |                        |                          |                          |
 |                        | 7. 攻击者拿到 Access Token，冒用资源访问            |
```

攻击成立的关键点：

1. 授权码在前端回调链路中可见。
2. Public Client 无法可靠保管 `client_secret`，无法形成强客户端证明。
3. 攻击者只要先一步拿到 code，就可能先换到 token。

> 这就是 PKCE 的威胁模型：防“授权码被截获后被他人兑换”。

---

## 三、客户端类型（Client Types）

| 类型 | 说明 | 典型例子 |
|------|------|---------|
| Confidential Client | 可安全保管 `client_secret` | 服务器端 Web 应用 |
| Public Client | 无法安全保管 `client_secret` | Android/iOS、SPA、桌面应用 |

### 为什么移动 App 是 Public Client

- 二进制可被反编译。
- `client_secret` 若写进包体，迟早可被提取。
- 混淆/加固只能提高门槛，不能从根本上保证机密性。

### 移动端 redirect_uri 的真实风险

移动端常用自定义 URL Scheme 回调，如 `myapp://callback`。

风险在于：

- 其他 App 也可能注册同名 Scheme。
- 系统路由可能被恶意 App 抢占或劫持。
- 结果是授权码回调可能被非预期 App 接收。

这不是理论问题，而是 PKCE 明确要防的现实攻击面。

---

## 四、不可信客户端的两难

```text
困境 1：带 client_secret
  -> 包体可逆向，secret 泄露

困境 2：不带 client_secret
  -> code 被截获后可被攻击者兑换
```

带了会泄露，不带又不安全。

**解决方案：PKCE**。

---

## 五、PKCE 核心思路

PKCE（Proof Key for Code Exchange）用“每次授权动态生成的一次性证明”替代静态 secret。

核心对象：

1. `code_verifier`：客户端本地生成的高熵随机串（机密）。
2. `code_challenge`：由 `code_verifier` 变换得到（可公开）。

授权阶段上送 challenge，换 token 阶段提交 verifier，授权服务器比对后决定是否发 token。

---

## 六、code_challenge_method：为什么必须用 S256 而不是 plain

PKCE 里 `code_challenge_method` 常见两种：

1. `plain`：
   - `code_challenge = code_verifier`
   - 问题：challenge 被截获就等于 verifier 泄露，失去 PKCE 保护。

2. `S256`：
   - `code_challenge = BASE64URL(SHA256(code_verifier))`
   - 优势：challenge 可公开，但无法反推 verifier。

结论：

- 生产环境应强制 `S256`。
- `plain` 仅为兼容历史/受限环境，不应作为默认或推荐方案。

---

## 七、PKCE 工作原理详解

### 7.1 前置准备

客户端注册后通常拿到：

- `client_id`（公开）
- Public Client 通常不依赖 `client_secret` 做前端安全保证

### 7.2 完整流程

#### 第 1 步：生成 `code_verifier`

```text
code_verifier = 高熵随机字符串（43-128 字符）
```

#### 第 2 步：计算 `code_challenge`（S256）

```text
code_challenge = BASE64URL(SHA256(code_verifier))
```

#### 第 3 步：发起授权请求

```http
GET /authorize?
  response_type=code&
  client_id=your_client_id&
  redirect_uri=myapp://callback&
  scope=openid profile&
  code_challenge=<...>&
  code_challenge_method=S256
```

#### 第 4 步：用户同意后返回授权码

```text
myapp://callback?code=AUTH_CODE_12345
```

#### 第 5 步：用 `code + code_verifier` 换 token

```http
POST /token
  grant_type=authorization_code
  code=AUTH_CODE_12345
  client_id=your_client_id
  code_verifier=<original_verifier>
```

#### 第 6 步：授权服务器校验

```text
BASE64URL(SHA256(code_verifier)) == 之前记录的 code_challenge ?
  是 -> 发 token
  否 -> 拒绝
```

---

## 八、为什么 PKCE 能挡住截获攻击

关键是信息分离：

- `code_challenge`：可见，但不可逆。
- `code_verifier`：不出现在授权请求，不应暴露给拦截者。

攻击者即使拿到授权码和 challenge，仍缺 verifier，无法完成兑换。

---

## 九、PKCE 在 SPA（单页应用）中的使用

### 9.1 为什么 SPA 必须关注 PKCE

SPA 运行在浏览器里，没有后端机密存储能力，本质上是 Public Client。

### 9.2 历史做法与现状

- 历史上很多 SPA 使用 Implicit Flow（直接在前端拿 token）。
- 该流程暴露面更大，安全性与可控性较弱。
- OAuth 2.1 已不再推荐/等效废弃 Implicit Flow，改为“授权码 + PKCE”。

### 9.3 现代推荐

SPA 使用 Authorization Code Flow with PKCE：

1. 前端发起授权并带 PKCE 参数。
2. 回调拿到 code。
3. 通过 PKCE 完成 token 兑换。

这样在不依赖前端保存 client_secret 的前提下，显著提升安全性。

---

## 十、PKCE vs 传统授权码流程

| 对比项 | 传统授权码（依赖静态 secret） | 授权码 + PKCE |
|--------|------------------------------|---------------|
| 客户端证明 | 静态 `client_secret` | 动态 `code_verifier` |
| 适配 Public Client | 弱 | 强 |
| 抗授权码截获 | 有短板 | 显著增强 |
| 实施复杂度 | 中 | 中（多一步 challenge/verifier） |

---

## 十一、OAuth 2.1 的方向

OAuth 2.1 强化了“授权码 + PKCE”作为主流路径，进一步统一了客户端实践，减少历史不安全流程使用。

---

## 十二、总结

```text
1) 问题：Public Client 无法安全保管 client_secret
2) 风险：授权码可能在前端回调链路被截获
3) 解法：PKCE 用一次性 code_verifier/code_challenge 绑定 code 兑换
4) 关键：强制 S256，避免 plain
5) 实践：移动端与 SPA 都应采用授权码 + PKCE
```

---

## 十三、延伸阅读

- 前置：认证与授权、OAuth 2.0、OIDC
- 进阶：OAuth 2.1、移动端回调安全（Universal Links/App Links）、Token 存储与轮换策略
