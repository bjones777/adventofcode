package day16;

import java.util.*;
import java.io.*;
import java.math.BigInteger;
import java.util.regex.*;
import java.util.stream.Collectors;

import lombok.*;


public class Main {

    //zone: 26-379 or 386-972
    static final Pattern RULE_PATTERN = Pattern.compile("([^:]+): ([0-9]+)-([0-9]+) or ([0-9]+)-([0-9]+)");
    
    @Value
    public static class Range {
        int lowerBound;
        int upperBound;

        public boolean isInRange(int val) {
            return val >= lowerBound && val <= upperBound;
        }
    }
    
    public static class Rule {
        String name;
        List<Range> ranges = new ArrayList<>();

        boolean isDeparture() {
            return name.startsWith("departure");
        }

        public Rule(String line) {
            val matcher = RULE_PATTERN.matcher(line);
            if(!matcher.matches()) {
                throw new RuntimeException("Could not match line: " + line);
            }
            name = matcher.group(1);
            ranges.add(new Range(Integer.parseInt(matcher.group(2)),Integer.parseInt(matcher.group(3))));
            ranges.add(new Range(Integer.parseInt(matcher.group(4)),Integer.parseInt(matcher.group(5))));
        }

        public boolean isValid(int val) {
            for(val range : ranges) {
                if(range.isInRange(val)) {
                    return true;
                }
            }
            return false;
        }
    }

    public static class Ticket {
        List<Integer> fields;

        Ticket(String line) {
            fields = Arrays.asList(line.split(",")).stream().map(s -> Integer.parseInt(s)).collect(Collectors.toList());
        }

        int getNumFields() {
            return fields.size();
        }

        int getFieldValue(int index) {
            return fields.get(index);
        }

        int getTicketScanningNumber(List<Rule> rules) {
            for(val field : fields) {
                boolean wasValid = false;
                for(val rule : rules) {
                    if(rule.isValid(field)) {
                        wasValid = true;
                        break;
                    }
                }
                if(!wasValid) {
                    return field;
                }
            }
            return 0;
        }

        public boolean isValid(List<Rule> rules) {
            for(val field : fields) {
                boolean isValid = false;
                for(val rule : rules) {
                    if(rule.isValid(field)) {
                        isValid = true;
                        break;
                    }
                }
                if(!isValid) {
                    return false;
                }
            }
            return true;
        }
    }

    List<Rule> rules = new ArrayList<>();
    Ticket myTicket;
    List<Ticket> nearbyTickets = new ArrayList<>();
    
    public Main() {   
    }

    public void readInput() throws IOException {
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day16.txt")))) {
            for(String line = br.readLine();line != null;line = br.readLine()) {
                if(line.isBlank()) {
                    break;
                }
                rules.add(new Rule(line));
            }
            {
                String line = br.readLine();
                if(!line.equals("your ticket:")) {
                    throw new RuntimeException("Unknown line: " + line);
                }
                myTicket = new Ticket(br.readLine());
                line = br.readLine();
                if(!line.isBlank()) {
                    throw new RuntimeException("Unknown line: " + line);
                }
            }
            {
                String line = br.readLine();
                if(!line.equals("nearby tickets:")) {
                    throw new RuntimeException("Unknown line: " + line);
                }
            }
            for(String line = br.readLine();line != null;line = br.readLine()) {
                nearbyTickets.add(new Ticket(line));
            }
       }
    }

    public String partA() {
        int val = 0;
        for(val ticket : nearbyTickets) {
            val += ticket.getTicketScanningNumber(rules);
        }
        return Integer.toString(val);
    }

    public String partB() {
        List<Ticket> validTickets = new ArrayList<>();
        validTickets.add(myTicket);
        for(val ticket : nearbyTickets) {
            if(ticket.isValid(rules)) {
                validTickets.add(ticket);
            }
        }
        Map<Integer,Set<Rule>> possMap = new HashMap<>();
        for(int i = 0;i < myTicket.getNumFields();++i) {
            Set<Rule> hs = new HashSet<Rule>();
            hs.addAll(rules);
            possMap.put(i,hs);
        }
        for(int i = 0;i < myTicket.getNumFields();++i) {
            Set<Rule> validRules = possMap.get(i);
            Set<Rule> toRemove = new HashSet<>();
            for(val rule : validRules) {
                if(!isValidRuleForField(validTickets,i,rule)) {
                    toRemove.add(rule);
                }
            } 
            validRules.removeAll(toRemove);
        }
    
        boolean didChange = false;
        do {
            didChange = updateChanges(possMap);
        } while(didChange);

        BigInteger value = BigInteger.ONE;
        for(val entry : possMap.entrySet()) {
            for(val rule :  entry.getValue()) {
                if(rule.isDeparture()) {
                    int field = myTicket.getFieldValue(entry.getKey());
                    value = value.multiply(new BigInteger(Integer.toString(field)));
                }
            }
        }
        return value.toString();
     }

     public void printPossMap(Map<Integer,Set<Rule>> possMap) {
        for(int i = 0;i < myTicket.getNumFields();++i) {
            Set<Rule> rules = possMap.get(i);
            String ruleNames = String.join(", ", rules.stream().map(rule -> rule.name).collect(Collectors.toList()));
            System.out.println(String.format("%d: %s",i,ruleNames));
        }
     }

     public boolean updateChanges(Map<Integer,Set<Rule>> possMap) {
         for(val entry : possMap.entrySet()) {
            val ruleSet = entry.getValue();
            if(ruleSet.size() == 1) {
                boolean removed = false;
                for(val otherEntry : possMap.entrySet()) {
                    if(otherEntry.getKey() != entry.getKey()) {
                        removed = otherEntry.getValue().removeAll(ruleSet) || removed;
                    }
                }
                if(removed) {
                    return true;
                }
            }
         }
         for(val entry : possMap.entrySet()) {
            val ruleSet = entry.getValue();
            if(ruleSet.size() != 1) {
                val copyRuleSet = new HashSet<>(ruleSet);
                for(val otherEntry : possMap.entrySet()) {
                    if(otherEntry.getKey() != entry.getKey()) {
                        copyRuleSet.removeAll(otherEntry.getValue());
                    }
                }
                if(copyRuleSet.size() == 1) {
                    ruleSet.clear();
                    ruleSet.addAll(copyRuleSet);
                    return true;
                }
            }
        }
         return false;
     }

     public boolean isValidRuleForField(List<Ticket> validTickets,int fieldIndex,Rule rule) {
        for(val ticket : validTickets) {
            if(!rule.isValid(ticket.getFieldValue(fieldIndex))) {
                return false;
            }
        }
        return true;
     }

    public static void main(String[] args) throws IOException {
        val m = new Main();
        m.readInput();
        System.out.println(String.format("Part A: %s", m.partA()));
        System.out.println(String.format("Part B: %s", m.partB()));
    }
}