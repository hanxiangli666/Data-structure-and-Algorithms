# 深入理解 JSON Web Token (JWT) 笔记

> 前置知识：建议先了解非对称加密（公钥/私钥、数字签名）和 Session-Cookie 机制。
>
> 核心概览：JWT 是一种无状态、自包含的认证令牌。服务器不需要保存会话映射，令牌本身携带认证信息，并用数字签名防伪造。

---

## 一、从 Session-Cookie 的痛点说起

### 1.1 Session-Cookie 认证回顾

传统流程：

```text
① 用户登录成功
② 服务器创建 Session，存入 Redis: abcd1234 -> { user_id, permissions }
③ 服务器通过 Set-Cookie 把 sessionID 发给浏览器
④ 后续请求浏览器自动带上 sessionID
⑤ 服务器拿 sessionID 去 Redis 查询用户信息
```

两个特征：

| 特征 | 说明 |
|------|------|
| 有状态 | 服务器依赖 Redis/数据库保存会话映射 |
| 非自包含 | Cookie 里通常只有随机 sessionID，不含业务身份信息 |

### 1.2 分布式场景下的问题

在微服务和多节点部署中，Session-Cookie 会带来共享存储、容量规划、故障恢复等额外复杂度。

---

## 二、JWT 的核心思想

### 2.1 直接把用户信息放客户端，为什么不行？

如果直接把用户信息放请求里：

```text
Cookie: userInfo={"user_id":"test-user","permissions":["read"]}
```

攻击者可以随意篡改为：

```text
Cookie: userInfo={"user_id":"admin","permissions":["read","write","delete"]}
```

服务器无法证明数据是自己签发的。

### 2.2 JWT 的解决方案：签名而不是“隐藏”

JWT 的核心不是“把内容藏起来”，而是“让内容不可伪造”：

- 信息可被读取（Base64 可解码）
- 但只要被改动，签名就会失效

### 2.3 JWT 两大特性

| 特性 | 含义 | 对比 Session-Cookie |
|------|------|-------------------|
| 无状态（Stateless） | 服务器验证时可不查会话存储 | Session 通常要查 Redis |
| 自包含（Self-contained） | 令牌中带有 user_id、权限、过期时间等 | Session ID 本身不带业务信息 |

---

## 三、JWT 认证流程

### 3.1 完整流程

```text
① 用户提交账号密码登录
② 服务器验证通过，构造 Payload
③ 服务器私钥签名，生成 JWT
④ 客户端后续请求携带 Authorization: Bearer <token>
⑤ 服务器公钥验签 + 校验 claims（exp/iss/aud）
   ✅ 通过：处理请求
   ❌ 失败：拒绝请求
```

### 3.2 与 Session 的时序对比

Session-Cookie（有状态）：

```text
浏览器               服务器              Redis
  | POST /login        |                   |
  |------------------->|                   |
  |                    | 存 Session         |
  |                    |------------------->|
  | Set-Cookie: sid    |                   |
  |<-------------------|                   |
  | GET /api (带 sid)   |                   |
  |------------------->|                   |
  |                    | 查 Session         |
  |                    |------------------->|
  |                    |<-------------------|
  | 响应数据            |                   |
  |<-------------------|                   |
```

JWT（无状态）：

```text
浏览器               服务器
  | POST /login        |
  |------------------->|
  |                    | 私钥签发 JWT
  | 返回 JWT           |
  |<-------------------|
  | GET /api (带 JWT)   |
  |------------------->|
  |                    | 公钥验签 + 校验 claims
  | 响应数据            |
  |<-------------------|
```

---

## 四、重要误区先澄清：JWT 不是加密

在看 JWT 三段式结构前，先记住这条：

- JWT 默认只是 Base64 编码 + 签名
- Base64 不是加密，任何人都能解码看见 Payload
- JWT 的安全性来自签名完整性，不来自“内容保密”

结论：不要把密码、银行卡号、身份证号等敏感数据放进 JWT。

---

## 五、JWT 的结构：三段式

JWT 由三部分构成，用 `.` 分隔：

```text
<Header>.<Payload>.<Signature>
```

### 5.1 Header

用于描述令牌元信息，常见字段：

```json
{
  "alg": "RS256",
  "typ": "JWT"
}
```

### 5.2 Payload

存放业务字段和标准字段（claims），例如：

```json
{
  "sub": "user-123",
  "role": "reader",
  "iss": "auth.example.com",
  "aud": "api.example.com",
  "iat": 1758974578,
  "exp": 1758975478
}
```

常见标准字段：`iss`、`sub`、`aud`、`iat`、`nbf`、`exp`、`jti`。

### 5.3 Signature

签名输入：

```text
Base64Url(Header) + "." + Base64Url(Payload)
```

用私钥（或对称密钥，取决于算法）签名后得到 Signature。服务器验证 Signature，即可判断令牌是否被篡改。

---

## 六、Session-Cookie vs JWT 全面对比

| 维度 | Session-Cookie | JWT |
|------|---------------|-----|
| 状态 | 有状态 | 无状态 |
| 自包含性 | 否 | 是 |
| 验证方式 | 查会话存储 | 本地验签 + 校验 claims |
| 扩展性 | 需共享存储 | 多服务天然易扩展 |
| 主动注销 | 简单（删 Session） | 天然困难（令牌在过期前可用） |
| 令牌体积 | 小 | 相对大 |
| 泄露风险 | sid 泄露可被会话劫持 | token 泄露在有效期内可被冒用 |

---

## 七、JWT 的常见误用（高风险）

### 7.1 把 JWT 当“加密容器”

误用：把敏感信息直接放进 Payload。

后果：任何拿到 token 的人都可解码阅读。

### 7.2 不校验关键 claims

误用：只验签，不校验 `exp`、`iss`、`aud`、`nbf`。

后果：过期 token、错误签发方 token 仍可能被接受。

### 7.3 未限制允许算法（alg: none 攻击）

历史上部分 JWT 库曾出现漏洞：攻击者把 Header 改成 `{"alg":"none"}`，服务端若错误接受该算法，可能跳过签名验证直接通过。

防御要点：

1. 服务端必须显式指定允许算法白名单（如仅 `RS256`）。
2. 拒绝 `none` 和任何未配置算法。
3. 不要完全相信 token header 里自报的算法。

### 7.4 误以为“JWT 不能吊销所以不能用”

“无法像 Session 那样直接删除”不等于无解，常见工程方案：

1. Token 黑名单（Redis）：
   优点：可立刻失效指定 token；缺点：引入状态存储和查询开销。
2. 短 Access Token + Refresh Token 轮换：
   优点：把泄露窗口压缩到短时；缺点：实现复杂，需要额外刷新链路管理。
3. 版本号机制（token version）：
   优点：可按用户级批量使旧 token 失效；缺点：验 token 需读取用户版本信息。

### 7.5 Refresh Token 设计错误

常见正确设计：

- Access Token：短期（例如 15 分钟）
- Refresh Token：长期（例如 30 天）

这样做的原因：

- Access Token 泄露后可用时间短，风险可控。
- Refresh Token 用于换新 Access Token，减少用户频繁登录。

如果 Refresh Token 泄露：

- 攻击者可持续换发新的 Access Token，风险高于 Access Token 泄露。
- 需要配合“Refresh Token 轮换 + 复用检测 + 立即吊销策略”。

---

## 八、JWT 和 Session 的混合使用场景

大型系统里通常不是“二选一”，而是分层使用：

1. 浏览器侧（Web 登录态）：常用 Session-Cookie
   原因：可控注销、配合 SameSite/HttpOnly、浏览器生态成熟。
2. 服务间调用（API Gateway -> 微服务）：常用 JWT
   原因：无状态、跨服务验证方便、减少中心存储依赖。

一句话：外部用户会话偏 Session，内部服务鉴权偏 JWT，各取所长。

---

## 九、在 jwt.io 上做直观验证

可在 `jwt.io` 做快速实验：

1. 粘贴 token，查看 Header/Payload 解码结果。
2. 修改 Payload 任意字段，观察签名立即失效。
3. 切换算法或错误密钥，验证失败。

这能帮助建立“可读但不可篡改”的直觉。

---

## 十、知识点速查

| 概念 | 一句话解释 |
|------|-----------|
| JWT | 自包含令牌，依靠签名实现完整性校验 |
| Stateless | 服务端可不保存会话映射 |
| Header | 声明算法和类型 |
| Payload | 携带 claims（业务 + 标准字段） |
| Signature | 防篡改、防伪造核心 |
| Bearer Token | `Authorization: Bearer <token>` |
| Access Token | 短期访问令牌 |
| Refresh Token | 长期刷新凭证 |

---

## 十一、JWT 使用注意事项

1. 不在 Payload 放敏感明文数据。
2. 强制 HTTPS 传输。
3. 显式限定允许算法，拒绝 `none`。
4. 验签同时校验 `exp/nbf/iss/aud`。
5. 采用短期 Access Token，并设计可控的刷新与吊销策略。
6. 私钥严格保密并做好轮换计划。
