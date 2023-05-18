package com.elo321;

import java.util.concurrent.Semaphore;

public class Bridge {
    private final Semaphore semaphore = new Semaphore(5, true);  // permitir un máximo de 5 personas en el puente a la vez
    private int currentDirection = -1;
    private int personsInBridge = 0;
    private int[] waitingPersons = {0, 0};

    public void enterBridge(int direction) {
        try {
            synchronized(this) {
                waitingPersons[direction]++;
                while (personsInBridge > 0 && direction != currentDirection) {
                    wait();
                }
                waitingPersons[direction]--;
                currentDirection = direction;
                personsInBridge++;
            }
            semaphore.acquire();
            System.out.println(Thread.currentThread().getName() + " has entered the bridge. Persons in bridge: " 
                               + personsInBridge + ". Waiting persons: Direction 0 -> " 
                               + waitingPersons[0] + ", Direction 1 -> " + waitingPersons[1] + "\n");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void leaveBridge(int direction) {
        try {
            synchronized(this) {
                personsInBridge--;
                System.out.println(Thread.currentThread().getName() + " has left the bridge. Persons in bridge: " 
                                   + personsInBridge + ". Waiting persons: Direction 0 -> " 
                                   + waitingPersons[0] + ", Direction 1 -> " + waitingPersons[1] + "\n");

                if (personsInBridge == 0) {
                    notifyAll(); 
                    currentDirection = -1;
                }
            }
        } finally {
            semaphore.release();
        }
    }
}
