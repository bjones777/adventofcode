package day25;


import java.util.*;
import java.io.*;
import java.math.BigInteger;

import lombok.*;


public class Main {

    List<BigInteger> numbers = new ArrayList<>();
    BigInteger divider = new BigInteger("20201227");

    List<BigInteger> primes;

    Map<BigInteger,Integer> primeFactorization(BigInteger bi) {
        val lastPrime = primes.get(primes.size()-1);
        val lastPrime2 = lastPrime.multiply(lastPrime);
        if(lastPrime2.compareTo(bi) < 0) {
            throw new RuntimeException("No guaranteed to work");
        }
        Map<BigInteger,Integer> primeFact = new HashMap<>();
        BigInteger cv = bi;
        for(int i = 0;i < primes.size();++i) {
            val p = primes.get(i);
            int numDivs = 0;
            while(p.compareTo(cv) <= 0) {
                val res = cv.divideAndRemainder(p);
                if(res[1].equals(BigInteger.ZERO)) {
                    numDivs += 1;
                    cv = res[0];
                }
                else {
                    break;
                }
            }
            if(numDivs > 0) {
                primeFact.put(p,numDivs);
            }
            if(p.compareTo(cv) > 0) {
                if(!cv.equals(BigInteger.ONE)) {
                    throw new RuntimeException();
                }
                break;
            }
            val p2 = p.multiply(p);
            if(p2.compareTo(cv) > 0) {
                break;
            }
        }
        if(!cv.equals(BigInteger.ONE)) {
            primeFact.put(cv,1);
        }
        return primeFact;
    }

    List<BigInteger> buildPrimes() {
        List<BigInteger> retVal = new ArrayList<>();
        for(int i = 2; i < 1000000;++i) {
            BigInteger cp = new BigInteger(Integer.toString(i));

            boolean isPrime = true;
            for(BigInteger p : retVal) {
                val res = cp.divideAndRemainder(p);
                if(res[1].equals(BigInteger.ZERO)) {
                    isPrime = false;
                }
                BigInteger p2 = p.multiply(p);
                if(cp.compareTo(p2) < 0) {
                    break;
                }
            }
            if(isPrime) {
                retVal.add(cp);
            }
        }
        return retVal;
    }
   
    public Main() {   
        //primes = buildPrimes();
    }

    
    public void readInput() throws IOException {
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day25.txt")))) {
            for(String line = br.readLine();line != null;line = br.readLine()) {
                numbers.add(new BigInteger(line));
            }
       }
    }

    public List<Map<BigInteger,Integer>> buildPrimeFactorizations(BigInteger num) {
        List<Map<BigInteger,Integer>> pfs = new ArrayList<>();
        pfs.add(primeFactorization(num));
        for(int i = 0;i < 10000;++i) {
            if((i % 1000 == 0)) {
                System.out.println(".");
            }
            num = num.add(divider);
            pfs.add(primeFactorization(num));
        }
        System.out.println();
        return pfs;
    }

    public Map<BigInteger,Integer> findLoopSizes() {
        Map<BigInteger,Integer> retVal = new HashMap<>();
        Set<BigInteger> toFind = new HashSet<>();
        toFind.addAll(numbers);
        
        BigInteger sn = new BigInteger("7");
        BigInteger cn = BigInteger.ONE;

        for(int loop = 1;;++loop) {
            cn = cn.multiply(sn).divideAndRemainder(divider)[1];
            if(toFind.contains(cn)) {
                toFind.remove(cn);
                retVal.put(cn, loop);
                if(toFind.isEmpty()) {
                    return retVal;
                }
            }
        }
    }


    // 13895905 -- Too high
    // Loop Sizes 1834, 3107
    // 9232416 8927518
    // 14144084 13240670
    public String partA() {
        
        BigInteger dn = numbers.get(0);

        BigInteger cdn = BigInteger.ONE;

        int loopCount = findLoopSizes().get(numbers.get(1));

        for(int loop = 1; loop <= loopCount;++loop) {    
            cdn = cdn.multiply(dn).divideAndRemainder(divider)[1];                       
        }    
        return cdn.toString();
    }

    
    public String partB() {
        return "";
    }
     
    public static void main(String[] args) throws IOException {
        val m = new Main();
        m.readInput();
        System.out.println(String.format("Part A: %s", m.partA()));
        System.out.println(String.format("Part B: %s", m.partB()));
    }
}