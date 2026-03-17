# 深入理解单点登录（SSO）完整笔记

---

## 一、背景：为什么需要 SSO？

### 问题场景

假设你在公司每天要用这些系统：

```
mail.company.com     → 企业邮箱
git.company.com      → 代码仓库
pm.company.com       → 项目管理
docs.company.com     → 文档系统
hr.company.com       → 人事系统
```

虽然都是 `company.com` 的子域名，但 **每个系统都要单独登录** ，非常麻烦。

---

### 最简单的想法：共享 Cookie

既然都是同一主域名下的子域名，能不能让 Cookie 对整个 `*.company.com` 生效？

```http
Set-Cookie: sessionID=abc123; domain=.company.com; path=/
```

技术上可行，但有 **严重问题** ：

| 问题类型                 | 具体说明                                                                              |
| ------------------------ | ------------------------------------------------------------------------------------- |
| **安全问题**       | 任一子域名被攻击，攻击者可窃取 Cookie，进而访问**所有**子域名的系统             |
| **架构问题**       | 所有服务必须共享同一个 Session 存储（如 Redis），Session 数据结构必须兼容，导致强耦合 |
| **无法集成第三方** | 第三方 SaaS（如 `yourcompany.slack.com`）有自己的域名，共享 Cookie 根本行不通       |

> **类比理解：**
>
> 共享 Cookie 就像公司给每个员工发同一把万能钥匙。一旦有人丢了钥匙，所有房间都不安全了。

---

### 正确答案：SSO（Single Sign-On，单点登录）

---

## 二、SSO 的核心思想

> 引入一个 **独立的认证中心** ，所有应用都信任它。用户只需在认证中心登录一次，就能访问所有相关应用，无需重复登录。

> **类比理解：**
>
> SSO 就像公司前台的访客系统——你只需要在前台登记一次，拿到访客证（身份凭证），就可以凭这张证进入所有已登记的办公区，而不用在每扇门前重新验证身份。

### SSO 解决了什么

| 问题         | SSO 的解法                                               |
| ------------ | -------------------------------------------------------- |
| 安全隔离     | 每个子系统有自己独立的 Session，一个被攻击不影响其他系统 |
| 跨域名支持   | 可以集成不同域名的服务，包括第三方 SaaS                  |
| 统一认证逻辑 | 子系统只需专注业务，不需要各自实现登录逻辑               |
| 统一账号管理 | 用户只需登录一次                                         |

---

## 三、SSO 核心角色

```
           ┌────────────────────────┐
           │   认证中心（OP）        │
           │   op.company.com       │
           │   - 管理账号密码        │
           │   - 验证用户身份        │
           │   - 颁发身份凭证        │
           └───────────┬────────────┘
                       │ 所有应用信任它
          ┌────────────┼────────────┬────────────┐
          ↓            ↓            ↓            ↓
       应用 A         应用 B       应用 C       应用 D
   mail.company.com  crm.c...   docs.c...   hr.c...
```

| 角色                                                             | 说明                     | 对应 OIDC 概念        |
| ---------------------------------------------------------------- | ------------------------ | --------------------- |
| **认证中心** （Authentication Server / Identity Provider） | 统一验证身份，颁发凭证   | OP（OpenID Provider） |
| **应用** （Application / Service Provider）                | 各业务系统，信任认证中心 | RP（Relying Party）   |
| **用户**                                                   | 只需在认证中心登录一次   | End User              |

---

## 四、SSO 基本工作流程

### 4.1 第一次访问应用 A（需要登录）

```
用户浏览器          应用 A                    认证中心（OP）
    │                │                             │
    │ 访问邮箱        │                             │
    │───────────────▶│                             │
    │                │ 未登录，重定向到认证中心      │
    │                │────────────────────────────▶│
    │◀────────────────────────────────────────────│
    │ 访问认证中心    │                             │
    │────────────────────────────────────────────▶│
    │                │                    显示登录页面
    │◀────────────────────────────────────────────│
    │ 输入用户名密码  │                             │
    │────────────────────────────────────────────▶│
    │                │              验证成功，颁发身份凭证
    │                │              设置 OP 的 Session Cookie
    │◀────────────────────────────────────────────│
    │ 带身份凭证访问应用 A                          │
    │───────────────▶│                             │
    │                │ 验证凭证（询问认证中心）       │
    │                │────────────────────────────▶│
    │                │            返回用户身份信息   │
    │                │◀───────────────────────────│
    │                │ 设置应用 A 的 Session Cookie │
    │ 登录成功，进入邮箱                            │
    │◀───────────────│                             │
```

**此时存在的 Session：**

* `op.company.com` 下的 Session Cookie（记录用户已在认证中心登录）
* `mail.company.com` 下的 Session Cookie（记录用户已登录邮箱）

---

### 4.2 第二次访问应用 B（SSO 生效，无需再登录）

```
用户浏览器          应用 B（CRM）              认证中心（OP）
    │                │                             │
    │ 访问 CRM        │                             │
    │───────────────▶│                             │
    │                │ 未登录，重定向到认证中心      │
    │◀────────────────────────────────────────────│
    │ 访问认证中心    │                             │
    │────────────────────────────────────────────▶│
    │                │         检查 Session Cookie  │
    │                │         ✅ 用户已登录！       │
    │                │         直接颁发新凭证，不问密码
    │◀────────────────────────────────────────────│
    │ 带身份凭证访问应用 B                          │
    │───────────────▶│                             │
    │                │ 验证凭证                     │
    │ 登录成功，进入 CRM                            │
    │◀───────────────│                             │
```

> 🔑 **SSO 的关键：** 第 3-4 步——认证中心通过自己域名下的 Session Cookie 知道用户已登录，直接颁发新凭证， **无需用户再输入密码** 。从用户视角看，只是页面跳转了一下，就自动登录了。

---

## 五、用 OIDC 实现 SSO

现代 SSO 最推荐使用 **OIDC（OpenID Connect）** 来实现。

### OIDC 与 SSO 的角色对应

| SSO 概念   | OIDC 概念             |
| ---------- | --------------------- |
| 认证中心   | OP（OpenID Provider） |
| 各业务应用 | RP（Relying Party）   |
| 身份凭证   | ID Token（JWT 格式）  |

---

### 5.1 第一次访问应用 A（OIDC 完整流程）

**步骤 1：** 用户访问 `mail.company.com`，应用 A 发现未登录，重定向到 OP：

```
https://op.company.com/authorize?
  response_type=code&
  client_id=mail_client_id&
  redirect_uri=https://mail.company.com/callback&
  scope=openid,email,profile&
  nonce=nonce456
```

**步骤 2：** OP 检查 `op.company.com` 下的 Session Cookie → **没有** → 显示登录页面

**步骤 3：** 用户输入用户名密码，OP 验证成功：

* 在后端创建 Session
* 在 `op.company.com` 域名下设置 Session Cookie ← **这个很关键**
* 重定向回应用 A，带上授权码：

```
https://mail.company.com/callback?code=AUTH_CODE_123
```

**步骤 4：** 应用 A 用授权码换取 Token：

```http
POST https://op.company.com/token

grant_type=authorization_code&
code=AUTH_CODE_123&
client_id=mail_client_id&
client_secret=mail_secret&
redirect_uri=https://mail.company.com/callback
```

**步骤 5：** OP 返回 Token：

```json
{
  "access_token": "ACCESS_TOKEN_XYZ",
  "id_token": "ID_TOKEN_ABC...",
  "token_type": "Bearer",
  "expires_in": 3600
}
```

ID Token 的 payload 中，`aud` 为 `mail_client_id`，说明这个 Token 是 **专门给邮箱应用的** 。

**步骤 6：** 应用 A 验证 ID Token 签名和 nonce，登录成功，在 `mail.company.com` 下设置 Session Cookie。

---

### 5.2 第二次访问应用 B（SSO 发生）

**步骤 1：** 用户访问 `crm.company.com`，应用 B 发现未登录，重定向到 OP：

```
https://op.company.com/authorize?
  response_type=code&
  client_id=crm_client_id&
  redirect_uri=https://crm.company.com/callback&
  scope=openid,email,profile&
  nonce=nonce012
```

**步骤 2：** OP 检查 `op.company.com` 下的 Session Cookie → ✅ **Session 仍有效！**

**步骤 3：** OP 直接颁发新的授权码， **无需用户再次登录** ：

```
https://crm.company.com/callback?code=AUTH_CODE_456
```

**步骤 4-6：** 应用 B 用授权码换取 Token，OP 返回一个 `aud=crm_client_id` 的新 ID Token，应用 B 验证后登录成功。

---

### 5.3 关键安全设计：每个应用的 Token 相互独立

```
应用 A 的 ID Token：  "aud": "mail_client_id"
应用 B 的 ID Token：  "aud": "crm_client_id"
```

| 设计要点        | 说明                            |
| --------------- | ------------------------------- |
| `aud`字段不同 | Token 只对特定应用有效          |
| 过期时间独立    | 各应用可设置不同的 Token 有效期 |
| 权限 scope 独立 | 各应用可请求不同的权限范围      |

> **安全保证：** 即使黑客拿到了应用 A 的 Token，也**无法**用它访问应用 B，因为 `aud` 不匹配。

---

## 六、SSO 登出

登出比登录复杂，因为需要清理多处 Session。

### 活跃 Session 全景图

用户登录了邮箱和 CRM 后，存在以下 Session：

```
┌─────────────────────────────────────────────────────┐
│              活跃中的 Session                        │
├─────────────────────────────────────────────────────┤
│  op.company.com   → OP 的 Session（SSO 核心）        │
│  mail.company.com → 邮箱应用的 Session               │
│  crm.company.com  → CRM 应用的 Session               │
│  ...（其他登录过的应用）                             │
└─────────────────────────────────────────────────────┘
```

### 两种登出方式

#### 方式一：RP-Initiated Logout（仅退出当前应用）

> 用户在邮箱点击"退出登录"，只退出邮箱，CRM 等其他应用 **不受影响** 。

```
步骤 1：mail.company.com 清除自己的 Session
步骤 2：重定向到 OP 的 /end_session 端点：

   https://op.company.com/end_session?id_token_hint=ID_TOKEN_ABC

步骤 3：OP 清除 op.company.com 的 Session
```

> ⚠️ **为什么必须清除 OP 的 Session？**
>
> 如果只清除邮箱的 Session，不清除 OP 的 Session：
> 用户再次访问邮箱 → 重定向到 OP → OP 的 Session 仍有效 → 直接自动登录 → **登出形同虚设！**

---

#### 方式二：Single Logout（单点登出，退出所有应用）

> 用户在邮箱点击"退出所有设备"，所有应用的登录状态全部失效。

```
步骤 1：mail.company.com 清除自己的 Session
步骤 2：通知 OP 执行单点登出
步骤 3：OP 清除 op.company.com 的 Session
步骤 4：OP 通过 Back-Channel Logout 向所有曾登录的应用发送登出通知
步骤 5：crm.company.com 等应用收到通知，各自清除 Session
```

**Back-Channel Logout 技术实现：**

OP 向每个应用的后端发送 POST 请求（服务器间通信，不经过浏览器）：

```http
POST https://crm.company.com/backchannel-logout

LOGOUT_TOKEN: <JWT格式的登出令牌>
```

logout_token 是一个 JWT，包含：

* 用户唯一标识（`sub`）
* 登出事件信息

应用收到后：

1. 验证 logout_token 的签名确实由 OP 颁发
2. 找到对应用户的 Session
3. 清除该 Session

---

### 两种登出方式对比

| 对比项                     | RP-Initiated Logout  | Single Logout                |
| -------------------------- | -------------------- | ---------------------------- |
| **效果**             | 仅退出当前应用       | 退出所有应用                 |
| **OP Session**       | 清除                 | 清除                         |
| **其他应用 Session** | 保留                 | 全部清除                     |
| **复杂度**           | 简单                 | 复杂（需 Back-Channel 通知） |
| **适用场景**         | 用户只想退出单个应用 | 用户想彻底登出               |

---

## 七、SSO 完整知识结构

```
┌─────────────────────────────────────────────────────────────┐
│                      SSO 核心要点                           │
├─────────────────────────────────────────────────────────────┤
│ 1. 解决的问题                                               │
│    多系统重复登录 / 共享 Cookie 有安全和架构缺陷             │
│                                                             │
│ 2. 核心机制                                                 │
│    独立认证中心（OP）维护自己的 Session                      │
│    用户在 OP 登录一次，Session 对所有应用的请求均有效        │
│                                                             │
│ 3. 实现方案                                                 │
│    现代 SSO 基于 OIDC，复用授权码流程                        │
│    每个应用获得独立的 ID Token（aud 不同）                   │
│                                                             │
│ 4. 安全设计                                                 │
│    各应用 Session 独立，Token 的 aud 不同                    │
│    一个应用被攻击，其他应用不受影响                         │
│                                                             │
│ 5. 登出                                                     │
│    必须清除 OP Session，否则登出无效                         │
│    单点登出通过 Back-Channel Logout 通知所有应用             │
└─────────────────────────────────────────────────────────────┘
```

---

## 八、延伸阅读

* **前置知识：** Session 和 Cookie、OAuth 2.0、OIDC
* **相关概念：** SAML（另一种 SSO 协议，多见于企业场景）、Back-Channel Logout 规范
