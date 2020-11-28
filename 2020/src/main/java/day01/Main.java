package day01;

import java.io.*;
import lombok.val;

public class Main {
    static int partA(int mass) {
        return mass / 3 - 2;
    }

    static int partB(int mass) {
        int amount = partA(mass);
        if(amount <= 0) {
          return 0; 
        }
        return amount + partB(amount);
      }
      

    public static void main(String[] args) throws IOException {
        int totalA = 0;
        int totalB = 0;
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day01.txt")))) {
            for(String line = br.readLine();line != null;line = br.readLine()) {
                int mass = Integer.parseInt(line);
                totalA += partA(mass);
                totalB += partB(mass);
            }
        }
        System.out.println("Hello from day1");
        System.out.println("partA " + totalA);
        System.out.println("partB " + totalB);
    }
}