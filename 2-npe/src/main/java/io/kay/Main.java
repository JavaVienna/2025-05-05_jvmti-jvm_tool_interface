package io.kay;

import java.util.Map;
import java.util.HashMap;

public class Main {

    // this is Java 11
    static class Data {
        private final String data;

        public Data(String data) {
            this.data = data;
        }

        public String getData() {
            return data;
        }
    }

    public static void main(String[] args) {
        // 3 possibilities which could be null in this line
        String value = Helper.getMap().get("data").getData().toLowerCase();

        System.out.println(value);
    }
}
