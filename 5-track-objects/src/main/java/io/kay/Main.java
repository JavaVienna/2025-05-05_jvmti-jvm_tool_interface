package io.kay;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.stream.Stream;

public final class Main {

    private static final Random RANDOM = new Random();
    private static final int BATCH_SIZE = 200;

    record Person(String name, int age) {
    }

    public static void main(String[] args) {
        List<Person> list = new ArrayList<>();

        new Thread(() -> {
            try {
                createPeopleForever(list);
            } catch (Exception e) {
                System.out.println(e);
            }
        }).start();
    }

    private static Person addPerson() {
        byte[] bytes = new byte[25];
        RANDOM.nextBytes(bytes);
        return new Person(new String(bytes), RANDOM.nextInt());
    }

    private static void createPeopleForever(List<Person> list) throws Exception {
        System.out.printf("Creating %d people\n", BATCH_SIZE);
        Stream.generate(Main::addPerson).limit(BATCH_SIZE).forEach(list::add);

        var personToFind = new Person("Mister X", 20);
        list.add(RANDOM.nextInt(list.size()), personToFind);

        var reflectedPerson = (Person) Person.class.getDeclaredConstructors()[0].newInstance("Reflected Person", 30);
        list.add(RANDOM.nextInt(list.size()), reflectedPerson);

        while (true) {
            list.remove(RANDOM.nextInt(list.size()));
            System.out.printf("current list size: %d%n", list.size());

            list.stream()
                    .filter(person -> List.of(personToFind.name, reflectedPerson.name).contains(person.name))
                    .forEach(person -> System.out.println("Found person: " + person));

            Thread.sleep(1_000);
        }
    }
}
