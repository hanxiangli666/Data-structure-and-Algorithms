// 通俗来讲，JavaScript 中的对象就可以理解为是哈希表，因为 JavaScript 对象就是若干键值对。不过 ES6 中引入了 Map 类型，所以我们就规范一些，用 Map 类型来创建哈希表
let map = new Map();

// 添加键值对
map.set("a", 1);
map.set("b", 2);
map.set("c", 3);

// 获取键值对
// 输出 1
console.log(map.get("a"));

// 删除键值对
map.delete("a");

// 判断是否存在某个键
// 输出 false
console.log(map.has("a"));
// 输出 true
console.log(map.has("b"));

// 遍历键值对
for (let key of map.keys()) {
    console.log(key, map.get(key));
}
// 输出 b 2 和 c 3