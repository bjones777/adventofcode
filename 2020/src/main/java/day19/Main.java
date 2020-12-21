package day19;


import java.util.*;
import java.util.regex.*;
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

    @Value
    static class QEntry {
        int partIndex;
        String leftOver;
    }

    
    public class Rule {
        @Getter
        final int number;
        
        List<String> choices;
        public Rule(String s) {
            String[] splits = s.split(":");
            number = Integer.parseInt(splits[0]);
            
            choices = Arrays.stream(splits[1].split("\\|")).map(ms -> ms.trim()).collect(Collectors.toList());
        }

        public List<MatchPair> matches(String input) {
            List<MatchPair> matches = new ArrayList<>();
            for(String choice : choices) {
                matches.addAll(matches(input, choice));                
            }
            return matches;
        }

        
        public List<MatchPair> matches(String input, String choice) {
            List<MatchPair> pairs = new ArrayList<>();
            Queue<QEntry> queue = new LinkedList<>();
            String[] parts = choice.split(" ");
            
            queue.add(new QEntry(0,input));
            while(!queue.isEmpty()) {
                val entry = queue.remove();
                val part = parts[entry.getPartIndex()];
                String current = entry.getLeftOver();
                Matcher m = numberPattern.matcher(part);
                if(m.matches()) {
                    int ruleNum = Integer.parseInt(m.group(1));
                    Rule newRule = rules.get(ruleNum);
                    val matchResults = newRule.matches(current);
                    for(val matchResult : matchResults) {
                        if(matchResult.isMatched()) {
                            if(entry.getPartIndex() + 1 == parts.length) {
                                pairs.add(new MatchPair(true, matchResult.getLeftOver()));
                            }
                            else {
                                queue.add(new QEntry(entry.getPartIndex() + 1, matchResult.getLeftOver()));
                            }
                        }
                        
                    }
                }
                else {
                    String nibble = part.substring(1, 2);
                    if(current.startsWith(nibble)) {
                        if(entry.getPartIndex() + 1 == parts.length) {
                            pairs.add(new MatchPair(true, current.substring(1)));
                        }
                        else {
                            queue.add(new QEntry(entry.getPartIndex() + 1, current.substring(1)));
                        }
                    }
                    
                }
            }
            return pairs;
        } 
    }

    public Main() {   
    }

    public void readInput() throws IOException {
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day19.txt")))) {
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
            if(res.stream().anyMatch(mp -> mp.matched && mp.getLeftOver().isEmpty())) {
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
        return partA();
     }

     
     
    public static void main(String[] args) throws IOException {
        val m = new Main();
        m.readInput();
        System.out.println(String.format("Part A: %s", m.partA()));
        System.out.println(String.format("Part B: %s", m.partB()));
    }
}