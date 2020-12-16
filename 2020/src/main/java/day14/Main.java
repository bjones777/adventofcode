package day14;

import java.util.*;
import java.util.regex.*;
import java.io.*;

import lombok.*;


public class Main {

    public static class Mask {
        long dontCare = 0;
        long oneBits = 0;
        long zeroBits = 0;

        public Mask(String s) {
            for(int i = 0;i < s.length();++i) {
                dontCare = dontCare << 1;
                oneBits = oneBits << 1;
                zeroBits = zeroBits << 1;
                char ch = s.charAt(i);
                switch(ch) {
                    case 'X':
                        dontCare |= 1;
                        break;
                    case '0':
                        zeroBits |= 1;
                        break;
                    case '1':
                        oneBits |= 1;
                        break;
                    default:
                        throw new RuntimeException(String.format("%s is a bad bitmask",s));

                }
            }
        }

        long apply(long value) {
            value = (value & dontCare);
            value = (value | oneBits);
            if((value & zeroBits) != 0) {
                throw new RuntimeException();
            }
            return value;
        }

        List<Long> apply2(long value) {
            long newValue = zeroBits & value;
            newValue = oneBits | newValue;
            List<Long> l = new ArrayList<Long>();
            apply2Recurse(0,newValue,l);
            return l;
        }

        void apply2Recurse(int index, long newValue, List<Long> addresses) {
            if(index == 36) {
                addresses.add(newValue);
                return;
            }
            long floatingMask = 1L << ((long)index);
            if((floatingMask & dontCare) != 0) {
                apply2Recurse(index + 1, newValue | floatingMask, addresses);
            } 
            apply2Recurse(index + 1, newValue, addresses);
        }
    }


    List<String> lines = new ArrayList<>();
    Pattern memPattern = Pattern.compile("mem\\[([0-9]+)\\] = ([0-9]+)");


    public Main() {   
    }

    public void readInput() throws IOException {
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day14.txt")))) {
            for(String line = br.readLine();line != null;line = br.readLine()) {
                lines.add(line);
            }
       }
    }

    public String partA() {
        Map<Long,Long> memory = new HashMap<>();
        Mask mask = new Mask("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
        for(String line : lines) {
            if(line.startsWith("mask = ")) {
                mask = new Mask(line.substring(7));
            }
            else {
                val m = memPattern.matcher(line);
                if(m.matches()) {
                    long address = Long.parseLong(m.group(1));
                    long value = Long.parseLong(m.group(2));
                    memory.put(address,mask.apply(value));
                }
                else {
                    throw new RuntimeException(String.format("Didn't match %s",line));
                }
            }
        }
        return Long.toString(memory.values().stream().reduce((prev,current) -> prev + current).get());
    }

    
    public String partB() {
        Map<Long,Long> memory = new HashMap<>();
        Mask mask = new Mask("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
        for(String line : lines) {
            if(line.startsWith("mask = ")) {
                mask = new Mask(line.substring(7));
            }
            else {
                val m = memPattern.matcher(line);
                if(m.matches()) {
                    long address = Long.parseLong(m.group(1));
                    long value = Long.parseLong(m.group(2));
                    List<Long> addresses = mask.apply2(address);
                    addresses.forEach(a -> memory.put(a, value));
                }
                else {
                    throw new RuntimeException(String.format("Didn't match %s",line));
                }
            }
        }
        return Long.toString(memory.values().stream().reduce((prev,current) -> prev + current).get());
     }

    public static void main(String[] args) throws IOException {
        val m = new Main();
        m.readInput();
        System.out.println(String.format("Part A: %s", m.partA()));
        System.out.println(String.format("Part B: %s", m.partB()));
    }
}