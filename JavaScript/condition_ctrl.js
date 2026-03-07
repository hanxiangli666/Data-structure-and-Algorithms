let a = 1;
if (a === 1) {
    console.log("a 等于 1");
} else {
    console.log("a 不等于 1");
}

// 这里也有一个小坑，就是 JavaScript 中的 == 和 === 的区别，== 是值相等返回 true，=== 是值和类型都相等才返回 true。反之亦然，!= 是值不相等返回 true，!== 是值和类型都不相等才返回 true。

// 比如 JavaScript 中的 null 和 undefined 都表示空值，null == undefined 是 true，但是 null === undefined 是 false。

// 这两种写法的区别也算是 JavaScript 的历史遗留问题，在做算法题的场景中，基本不会涉及到这么细节的问题，所以可以认为两种写法都是一样的。