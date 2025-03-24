package io.kay;
/**
 * Copyright 2019 Netflix, Inc.
 * <p>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * <p>
 * http://www.apache.org/licenses/LICENSE-2.0
 * <p>
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Source: https://github.com/Netflix-Skunkworks/jvmquake/blob/master/tests/EasyThreadOOM.java
 */

import java.util.ArrayList;
import java.util.List;

public final class Main {

    public static void main(String[] args) {
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
