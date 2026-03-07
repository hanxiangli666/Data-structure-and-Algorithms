package main

import (
	"fmt"
)

// Golang 的内置类型 map 提供了哈希表的功能，支持基于键值对（key-value）的存储，具有常数时间复杂度的查找、插入和删除操作。
func main() {
	// 初始化一个空的哈希表 hashmap
	var hashmap map[int]string
	hashmap = make(map[int]string)

	// 初始化一个包含一些键值对的哈希表 hashmap
	hashmap = map[int]string{
		1: "one",
		2: "two",
		3: "three",
	}

	fmt.Println(hashmap)
}
