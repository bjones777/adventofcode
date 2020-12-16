package day12;

import common.*;
import java.util.*;
import java.io.*;

import lombok.*;


public class Main {
    

    public static enum Command {
        N,
        S,
        E,
        W,
        F, 
        L,
        R,
    }

    @Value
    public static class Location {
        int x;
        int y;

        public int getManhattanDistance() {
            return Math.abs(x) + Math.abs(y);
        }

        public Location moved(Direction d, int amount) {
            switch(d) {
                case N:
                    return new Location(x, y - amount);
                case S:
                    return new Location(x, y + amount);
                case E:
                    return new Location(x + amount, y);
                case W:
                    return new Location(x - amount, y);
            }
            throw new RuntimeException("Bad direction");
        }
    }

    public static class Ship {
        @Getter
        Location location = new Location(0,0);
        Direction dir = Direction.E;
        Ship() {}

        void move(Command command, int amount) {
            Direction toMove = dir;
            switch(command) {
                case N:
                    toMove = Direction.N;
                    break;
                case S:
                    toMove = Direction.S;
                    break;
                case E:
                    toMove = Direction.E;
                    break;
                case W:
                    toMove = Direction.W;
                    break;
                case F:
                    break;
                case R:
                    while(amount > 0) {
                        dir = dir.turnedRight();
                        amount -= 90;
                    }
                    return;
                case L:
                    while(amount > 0) {
                        dir = dir.turnedLeft();
                        amount -= 90;
                    }
                    return;
            }
            location = location.moved(toMove, amount); 
        }   
    }

    @Value
    public static class Instruction {
        Command command;
        int amount;
    }

    List<Instruction> instructions = new ArrayList<>();

    public Main() {   
    }

    public static Command toCommand(char letter) {
        switch(letter) {
            case 'N':
                return Command.N;
            case 'S':
                return Command.S;
            case 'E':
                return Command.E;
            case 'W':
                return Command.W;
            case 'F':
                return Command.F;
            case 'L':
                return Command.L;
            case 'R':
                return Command.R;
        }
        throw new RuntimeException("Bad letter " + letter);
    }

    public void readInput() throws IOException {
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day12.txt")))) {
            for(String line = br.readLine();line != null;line = br.readLine()) {
                char commandLetter = line.charAt(0);
                int amount = Integer.parseInt(line.substring(1));
                instructions.add(new Instruction(toCommand(commandLetter),amount));

            }
       }
    }

    public String partA() {
        Ship s = new Ship();
        instructions.forEach(i -> s.move(i.getCommand(), i.getAmount()));
        return Integer.toString(s.getLocation().getManhattanDistance());
    }

    public static class Waypoint {
        @Getter
        Location location = new Location(10,-1);
        Waypoint() {}
        
        void move(Direction d,int amount) {
            location = location.moved(d, amount);
        }

        public void rotateRight() {
            location = new Location(-location.getY(), location.getX());
        }

        public void rotateLeft() {
            location = new Location(location.getY(), -location.getX());
        }
    }

    public static class Ship2 {
        @Getter
        Location location = new Location(0,0);
        Waypoint waypoint = new Waypoint();
        Ship2() {}

        void move(Command command, int amount) {
            switch(command) {
                case N:
                    waypoint.move(Direction.N, amount);
                    break;
                case S:
                    waypoint.move(Direction.S, amount);
                    break;
                case E:
                    waypoint.move(Direction.E, amount);
                    break;
                case W:
                    waypoint.move(Direction.W, amount);
                    break;
                case F:
                    location = new Location(location.getX() + amount * waypoint.getLocation().getX(), location.getY() + amount * waypoint.getLocation().getY());
                    break;
                case R:
                    while(amount > 0) {
                        waypoint.rotateRight();
                        amount -= 90;
                    }
                    return;
                case L:
                    while(amount > 0) {
                        waypoint.rotateLeft();
                        amount -= 90;
                    }
                    return;
            } 
        }   
    }

    public String partB() {
        Ship2 s = new Ship2();
        instructions.forEach(i -> s.move(i.getCommand(), i.getAmount()));
        return Integer.toString(s.getLocation().getManhattanDistance());
     }

    public static void main(String[] args) throws IOException {
        val m = new Main();
        m.readInput();
        System.out.println(String.format("Part A: %s", m.partA()));
        System.out.println(String.format("Part B: %s", m.partB()));
    }
}