package day02;

import java.io.*;
import java.util.*;
import lombok.val;
import java.util.regex.*;

public class Main {

    public static void main(String[] args) throws IOException {
        List<String> lines = new ArrayList<String>();
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day02.txt")))) {
            for(String line = br.readLine();line != null;line = br.readLine()) {
                lines.add(line);
            }
        }
        //4-7 k: wkkkcfkrkhxd
        val pattern = Pattern.compile("([0-9]+)-([0-9]+) ([A-Za-z]): ([A-Za-z]+)");
        int countA = 0;
        int countB = 0;
        
        for(val line : lines) {
            val match = pattern.matcher(line);
            if(match.matches()) {
                int lb = Integer.parseInt(match.group(1));
                int ub = Integer.parseInt(match.group(2));
                char l = match.group(3).charAt(0);
                String pass = match.group(4);
                int charCount = 0;
                for(int i = 0;i < pass.length();++i) {
                    if(pass.charAt(i) == l) {
                        charCount += 1;
                    }
                }
                if(charCount >= lb && charCount <= ub) {
                    countA += 1;
                }    
                if((pass.charAt(lb-1) == l && pass.charAt(ub-1) != l) 
                || pass.charAt(lb-1) != l && pass.charAt(ub-1) == l) {
                    countB += 1;
                }
            }
            else {
                throw new RuntimeException("Line didn't match" + line);
            }
        }
        System.out.println(countA);
        System.out.println(countB);
        
    }
}