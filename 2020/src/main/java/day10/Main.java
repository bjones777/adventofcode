package day10;

import java.util.*;
import java.io.*;
import java.math.BigInteger;

import lombok.*;

public class Main {


    public static int partA(List<Integer> numbers) {
        int joltsInput = 0;
        int diff1 = 0;
        int diff3 = 0;
        for(int i = 0;i < numbers.size();++i) {
            int adapter = numbers.get(i);
            int diff = adapter - joltsInput;
            if(diff < 0 || diff > 3) {
                throw new RuntimeException(String.format("What? %d %d", joltsInput, adapter));
            }
            if(diff == 1) {
                diff1 += 1;
            }
            if(diff == 3) {
                diff3 += 1;
            }
            joltsInput = adapter;
        }
        diff3 += 1;
        return diff1 * diff3;
    } 

    @Value
    public static class Entry {
        int inputVoltage;
        int index;
    }

    public static BigInteger partBRecursive(List<Integer> numbers, int inputVoltage, int index, Map<Entry, BigInteger> memo) {
        Entry newEntry = new Entry(inputVoltage,index);
        BigInteger v = memo.get(newEntry);
        if(v != null) {
            return v;
        }
        int adapter = numbers.get(index);
        if(adapter - inputVoltage > 3) {
            memo.put(newEntry, BigInteger.ZERO);
            return memo.get(newEntry);
        }
        if((index+1) == numbers.size()) {
            memo.put(newEntry, BigInteger.ONE);
            return memo.get(newEntry);
        }
        BigInteger retVal = BigInteger.ZERO;
        for(int i = index+1;i < numbers.size();++i) {
            retVal = retVal.add(partBRecursive(numbers, adapter, i, memo));    
        }
        memo.put(newEntry, retVal);
        return memo.get(newEntry);
    }

    public static BigInteger partB(List<Integer> numbers) {
        Map<Entry, BigInteger> memo = new HashMap<>();
        BigInteger retVal = BigInteger.ZERO;
        for(int i = 0;i < numbers.size();++i) {
            retVal = retVal.add(partBRecursive(numbers, 0, i, memo));    
        }
        return retVal;
    }

    public static void main(String[] args) throws IOException {
        List<Integer> numbers = new ArrayList<>();
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day10.txt")))) {
            for(String line = br.readLine();line != null;line = br.readLine()) {
                numbers.add(Integer.parseInt(line));       
            }
        }
        Collections.sort(numbers);
        System.out.println(String.format("Part A: %d", partA(numbers)));
        System.out.println(String.format("Part B: %s", partB(numbers).toString()));
    }
}