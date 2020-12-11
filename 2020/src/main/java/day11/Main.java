package day11;

import java.util.*;
import java.io.*;

import lombok.*;


public class Main {
    public static enum SeatState {
        OCCUPIED,
        EMPTY 
    }

    @Value
    public static class SeatLocation {
        int x;
        int y;
    }

    Map<SeatLocation, SeatState> seatStateMap = new HashMap<>();

    public Main() {   
    }

    public void readInput() throws IOException {
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day11.txt")))) {
            int y = 0;
            for(String line = br.readLine();line != null;line = br.readLine()) {
                for(int x = 0;x < line.length();++x) {
                    char type = line.charAt(x);
                    if(type == 'L') {
                        seatStateMap.put(new SeatLocation(x, y), SeatState.EMPTY);
                    }
                    else if(type == '#') {
                        seatStateMap.put(new SeatLocation(x, y), SeatState.OCCUPIED);
                    }
                }
                ++y;
            }
        }
    }

    private int countOccupiedAdjacentSeats(Map<SeatLocation, SeatState> in, SeatLocation l) {
        int count = 0;
        for(int dx = -1;dx <= 1;++dx) {
            for(int dy = -1;dy <= 1;++dy) {
                if(dx == 0 && dy == 0) continue;
                SeatState state = in.getOrDefault(new SeatLocation(l.getX()+dx, l.getY()+dy), SeatState.EMPTY);
                if(state == SeatState.OCCUPIED) {
                    ++count;
                }
            }
        }
        return count;
    }

    private boolean foundOccupiedSeat(Map<SeatLocation, SeatState> in, SeatLocation l,int dx,int dy) {
        SeatLocation cl = new SeatLocation(l.getX()+dx, l.getY()+dy);

        // TODO: Probably should have a way to drive this upper bound from the data
        while((cl.getX() >= 0 && cl.getX() < 100) && (cl.getY() >= 0 && cl.getY() < 100)) {
            if(in.containsKey(cl)) {
                SeatState state = in.getOrDefault(cl, SeatState.EMPTY);
                return state == SeatState.OCCUPIED;
            }
            cl = new SeatLocation(cl.getX()+dx, cl.getY()+dy);
        }
        return false;
    }

    private int countOccupiedAdjacentSeats2(Map<SeatLocation, SeatState> in, SeatLocation l) {
        int count = 0;
        for(int dx = -1;dx <= 1;++dx) {
            for(int dy = -1;dy <= 1;++dy) {
                if(dx == 0 && dy == 0) continue;
                if(foundOccupiedSeat(in,l,dx,dy)) {
                    ++count;
                }
            }
        }
        return count;
    }

    private Map<SeatLocation, SeatState> step(Map<SeatLocation, SeatState> in) {
        Map<SeatLocation, SeatState> out = new HashMap<>();
        in.entrySet().forEach(e -> {
            int count = countOccupiedAdjacentSeats(in, e.getKey());
            val state = e.getValue();
            if(state == SeatState.EMPTY && count == 0) {
                out.put(e.getKey(), SeatState.OCCUPIED);
            }
            else if(state == SeatState.OCCUPIED && count >= 4) {
                out.put(e.getKey(), SeatState.EMPTY);
            }
            else {
                out.put(e.getKey(), state);
            }
        });
        return out;
    }

    private Map<SeatLocation, SeatState> step2(Map<SeatLocation, SeatState> in) {
        Map<SeatLocation, SeatState> out = new HashMap<>();
        in.entrySet().forEach(e -> {
            int count = countOccupiedAdjacentSeats2(in, e.getKey());
            val state = e.getValue();
            if(state == SeatState.EMPTY && count == 0) {
                out.put(e.getKey(), SeatState.OCCUPIED);
            }
            else if(state == SeatState.OCCUPIED && count >= 5) {
                out.put(e.getKey(), SeatState.EMPTY);
            }
            else {
                out.put(e.getKey(), state);
            }
        });
        return out;
    }

    private int countOccupied(Map<SeatLocation, SeatState> current) {
        int count = 0;
        for(val entry : current.entrySet()) {
            if(entry.getValue() == SeatState.OCCUPIED) {
                ++count;
            }
        }
        return count;
    }

    public String partA() {
        Map<SeatLocation, SeatState> prev = seatStateMap;
        Map<SeatLocation, SeatState> current = step(prev);

        while(!current.equals(prev)) {
            prev = current;
            current = step(current);            
        }

        return Integer.toString(countOccupied(current));
    }

    public String partB() {
        Map<SeatLocation, SeatState> prev = seatStateMap;
        Map<SeatLocation, SeatState> current = step2(prev);

        while(!current.equals(prev)) {
            prev = current;
            current = step2(current);   
        }

        return Integer.toString(countOccupied(current));
    }

    public static void main(String[] args) throws IOException {
        val m = new Main();
        m.readInput();
        System.out.println(String.format("Part A: %s", m.partA()));
        System.out.println(String.format("Part B: %s", m.partB()));
    }
}