package io.kay;

import java.util.Map;

public class Helper {

    // this is Java 11, no records available yet
    static class Data {
        private final String data;

        public Data(String data) {
            this.data = data;
        }

        public String getData() {
            return data;
        }
    }

    public static Map<String, Data> getMap() {
        return Map.of("data", createData());
    }

    private static Data createData() {
        return new Data(null);
    }
}
