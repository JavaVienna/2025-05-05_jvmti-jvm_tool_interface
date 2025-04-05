package io.kay;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class SlowOOM {

    public static void execute() {
        Random random = new Random();
        List<Object> list = new ArrayList<>();
        double cmfZone = 0.005 * Runtime.getRuntime().maxMemory();
        System.out.printf("triggering OutOfMemory by allocating %.2f bytes\n", cmfZone);
        try {
            int x = 30_000;
            while (true) {
                byte[] bytes = new byte[x];
                list.add(bytes);
                if ((double)(list.size() * x) > cmfZone)
                    list.remove(random.nextInt(list.size()));
            }
        }
        catch (Exception t) {
            System.out.println(t);
        }
        System.out.println("final list size: " + list.size());
    }
}
