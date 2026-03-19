# OAuth 2.0 与 OIDC 认证 完整笔记

---

## 一、先看路标：OIDC vs OAuth 2.0 全面对比

先看这张表，再进入细节会更清晰。

| 特性 | OAuth 2.0 | OIDC |
|------|-----------|------|
| 主要用途 | 授权（你能做什么） | 认证 + 授权（你是谁 + 你能做什么） |
| 核心产物 | Access Token | ID Token + Access Token |
| Token 语义 | 访问资源凭证 | 身份凭证（ID Token）+ 访问凭证 |
| scope 关键值 | 业务 scope（如 `drive.readonly`） | 必须包含 `openid` |
| nonce | 可选 | 认证流程中关键参数 |
| 典型场景 | 第三方应用访问用户资源 | 第三方登录、单点登录（SSO） |

---

## 二、背景：为什么需要 OIDC？

### 2.1 OAuth 2.0 的局限性

标准 OAuth 2.0 主要解决“授权”，并不直接定义“可靠认证”。

> 类比：OAuth 2.0 像访客通行证，证明“可访问某资源”，不直接证明“持证人真实身份是谁”。

### 2.2 OIDC 的解决方案

OpenID Connect（OIDC）构建在 OAuth 2.0 之上，增加认证语义和机制：

1. `nonce`（防重放与结果绑定）
2. `id_token`（标准身份令牌，JWT）
3. `/userinfo`（可选的标准用户信息接口）
4. Discovery 与 JWKS（动态发现端点和公钥）

---

## 三、OIDC 的应用场景

| 场景 | 说明 | 举例 |
|------|------|------|
| 跨组织认证（第三方登录） | A 应用借用 B 的身份系统 | 用 Google 账号登录 ExampleNote |
| 单点登录（SSO） | 统一身份，多个系统共享登录态 | 登录一次后访问 Gmail/Drive/Docs |

---

## 四、核心术语

| 术语 | 含义 | 对应 OAuth 2.0 概念 |
|------|------|--------------------|
| OP（OpenID Provider） | 认证提供方 | Authorization Server |
| RP（Relying Party） | 依赖方应用 | Client |
| ID Token | 身份令牌（JWT） | OIDC 新增 |
| UserInfo Endpoint | 用户信息接口 | OIDC 新增 |

---

## 五、OIDC 与 OAuth 2.0 的关系

```text
OIDC
└─ OAuth 2.0（授权码流程、Access Token）
   + ID Token
   + nonce
   + UserInfo / Discovery 等标准能力
```

---

## 六、两个关键新增机制

### 6.1 nonce 参数

#### 先看工作原理图

```text
用户浏览器                RP（ExampleNote）                 OP（Google）
   |                              |                             |
   | 点击“用Google登录”            |                             |
   |----------------------------->|                             |
   |                              | 生成 nonce=nonce_abc123     |
   |                              | 保存到本地会话 Session       |
   |                              |----------------------------->|
   |                              | 发起授权请求（带 nonce）      |
   |----------------------------------------------------------->|
   |                              |                             |
   |<-----------------------------| 回调带 code + (ID Token中nonce)
   |                              |                             |
   |                              | 对比：ID Token.nonce
   |                              |       == Session.nonce ?
   |                              |   是 -> 继续登录
   |                              |   否 -> 拒绝（疑似重放/串改）
```

#### 再看文字解释

`nonce` 是 RP 生成的一次性随机值，用来把“这次认证返回结果”绑定到“这次认证请求”。

核心意义：防重放、抗认证结果混淆。

### 6.2 ID Token（JWT 格式）

ID Token 是 OIDC 的身份凭证，包含用户身份与上下文字段。

示例 Payload：

```json
{
  "iss": "https://accounts.google.com",
  "sub": "tom_user_id",
  "aud": "examplenote_client_id",
  "exp": 1738278000,
  "iat": 1735686000,
  "nonce": "nonce_abc123",
  "email": "tom@gmail.com",
  "name": "Tom"
}
```

---

## 七、OIDC 完整授权码流程

### 7.1 请求参数差异

OAuth 2.0（纯授权）：

```text
.../authorize?
response_type=code&
client_id=...&
redirect_uri=...&
scope=drive.readonly
```

OIDC（认证 + 授权）：

```text
.../authorize?
response_type=code&
client_id=...&
redirect_uri=...&
scope=openid,drive.readonly&
nonce=nonce_abc123&
state=random_state_xyz
```

说明：

- `openid`：触发 OIDC 认证语义。
- `nonce`：绑定 ID Token 与本次登录交互。
- `state`：防 CSRF，绑定前后端授权请求。

### 7.2 时序图（简化）

```text
浏览器 -> RP: 发起登录
RP -> 浏览器: 重定向到 OP 授权页（含 state、nonce）
浏览器 -> OP: 用户登录并同意
OP -> 浏览器: 回调 RP（code + state）
RP -> OP: code 换 token（后端通道）
OP -> RP: 返回 id_token + access_token
RP: 校验 state、验签 id_token、校验 claims、校验 nonce
RP -> 浏览器: 登录成功
```

---

## 八、UserInfo 端点：ID Token 之外的补充信息

OIDC 标准还定义了 `/userinfo` 接口。RP 可以用 Access Token 获取更多用户资料。

### 8.1 什么时候 ID Token 里的信息就够用

适合场景：

- 你只需要最小身份字段（`sub`、`email`、`name`）完成登录和账号绑定。
- 希望减少一次网络调用，提高登录响应速度。

### 8.2 什么时候需要额外调用 UserInfo

适合场景：

- 需要更完整或最新的用户资料（头像、地区、语言等）。
- 希望按 scope 动态获取不同信息集。
- ID Token 里没有你需要的字段，或字段不是最新状态。

请求示意：

```http
GET /userinfo
Authorization: Bearer <access_token>
```

---

## 九、ID Token 验证步骤为什么不能省

任何一步省略，都可能映射到真实攻击面。

1. 验签（Signature）：
   不验签 -> 攻击者可伪造 token 内容。

2. 验 `aud`（受众）：
   不验 aud -> A 应用的 ID Token 可能被拿去骗 B 应用（令牌错投攻击）。

3. 验 `iss`（签发者）：
   不验 iss -> 非预期提供方签发的 token 也可能被接受。

4. 验 `exp` / `nbf` / `iat`（时间相关）：
   不验 exp -> 过期 token 可被重放；
   不验 nbf/iat -> 时序异常 token 可能被接受。

5. 验 `nonce`（前端交互绑定）：
   不验 nonce -> 可能被重放旧认证结果，导致会话混淆。

结论：ID Token 验证是一个“组合防线”，不是单点校验。

---

## 十、Discovery 端点：工程实践里的关键能力

OIDC 提供固定发现地址：

```text
https://<issuer>/.well-known/openid-configuration
```

这个文档会告诉客户端：

- `authorization_endpoint`
- `token_endpoint`
- `userinfo_endpoint`
- `jwks_uri`（公钥集合地址）
- 支持的签名算法、claims、response types 等

工程价值：

1. 客户端无需硬编码所有端点地址。
2. 提供方变更端点时，客户端可动态发现。
3. 公钥轮换可通过 `jwks_uri` 自动适配，降低运维风险。

---

## 十一、知识总结

```text
OIDC = OAuth 2.0 + 认证能力
关键新增：openid scope、ID Token、nonce、UserInfo、Discovery
核心校验：state（防CSRF）+ ID Token 全量 claim 校验
```

---

## 十二、延伸阅读

- 前置：认证与授权、OAuth 2.0、Session/Cookie、JWT
- 后续：PKCE、企业级 SSO、前后端分离下的 Token 存储策略
