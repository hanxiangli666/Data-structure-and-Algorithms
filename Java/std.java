public class std {
    public static void main(String[] args) {
        int a = 10;

        // 输出：10
        System.out.println(a);

        // 可以串联输出
        // 输出：Hello, World!
        System.out.println("Hello" + ", " + "World!");

        String s = "abc";
        // 输出：abc 10
        System.out.println(s + " " + a);

        // 格式化输出
        // 输出：abc 10
        System.out.printf("%s %d\n", s, a);
    }
}
