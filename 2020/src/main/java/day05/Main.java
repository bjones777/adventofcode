package day05;

import lombok.*;
import java.io.*;
import java.util.*;

import javax.management.RuntimeErrorException;

public class Main {



    
    
    public static int toSeat(String line) {
        int lbr = 0;
        int ubr = 128;
        for(int i = 0;i < 7;++i) {
            int m = (lbr + ubr) / 2;
            if(line.charAt(i) == 'F') {
                ubr = m;
            }
            else if(line.charAt(i) == 'B') {
                lbr = m;
            }
            else throw new RuntimeException("Bad line " + line);
        }
        if(lbr + 1 != ubr) {
            throw new RuntimeException("Bad line 2 " + line);
        }
        int lbc = 0;
        int ubc = 8;
        for(int i = 7;i < 10;++i) {
            int m = (lbc + ubc) / 2;
            if(line.charAt(i) == 'L') {
                ubc = m;
            }
            else if(line.charAt(i) == 'R') {
                lbc = m;
            }
            else throw new RuntimeException("Bad line 3 " + line);
        }
        if(lbc + 1 != ubc) {
            throw new RuntimeException("Bad line 4 " + line);
        }
        return lbr * 8 + lbc;
        
    }

    public static int partA(List<String> lines) {
        int max = toSeat(lines.get(0));
        for(int i = 1;i < lines.size();++i) {
            max = Math.max(max, toSeat(lines.get(i)));
        }
        return max;
    }

    public static int partB(List<String> lines) {
        Set<Integer> seats = new HashSet<>();
        int max = toSeat(lines.get(0));
        int min = max;
        seats.add(min);
        for(int i = 1;i < lines.size();++i) {
            int seatId = toSeat(lines.get(i));
            max = Math.max(max, seatId);
            min = Math.min(min, seatId);
            seats.add(seatId);
        }
        for(int seatId = min+1;seatId < max;++seatId) {
            if(!seats.contains(seatId) && seats.contains(seatId-1)
            && seats.contains(seatId+1)) {
                return seatId;
            }
        }
        throw new RuntimeException("Could not find seat");
    }
    public static void main(String[] args) throws IOException {
        List<String> lines = new ArrayList<>();
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day05.txt")))) {
            for(String line = br.readLine();line != null;line = br.readLine()) {
                lines.add(line);
            }
        }
        System.out.println(String.format("part A: %d", partA(lines)));
        System.out.println(String.format("part A: %d", partB(lines)));
    }
}