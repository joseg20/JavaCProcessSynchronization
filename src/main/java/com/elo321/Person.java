package com.elo321;

import java.util.Random;

public class Person extends Thread {
    private final Bridge bridge;
    private final int direction;

    public Person(Bridge bridge, int direction, int id) {
        this.bridge = bridge;
        this.direction = direction;
        this.setName("Person-" + id + "-Direction-" + direction);
    }

    @Override
    public void run() {
        try {
            sleep(new Random().nextInt(3000)); // Simula tiempo de llegada aleatorio
            bridge.enterBridge(this.direction); // Intenta entrar al puente
            sleep(5000); // Simula el tiempo que se tarda en cruzar el puente
            bridge.leaveBridge(this.direction); // Sale del puente
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
