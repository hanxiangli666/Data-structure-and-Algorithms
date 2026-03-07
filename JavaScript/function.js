function add(a, b) {
    return a + b;
}
// 输出 3
console.log(add(1, 2));

// 把 let 换成 var 也是一样的效果
let add2 = function(a, b) {
    return a + b;
}
// 输出 3
console.log(add2(1, 2));

// 用 ES6 的箭头函数声明
let add3 = (a, b) => {
    return a + b;
}
// 输出 3
console.log(add3(1, 2));
// 用 function 关键词声明的函数和用 () => {} 这种方式声明的箭头函数有一些关键差别，主要是函数体内使用 this 指针的行为不同，但是在算法题中基本不会用到这个特性，所以你可以认为这两种方式是一样的。