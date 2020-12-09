package day07;

import java.util.*;
import java.util.regex.*;
import java.io.*;
import java.math.BigInteger;

import lombok.*;
import java.util.stream.Collectors;

public class Main {
    static public class Rule {
        private static Pattern bagsPattern = Pattern.compile(" (\\d)+ (.+) (bags?\\.?)");

        @Getter
        private final String containingBag;
        private Map<String,Integer> containedBags = new HashMap<>();

        public Rule(String line) {
            String[] split = line.split(" bags contain");
            containingBag = split[0];
            if(split.length != 2) {
                throw new RuntimeException("Bad line " + line);
            }
            split = split[1].split(",");
            if(split.length == 1 && split[0].equals(" no other bags.")) {
                return;
            }
            for(String bags : split) {
                val m = bagsPattern.matcher(bags);
                if(m.matches()) {
                    containedBags.put(m.group(2), Integer.parseInt(m.group(1)));
                }
                else {
                    throw new RuntimeException("Could not match " + bags + " in " + line);
                }
            }
        }

        public boolean canDirectlyContain(String bagName) {
            return containedBags.keySet().contains(bagName);
        }

        public Set<String> getContainedBags() {
            return containedBags.keySet();
        }

        public int getNumForBag(String bag) {
            return containedBags.get(bag);
        }
    } 

    public static boolean canContain(String currentBag, Map<String, Rule> rules, Map<String,Boolean> canContainMap) {
        Boolean value = canContainMap.get(currentBag);
        if(value != null) {
            return value.booleanValue();
        }
        Rule r = rules.get(currentBag);
        if(r == null) {
            throw new RuntimeException("No rule for " + currentBag);
        }
        if(r.canDirectlyContain("shiny gold")) {
            canContainMap.put(currentBag,true);
            return true;
        }
        boolean retVal = false;
        for(String b : r.getContainedBags()) {
            retVal = canContain(b, rules, canContainMap);
            if(retVal) {
                canContainMap.put(currentBag,true);
                return true;
            }
        }
        canContainMap.put(currentBag,false);
        return false;   
    }

    public static BigInteger containedBagCount(String currentBag, Map<String, Rule> rules, Map<String,BigInteger> bagCount) {
        val value = bagCount.get(currentBag);
        if(value != null) {
            return value;
        }
        Rule r = rules.get(currentBag);
        if(r == null) {
            throw new RuntimeException("No rule for " + currentBag);
        }
        BigInteger count = BigInteger.ZERO;
        for(String b : r.getContainedBags()) {
            BigInteger total = BigInteger.valueOf(r.getNumForBag(b));
            total = total.add(total.multiply(containedBagCount(b, rules, bagCount)));
            count = count.add(total);
        }
        bagCount.put(currentBag,count);
        return count;   
    }

    public static int partA(List<Rule> rules) {
        Set<String> allBags = rules.stream().map(rule -> rule.getContainingBag()).collect(Collectors.toSet());
        Map<String, Rule> bagToRule = rules.stream().collect(Collectors.toMap(r -> r.getContainingBag(), r -> r));
        Map<String, Boolean> canContainMap = new HashMap<>();
        canContainMap.put("shiny gold", false);
        int count = 0;
        for(val bag : allBags) {
            if(canContain(bag, bagToRule, canContainMap)) {
                ++count;
            }
        }
        return count;
    }

    public static BigInteger countBags(List<Rule> rules) {
        Map<String, Rule> bagToRule = rules.stream().collect(Collectors.toMap(r -> r.getContainingBag(), r -> r));
        Map<String, BigInteger> bagCount = new HashMap<>();
        return containedBagCount("shiny gold", bagToRule, bagCount);
    }

    public static void main(String[] args) throws IOException {
        List<Rule> rules = new ArrayList<>();
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day07.txt")))) {
            for(String line = br.readLine();line != null;line = br.readLine()) {
                rules.add(new Rule(line));
            }
        }
        System.out.println(String.format("Part A: %d", partA(rules)));
        System.out.println(String.format("Part B: %s", countBags(rules).toString(10)));
    }
}