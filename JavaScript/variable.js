let str = "hello world";
str = "world"
// 输出 world
console.log(str);
// 我的可视化代码中的顶层函数一般是用 var 声明的，这是因为 LeetCode 上给的 JS 函数签名都是用 var 声明的，所以我也沿用 var 了，不过你非要改成 let 也是没问题的
if (true) {
    // 这里的 str 和外面的 str 是两个不同的变量
    let str = "hello";
    // 输出 hello
    console.log(str);
}