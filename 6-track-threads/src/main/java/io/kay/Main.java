package io.kay;

import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.stream.IntStream;

public final class Main {

    private static final Random RANDOM = new Random();
    private static final NumberHolder NUMBER = new NumberHolder();

    static class NumberHolder {
        int number = 0;
    }

    public static void main(String[] args) {
        int numberOfThreads = RANDOM.nextInt(5) + 5;
        System.out.printf("Having %d threads\n", numberOfThreads);

        try (ExecutorService executorService = Executors.newFixedThreadPool(numberOfThreads)) {
            IntStream.range(0, numberOfThreads).forEach(i -> executorService.submit(() -> setNumber(i)));
        }
    }

    private static void setNumber(int newNumber) {
        int sleepTime = RANDOM.nextInt(10) + 10;
        String threadName = Thread.currentThread().getName();

        try {
            System.out.printf("Thread %s sleeps for %dms\n", threadName, sleepTime);
            Thread.sleep(sleepTime);
        } catch (InterruptedException e) {
            // ignore
        }

        setNumber(threadName, newNumber);
    }

    private static void setNumber(String threadName, int newNumber) {
        synchronized (NUMBER) {
            System.out.printf("Thread %s sets number to %d\n", threadName, newNumber);
            NUMBER.number = newNumber;
        }
    }
}
