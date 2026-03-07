package main

import (
	"fmt"
)

// 栈是一种后进先出（LIFO）的数据结构，Golang 标准库没有单独提供一个栈的类型，但可以使用切片或者双链表来模拟栈的功能，因为切片和双链表在尾部添加和删除元素都比较高效。
func main() {
	// 初始化一个空的整型栈 s
	var s []int

	// 向栈顶（切片末尾）添加元素
	s = append(s, 10)
	s = append(s, 20)
	s = append(s, 30)

	// 检查栈是否为空，输出：false
	fmt.Println(len(s) == 0)

	// 获取栈的大小，输出：3
	fmt.Println(len(s))

	// 获取栈顶元素，输出：30
	fmt.Println(s[len(s)-1])

	// 删除栈顶元素
	s = s[:len(s)-1]

	// 输出新的栈顶元素：20
	fmt.Println(s[len(s)-1])
}
