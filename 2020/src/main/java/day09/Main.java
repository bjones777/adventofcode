package day09;

import java.util.*;
import java.io.*;
import java.math.BigInteger;

import lombok.*;

public class Main {

    public static boolean isSum(BigInteger bi, List<BigInteger> numbers, int si, int ei) {
        for(int i = si; i < ei-1;++i) {
            for(int j = i+1;j < ei;++j) {
                if(bi.equals(numbers.get(i).add(numbers.get(j)))) {
                    return true;
                }
            }
        }
        return false;
    }

    public static BigInteger partA(List<BigInteger> numbers) {
        for(int i = 25;i < numbers.size();++i) {
            if(!isSum(numbers.get(i),numbers,i-25,i)) {
                return numbers.get(i);
            }
        }
        throw new RuntimeException("Unknown sum");
    }

    public static BigInteger partB(List<BigInteger> numbers, BigInteger targetNumber) {
        List<BigInteger> sums = new ArrayList<>();
        BigInteger sum = BigInteger.ZERO;
        sums.add(sum);
        for(int i = 0;i < numbers.size();++i) {
            sum = sum.add(numbers.get(i));
            sums.add(sum);
        }

        for(int i = 1;i < sums.size()-1;++i) {
            for(int j = i+1;j < sums.size();++j) {
                BigInteger diff = sums.get(j).subtract(sums.get(i-1));
                if(diff.equals(targetNumber)) {
                    BigInteger min = numbers.get(i-1);
                    BigInteger max = numbers.get(i-1);
                    for(int ii = i;ii < j;++ii) {
                        BigInteger v = numbers.get(ii);
                        if(min.compareTo(v) < 0) min = v;
                        if(max.compareTo(v) > 0) max = v;
                    }
                    return min.add(max);
                } 
            }
        }
        throw new RuntimeException("Unknown sum");
    }


    public static void main(String[] args) throws IOException {
        List<BigInteger> numbers = new ArrayList<>();
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day09.txt")))) {
            for(String line = br.readLine();line != null;line = br.readLine()) {
                numbers.add(new BigInteger(line));       
            }
        }
        BigInteger targetNumber = partA(numbers);
        System.out.println(String.format("Part A: %s", targetNumber.toString()));
        System.out.println(String.format("Part B: %s", partB(numbers,targetNumber).toString()));
    }
}