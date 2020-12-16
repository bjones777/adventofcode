package day13;

import java.util.*;
import java.io.*;
import java.math.BigInteger;

import lombok.*;


public class Main {
    
    int earliestTimeStamp;
    List<Integer> busIds = new ArrayList<>();
    List<Integer> timeOffsets = new ArrayList<>();

    public Main() {   
    }
    public void readInput() throws IOException {
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day13.txt")))) {
            earliestTimeStamp = Integer.parseInt(br.readLine());
            String[] busIdStrings = br.readLine().split(",");
//            busIdStrings = "17,x,13,19".split(",");
            int x = 0;
            for(val busIdString : busIdStrings) {
                ++x;
                if(busIdString.equals("x")) {
                    continue;
                }
                busIds.add(Integer.parseInt(busIdString));
                timeOffsets.add(x-1);
            }
            
       }
    }

    public static int getNextDeparture(int timestamp, int busId) {
        int remainder = timestamp % busId;
        int div = timestamp / busId;
        if(remainder == 0) {
            return timestamp;
        }
        return (div+1) * busId;
    }

    public String partA() {
        int busId = busIds.get(0);
        int earliest = getNextDeparture(earliestTimeStamp, busIds.get(0));
        for(int i = 1;i < busIds.size();++i) {
            int nextDeparture = getNextDeparture(earliestTimeStamp, busIds.get(i));
            if(nextDeparture < earliest) {
                busId = busIds.get(i);
                earliest = nextDeparture;
            }
        }
        BigInteger num = new BigInteger(Integer.toString(busId));
        num = num.multiply(new BigInteger(Integer.toString(earliest - earliestTimeStamp)));
        return num.toString();
    }

    @Value 
    public static class ExtendedEuclidResult {
        BigInteger a;
        BigInteger b;
        BigInteger gcd;
        BigInteger x;
        BigInteger y;
    }
    private static ExtendedEuclidResult extendedGcd(BigInteger a, BigInteger b) {
        BigInteger oldR = a;
        BigInteger r = b;
        BigInteger oldS = BigInteger.ONE;
        BigInteger s = BigInteger.ZERO;
        BigInteger oldT = BigInteger.ZERO;
        BigInteger t = BigInteger.ONE;

        while(!r.equals(BigInteger.ZERO)) {
            val q = oldR.divide(r);
            
            BigInteger temp = oldR.subtract(q.multiply(r));
            oldR = r;
            r = temp;

            temp = oldS.subtract(q.multiply(s));
            oldS = s;
            s = temp;

            temp = oldT.subtract(q.multiply(t));
            oldT = t;
            t = temp;
        }

        return new ExtendedEuclidResult(oldS, oldT, oldR, t, s);
    }
    

    @Value 
    public static class Departure {
        BigInteger leastCommonMultiple;
        BigInteger startingPoint;

        Departure combine(Departure other) {
            // the equation we want to solve is when
            // lcm * x + n === busid * y + offset
            // put into linear form
            // lcm *x - busId * y === offset - n 
            // so we use extended eucled which will find a solution
            // to lcm * x - busId * y === 1
            val result = extendedGcd(getLeastCommonMultiple(), other.getLeastCommonMultiple().negate());
            
            
            System.out.println(String.format("Solving Equation %s * x + %s = %s * y + %s", leastCommonMultiple.toString(), startingPoint.toString(), other.leastCommonMultiple.toString(), other.startingPoint.toString()));
            System.out.println(String.format("As linear %s * x + %s * y =  %s", leastCommonMultiple.toString(), other.leastCommonMultiple.negate().toString(), other.startingPoint.subtract(startingPoint).toString()));

            // solutions are result.getA().multiply * d.getLeastCommonMultiple() + busId *k
            BigInteger X0 = result.getA().multiply(other.startingPoint.subtract(getStartingPoint()));
            if(result.gcd.compareTo(BigInteger.ZERO) < 0) {
                X0 = X0.negate();
                if(result.gcd.compareTo(BigInteger.ONE.negate()) != 0) {
                    throw new RuntimeException(String.format("WHAT THE GCD? %s", result.gcd.toString()));
                } 
            }
            else {
                if(result.gcd.compareTo(BigInteger.ONE) != 0) {
                    throw new RuntimeException(String.format("WHAT THE GCD? %s", result.gcd.toString()));
                }
            }
            System.out.println(String.format("X0 = %s", X0.toString()));
            System.out.println(String.format("X  = %s + %s * k", X0.toString(), other.leastCommonMultiple.toString()));
            
            BigInteger aSolution = X0.multiply(getLeastCommonMultiple()).add(getStartingPoint());
            if(aSolution.compareTo(BigInteger.ZERO) < 0) {
                BigInteger q = aSolution.divide(other.getLeastCommonMultiple().multiply(getLeastCommonMultiple()));
                q = q.negate().add(BigInteger.ONE);
                System.out.println(String.format("First positive k was %s", q.toString()));
                
                BigInteger xk = X0.add(q.multiply(other.getLeastCommonMultiple()));
                System.out.println(String.format("Xk  = %s + %s * (k = %s) = %s", X0.toString(), other.leastCommonMultiple.toString(), q.toString(), xk.toString()));
            
                aSolution = getLeastCommonMultiple().multiply(xk).add(getStartingPoint());
            }
            else if(aSolution.compareTo(BigInteger.ZERO) > 0) {
                val q = aSolution.divide(other.getLeastCommonMultiple().multiply(getLeastCommonMultiple())).negate();
                System.out.println(String.format("First positive k was %s", q.toString()));
                
                BigInteger xk = X0.add(q.multiply(other.getLeastCommonMultiple()));
                System.out.println(String.format("Xk  = %s + %s * (k = %s) = %s", X0.toString(), other.leastCommonMultiple.toString(), q.toString(), xk.toString()));

                aSolution = getLeastCommonMultiple().multiply(xk).add(getStartingPoint());
            }
            return new Departure(leastCommonMultiple.multiply(other.getLeastCommonMultiple()), aSolution);
        }
    }

    public static Departure findNewDeparture(Departure d, int inBusId, int inOffset) {
        BigInteger busId = new BigInteger(Integer.toString(inBusId));
        BigInteger offset = new BigInteger(Integer.toString(inBusId - inOffset));
        BigInteger newStartTime = d.getStartingPoint();

        BigInteger[] dAndR = newStartTime.divideAndRemainder(busId);
        while(!dAndR[1].equals(offset)) {
            newStartTime = newStartTime.add(d.leastCommonMultiple);
            dAndR = newStartTime.divideAndRemainder(busId);
        }
        return new Departure(d.getLeastCommonMultiple().multiply(busId), newStartTime);
    }

    public String partBSlow() {
        Departure start = new Departure(new BigInteger(Integer.toString(busIds.get(0))),BigInteger.ZERO);
        for(int i = 1;i < busIds.size();++i) {
            System.out.println(String.format("Current departure %s + k * %s",start.startingPoint.toString(),start.leastCommonMultiple.toString()));
            start = findNewDeparture(start, busIds.get(i), timeOffsets.get(i));
        }
        System.out.println(String.format("Current departure %s + k * %s",start.startingPoint.toString(),start.leastCommonMultiple.toString()));
        return start.startingPoint.toString();
    }

    public static Departure findNewDepartureFast(Departure d, int inBusId, int inOffset) {
        BigInteger busId = new BigInteger(Integer.toString(inBusId));
        BigInteger offset = new BigInteger(Integer.toString(inBusId - inOffset));

        Departure toCombine = new Departure(busId, offset);
        
        return d.combine(toCombine);
    }

    
    public String partB() {
        Departure current = new Departure(new BigInteger(Integer.toString(busIds.get(0))),BigInteger.ZERO);
        for(int i = 1;i < busIds.size();++i) {
            System.out.println(String.format("Current departure %s + k * %s",current.startingPoint.toString(),current.leastCommonMultiple.toString()));
            current = findNewDepartureFast(current, busIds.get(i), timeOffsets.get(i));
        }
        System.out.println(String.format("Current departure %s + k * %s",current.startingPoint.toString(),current.leastCommonMultiple.toString()));
        return current.startingPoint.toString();
    }
    
    public static void main(String[] args) throws IOException {
        val m = new Main();
        m.readInput();
        System.out.println(String.format("Part A: %s", m.partA()));
        //System.out.println(String.format("Part Ba: %s", m.partBSlow()));
        System.out.println(String.format("Part Bb: %s", m.partB()));
    }
}