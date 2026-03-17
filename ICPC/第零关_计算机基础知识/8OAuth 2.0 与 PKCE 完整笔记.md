# OAuth 2.0 与 PKCE 完整笔记

---

## 一、背景：授权码流程的隐含前提

在标准 OAuth 2.0 授权码流程中，有一个关键步骤：

```
客户端服务器 用 授权码 + client_secret → 换取 Access Token
```

这个流程有一个 **隐含的前提** ：客户端能够安全保管 `client_secret`。

> **类比理解：**
>
> `client_secret` 就像是应用的"身份证密码"。服务器端应用把密码锁在保险箱里，任何外人都拿不到。但如果你把保险箱直接发给用户（比如打包进 App），用户可以撬开它，密码就暴露了。

---

## 二、客户端类型（Client Types）

OAuth 2.0 根据能否安全保管 `client_secret`，将客户端分为两类：

| 类型                                         | 说明                           | 典型例子                                             |
| -------------------------------------------- | ------------------------------ | ---------------------------------------------------- |
| **可信客户端** （Confidential Client） | 能安全保管 `client_secret`   | 服务器端 Web 应用（secret 存在服务器上，用户不可见） |
| **不可信客户端** （Public Client）     | 无法安全保管 `client_secret` | 移动 App（Android/iOS）、SPA 单页应用、桌面应用      |

### 为什么移动 App 是不可信客户端？

以 Android App 为例：

* `client_secret` 必须写入代码或配置文件
* **Android APK 可以被反编译**
* 任何人都能从 APK 中提取出 `client_secret`
* 即使做代码混淆、加固，也只是增加难度，**无法从根本上解决问题**

---

## 三、不可信客户端的两难困境

```
┌──────────────────────────────────────────────────────┐
│                   不可信客户端的困境                  │
├──────────────────────────────────────────────────────┤
│  困境 1：带 client_secret                            │
│    APK 被反编译 → 黑客提取 secret → 冒充你的应用     │
│                                                      │
│  困境 2：不带 client_secret                          │
│    黑客截获授权码 → 直接换 Token → 获取用户资源      │
│                        ↑                             │
│              授权码拦截攻击（Code Interception）      │
└──────────────────────────────────────────────────────┘
           带了会泄露，不带又不安全，怎么办？
```

**解决方案：PKCE**

---

## 四、PKCE 核心思路

 **PKCE** （Proof Key for Code Exchange，读作 "pixy"）的核心思想：

> 既然**静态的** `client_secret` 会泄露，那就用**动态生成的「一次性密钥」**代替。

> **类比理解：**
>
> 传统方式就像所有人用同一把钥匙（`client_secret`）开锁，一旦钥匙被复制，谁都能开。
>
> PKCE 就像每次都临时换锁——每次授权流程开始前，App 自己造一把新锁（`code_challenge`），把锁交给服务器，然后用钥匙（`code_verifier`）去开。锁是公开的，但钥匙只在自己手里，别人就算拿到锁也没用。

### 「一次性密钥」的特点

| 特点               | 说明                               |
| ------------------ | ---------------------------------- |
| **动态生成** | 每次授权请求都不同，无法提前获取   |
| **仅存内存** | 不写入代码或配置文件               |
| **哈希验证** | 利用单向哈希函数的不可逆性保证安全 |

---

## 五、PKCE 工作原理详解

### 前置准备

Android App 开发者在 Google 开发者平台注册，选择"Android 应用类型"：

* ✅ 获得 `client_id`（公开标识符）
* ❌ **没有** `client_secret`（因为是不可信客户端）

---

### 完整流程（以 Android App 接入 Google Drive 为例）

#### 第 1 步：生成 `code_verifier`

```
App 生成一个 43-128 位的随机字符串：

code_verifier = "random_code_verifier_string"
```

> ⚠️ **这个值只存在于 App 内存中，不会通过网络发送，属于机密信息。**

---

#### 第 2 步：计算 `code_challenge`

```
用 SHA256 哈希算法对 code_verifier 进行运算：

code_challenge = SHA256(code_verifier)
              = "code_challenge_sha256_string"
```

> **关键：SHA256 是单向哈希函数，从 `code_challenge` 无法反推出 `code_verifier`。**
> 这是 PKCE 安全性的数学基础。

> **类比理解：**
>
> SHA256 就像绞肉机——你可以把牛肉绞成肉馅，但绝对无法把肉馅还原成牛肉。`code_challenge` 是肉馅（公开），`code_verifier` 是原来的牛肉（私密）。

---

#### 第 3 步：发起授权请求（携带 `code_challenge`）

```http
https://accounts.google.com/oauth/authorize?
  response_type=code&
  client_id=your_app_client_id&
  redirect_uri=myapp://callback&
  scope=openid&
  code_challenge=code_challenge_sha256_string&    ← 新增
  code_challenge_method=S256                      ← 新增（S256 = SHA256）
```

> `code_challenge` 会暴露在 URL 中，黑客可以看到，但 **没有任何用处** 。

---

#### 第 4 步：用户授权

* 用户在 Google 页面登录并同意授权
* Google 服务器 **记住这次请求的 `code_challenge`** ，并与授权码绑定

---

#### 第 5 步：Google 返回授权码

```
Google 重定向回 App：
myapp://callback?code=AUTH_CODE_12345
```

> ⚠️ 授权码出现在 URL 中，黑客可能截获。但有了 PKCE，截获了也没用（下面解释）。

---

#### 第 6 步：用授权码 + `code_verifier` 换 Token

```http
POST https://oauth2.googleapis.com/token

grant_type=authorization_code&
code=AUTH_CODE_12345&
client_id=your_app_client_id&
code_verifier=random_code_verifier_string    ← 发送原始 code_verifier（非哈希值）
```

---

#### 第 7 步：Google 验证并返回 Token

```
Google 的验证逻辑：

1. 取出之前存储的 code_challenge
2. 计算 SHA256(code_verifier)
3. 对比：SHA256(code_verifier) == code_challenge ？
   ✅ 匹配 → 返回 Access Token 和 ID Token
   ❌ 不匹配 → 拒绝请求
```

---

#### 第 8 步：授权成功，访问 Google Drive

App 拿到 Access Token，调用 Google Drive API 访问用户文件。

---

### 完整时序图

```
用户          Android App              Google 授权服务器       Google Drive
 │                │                           │                    │
 │ 点击"连接Drive" │                           │                    │
 │────────────────▶│                           │                    │
 │                │ 生成 code_verifier         │                    │
 │                │ 计算 code_challenge        │                    │
 │                │                           │                    │
 │                │ 跳转授权页（携带 code_challenge）              │
 │                │──────────────────────────▶│                    │
 │                │                           │ 记住 code_challenge │
 │ 登录并同意授权  │                           │                    │
 │────────────────────────────────────────────▶                   │
 │                │                           │                    │
 │                │ 重定向回 App（携带 code）  │                    │
 │                │◀──────────────────────────│                    │
 │                │                           │                    │
 │                │ 发送 code + code_verifier  │                    │
 │                │──────────────────────────▶│                    │
 │                │                           │ 验证 SHA256(verifier)│
 │                │                           │ == code_challenge ? │
 │                │ 返回 Access Token          │                    │
 │                │◀──────────────────────────│                    │
 │                │                           │                    │
 │                │ 访问 Google Drive          │                    │
 │                │───────────────────────────────────────────────▶│
 │                │ 返回文件列表               │                    │
 │                │◀───────────────────────────────────────────────│
```

---

## 六、为什么 PKCE 能防住黑客？

### 两段信息的分离是关键

```
┌─────────────────────────────────────────────────────────┐
│                  PKCE 的安全设计                         │
├──────────────────┬──────────────────────────────────────┤
│  code_challenge  │  哈希值，通过 URL 传输，黑客可见       │
│  （公开信息）    │  → 知道了也没用，无法反推原始值        │
├──────────────────┼──────────────────────────────────────┤
│  code_verifier   │  原始值，只存在于 App 内存中           │
│  （机密信息）    │  → 黑客根本无法获取                    │
└──────────────────┴──────────────────────────────────────┘
```

### 黑客截获了授权码，然后呢？

```
黑客拿到：
  ✅ 授权码（code）        → 从 URL 截获
  ✅ code_challenge        → 从 URL 截获
  ❌ code_verifier         → 只在 App 内存里，根本拿不到

黑客的尝试：
  ❌ 用 code 直接换 Token  → 服务器要求提供 code_verifier，拒绝
  ❌ 暴力猜测 code_verifier → 43-128 位随机字符串，几乎不可能
  ❌ 从 code_challenge 反推 → SHA256 不可逆，数学上无法做到
  
结果：黑客无功而返 ✅
```

---

## 七、PKCE vs 传统授权码流程

| 对比项                 | 传统授权码（可信客户端） | PKCE（不可信客户端）       |
| ---------------------- | ------------------------ | -------------------------- |
| **验证方式**     | 静态 `client_secret`   | 动态 `code_verifier`     |
| **密钥存储位置** | 服务器（安全）           | App 内存（每次生成）       |
| **是否可能泄露** | 不会（服务器保护）       | 不会（内存中，不写入代码） |
| **防截获能力**   | 依赖 secret 保密         | 依赖哈希不可逆性           |
| **适用客户端**   | 服务器端 Web 应用        | 移动 App、SPA、桌面应用    |

---

## 八、OAuth 2.1 的最新动向

> 在最新的 **OAuth 2.1 规范**中，PKCE 已成为 **所有客户端类型的强制要求** ，不再区分可信和不可信客户端。

**原因：**

1. PKCE 为授权码流程提供了额外的安全层
2. 统一了可信与不可信客户端的实现方式
3. 即使是服务器端应用，使用 PKCE 也更安全（双重保险）

---

## 九、总结

```
┌─────────────────────────────────────────────────────────────┐
│                      PKCE 核心要点                          │
├─────────────────────────────────────────────────────────────┤
│ 1. 问题根源                                                 │
│    不可信客户端（移动/SPA）无法安全保管 client_secret        │
│                                                             │
│ 2. PKCE 解法                                                │
│    用动态一次性的 code_verifier 替代静态 client_secret       │
│                                                             │
│ 3. 安全原理                                                 │
│    code_challenge（公开）= SHA256(code_verifier)            │
│    code_verifier（私密）只存内存，网络上从不出现             │
│    单向哈希不可逆 → 知道 challenge 也推不出 verifier         │
│                                                             │
│ 4. 防攻击效果                                               │
│    黑客截获授权码 → 无 code_verifier → 换不了 Token ✅      │
│                                                             │
│ 5. 发展趋势                                                 │
│    OAuth 2.1 已将 PKCE 设为所有客户端的强制要求              │
└─────────────────────────────────────────────────────────────┘
```

---

## 十、延伸阅读

* **前置知识：** 认证与授权的区别、OAuth 2.0 授权框架、OIDC
* **相关概念：** OAuth 2.1 规范、移动端安全最佳实践
