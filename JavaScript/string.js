let str = "hello world";
// 输出 11
console.log(str.length);
// 输出 h
console.log(str[0]);
// 输出 true
console.log(str === "hello world");

// 字符串分割
let arr = str.split(" ");
// 输出 ["hello", "world"]
console.log(arr);

// 获取子串，多种方式都可以
// 输出 hello
console.log(str.substring(0, 5));
// 输出 hello
console.log(str.slice(0, 5));
// 输出 hello
console.log(str.substring(0, 5));

// 字符串拼接
let str2 = "world";
// 输出 hello world world
console.log(str + " " + str2);