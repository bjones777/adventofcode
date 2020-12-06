package day06;

import java.util.*;
import java.io.*;
import lombok.*;

public class Main {
    public static class Decl {
        Set<String> answers = new HashSet<String>();
        Set<String> everyone;

        public void addLine(String line) {
            Set<String> lineSet = new HashSet<>(); 
            for(int i = 0;i < line.length();++i) {
                String s = line.substring(i, i+1);
                if(!s.isBlank()) {
                    answers.add(s);
                    lineSet.add(s);
                }
            }
            if(everyone == null) {
                everyone = lineSet;
            }
            else {
                everyone.retainAll(lineSet);
            }
        }

        public int getCount() {
            return answers.size();
        }

        public int getCount2() {
            return everyone != null ? everyone.size() : 0;
        }
    }

    public static int getCount(List<Decl> decls) {
        int count = 0;
        for(int i = 0;i < decls.size();++i) {
            val d = decls.get(i);
            count += d.getCount();
        }
        return count;
    }

    public static int getCount2(List<Decl> decls) {
        int count = 0;
        for(int i = 0;i < decls.size();++i) {
            val d = decls.get(i);
            count += d.getCount2();
        }
        return count;
    }

    public static void main(String[] args) throws IOException {
        List<Decl> decls = new ArrayList<>();
        Decl buildingDecl = null;
        try(val br = new BufferedReader(new InputStreamReader(Main.class.getClassLoader().getResourceAsStream("day06.txt")))) {
            for(String line = br.readLine();line != null;line = br.readLine()) {
                if(line.isBlank()) {
                    if(buildingDecl != null) {
                        decls.add(buildingDecl);
                        buildingDecl = null;
                    }
                }
                else {
                    if(buildingDecl == null) {
                        buildingDecl = new Decl();
                    }
                    buildingDecl.addLine(line);
                }
            }
        }    
        if(buildingDecl != null) {
            decls.add(buildingDecl);
            buildingDecl = null;
        }
        System.out.println(String.format("Part A: %d", getCount(decls)));
        System.out.println(String.format("Part B: %d", getCount2(decls)));
    }
}