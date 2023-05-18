package com.elo321;

import java.util.stream.IntStream;

public class Main {
    public static void main(String[] args) {
        Bridge bridge = new Bridge();
        IntStream.range(0, 10).forEach(i -> {
            try {
                new Person(bridge, i % 2, i).start(); // Crea e inicia hilos de personas
                Thread.sleep(500); // Espera medio segundo antes de crear la siguiente persona
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        });
    }
}
