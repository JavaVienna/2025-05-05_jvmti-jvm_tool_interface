package io.kay;

import java.util.ArrayList;
import java.util.List;

public class ClassicOOM {

    public static void execute() {
        System.out.println("triggering OutOfMemory...");
        List<Object> list = new ArrayList<>();
        try {
            while (true) {
                byte[] bytes = new byte[1024 * 1024 * 5];
                list.add(bytes);
                System.out.println("list size: " + list.size());
            }
        }
        catch (Exception t) {
            System.out.println(t);
        }
        System.out.println("final list size: " + list.size());
    }
}
