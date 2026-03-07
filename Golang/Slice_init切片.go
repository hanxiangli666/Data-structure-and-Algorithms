package main

import (
	"fmt"
)

// 在 Golang 中，切片（Slice）是动态数组的实现。相比于固定大小的数组，切片可以根据需要动态调整大小 并且提供了更多的功能。以下是一些常见的切片初始化方法：
func main() {
	// 初始化一个空的切片 nums
	var nums []int

	// 初始化一个大小为 7 的切片 nums，元素值默认都为 0
	nums = make([]int, 7)

	// 初始化一个包含元素 1, 3, 5 的切片 nums
	nums = []int{1, 3, 5}

	// 初始化一个大小为 7 的切片 nums，其值全都为 2
	nums = make([]int, 7)
	for i := range nums {
		nums[i] = 2
	}

	fmt.Println(nums)

	// 初始化一个大小为 3 * 3 的布尔切片 dp，其中的值都初始化为 true
	var dp [][]bool
	dp = make([][]bool, 3)
	for i := 0; i < len(dp); i++ {
		row := make([]bool, 3)
		for j := range row {
			row[j] = true
		}
		dp[i] = row
	}

	fmt.Println(dp)
}
