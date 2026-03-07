// 遍历数组
let arr = [1, 2, 3, 4, 5];
for (let i = 0; i < arr.length; i++) {
    console.log(arr[i]);
}
// 输出 1 2 3 4 5

// 遍历数组
let arr2 = [1, 2, 3, 4, 5];
for (let item of arr2) {
    console.log(item);
}
// 输出 1 2 3 4 5

// 注意 JavaScript 用的是 of 来遍历数组元素，不要用 in。in 有其他的作用，不过我们做算法题基本用不到，我就不啰嗦了。

// 我知道有些其他语言会用 in 遍历数组元素，所以特此提醒，不要搞错了

let arr3 = [1, 2, 3, 4, 5];
let i = 0;
while (i < arr3.length) {
    console.log(arr3[i]);
    i++;
}
// 输出 1 2 3 4 5