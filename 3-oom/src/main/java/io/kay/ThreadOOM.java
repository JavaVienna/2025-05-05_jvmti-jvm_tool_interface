package io.kay;

import java.util.ArrayList;
import java.util.List;

public class ThreadOOM {

    public static void execute() {
        System.out.println("Thread ~fork bombing the JVM ...");
        List<Thread> list = new ArrayList<>();
        try {
            for (int i = 0; i < 128 * 1024 * 1024; i++) {
                Thread thread = new Thread(() -> {
                    while (true) {
                        try {
                            Thread.sleep(80000);
                            System.out.println("Done!");
                        } catch (InterruptedException ign) {
                        }
                    }
                });
                thread.start();
                // Hold onto the thread
                list.add(thread);
                if (list.size() % 10000 == 0) {
                    System.out.println("Spawned [" + list.size() + "] threads ...");
                }
            }
        } catch (Exception t) {
            System.out.println(t);
        } finally {
            System.out.println("final list size: " + list.size());
        }
    }
}
