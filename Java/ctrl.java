public class ctrl {
    // for 和 while 都可以用来做循环，for 循环一般用于已知循环次数的情况，while 循环一般用于未知循环次数的情况
    public static void main(String[] args) {
        int a = 10;

        if (a > 5) {
            System.out.println("a > 5");
        } else if (a == 5) {
            System.out.println("a == 5");
        } else {
            System.out.println("a < 5");
        }
        // 输出：a > 5

        // 输出：0 1 2 3 4 
        for (int i = 0; i < 5; i++) {
            System.out.print(i + " ");
        }
        System.out.println();

        int num = 100;
        // 输出：100 50 25 12 6 3 1 
        while (num > 0) {
            System.out.print(num + " ");
            num /= 2;
        }
        System.out.println();
    }
}