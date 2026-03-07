package main

import (
	"fmt"
)

func main() {
	a := 10

	if a > 5 {
		fmt.Println("a > 5")
	} else if a == 5 {
		fmt.Println("a == 5")
	} else {
		fmt.Println("a < 5")
	}
	// 输出：a > 5

	// 输出：0 1 2 3 4
	for i := 0; i < 5; i++ {
		fmt.Print(i, " ")
	}
	fmt.Println()

	num := 100
	// 输出：100 50 25 12 6 3 1
	for num > 0 {
		fmt.Print(num, " ")
		num /= 2
	}
	fmt.Println()
}
