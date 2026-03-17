# 深入理解 OAuth 2.0 授权框架 笔记

> **前置知识**：阅读本笔记前，建议先了解[认证与授权的区别](./认证与授权的区别与联系笔记.md)（认证 = 你是谁，授权 = 你能做什么）。
>
> **核心概览**：OAuth 2.0 是一套**授权**协议规范，核心目的是让第三方应用获得**有限的访问权限**，而不需要知道用户的密码。它的核心产物是 **Access Token**——一个临时的、受限的访问凭证。

---

## 一、纠正常见误区

在正式学习之前，先澄清两个容易搞混的概念：

| 误区 | 事实 |
|------|------|
| "OAuth 2.0 就是第三方登录" | ❌ OAuth 2.0 是**授权**框架，不是认证/登录协议 |
| "用 OAuth 2.0 实现登录就行了" | ❌ 标准 OAuth 2.0 做登录**不安全**（后面会解释原因） |

**那为什么第三方登录总和 OAuth 2.0 一起出现？**

因为第三方登录使用的是 **OpenID Connect（OIDC）** 技术，而 OIDC 是**构建在 OAuth 2.0 之上**的认证协议。所以人们容易把两者混为一谈。

```
┌──────────────────────────────┐
│  OIDC（认证层：你是谁）        │  ← 负责第三方登录
│  ┌────────────────────────┐  │
│  │  OAuth 2.0（授权层：     │  │  ← 负责权限授予
│  │  你能做什么）            │  │
│  └────────────────────────┘  │
└──────────────────────────────┘
```

---

## 二、为什么需要 OAuth 2.0？

### 2.1 一个实际场景

假设有一个笔记软件 **ExampleNote**，想帮你把 **Google Drive** 里的文件导入。

**最直接但最危险的方式**：让你输入 Google 账号密码，ExampleNote 用你的密码登录 Google 读取文件。

**问题一堆**：

| 风险 | 说明 |
|------|------|
| 密码泄露 | 你的 Google 密码交给了第三方，万一 ExampleNote 被黑客攻击呢？ |
| 权限过大 | ExampleNote 拿到密码后能做**任何事**（删文件、发邮件、改密码），而它只需要读文件 |
| 无法撤销 | 你想取消授权？只能改 Google 密码，但这也影响其他应用 |

### 2.2 OAuth 2.0 的解决思路

> **类比**：把密码交给第三方，就像把家里的钥匙给装修工人——他能进你所有房间。OAuth 2.0 的做法是给工人一张**临时门禁卡**，只能进指定房间（只读 Google Drive），到期自动失效，你随时可以注销。

**核心思想**：用**临时的、受限的凭证**（Access Token）代替**永久的、全能的凭证**（密码）。

---

## 三、OAuth 2.0 的四个角色

OAuth 2.0 定义了四个角色（RFC 6749 标准术语）：

| 角色 | 英文 | 在例子中 | 说明 |
|------|------|---------|------|
| **资源所有者** | Resource Owner | 你（用户） | 拥有 Google Drive 里的文件 |
| **资源服务器** | Resource Server | Google Drive 服务器 | 存储受保护资源的服务器 |
| **客户端** | Client | ExampleNote | 想要访问资源的**第三方应用** |
| **授权服务器** | Authorization Server | Google 的授权服务器 | 验证用户身份，颁发 Access Token |

**两个注意点**：

1. **Resource Server 和 Authorization Server** 通常是同一家公司（如都是 Google），但在架构上是**分开的两个服务**
2. **Client 不是用户！** 在 OAuth 2.0 中，Client 是第三方应用程序（ExampleNote），用户是 Resource Owner

> **类比**：你（Resource Owner）在银行（Authorization Server + Resource Server）有一个保险柜。律师（Client/ExampleNote）需要代你取文件。你不会把银行密码给律师，而是去银行办一张**临时授权书**（Access Token），律师凭授权书只能取指定的文件。

---

## 四、授权码流程（Authorization Code Flow）

OAuth 2.0 有多种授权方式，最常用、最安全的是**授权码流程**。

### 4.1 前提准备

ExampleNote 的开发者需要提前在 Google 开发者平台注册，获得：
- **Client ID**：ExampleNote 在 Google 系统中的身份标识（公开的）
- **Client Secret**：ExampleNote 的密钥（严格保密，只存在服务器端）

### 4.2 完整流程六步走

```
用户浏览器          ExampleNote 服务器       Google 授权服务器       Google Drive

  ① 点击"导入 Google Drive 文件"
  │───────────────→│
  │                │
  │  ② 重定向到 Google 授权页面
  │←───────────────│
  │
  │  ③ 访问 Google 授权页面，用户点击"同意"
  │─────────────────────────────────────→│
  │                                      │
  │  ④ Google 重定向回 ExampleNote，URL 中带授权码
  │←─────────────────────────────────────│
  │  (callback?code=AUTH_CODE_12345)
  │───────────────→│
  │                │
  │                │  ⑤ 服务器用授权码 + client_secret 换 Access Token
  │                │────────────────────→│
  │                │                     │ 验证通过
  │                │  返回 Access Token   │
  │                │←────────────────────│
  │                │
  │                │  ⑥ 用 Access Token 请求用户文件
  │                │──────────────────────────────────────→│
  │                │                                       │ 验证 Token
  │                │  返回文件列表                           │
  │                │←──────────────────────────────────────│
  │                │
  │  显示导入的文件  │
  │←───────────────│
```

### 4.3 每一步详解

#### 第 1 步：用户触发授权

你在 ExampleNote 上点击"导入 Google Drive 文件"。ExampleNote 将浏览器**重定向**到 Google 的授权页面：

```
https://accounts.google.com/oauth/authorize?
  response_type=code&
  client_id=examplenote_client_id&
  redirect_uri=https://examplenote.com/callback&
  scope=drive.readonly
```

| 参数 | 含义 |
|------|------|
| `response_type=code` | 告诉 Google 使用授权码流程 |
| `client_id` | ExampleNote 在 Google 的身份标识 |
| `redirect_uri` | 授权完成后，Google 把用户重定向到哪个地址 |
| `scope=drive.readonly` | 申请的权限范围：Google Drive **只读**权限 |

#### 第 2 步：用户同意授权

Google 显示授权确认页面：

```
┌─────────────────────────────────────┐
│  ExampleNote 想要访问你的              │
│  Google Drive（只读权限）              │
│                                      │
│  你同意吗？                           │
│                                      │
│      [ 同意 ]    [ 拒绝 ]             │
└─────────────────────────────────────┘
```

你点击"同意"。

#### 第 3 步：Google 返回授权码

Google 将浏览器重定向回 ExampleNote 的回调地址，URL 中带上一个**授权码（Authorization Code）**：

```
https://examplenote.com/callback?code=AUTH_CODE_12345
```

> ⚠️ 授权码是**一次性的中间凭证**，有效期很短（通常不到 30 秒），不能直接用来访问资源。

#### 第 4 步：用授权码换 Access Token

ExampleNote 的**服务器**（不是浏览器！）向 Google 发起请求：

```http
POST https://oauth2.googleapis.com/token

grant_type=authorization_code&
code=AUTH_CODE_12345&
redirect_uri=https://examplenote.com/callback&
client_id=examplenote_client_id&
client_secret=examplenote_secret
```

| 关键参数 | 含义 |
|---------|------|
| `code` | 刚拿到的授权码 |
| `client_secret` | ExampleNote 的密钥（证明请求确实来自 ExampleNote 服务器） |

#### 第 5 步：Google 返回 Access Token

Google 验证授权码和 client_secret 后，返回：

```json
{
  "access_token": "example.abcdefg...",
  "token_type": "Bearer",
  "expires_in": 3600,
  "scope": "drive.readonly",
  "refresh_token": "example.refresh..."
}
```

| 字段 | 含义 |
|------|------|
| `access_token` | 临时访问凭证，ExampleNote 可以用它访问你的 Google Drive |
| `expires_in` | 有效期（秒），这里是 1 小时 |
| `scope` | 权限范围：只读 |
| `refresh_token` | 刷新令牌（可选），access_token 过期后用它换新的，无需用户重新授权 |

#### 第 6 步：访问受保护资源

ExampleNote 带着 Access Token 请求 Google Drive API：

```http
GET https://www.googleapis.com/drive/v3/files
Authorization: Bearer example.abcdefg...
```

Google Drive 验证 Token 有效 → 返回用户的文件列表。

**整个过程中，ExampleNote 从未接触过用户的 Google 密码。**

---

## 五、为什么要用授权码？为什么不直接返回 Token？

这是 OAuth 2.0 设计中最精妙的地方。

### 5.1 直接返回 Token 的风险

如果第 3 步直接在 URL 中返回 Access Token：

```
https://examplenote.com/callback?access_token=example.abcdefg...
```

URL 参数**毫无安全保障**——浏览器插件、JavaScript 代码、浏览器历史记录都能读到 URL。一旦 Access Token 泄露，黑客就能直接访问你的 Google Drive。

### 5.2 授权码 + 后端通道的安全设计

```
                   ⚠️ 不安全通道（浏览器/URL）
用户浏览器 ←────── 授权码（一次性，30秒过期）──────── Google

                   🔒 安全通道（服务器到服务器）
ExampleNote 服务器 ─── 授权码 + client_secret ──→ Google
ExampleNote 服务器 ←── Access Token ─────────────── Google
```

**关键设计**：

| 安全措施 | 说明 |
|---------|------|
| 授权码一次性使用 | 即使被截获，用过即废 |
| 授权码有效期极短 | 通常不到 30 秒 |
| 换 Token 需要 client_secret | 只有 ExampleNote 服务器知道，黑客不知道 |
| Access Token 走后端通道 | 服务器到服务器的通信，不经过浏览器 |

> **类比**：授权码就像一个"取件码"——快递员（Google）给你发一条短信说"取件码 1234"（URL 中的授权码），你拿取件码去快递柜（ExampleNote 服务器拿授权码 + client_secret 去换 Token），柜子打开拿到包裹（Access Token）。即使别人看到了你的取件码，没有你的手机验证（client_secret）也打不开柜子。

---

## 六、为什么 OAuth 2.0 不能用来做第三方登录？

### 6.1 登录 = 认证，OAuth 2.0 = 授权

登录的本质是**认证**——确认"你是谁"。OAuth 2.0 是**授权**框架——确认"你能做什么"。

### 6.2 授权码劫持攻击

你可能觉得：能成功拿到 Access Token，不就说明用户是 Google 账号的所有者吗？

**不一定！** 看这个攻击场景：

```
正常流程:
用户点同意 → Google 返回授权码到 callback URL → ExampleNote 拿授权码换 Token

攻击场景:
用户点同意 → Google 返回授权码到 callback URL
                 ↑
            黑客截获了 URL 中的授权码！
            抢先访问 callback?code=AUTH_CODE_12345
                 ↓
ExampleNote 服务器用授权码换到 Token
ExampleNote 误以为黑客就是该 Google 账号的用户
→ 黑客成功冒充用户登录了 ExampleNote！
```

**问题根源**：授权码通过 URL 传递，不安全。OAuth 2.0 的设计**保护了 Access Token 不被泄露**，但**不能保证提交授权码的人就是账号所有者**。

> 授权码被一次性使用后，真正的用户反而会收到"授权码已被使用"的错误。

### 6.3 怎么解决？

需要引入 **OpenID Connect（OIDC）**——在 OAuth 2.0 基础上构建的**认证**协议层，通过额外的机制（如 ID Token）来安全地确认用户身份。

---

## 七、完整对比总结

| 维度 | 说明 |
|------|------|
| **OAuth 2.0 是什么** | 授权框架，让第三方应用获得有限权限，无需用户密码 |
| **四个角色** | Resource Owner（用户）、Resource Server（资源服务器）、Client（第三方应用）、Authorization Server（授权服务器） |
| **核心流程** | 用户授权 → 返回授权码 → 后端用授权码换 Access Token → 用 Token 访问资源 |
| **核心产物** | Access Token：临时的、受限的访问凭证 |
| **授权码的意义** | 中间凭证，保证 Access Token 不经过不安全的浏览器 |
| **不能做登录** | 授权码可被截获，无法确认提交者就是账号所有者 |
| **要做登录认证** | 需要 OIDC（OpenID Connect），构建在 OAuth 2.0 之上 |

---

## 八、知识点速查

| 概念 | 一句话解释 |
|------|-----------|
| **OAuth 2.0** | 行业标准的**授权**框架，用临时 Token 代替密码 |
| **Resource Owner** | 资源所有者，通常是用户（你） |
| **Client** | 第三方应用（不是用户！），如 ExampleNote |
| **Authorization Server** | 颁发 Token 的服务器，如 Google 授权服务 |
| **Resource Server** | 存储资源的服务器，如 Google Drive |
| **Authorization Code** | 授权码，一次性中间凭证，用来换 Access Token |
| **Access Token** | 临时访问凭证，携带在 `Authorization: Bearer` 头中 |
| **Refresh Token** | 刷新令牌，用于在 Access Token 过期后获取新的 Token |
| **Client ID / Secret** | 第三方应用在授权服务器的身份标识和密钥 |
| **Scope** | 权限范围，如 `drive.readonly` 表示只读 |
| **Back Channel** | 后端通道，服务器到服务器的安全通信 |
| **OIDC** | 构建在 OAuth 2.0 上的认证协议，解决第三方登录问题 |
