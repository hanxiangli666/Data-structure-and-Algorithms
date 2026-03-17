# OAuth 2.0 与 OIDC 认证 完整笔记

---

## 一、背景：为什么需要 OIDC？

### OAuth 2.0 的局限性

标准的 OAuth 2.0  **只能做授权，不能做认证** 。

> **类比理解：**
>
> OAuth 2.0 就像一张"访客通行证"——保安（Google）给了快递员（ExampleNote）一张通行证，让他可以进入仓库取货（访问 Google Drive）。但这张通行证 **并不能证明快递员是谁** ，只说明他有权限拿货。
>
> 如果有人偷了这张通行证，保安也无从分辨真假。

**具体问题：** 授权码明文暴露在 URL 中，黑客截获授权码后，可以在真实用户之前抢先访问回调地址，从而 **冒充用户身份登录** 。

### OIDC 的解决方案

**OpenID Connect（OIDC）** 是构建在 OAuth 2.0 之上的 **认证协议层** ，在保留 OAuth 2.0 授权能力的基础上，增加了安全的身份认证能力。

---

## 二、OIDC 的应用场景

| 场景                               | 说明                            | 举例                                       |
| ---------------------------------- | ------------------------------- | ------------------------------------------ |
| **跨组织认证（第三方登录）** | A 应用借用 B 的认证服务实现登录 | 用 Google 账号登录 ExampleNote             |
| **单点登录（SSO）**          | 同一组织内多个子系统统一登录    | 登录一次 Google，Gmail/Drive/Docs 均可访问 |

---

## 三、核心术语

| 术语                             | 含义                         | 对应 OAuth 2.0 概念     |
| -------------------------------- | ---------------------------- | ----------------------- |
| **OP** （OpenID Provider） | 认证服务器，负责验证用户身份 | Authorization Server    |
| **RP** （Relying Party）   | 依赖方，即各个应用           | Client                  |
| **ID Token**               | 存储用户身份信息的 JWT 令牌  | 无对应概念（OIDC 新增） |

---

## 四、OIDC 与 OAuth 2.0 的关系

```
┌─────────────────────────────────────┐
│              OIDC                   │
│  ┌───────────────────────────────┐  │
│  │         OAuth 2.0             │  │
│  │  授权码流程 / Access Token    │  │
│  └───────────────────────────────┘  │
│  + nonce 参数                       │
│  + ID Token（JWT格式）              │
└─────────────────────────────────────┘
```

**OIDC = OAuth 2.0 的全部能力 + 身份认证能力**

---

## 五、两个关键新增机制

### 5.1 nonce 参数

#### 是什么？

`nonce`（Number used ONCE，一次性数字）是由客户端（RP）生成的 **随机字符串** ，与用户的 Session 绑定。

#### 解决什么问题？

防止 **重放攻击** （Replay Attack）——即黑客截获授权码后冒充用户登录。

#### 工作原理（关键！）

> **类比理解：**
>
> nonce 就像取号机的号码牌。你（用户）去银行（Google）取了一个号 `A001`，银行把你的号码记在了你的账户里。
>
> 黑客偷看到了你的号码 `A001`，但他自己的账户里记的是另一个号 `A002`。
>
> 当黑客拿着 `A001` 去柜台（ExampleNote 服务器）时，柜台一查他的账户，发现对应的号应该是 `A002`，不匹配，直接拒绝。

**具体流程：**

1. 用户点击「用 Google 登录」，ExampleNote 服务器为**该用户**生成专属 Session：
   ```json
   "user_session_xyz456": {  "nonce": "nonce_abc123"}
   ```
2. Session ID `user_session_xyz456` 通过 `Set-Cookie` 发送给用户浏览器（存在 Cookie 中，黑客无法获取）。
3. `nonce=nonce_abc123` 随 URL 参数传给 Google（黑客可以看到，但没用）。
4. 黑客同时登录时，服务器为黑客生成**另一套** Session 和 nonce：
   ```json
   "user_session_aaaa": {  "nonce": "nonce_bbbb"}
   ```
5. 黑客拿着截获的 `nonce_abc123`，但其 Session 关联的是 `nonce_bbbb`， **服务端验证不通过，直接拒绝** 。

#### 关键假设

> ⚠️ Cookie 是安全的——浏览器对 Cookie 有严格的安全限制，防止恶意代码访问。
>
> 如果黑客能窃取 Cookie，那就相当于直接拿到了"钥匙"，所有认证机制都会失效。
>
> **安全建议：不要泄露 Cookie，不要使用非官方浏览器，不要在来路不明的应用上登录。**

---

### 5.2 ID Token（JWT 格式）

#### 是什么？

ID Token 是一个 **JWT（JSON Web Token）格式**的签名令牌，包含用户的身份信息，专门用于 **认证** （证明"你是谁"）。

> **类比理解：**
>
> ID Token 就像护照——由权威机构（Google）签发，包含持有人的身份信息，且带有防伪签名，任何人都可以用已知的方式验证真伪，但无法伪造。

#### ID Token vs Access Token 对比

| 对比项           | Access Token                     | ID Token                             |
| ---------------- | -------------------------------- | ------------------------------------ |
| **用途**   | 访问资源（授权）——"你能做什么" | 证明身份（认证）——"你是谁"         |
| **格式**   | 任意（客户端无需解析）           | JWT（客户端需要解析和验证）          |
| **使用方** | 直接用于访问资源服务器           | ExampleNote 服务器需要验证后才能登录 |

#### ID Token 的 Payload 示例

```json
{
  "iss": "https://accounts.google.com",    // 颁发者：Google 授权服务器
  "sub": "tom_user_id",                    // 主体：用户在 Google 的唯一 ID
  "aud": "examplenote_client_id",          // 受众：这个 Token 是给 ExampleNote 的
  "exp": 1738278000,                       // 过期时间：2025-01-31 07:00:00
  "iat": 1735686000,                       // 颁发时间：2025-01-01 07:00:00
  "nonce": "nonce_abc123",                 // 防重放攻击的随机数
  "email": "tom@gmail.com",               // 用户邮箱
  "name": "Tom"                           // 用户姓名
}
```

**翻译成人话：**

> 我是 Google 服务器，我给 ExampleNote 颁发了这个凭证。
> 刚才有个用户（ID: `tom_user_id`，邮箱: `tom@gmail.com`，名字: `Tom`）在我这里成功登录了。
> 这个凭证 2025-01-01 生效，2025-01-31 过期。
> 你可以用我（Google）的公钥验证这条信息的真实性。

#### ExampleNote 收到 ID Token 后的验证步骤

```
1. 用 Google 公钥验证 JWT 签名（确保内容未被篡改）
2. 检查 aud 字段（确认这个 Token 是给自己的）
3. 检查 iat 和 exp 字段（确认 Token 未过期）
4. 检查 nonce 字段（与用户 Session 中的 nonce 对比，防重放攻击）
5. 提取用户信息，登录成功 ✅
```

---

## 六、OIDC 完整授权码流程

### 与 OAuth 2.0 的请求参数区别

**OAuth 2.0（纯授权）：**

```
https://accounts.google.com/oauth/authorize?
  response_type=code&
  client_id=examplenote_client_id&
  redirect_uri=https://examplenote.com/callback&
  scope=drive.readonly                       ← 只有业务权限
```

**OIDC（授权 + 认证）：**

```
https://accounts.google.com/oauth/authorize?
  response_type=code&
  client_id=examplenote_client_id&
  redirect_uri=https://examplenote.com/callback&
  scope=openid,drive.readonly&               ← 新增 openid（触发 OIDC）
  nonce=nonce_abc123                         ← 新增 nonce（防重放攻击）
```

> 💡 一次请求中可以同时包含多个 scope，用户只需确认一次，即可同时完成 **认证** （openid）和 **授权** （drive.readonly）。

---

### 完整流程时序图

```
用户浏览器          ExampleNote 服务器          Google 授权服务器
    │                      │                           │
    │  点击"用Google登录"   │                           │
    │──────────────────────▶│                           │
    │                      │  生成 nonce，存入 Session  │
    │                      │  通过 Set-Cookie 发 Session ID
    │◀─────────────────────│                           │
    │                      │                           │
    │  重定向到 Google 授权页（URL含nonce和scope=openid,drive.readonly）
    │──────────────────────────────────────────────────▶│
    │                      │                           │
    │                      │                    显示登录页面
    │◀──────────────────────────────────────────────────│
    │                      │                           │
    │  输入账号密码，点击授权│                           │
    │──────────────────────────────────────────────────▶│
    │                      │                           │
    │  重定向回 ExampleNote（URL含授权码 code）          │
    │◀──────────────────────────────────────────────────│
    │                      │                           │
    │  浏览器访问回调地址（携带 code）                   │
    │──────────────────────▶│                           │
    │                      │  用 code + client_secret  │
    │                      │  换取 ID Token + Access Token
    │                      │──────────────────────────▶│
    │                      │                           │
    │                      │  返回 ID Token + Access Token
    │                      │◀──────────────────────────│
    │                      │                           │
    │                      │  验证 ID Token 签名和 nonce│
    │                      │  更新 Session，用户登录成功│
    │◀─────────────────────│                           │
    │  登录成功              │                           │
```

### 分步骤详解

#### 步骤 1：前置准备

ExampleNote 的开发者提前在 Google 开发者平台注册，获得：

* `client_id`（公开标识符）
* `client_secret`（私密密钥，服务器端保存）

#### 步骤 2：生成 nonce，发起授权请求

```
用户点击登录 →
ExampleNote 服务器生成 nonce（如 nonce_abc123），存入用户 Session →
将 Session ID 通过 Set-Cookie 发送给浏览器 →
重定向浏览器到 Google 授权页（URL 携带 nonce 和 scope=openid,...）
```

#### 步骤 3：用户在 Google 登录并授权

用户输入 Google 账号密码，并同意授权 Google Drive 只读权限。

#### 步骤 4：Google 返回授权码

```
Google 重定向浏览器回：
https://examplenote.com/callback?code=AUTH_CODE_12345
```

#### 步骤 5：服务器用授权码换取 Token

ExampleNote 服务器向 Google Token 端点发起请求（ **服务器间通信，不经过浏览器** ）：

```http
POST https://oauth2.googleapis.com/token
Content-Type: application/x-www-form-urlencoded

grant_type=authorization_code&
code=AUTH_CODE_12345&
redirect_uri=https://examplenote.com/callback&
client_id=examplenote_client_id&
client_secret=examplenote_secret
```

#### 步骤 6：Google 返回 ID Token 和 Access Token

```json
{
  "access_token": "example.abcdefg...",
  "token_type": "Bearer",
  "expires_in": 3600,
  "id_token": "eyJhbG..."          ← OIDC 新增的 ID Token
}
```

#### 步骤 7：验证 ID Token

```
验证 JWT 签名（Google 公钥） →
检查 aud 是否为自己的 client_id →
检查 exp 确认未过期 →
检查 nonce 与 Session 中的值是否一致 →
提取用户信息（sub, email, name...）
```

#### 步骤 8：登录成功

```
更新用户信息 →
更新 Session，标记用户已登录 →
（可选）使用 Access Token 访问用户的 Google Drive
```

---

## 七、OIDC vs OAuth 2.0 全面对比

| 特性                 | OAuth 2.0                       | OIDC                               |
| -------------------- | ------------------------------- | ---------------------------------- |
| **主要用途**   | 授权（你能做什么）              | 认证 + 授权（你是谁 + 你能做什么） |
| **核心产物**   | Access Token                    | ID Token + Access Token            |
| **Token 格式** | 任意，客户端无需解析            | JWT，客户端需要解析和验证          |
| **scope 参数** | 自定义（如 `drive.readonly`） | 必须包含 `openid`                |
| **nonce 参数** | 可选                            | 必须（防重放攻击）                 |
| **典型场景**   | 第三方应用访问用户资源          | 第三方登录、单点登录（SSO）        |

---

## 八、知识总结

```
┌─────────────────────────────────────────────────────────────┐
│                        OIDC 核心要点                        │
├─────────────────────────────────────────────────────────────┤
│ 1. OIDC = OAuth 2.0 + 身份认证                             │
│    复用授权码流程，额外增加 nonce 和 ID Token               │
│                                                             │
│ 2. nonce 防重放攻击                                         │
│    nonce 与用户 Session（存在 Cookie 中）绑定               │
│    即使授权码被截获，无法伪造对应的 nonce+Session           │
│                                                             │
│ 3. ID Token（JWT）用于身份证明                              │
│    包含 iss/sub/aud/exp/iat/nonce 等标准字段                │
│    RP 需要验证签名、受众、有效期、nonce                     │
│                                                             │
│ 4. 一次流程，同时完成认证和授权                             │
│    scope=openid（认证）+ drive.readonly（授权）             │
└─────────────────────────────────────────────────────────────┘
```

---

## 九、延伸阅读（前置 & 后续）

* **前置知识：** 认证与授权的区别、OAuth 2.0 授权框架、Session 和 Cookie、JWT
* **后续知识：** 单点登录（SSO）——现代 SSO 同样基于 OIDC
