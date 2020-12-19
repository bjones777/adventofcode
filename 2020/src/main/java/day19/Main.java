package day19;

import common.*;

import java.util.*;
import java.util.regex.*;
import java.util.stream.Collector;
import java.util.stream.Collectors;
import java.io.*;

import lombok.*;


public class Main {

    @Value
    public static class MatchPair {
        boolean matched;
        String leftOver;
    } 

    Pattern numberPattern = Pattern.compile("([0-9]+)");
    Map<Integer, Rule> rules = new HashMap<>();
    List<String> lines = new ArrayList<>();
    
    public class Rule {
        @Getter
        final int number;
        
        List<String> choices;
        public Rule(String s) {
            String[] splits = s.split(":");
            number = Integer.parseInt(splits[0]);
            
            choices = Arrays.stream(splits[1].split("\\|")).map(ms -> ms.trim()).collect(Collectors.toList());
        }

        public MatchPair matches(String input) {
            for(String choice : choices) {
                val result = matches(input, choice);
                if(result.isMatched()) {
                    return result;
                }
            }
            return new MatchPair(false, input);
        }

        public MatchPair matches(String input, String choice) {
            String current = input;
            String[] parts = choice.split(" ");
            for(String part : parts) {
                Matcher m = numberPattern.matcher(part);
                if(m.matches()) {
                    int ruleNum = Integer.parseInt(m.group(1));
                    Rule newRule = rules.get(ruleNum);
                    val matchResult = newRule.matches(current);
                    if(!matchResult.isMatched()) {
                        return new MatchPair(false,input);
                    }
                    current = matchResult.getLeftOver();
                }
                else {
                    String nibble = part.substring(1, 2);
                    if(!current.startsWith(nibble)) {
                        return new MatchPair(false,input);
                    }
                    current = current.substring(1);
                }
            }
            return new MatchPair(true, current);
        }
    } 

    public Main() {   
    }

    public void readInput() throws IOException {
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day19sample.txt")))) {
            for(String line = br.readLine();line != null;line = br.readLine()) {
                if(line.isBlank()) {
                    break;
                }
                Rule r = new Rule(line);
                rules.put(r.getNumber(), r);
            }
            for(String line = br.readLine();line != null;line = br.readLine()) {
                lines.add(line);
            }
       }
    }

    public String partA() {
        Rule r = rules.get(0);
        int cnt = 0;
        for(String line : lines) {
            val res = r.matches(line);
            if(res.matched && res.getLeftOver().isEmpty()) {
                ++cnt;
            }
        }
        return Integer.toString(cnt);
    }

    public String partB() {
        Rule newRule8 = new Rule("8: 42 8 | 42");
        Rule newRule11 = new Rule("11: 42 11 31 | 42 31");
        rules.put(newRule8.getNumber(),newRule8);
        rules.put(newRule11.getNumber(),newRule11);
        Rule r = rules.get(0);
        int cnt = 0;
        for(String line : lines) {
            val res = r.matches(line);
            if(res.matched && res.getLeftOver().isEmpty()) {
                ++cnt;
            }
        }
        return Integer.toString(cnt);
     }

     public void test() {
         Rule r = new Rule("0: \"a\" 0 | \"a\"");
         rules.put(r.getNumber(),r);
         String line = "aaaaaa";
         val res = r.matches(line);
         if(res.matched) {
             System.out.println("Matched");
         }
         else {
            System.out.println("Not matched");
         }
     }
     
    public static void main(String[] args) throws IOException {
        val m = new Main();
        m.test();
        m.readInput();
        System.out.println(String.format("Part A: %s", m.partA()));
        System.out.println(String.format("Part B: %s", m.partB()));
    }
}