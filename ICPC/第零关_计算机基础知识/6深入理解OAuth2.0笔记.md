# 深入理解 OAuth 2.0 授权框架 笔记

> 前置知识：建议先了解认证与授权的区别（认证 = 你是谁，授权 = 你能做什么）。
>
> 核心概览：OAuth 2.0 是一套授权框架，目标是让第三方应用拿到临时、受限的访问凭证（Access Token），而不是拿到用户密码。

---

## 一、先纠正最容易走偏的认知

### 1.1 OAuth 2.0 不是登录协议

| 常见说法 | 正确认知 |
|---------|---------|
| OAuth 2.0 就是第三方登录 | 错。OAuth 2.0 是授权框架 |
| 有了 OAuth 2.0 就能安全登录 | 不完整。做登录要用 OIDC |

OAuth 2.0 解决的是授权问题，不直接解决认证问题。

### 1.2 为什么不能直接拿 OAuth 2.0 做登录

登录的本质是确认用户身份（认证），但 OAuth 2.0 只定义了如何给第三方发访问权限（授权）。

风险直觉：

- OAuth 2.0 的授权结果是 Access Token，表示“可访问资源”，不等于“已完成可靠身份认证”。
- 若把授权结果硬当登录凭证，容易在边界条件上出现身份混淆与安全漏洞。

实践结论：

- 只做资源授权：OAuth 2.0。
- 做第三方登录：OAuth 2.0 + OIDC（通过 ID Token、nonce 等机制补齐认证语义）。

### 1.3 OIDC 和 OAuth 2.0 的关系

```text
OIDC（认证层：你是谁）
  └─ 构建在 OAuth 2.0（授权层：你能做什么）之上
```

---

## 二、为什么需要 OAuth 2.0

### 2.1 典型场景

ExampleNote 需要读取你在 Google Drive 的文件。

危险做法：让你把 Google 密码直接给 ExampleNote。

问题：

1. 第三方持有你的长期高权限密码。
2. 权限无法最小化。
3. 撤销困难，通常只能改密码影响全局。

### 2.2 OAuth 2.0 的核心思路

用临时、受限、可撤销的 Access Token 替代密码。

---

## 三、OAuth 2.0 的四个角色

| 角色 | 英文 | 在例子中 | 说明 |
|------|------|---------|------|
| 资源所有者 | Resource Owner | 你 | 资源归属者 |
| 客户端 | Client | ExampleNote | 第三方应用 |
| 授权服务器 | Authorization Server | Google 授权服务 | 颁发 Token |
| 资源服务器 | Resource Server | Google Drive API | 托管受保护资源 |

注意：Client 是应用，不是用户。

---

## 四、授权码流程（Authorization Code Flow）

这是最常见、最推荐的用户授权流程。

### 4.1 前置准备

客户端先在授权服务器注册，获得：

- Client ID（公开标识）
- Client Secret（仅后端保存）

### 4.2 六步流程

```text
用户浏览器          ExampleNote 服务器       Google 授权服务器       Google Drive

① 用户点击导入 Google Drive
② 跳转到 Google 授权页
③ 用户同意授权
④ Google 回调并附带授权码 code
⑤ ExampleNote 后端用 code + client_secret 换 Access Token
⑥ ExampleNote 用 Access Token 调 Google Drive API
```

### 4.3 state 参数为什么必须要有

state 用于防止 CSRF 和请求串改，是 OAuth 客户端的基础防线。

正确做法：

1. 发起授权请求前，客户端生成随机 state 并本地保存。
2. 把 state 放进授权请求。
3. 回调时校验返回的 state 与本地是否一致。
4. 不一致立即拒绝流程。

和 OIDC nonce 的关系：

- state：绑定前后端这次授权请求，防 CSRF。
- nonce：绑定 ID Token 与本次认证交互，防重放和令牌替换。

二者相似点：都用于把“返回结果”绑定到“发起请求”。

### 4.4 为什么先拿授权码，再换 Token

因为浏览器前端通道不可信，授权码是短时一次性中间凭证，真正 Access Token 通过后端安全通道获取，泄露面更小。

---

## 五、除了授权码，还有哪些流程

### 5.1 客户端凭证流程（Client Credentials）

适用场景：机器对机器调用，没有用户参与。

示例：订单服务调用库存服务、监控系统调用日志 API。此时主体是应用本身，授权服务器直接根据客户端身份发 Token，不涉及用户授权页面。

### 5.2 设备码流程（Device Authorization Grant）

适用场景：智能电视、机顶盒、游戏机等输入能力弱设备。

流程特点：设备先拿到 device_code 和 user_code，用户在手机或电脑打开验证页面输入 user_code 完成授权，设备端轮询获取 Access Token。这样避免在电视遥控器上输入账号密码。

---

## 六、Access Token 格式：OAuth 2.0 不做强制

OAuth 2.0 规范只定义“怎么拿 Token”，不强制 Token 长什么样。

常见两类：

### 6.1 不透明字符串（Opaque Token）

特点：Token 本身不可读，资源服务器通常需要调用授权服务器做 introspection 或查缓存。

权衡：

- 优点：可集中控制、易做即时吊销。
- 缺点：资源访问链路多一次远程校验，复杂度和延迟更高。

### 6.2 JWT 令牌（Self-contained Token）

特点：Token 内含 claims，可由资源服务器本地验签。

权衡：

- 优点：本地验证，性能好，适合分布式。
- 缺点：吊销和立即失效较难，需结合短过期与黑名单策略。

---

## 七、Scope 设计原则：最小权限在 OAuth 里的落地

Scope 决定“这张 Token 到底能干什么”，应尽可能细粒度。

示例对比：

- drive.readonly 优于 drive（只读优于读写全权）。
- drive.file 优于 drive.readonly（仅访问当前应用创建文件，权限更收敛）。

设计建议：

1. 默认最小 Scope，按需升级，不一次申请全量权限。
2. Scope 命名要语义清晰，便于审计与告警。
3. 高风险 Scope 单独审批，并记录用户同意日志。

---

## 八、完整对比总结

| 维度 | 说明 |
|------|------|
| OAuth 2.0 是什么 | 授权框架，不是认证协议 |
| 核心产物 | Access Token（临时、受限） |
| 关键流程 | 用户同意 -> code -> 后端换 token -> 访问资源 |
| 安全关键点 | client_secret 仅后端保存，必须校验 state |
| Token 形态 | 可是 opaque，也可是 JWT |
| 做登录怎么办 | 使用 OIDC 叠加认证语义 |

---

## 九、知识点速查

| 概念 | 一句话解释 |
|------|-----------|
| OAuth 2.0 | 授权框架，用 Token 代替密码授权第三方访问资源 |
| Authorization Code | 一次性短期中间凭证，用于后端换 Token |
| Access Token | 访问资源服务器的凭证 |
| Refresh Token | 用于换新 Access Token，减少重复登录 |
| state | 防 CSRF 的请求绑定参数 |
| nonce | OIDC 中绑定认证结果、防重放参数 |
| Client Credentials | 无用户参与的机器对机器授权流程 |
| Device Code | 无键盘设备的授权流程 |
| Opaque Token | 不透明 token，常需远程校验 |
| JWT Token | 可本地验签的自包含 token |
| Scope | Token 的权限边界 |
| OIDC | 构建在 OAuth 2.0 上的认证协议 |
