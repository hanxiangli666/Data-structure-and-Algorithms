// ArrayList 是 Java 标准库中的动态数组实现。相比于固定大小的数组，ArrayList 可以根据需要动态调整大小。
import java.util.ArrayList;
import java.util.Arrays;

class ArrayList_init {
    public static void main(String[] args) {
        // 初始化一个空的 ArrayList nums
        ArrayList<Integer> nums = new ArrayList<>();

        // 初始化一个包含元素 1, 3, 5 的 ArrayList nums
        ArrayList<Integer> numsWithElements = new ArrayList<>(Arrays.asList(1, 3, 5));
    }
}